/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_SCAN = mpi_scan_f
#pragma weak pmpi_scan = mpi_scan_f
#pragma weak pmpi_scan_ = mpi_scan_f
#pragma weak pmpi_scan__ = mpi_scan_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_SCAN,
                           pmpi_scan,
                           pmpi_scan_,
                           pmpi_scan__,
                           pmpi_scan_f,
                           (char *sendbuf, char *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr),
                           (sendbuf, recvbuf, count, datatype, op, comm, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_SCAN = mpi_scan_f
#pragma weak mpi_scan = mpi_scan_f
#pragma weak mpi_scan_ = mpi_scan_f
#pragma weak mpi_scan__ = mpi_scan_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_SCAN,
                           mpi_scan,
                           mpi_scan_,
                           mpi_scan__,
                           mpi_scan_f,
                           (char *sendbuf, char *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr),
                           (sendbuf, recvbuf, count, datatype, op, comm, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/f77/profile/defines.h"
#endif

void mpi_scan_f(char *sendbuf, char *recvbuf, MPI_Fint *count,
		MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm,
		MPI_Fint *ierr)
{
    MPI_Comm c_comm;
    MPI_Datatype c_type;
    MPI_Op c_op;

    c_type = MPI_Type_f2c(*datatype);
    c_op = MPI_Op_f2c(*op);
    c_comm = MPI_Comm_f2c(*comm);

    *ierr = OMPI_INT_2_FINT(MPI_Scan(sendbuf, recvbuf,
				     OMPI_FINT_2_INT(*count),
				     c_type, c_op, 
				     c_comm));
}
