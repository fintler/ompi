/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007      Los Alamos National Security, LLC.  All rights
 *                         reserved. 
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
#include "orte_config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>
#include <string.h>

#include "opal/class/opal_list.h"
#include "opal/util/output.h"
#include "opal/util/argv.h"
#include "opal/mca/mca.h"
#include "opal/mca/base/base.h"
#include "opal/util/show_help.h"

#include "orte/orte_constants.h"
#include "orte/util/sys_info.h"
#include "orte/mca/ns/ns.h"
#include "orte/mca/errmgr/errmgr.h"
#include "orte/mca/ras/ras.h"
#include "orte/mca/ras/base/ras_private.h"
#include "orte/runtime/runtime_types.h"

#include "orte/mca/rds/rds.h"
#include "orte/mca/rds/base/rds_private.h"
#include "orte/mca/rds/hostfile/rds_hostfile.h"
#include "orte/mca/rds/hostfile/rds_hostfile_lex.h"

static bool orte_rds_hostfile_queried = false;
static char *cur_hostfile_name = NULL;

static void orte_rds_hostfile_parse_error(int token)
{
    switch (token) {
    case ORTE_RDS_HOSTFILE_STRING:
        opal_show_help("help-rds-hostfile.txt", "rds:parse_error_string",
                       true,
                       cur_hostfile_name, 
                       orte_rds_hostfile_line, 
                       token, 
                       orte_rds_hostfile_value.sval);
        break;
    case ORTE_RDS_HOSTFILE_IPV4:
    case ORTE_RDS_HOSTFILE_IPV6:
    case ORTE_RDS_HOSTFILE_INT:
        opal_show_help("help-rds-hostfile.txt", "rds:parse_error_int",
                       true,
                       cur_hostfile_name, 
                       orte_rds_hostfile_line, 
                       token, 
                       orte_rds_hostfile_value.ival);
        break;
     default:
        opal_show_help("help-rds-hostfile.txt", "rds:parse_error",
                       true,
                       cur_hostfile_name, 
                       orte_rds_hostfile_line, 
                       token );
        break;
    }
}

 /**
  * Return the integer following an = (actually may only return positive ints)
  */
static int orte_rds_hostfile_parse_int(void)
{
    if (ORTE_RDS_HOSTFILE_EQUAL != orte_rds_hostfile_lex())
        return -1;
    if (ORTE_RDS_HOSTFILE_INT != orte_rds_hostfile_lex())
        return -1;
    return orte_rds_hostfile_value.ival;
}

/**
 * Return the string following an = (option to a keyword)
 */
static char * orte_rds_hostfile_parse_string(void)
{
    int rc;
    if (ORTE_RDS_HOSTFILE_EQUAL != orte_rds_hostfile_lex()){
        return NULL;
    }
    rc = orte_rds_hostfile_lex();
    if (ORTE_RDS_HOSTFILE_STRING != rc){
        return NULL;
    }
    return strdup(orte_rds_hostfile_value.sval);
}

static char * orte_rds_hostfile_parse_string_or_int(void)
{
  int rc;
  char tmp_str[64];

  if (ORTE_RDS_HOSTFILE_EQUAL != orte_rds_hostfile_lex()){
      return NULL;
  }
  rc = orte_rds_hostfile_lex();
  switch (rc) {
  case ORTE_RDS_HOSTFILE_STRING:
      return strdup(orte_rds_hostfile_value.sval);
  case ORTE_RDS_HOSTFILE_INT:
      sprintf(tmp_str,"%d",orte_rds_hostfile_value.ival);
      return strdup(tmp_str);
  default:
      return NULL;
  }

}

static orte_ras_node_t* orte_rds_hostfile_lookup(opal_list_t* nodes, const char* name)
{
    opal_list_item_t* item;
    for(item =  opal_list_get_first(nodes);
        item != opal_list_get_end(nodes);
        item =  opal_list_get_next(item)) {
        orte_ras_node_t* node = (orte_ras_node_t*)item;
        if(strcmp(node->node_name, name) == 0) {
            opal_list_remove_item(nodes, item);
            return node;
        }
    }
    return NULL;
}

static int validate_slot_list (char *cpu_list)
{
    int i,list_len = strlen(cpu_list);

    for(i=0; i < list_len; i++){
        if ('0' <= cpu_list[i] && '9' >= cpu_list[i]) {
            continue;
        }
        else if (':' == cpu_list[i]) {
            continue;
        }
        else if (',' == cpu_list[i]) {
            continue;
        }
        else if ('*' == cpu_list[i]) {
            continue;
        }
        else if ('-' == cpu_list[i]) {
            continue;
        }
        return ORTE_ERROR;
    }
    return ORTE_SUCCESS;
}

static int orte_rds_hostfile_parse_line(int token, opal_list_t* existing, opal_list_t* updates, opal_list_t* procs)
{
    char buff[64];
    char *tmp;
    int rc;
    orte_ras_node_t* node;
    orte_ras_proc_t* proc;
    bool update = false;
    bool got_count = false;
    bool got_max = false;
    char* value;
    char** argv;
    char* node_name = NULL;
    char* username = NULL;
    int cnt;
    int number_of_slots = 0;

    if (ORTE_RDS_HOSTFILE_STRING == token ||
        ORTE_RDS_HOSTFILE_HOSTNAME == token ||
        ORTE_RDS_HOSTFILE_INT == token ||
        ORTE_RDS_HOSTFILE_IPV4 == token ||
        ORTE_RDS_HOSTFILE_IPV6 == token) {

        if(ORTE_RDS_HOSTFILE_INT == token) {
            sprintf(buff,"%d", orte_rds_hostfile_value.ival);
            value = buff;
        } else {
            value = orte_rds_hostfile_value.sval;
        }
        argv = opal_argv_split (value, '@');

        cnt = opal_argv_count (argv);
        if (1 == cnt) {
            node_name = strdup(argv[0]);
        } else if (2 == cnt) {
            username = strdup(argv[0]);
            node_name = strdup(argv[1]);
        } else {
            opal_output(0, "WARNING: Unhandled user@host-combination\n"); /* XXX */
        }
        opal_argv_free (argv);

        /* convert this into something globally unique */
        if(strcmp(node_name, "localhost") == 0) {
            /* Nodename has been allocated, that is for sure */
            free (node_name);
            node_name = strdup(orte_system_info.nodename);
        }

        /* Do we need to make a new node object?  First check to see
           if it's in the existing list. */

        if (NULL == (node = orte_rds_hostfile_lookup(existing, node_name))) {

            /* If it wasn't, see if it's already in the updates list */

            if (NULL == (node = orte_rds_hostfile_lookup(updates,
                                                         node_name))) {
                node = OBJ_NEW(orte_ras_node_t);
                node->node_name = node_name;
                node->node_username = username;
                node->node_slots = 0;
            }

            /* Note that we need to set update to true regardless of
               whether the node was found on the updates list or not.
               If it was found, we just removed it (in
               orte_rds_hostfile_lookup()), so the update puts it back
               (potentially after updating it, of course).  If it was
               not found, then we have a new node instance that needs
               to be added to the updates list. */

            update = true;
        }
    } else {
        orte_rds_hostfile_parse_error(token);
        return ORTE_ERROR;
    }

    got_count = false;
    while (!orte_rds_hostfile_done) {
        token = orte_rds_hostfile_lex();

        switch (token) {
        case ORTE_RDS_HOSTFILE_DONE:
            goto done;

        case ORTE_RDS_HOSTFILE_NEWLINE:
            goto done;

        case ORTE_RDS_HOSTFILE_USERNAME:
            node->node_username = orte_rds_hostfile_parse_string();
            break;

        case ORTE_RDS_HOSTFILE_COUNT:
        case ORTE_RDS_HOSTFILE_CPU:
        case ORTE_RDS_HOSTFILE_SLOTS:
            rc = orte_rds_hostfile_parse_int();
            if (rc < 0) {
                opal_show_help("help-rds-hostfile.txt", "rds:slots",
                               true,
                               cur_hostfile_name, rc);
                OBJ_RELEASE(node);
                return ORTE_ERROR;
            }
            node->node_slots += rc;
            update = true;
            got_count = true;

            /* Ensure that node_slots_max >= node_slots */
            if (node->node_slots_max != 0 && node->node_slots_max < node->node_slots) {
                node->node_slots_max = node->node_slots;
            }
            break;

        case ORTE_RDS_HOSTFILE_SLOT:
            tmp = orte_rds_hostfile_parse_string_or_int();
            proc = OBJ_NEW(orte_ras_proc_t);
            proc->node_name = strdup(node_name);
            argv = opal_argv_split (tmp, '@');
            cnt = opal_argv_count (argv);
            switch (cnt) {
            case 1:
                proc->cpu_list = strdup(argv[0]);
                if (ORTE_SUCCESS != (rc = validate_slot_list(proc->cpu_list))) {
                    OBJ_RELEASE(proc);
                    OBJ_RELEASE(node);
                    opal_argv_free (argv);
                    ORTE_ERROR_LOG(ORTE_ERR_BAD_PARAM);
                    orte_rds_hostfile_parse_error(token);
                    return ORTE_ERROR;
                }
                break;
            case 2:
                proc->rank = strtol(argv[0],(char **)NULL,0);
                proc->cpu_list = strdup(argv[1]);
                if (ORTE_SUCCESS != (rc = validate_slot_list(proc->cpu_list))) {
                    OBJ_RELEASE(proc);
                    OBJ_RELEASE(node);
                    opal_argv_free (argv);
                    ORTE_ERROR_LOG(ORTE_ERR_BAD_PARAM);
                    orte_rds_hostfile_parse_error(token);
                    return ORTE_ERROR;
                }
                break;
            default:
                OBJ_RELEASE(proc);
                OBJ_RELEASE(node);
                opal_argv_free (argv);
                ORTE_ERROR_LOG(ORTE_ERR_BAD_PARAM);
                orte_rds_hostfile_parse_error(token);
                return ORTE_ERROR;
            }
            opal_argv_free (argv);
            opal_list_append(procs, &proc->super);
            number_of_slots++;
            break;
        case ORTE_RDS_HOSTFILE_SLOTS_MAX:
            rc = orte_rds_hostfile_parse_int();
            if (rc < 0) {
                opal_show_help("help-rds-hostfile.txt", "rds:max_slots",
                               true,
                               cur_hostfile_name, ((size_t) rc));
                OBJ_RELEASE(node);
                return ORTE_ERROR;
            }
            /* Only take this update if it puts us >= node_slots */
            if (rc >= node->node_slots) {
                if (node->node_slots_max != rc) {
                    node->node_slots_max = rc;
                    update = true;
                    got_max = true;
                }
            } else {
                opal_show_help("help-rds-hostfile.txt", "rds:max_slots_lt",
                               true,
                               cur_hostfile_name, node->node_slots, rc);
                ORTE_ERROR_LOG(ORTE_ERR_BAD_PARAM);
                OBJ_RELEASE(node);
                return ORTE_ERROR;
            }
            break;

        default:
            orte_rds_hostfile_parse_error(token);
            OBJ_RELEASE(node);
            return ORTE_ERROR;
        }
        if (number_of_slots > node->node_slots) {
            ORTE_ERROR_LOG(ORTE_ERR_BAD_PARAM);
            OBJ_RELEASE(node);
            return ORTE_ERROR;
        }
    }

done:
    if (update) {
        if (!got_count) {
            if (got_max) {
                node->node_slots = node->node_slots_max;
            } else {
                ++node->node_slots;
            }
        }
        opal_list_append(updates, &node->super);
    } else {
        OBJ_RELEASE(node);
    }
    return ORTE_SUCCESS;
}


/**
 * Parse the specified file into a node list.
 */

static int orte_rds_hostfile_parse(const char *hostfile, opal_list_t* existing, opal_list_t* updates, opal_list_t* procs)
{
    int token;
    int rc = ORTE_SUCCESS;

    OPAL_THREAD_LOCK(&mca_rds_hostfile_component.lock);
    
    cur_hostfile_name = strdup(hostfile);
    
    orte_rds_hostfile_done = false;
    orte_rds_hostfile_in = fopen(hostfile, "r");
    if (NULL == orte_rds_hostfile_in) {
        rc = ORTE_ERR_NOT_FOUND;
        goto unlock;
    }

    while (!orte_rds_hostfile_done) {
        token = orte_rds_hostfile_lex();

        switch (token) {
        case ORTE_RDS_HOSTFILE_DONE:
            orte_rds_hostfile_done = true;
            break;

        case ORTE_RDS_HOSTFILE_NEWLINE:
            break;

        /*
         * This looks odd, since we have several forms of host-definitions:
         *   hostname              just plain as it is, being a ORTE_RDS_HOSTFILE_STRING
         *   IP4s and user@IPv4s
         *   hostname.domain and user@hostname.domain
         */
        case ORTE_RDS_HOSTFILE_STRING:
        case ORTE_RDS_HOSTFILE_INT:
        case ORTE_RDS_HOSTFILE_HOSTNAME:
        case ORTE_RDS_HOSTFILE_IPV4:
        case ORTE_RDS_HOSTFILE_IPV6:
            rc = orte_rds_hostfile_parse_line(token, existing, updates, procs);
            if (ORTE_SUCCESS != rc) {
                goto unlock;
            }
            break;

        default:
            orte_rds_hostfile_parse_error(token);
            goto unlock;
        }
    }
    fclose(orte_rds_hostfile_in);
    orte_rds_hostfile_in = NULL;

unlock:
    if(NULL != cur_hostfile_name) {
        free(cur_hostfile_name);
        cur_hostfile_name = NULL;
    }

    OPAL_THREAD_UNLOCK(&mca_rds_hostfile_component.lock);
    return rc;
}

/**
 * Parse the default file as specified by the MCA parameter,
 * rds_hostfile_path, and add the nodes to the registry.
 */

static int orte_rds_hostfile_query(orte_jobid_t job)
{
    opal_list_t existing;
    opal_list_t updates, procs;
    opal_list_item_t *item;
    int rc;

    if (orte_rds_hostfile_queried) {
        /* if we have already been queried, then
         * our info is on the registry, so just
         * return. Note that this restriction
         * may eventually be lifted - ideally, 
         * we might check to see if this is a
         * new file name and go ahead with the
         * query if so.
         */
        return ORTE_SUCCESS;
    }
    orte_rds_hostfile_queried = true;
    
    OBJ_CONSTRUCT(&existing, opal_list_t);
    OBJ_CONSTRUCT(&updates, opal_list_t);
    OBJ_CONSTRUCT(&procs, opal_list_t);
    rc = orte_ras_base_node_query(&existing);
    if(ORTE_SUCCESS != rc) {
        goto cleanup;
    }

    rc = mca_base_param_find("rds", "hostfile", "path");
    mca_base_param_lookup_string(rc, &mca_rds_hostfile_component.path);

    rc = orte_rds_hostfile_parse(mca_rds_hostfile_component.path, &existing, &updates, &procs);
    if (ORTE_ERR_NOT_FOUND == rc) {
        if(mca_rds_hostfile_component.default_hostfile) {
            rc = ORTE_SUCCESS;
        } else {
            opal_show_help("help-rds-hostfile.txt", "rds:no-hostfile",
                           true,
                           mca_rds_hostfile_component.path);
        }
        goto cleanup;
    } else if (ORTE_SUCCESS != rc) {
        goto cleanup;
    }

    if ( !opal_list_is_empty(&updates) ) {

        /* Insert the results into the RAS, since we can assume that any
         * resources listed in the hostfile have been
         * already allocated for our use.
         */
        rc = orte_ras_base_node_insert(&updates);
        if (ORTE_SUCCESS != rc) {
            goto cleanup;
        }
        
        /* and now, indicate that ORTE should override any oversubscribed conditions
         * based on local hardware limits since the user (a) might not have
         * provided us any info on the #slots for a node, and (b) the user
         * might have been wrong! If we don't check the number of local physical
         * processors, then we could be too aggressive on our sched_yield setting
         * and cause performance problems.
         */
        rc = orte_ras_base_set_oversubscribe_override(job);
        if (ORTE_SUCCESS != rc) {
            goto cleanup;
        }
    }
    if ( !opal_list_is_empty(&procs) ) {
        rc = orte_ras_base_proc_insert(&procs, job);
        if (ORTE_SUCCESS != rc) {
            goto cleanup;
        }
    }

cleanup:
    if (NULL != mca_rds_hostfile_component.path) {
        free(mca_rds_hostfile_component.path);
        mca_rds_hostfile_component.path = NULL;
    }

    while(NULL != (item = opal_list_remove_first(&existing))) {
        OBJ_RELEASE(item);
    }

    while(NULL != (item = opal_list_remove_first(&updates))) {
        OBJ_RELEASE(item);
    }
    while(NULL != (item = opal_list_remove_first(&procs))) {
        OBJ_RELEASE(item);
    }

    OBJ_DESTRUCT(&existing);
    OBJ_DESTRUCT(&updates);
    OBJ_DESTRUCT(&procs);

    return rc;
}


orte_rds_base_module_t orte_rds_hostfile_module = {
    orte_rds_hostfile_query,
    orte_rds_base_store_resource
};
