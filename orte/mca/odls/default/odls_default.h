/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2006 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
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
/**
 * @file:
 */

#ifndef ORTE_ODLS_H
#define ORTE_ODLS_H

#include "orte_config.h"

#include "opal/threads/mutex.h"
#include "opal/threads/condition.h"
#include "opal/mca/mca.h"

#include "orte/mca/ns/ns_types.h"
#include "orte/mca/gpr/gpr_types.h"

#include "orte/mca/odls/odls.h"

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * Module open / close
 */
int orte_odls_default_component_open(void);
int orte_odls_default_component_close(void);
orte_odls_base_module_t* orte_odls_default_component_init(int *priority);

/*
 * Startup / Shutdown
 */
int orte_odls_default_finalize(void);

/*
 * Interface
 */
int orte_odls_default_subscribe_launch_data(orte_jobid_t job, orte_gpr_notify_cb_fn_t cbfunc);
int orte_odls_default_launch_local_procs(orte_gpr_notify_data_t *data);
int orte_odls_default_kill_local_procs(orte_jobid_t job, bool set_state);
int orte_odls_default_signal_local_procs(orte_process_name_t *proc,
                                         int32_t signal);

/**
 * ODLS Default globals
 */
typedef struct orte_odls_default_globals_t {
    opal_mutex_t mutex;
    opal_condition_t cond;
    opal_list_t children;
} orte_odls_default_globals_t;

extern orte_odls_default_globals_t orte_odls_default;

/*
 * List object to locally store the process names and pids of
 * our children. This can subsequently be used to order termination
 * or pass signals without looking the info up again.
 */
typedef struct odls_default_child_t {
    opal_list_item_t super;      /* required to place this on a list */
    orte_process_name_t *name;   /* the OpenRTE name of the proc */
    pid_t pid;                   /* local pid of the proc */
    orte_std_cntr_t app_idx;     /* index of the app_context for this proc */
    bool alive;                  /* is this proc alive? */
} odls_default_child_t;
ORTE_DECLSPEC OBJ_CLASS_DECLARATION(odls_default_child_t);

/*
 * List object to locally store app_contexts returned by the
 * registry subscription. Since we don't know how many app_contexts will
 * be returned, we need to store them on a list.
 */
typedef struct odls_default_app_context_t {
    opal_list_item_t super;      /* required to place this on a list */
    orte_app_context_t *app_context;
} odls_default_app_context_t;
ORTE_DECLSPEC OBJ_CLASS_DECLARATION(odls_default_app_context_t);

/*
 * ODLS Default module
 */
extern orte_odls_base_module_t orte_odls_default_module;

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif /* ORTE_ODLS_H */
