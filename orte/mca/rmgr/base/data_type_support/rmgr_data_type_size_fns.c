/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "orte_config.h"

#include <sys/types.h>
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include "opal/util/argv.h"

#include "orte/mca/errmgr/errmgr.h"
#include "orte/dss/dss_internal.h"

#include "orte/mca/rmgr/base/base.h"

/*
 * APP CONTEXT
 */
int orte_rmgr_base_size_app_context(size_t *size, orte_app_context_t *src, orte_data_type_t type)
{
    int j, rc, count;
    size_t i, map_size;

    /* account for the object itself */
    *size = sizeof(orte_app_context_t);

    /* if src is NULL, then that's all we wanted */
    if (NULL == src) return ORTE_SUCCESS;

    if (NULL != src->app) {
        *size += strlen(src->app);
    }

    count = opal_argv_count(src->argv);
    if (0 < count) {
        /* account for array of char* */
        *size += count * sizeof(char*);
        for (j=0; j < count; j++) {
            *size += strlen(src->argv[j]);
        }
    }
    *size += sizeof(char**);  /* account for size of the argv pointer itself */

    count = opal_argv_count(src->env);
    if (0 < count) {
        /* account for array of char* */
        *size += count * sizeof(char*);
        for (i=0; i < (size_t)count; i++) {
            *size += strlen(src->env[i]);
        }
    }
    *size += sizeof(char**);  /* account for size of the env pointer itself */

    if (NULL != src->cwd) {
        *size += strlen(src->cwd);  /* working directory */
    }

    if (0 < src->num_map) {
        for (i=0; i < src->num_map; i++) {
            if (ORTE_SUCCESS != (rc = orte_rmgr_base_size_app_context_map(&map_size, src->map_data[i], ORTE_APP_CONTEXT_MAP))) {
                ORTE_ERROR_LOG(rc);
                *size = 0;
                return rc;
            }
        }
    }

    if (NULL != src->prefix_dir) {
        *size += strlen(src->prefix_dir);
    }

    return ORTE_SUCCESS;
}

/*
 * APP CONTEXT MAP
 */
int orte_rmgr_base_size_app_context_map(size_t *size, orte_app_context_map_t *src, orte_data_type_t type)
{
    /* account for the object itself */
    *size = sizeof(orte_app_context_map_t);

    /* if src is NULL, then that's all we wanted */
    if (NULL == src) return ORTE_SUCCESS;

    *size += strlen(src->map_data);

    return ORTE_SUCCESS;
}
