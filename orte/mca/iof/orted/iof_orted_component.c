/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007      Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "orte_config.h"

#include "opal/runtime/opal_progress.h"
#include "opal/mca/base/base.h"
#include "opal/mca/base/mca_base_param.h"

#include "orte/mca/rml/rml.h"
#include "orte/mca/rml/rml_types.h"
#include "orte/util/proc_info.h"
#include "orte/mca/errmgr/errmgr.h"
#include "orte/runtime/orte_globals.h"

#include "iof_orted.h"

/*
 * Local functions
 */
static int orte_iof_orted_open(void);
static int orte_iof_orted_close(void);
static int orte_iof_orted_query(mca_base_module_t **module, int *priority);


/*
 * Local variables
 */
static bool initialized = false;

/*
 * Public string showing the iof orted component version number
 */
const char *mca_iof_orted_component_version_string =
"Open MPI orted iof MCA component version " ORTE_VERSION;


orte_iof_orted_component_t mca_iof_orted_component = {
    {
        {
            ORTE_IOF_BASE_VERSION_2_0_0,
            
            "orted", /* MCA component name */
            ORTE_MAJOR_VERSION,  /* MCA component major version */
            ORTE_MINOR_VERSION,  /* MCA component minor version */
            ORTE_RELEASE_VERSION,  /* MCA component release version */
            
            /* Component open, close, and query functions */
            orte_iof_orted_open,
            orte_iof_orted_close,
            orte_iof_orted_query
        },
        {
            /* The component is checkpoint ready */
            MCA_BASE_METADATA_PARAM_CHECKPOINT
        }
    }
};

/**
  * component open/close/init function
  */
static int orte_iof_orted_open(void)
{
    /* Nothing to do */
    return ORTE_SUCCESS;
}

static int orte_iof_orted_close(void)
{
    int rc = ORTE_SUCCESS;
    opal_list_item_t *item;
    
    if (initialized) {
        OPAL_THREAD_LOCK(&mca_iof_orted_component.lock);
        while ((item = opal_list_remove_first(&mca_iof_orted_component.sinks)) != NULL) {
            OBJ_RELEASE(item);
        }
        OBJ_DESTRUCT(&mca_iof_orted_component.sinks);
        while ((item = opal_list_remove_first(&mca_iof_orted_component.procs)) != NULL) {
            OBJ_RELEASE(item);
        }
        OBJ_DESTRUCT(&mca_iof_orted_component.procs);
        /* Cancel the RML receive */
        rc = orte_rml.recv_cancel(ORTE_NAME_WILDCARD, ORTE_RML_TAG_IOF_PROXY);
        OPAL_THREAD_UNLOCK(&mca_iof_orted_component.lock);
        OBJ_DESTRUCT(&mca_iof_orted_component.lock);
    }
    return rc;
}


static int orte_iof_orted_query(mca_base_module_t **module, int *priority)
{
    int rc;

    /* set default */
    *module = NULL;
    *priority = -1;

    /* if we are not a daemon, then don't use this module */
    if (!orte_proc_info.daemon) {
        return ORTE_ERROR;
    }

    /* post a non-blocking RML receive to get messages
       from the HNP IOF component */
    if (ORTE_SUCCESS != (rc = orte_rml.recv_buffer_nb(ORTE_NAME_WILDCARD,
                                                      ORTE_RML_TAG_IOF_PROXY,
                                                      ORTE_RML_NON_PERSISTENT,
                                                      orte_iof_orted_recv,
                                                      NULL))) {
        ORTE_ERROR_LOG(rc);
        return rc;
        
    }

    /* setup the local global variables */
    OBJ_CONSTRUCT(&mca_iof_orted_component.lock, opal_mutex_t);
    OBJ_CONSTRUCT(&mca_iof_orted_component.sinks, opal_list_t);
    OBJ_CONSTRUCT(&mca_iof_orted_component.procs, opal_list_t);
    mca_iof_orted_component.xoff = false;
    
    /* we must be selected */
    *priority = 100;
    *module = (mca_base_module_t *) &orte_iof_orted_module;
    initialized = true;
    
    return ORTE_SUCCESS;
}

