/*
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
/**
 * @file
 *
 * Universal Resource Manager (URM)
 */
#ifndef ORTE_RMGR_URM_H
#define ORTE_RMGR_URM_H

#include "mca/rmgr/rmgr.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/**
* URM component structure -- add some stuff beyond what is in the
* normal rmgr component.
*/
struct orte_rmgr_proxy_component_t {
    /** Base rmgr component */
    orte_rmgr_base_component_t super;
};
/** Convenience typedef */
typedef struct orte_rmgr_proxy_component_t orte_rmgr_proxy_component_t;

/** Global URM component */
OMPI_COMP_EXPORT extern orte_rmgr_proxy_component_t mca_rmgr_proxy_component;
/** Global URM module */
OMPI_COMP_EXPORT extern orte_rmgr_base_module_t orte_rmgr_proxy_module;

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif

#endif
