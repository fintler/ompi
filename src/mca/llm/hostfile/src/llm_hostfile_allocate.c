/* -*- C -*-
 *
 * Copyright (c) 2004-2005 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"
#include "llm_hostfile.h"

#include "mca/llm/llm.h"
#include "mca/llm/base/base.h"
#include "mca/llm/base/base_internal.h"
#include "mca/ns/ns.h"

#include <stdio.h>

ompi_list_t*
mca_llm_hostfile_allocate_resources(mca_llm_base_module_t *me_base,
                                    mca_ns_base_jobid_t jobid, 
                                    int nodes, int procs)
{
    mca_llm_hostfile_module_t *me = (mca_llm_hostfile_module_t*) me_base;
    ompi_list_t *hostlist = NULL;
    ompi_list_t *nodelist = NULL;
    int ret;

    /* start by getting the full list of available resources */
    hostlist = mca_llm_base_parse_hostfile(me->hostfile_filename);
    if (NULL == hostlist) {
        return NULL;
    }

    ret = mca_llm_base_collapse_resources(hostlist);
    if (OMPI_SUCCESS != ret) {
        mca_llm_base_deallocate(hostlist);
        return NULL;
    }

    ret = mca_llm_base_map_resources(nodes, procs, hostlist);
    if (OMPI_SUCCESS != ret) {
        mca_llm_base_deallocate(hostlist);
        return NULL;
    }

    nodelist = mca_llm_base_create_node_allocation(hostlist);
    if (OMPI_SUCCESS != ret) {
        mca_llm_base_deallocate(hostlist);
        return NULL;
    }

    return nodelist;
}
