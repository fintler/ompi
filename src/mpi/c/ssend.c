/*
 * $HEADERS$
 */
#include "lam_config.h"
#include <stdio.h>

#include "mpi.h"
#include "runtime/runtime.h"
#include "mpi/c/bindings.h"
#include "mca/pml/pml.h"


#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILING_DEFINES
#pragma weak MPI_Ssend = PMPI_Ssend
#endif

int MPI_Ssend(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm) 
{
    if (dest == MPI_PROC_NULL) {
        return MPI_SUCCESS;
    }
                                                                                                                        
    if ( MPI_PARAM_CHECK ) {
        int rc = MPI_SUCCESS;
        if (lam_mpi_finalized) {
            rc = MPI_ERR_INTERN;
        } else if (count < 0) {
            rc = MPI_ERR_COUNT;
#if 0
        } else if (type == MPI_DATATYPE_NULL) {
            rc = MPI_ERR_TYPE;
#endif
        } else if (tag < 0 || tag > MPI_TAG_UB_VALUE) {
            rc = MPI_ERR_TAG;
        } else if (lam_comm_invalid(comm)) {
            rc = MPI_ERR_COMM;
        } else if (lam_comm_peer_invalid(comm, dest)) {
            rc = MPI_ERR_RANK;
        }
        if (rc != MPI_SUCCESS) {
            return rc;
        }
    }
                                                                                                                        
    return mca_pml.pml_send(buf, count, type, dest, tag, MCA_PML_BASE_SEND_SYNCHRONOUS, comm);
}

