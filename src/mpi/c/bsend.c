/*
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "mca/pml/pml.h"
#include "mca/pml/base/pml_base_bsend.h"


#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Bsend = PMPI_Bsend
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static const char FUNC_NAME[] = "MPI_Bsend";


int MPI_Bsend(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm) 
{
    int rc;
    ompi_request_t* request;

    if (dest == MPI_PROC_NULL) {
        return MPI_SUCCESS;
    }
   
    if ( MPI_PARAM_CHECK ) {
        int rc = MPI_SUCCESS;
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        if (ompi_comm_invalid(comm)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, FUNC_NAME);
        } else if (count < 0) {
            rc = MPI_ERR_COUNT;
        } else if (type == MPI_DATATYPE_NULL) {
            rc = MPI_ERR_TYPE;
        } else if (tag < 0 || tag > MPI_TAG_UB_VALUE) {
            rc = MPI_ERR_TAG;
        } else if (ompi_comm_peer_invalid(comm, dest)) {
            rc = MPI_ERR_RANK;
        }
        OMPI_ERRHANDLER_CHECK(rc, comm, rc, FUNC_NAME);
    }

    rc = mca_pml.pml_isend_init(buf, count, type, dest, tag, MCA_PML_BASE_SEND_BUFFERED, comm, &request);
    if(OMPI_SUCCESS != rc)
        goto error_return;

    rc = mca_pml_base_bsend_request_init(request, false);
    if(OMPI_SUCCESS != rc)
        goto error_return;

    rc = mca_pml.pml_start(1, &request);
    if(OMPI_SUCCESS != rc)
        goto error_return;

    rc = ompi_request_wait(&request, MPI_STATUS_IGNORE);
    if(OMPI_SUCCESS != rc) {
        ompi_request_free(&request);
        return rc;
    }

error_return:
    OMPI_ERRHANDLER_RETURN(rc, comm, rc, FUNC_NAME);
}

