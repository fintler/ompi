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
#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "request/request.h"
#include "communicator/communicator.h"
#include "errhandler/errhandler.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Request_get_status = PMPI_Request_get_status
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Request_get_status";

/* Non blocking test for the request status. Upon completion, the request will
 * not be freed (unlike the test function). A subsequent call to test, wait
 * or free should be executed on the request.
 */
int MPI_Request_get_status(MPI_Request request, int *flag,
                           MPI_Status *status) 
{
    if( MPI_PARAM_CHECK ) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if( (NULL == flag) || (NULL == status) ) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_ARG, FUNC_NAME);
        }
    }

    opal_atomic_mb();
    if( (request == MPI_REQUEST_NULL) || (request->req_state == OMPI_REQUEST_INACTIVE) ) {
        *flag = true;
        if( MPI_STATUS_IGNORE != status ) {
            *status = ompi_status_empty;
        }
        return MPI_SUCCESS;
    }
    if( request->req_complete ) { 
        *flag = true; 
        if (MPI_STATUS_IGNORE != status) {
            *status = request->req_status;
        }
        return MPI_SUCCESS;
    }
    *flag = false;
#if OMPI_ENABLE_PROGRESS_THREADS == 0
    opal_progress();
#endif
    return MPI_SUCCESS;
}
