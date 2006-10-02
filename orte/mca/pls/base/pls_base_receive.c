/* -*- C -*-
 *
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
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
/** @file:
 *
 */

/*
 * includes
 */
#include "orte_config.h"

#include "orte/orte_constants.h"
#include "orte/orte_types.h"

#include "opal/util/output.h"
#include "opal/mca/mca.h"
#include "opal/mca/base/mca_base_param.h"

#include "orte/dss/dss.h"
#include "orte/util/proc_info.h"
#include "orte/mca/errmgr/errmgr.h"
#include "orte/mca/rml/rml.h"

#include "orte/mca/pls/pls_types.h"
#include "orte/mca/pls/pls.h"
#include "orte/mca/pls/base/pls_private.h"

static bool recv_issued=false;

int orte_pls_base_comm_start(void)
{
    int rc;

    if (recv_issued) {
        return ORTE_SUCCESS;
    }
    
    if (ORTE_SUCCESS != (rc = orte_rml.recv_buffer_nb(ORTE_RML_NAME_ANY,
                                                      ORTE_RML_TAG_PLS,
                                                      ORTE_RML_PERSISTENT,
                                                      orte_pls_base_recv,
                                                      NULL))) {
        ORTE_ERROR_LOG(rc);
    }
    recv_issued = true;
    
    return rc;
}


int orte_pls_base_comm_stop(void)
{
    int rc;
    
    if (!recv_issued) {
        return ORTE_SUCCESS;
    }
    
    if (ORTE_SUCCESS != (rc = orte_rml.recv_cancel(ORTE_RML_NAME_ANY, ORTE_RML_TAG_PLS))) {
        ORTE_ERROR_LOG(rc);
    }
    recv_issued = false;
    
    return rc;
}


/*
 * handle message from proxies
 * NOTE: The incoming buffer "buffer" is OBJ_RELEASED by the calling program.
 * DO NOT RELEASE THIS BUFFER IN THIS CODE
 */

void orte_pls_base_recv(int status, orte_process_name_t* sender,
                        orte_buffer_t* buffer, orte_rml_tag_t tag,
                        void* cbdata)
{
    orte_buffer_t answer;
    orte_pls_cmd_flag_t command;
    orte_std_cntr_t count;
    orte_jobid_t job;
    orte_process_name_t *name;
    int32_t signal;
    int rc;

    count = 1;
    if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &command, &count, ORTE_PLS_CMD))) {
        ORTE_ERROR_LOG(rc);
        return;
    }

    OBJ_CONSTRUCT(&answer, orte_buffer_t);

    if (ORTE_SUCCESS != (rc = orte_dss.pack(&answer, &command, 1, ORTE_PLS_CMD))) {
        ORTE_ERROR_LOG(rc);
    }
    
    switch (command) {
        case ORTE_PLS_LAUNCH_JOB_CMD:
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &job, &count, ORTE_JOBID))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            if (ORTE_SUCCESS != (rc = orte_pls.launch_job(job))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        case ORTE_PLS_TERMINATE_JOB_CMD:
            opal_output(0, "pls_base_recv: terminate job");
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &job, &count, ORTE_JOBID))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                opal_output(0, "pls_base_recv: terminate job with jobid %ld", (long)job);
                
            if (ORTE_SUCCESS != (rc = orte_pls.terminate_job(job))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        case ORTE_PLS_TERMINATE_ORTEDS_CMD:
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &job, &count, ORTE_JOBID))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            opal_output(0, "pls_base_recv: terminate orteds with jobid %ld", (long)job);

            if (ORTE_SUCCESS != (rc = orte_pls.terminate_orteds(job))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        case ORTE_PLS_SIGNAL_JOB_CMD:
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &job, &count, ORTE_JOBID))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &signal, &count, ORTE_INT32))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            if (ORTE_SUCCESS != (rc = orte_pls.signal_job(job, signal))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        case ORTE_PLS_TERMINATE_PROC_CMD:
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &name, &count, ORTE_NAME))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            if (ORTE_SUCCESS != (rc = orte_pls.terminate_proc(name))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        case ORTE_PLS_SIGNAL_PROC_CMD:
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &name, &count, ORTE_NAME))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            count = 1;
            if (ORTE_SUCCESS != (rc = orte_dss.unpack(buffer, &signal, &count, ORTE_INT32))) {
                ORTE_ERROR_LOG(rc);
                goto SEND_ANSWER;
            }
                
            if (ORTE_SUCCESS != (rc = orte_pls.signal_proc(name, signal))) {
                ORTE_ERROR_LOG(rc);
            }
            break;
            
        default:
            ORTE_ERROR_LOG(ORTE_ERR_VALUE_OUT_OF_BOUNDS);
    }
   
SEND_ANSWER:  /* send the answer */
    if (0 > orte_rml.send_buffer(sender, &answer, tag, 0)) {
        ORTE_ERROR_LOG(ORTE_ERR_COMM_FAILURE);
    }
    
    /* cleanup */
    OBJ_DESTRUCT(&answer);
}

