/*
 * $HEADER$
 */

#include "lam_config.h"

#include "mpi.h"
#include "mpi/interface/c/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILING_DEFINES
#pragma weak MPI_Type_create_hvector = PMPI_Type_create_hvector
#endif

int
MPI_Type_create_hvector(int count,
                        int blocklength,
                        MPI_Aint stride,
                        MPI_Datatype oldtype,
                        MPI_Datatype *newtype)
{
    return MPI_SUCCESS;
}
