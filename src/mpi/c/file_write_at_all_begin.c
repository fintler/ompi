/*
 * $HEADER$
 */

#include "lam_config.h"

#include "mpi.h"
#include "mpi/c/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILING_DEFINES
#pragma weak MPI_File_write_at_all_begin = PMPI_File_write_at_all_begin
#endif

#if LAM_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, void *buf,
		                        int count, MPI_Datatype datatype) {
    return MPI_SUCCESS;
}
