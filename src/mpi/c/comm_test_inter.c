/*
 * $HEADERS$
 */
#include "lam_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILING_DEFINES
#pragma weak MPI_Comm_test_inter = PMPI_Comm_test_inter
#endif

int MPI_Comm_test_inter(MPI_Comm comm, int *flag) {
    return MPI_SUCCESS;
}
