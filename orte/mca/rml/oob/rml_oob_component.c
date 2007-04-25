/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
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

#include "orte_config.h"
#include "orte/orte_constants.h"
#include "opal/util/output.h"
#include "opal/mca/base/base.h"
#include "opal/mca/base/mca_base_param.h"
#include "orte/mca/rml/base/base.h"
#include "orte/mca/oob/oob.h"
#include "orte/mca/oob/base/base.h"
#include "rml_oob.h"
#include "orte/mca/errmgr/errmgr.h"

static orte_rml_module_t* orte_rml_oob_init(int* priority);
static int orte_rml_oob_open(void);
static int orte_rml_oob_close(void);


/**
 * component definition
 */
orte_rml_component_t mca_rml_oob_component = {
      /* First, the mca_base_component_t struct containing meta
         information about the component itself */

      {
        /* Indicate that we are a rml v1.0.0 component (which also
           implies a specific MCA version) */

        ORTE_RML_BASE_VERSION_1_0_0,

        "oob", /* MCA component name */
        ORTE_MAJOR_VERSION,  /* MCA component major version */
        ORTE_MINOR_VERSION,  /* MCA component minor version */
        ORTE_RELEASE_VERSION,  /* MCA component release version */
        orte_rml_oob_open,  /* component open */
        orte_rml_oob_close, /* component close */
      },

      /* Next the MCA v1.0.0 component meta data */
      {
          /* The component is checkpoint ready */
          MCA_BASE_METADATA_PARAM_CHECKPOINT
      },
      orte_rml_oob_init
};

orte_rml_module_t orte_rml_oob_module = {
    mca_oob_base_module_init,
    NULL,
    (orte_rml_module_get_uri_fn_t)mca_oob_get_contact_info,
    (orte_rml_module_set_uri_fn_t)mca_oob_set_contact_info,
    (orte_rml_module_parse_uris_fn_t)mca_oob_parse_contact_info,
    (orte_rml_module_ping_fn_t)mca_oob_ping,
    (orte_rml_module_send_fn_t)mca_oob_send,
    (orte_rml_module_send_nb_fn_t)mca_oob_send_nb,
    (orte_rml_module_send_buffer_fn_t)mca_oob_send_packed,
    (orte_rml_module_send_buffer_nb_fn_t)mca_oob_send_packed_nb,
    (orte_rml_module_recv_fn_t)mca_oob_recv,
    (orte_rml_module_recv_nb_fn_t)mca_oob_recv_nb,
    (orte_rml_module_recv_buffer_fn_t)mca_oob_recv_packed,
    (orte_rml_module_recv_buffer_nb_fn_t)mca_oob_recv_packed_nb,
    (orte_rml_module_recv_cancel_fn_t)mca_oob_recv_cancel,
    (orte_rml_module_xcast_fn_t)mca_oob_xcast,
    (orte_rml_module_exception_fn_t)mca_oob_add_exception_handler,
    (orte_rml_module_exception_fn_t)mca_oob_del_exception_handler,
    (orte_rml_module_ft_event_fn_t)orte_rml_oob_ft_event
};


static orte_rml_module_t* orte_rml_oob_init(int* priority)
{
    if(mca_oob_base_init() != ORTE_SUCCESS)
        return NULL;
    *priority = 1;
    return &orte_rml_oob_module;
}


/*
 * initialize the underlying oob infrastructure so that all the
 * pointers in the RML struct can be valid.
 */
static int
orte_rml_oob_open(void)
{
    int rc;

    if (ORTE_SUCCESS != (rc = mca_oob_base_open())) {
        ORTE_ERROR_LOG(rc);
        return rc;
    }

    return rc;
}


/*
 * shut down the OOB, since we started it.
 */
static int
orte_rml_oob_close(void)
{
    int rc;


    if (ORTE_SUCCESS != (rc = mca_oob_base_close())) {
        return rc;
    }

    return rc;
}

int orte_rml_oob_ft_event(int state) {
    int exit_status = ORTE_SUCCESS;
    int ret;

    if(OPAL_CRS_CHECKPOINT == state) {
        ;
    }
    else if(OPAL_CRS_CONTINUE == state) {
        ;
    }
    else if(OPAL_CRS_RESTART == state) {
        ;
    }
    else if(OPAL_CRS_TERM == state ) {
        ;
    }
    else {
        ;
    }

    if( ORTE_SUCCESS != (ret = mca_oob.oob_ft_event(state)) ) {
        ORTE_ERROR_LOG(ret);
        exit_status = ret;
        goto cleanup;
    }


    if(OPAL_CRS_CHECKPOINT == state) {
        ;
    }
    else if(OPAL_CRS_CONTINUE == state) {
        ;
    }
    else if(OPAL_CRS_RESTART == state) {
        if( ORTE_SUCCESS != (ret = mca_oob_base_close())) {
            ORTE_ERROR_LOG(ret);
            exit_status = ret;
            goto cleanup;
        }

        if( ORTE_SUCCESS != (ret = mca_oob_base_open())) {
            ORTE_ERROR_LOG(ret);
            exit_status = ret;
            goto cleanup;
        }

        if( ORTE_SUCCESS != (ret = mca_oob_base_init())) {
            ORTE_ERROR_LOG(ret);
            exit_status = ret;
            goto cleanup;
        }

        if(NULL != orte_process_info.ns_replica_uri) {
            mca_oob_set_contact_info(orte_process_info.ns_replica_uri);
        }

        if(NULL != orte_process_info.gpr_replica_uri) {
            mca_oob_set_contact_info(orte_process_info.gpr_replica_uri);
        }
    }
    else if(OPAL_CRS_TERM == state ) {
        ;
    }
    else {
        ;
    }

 cleanup:
    return exit_status;
}
