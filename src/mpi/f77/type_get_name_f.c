/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "include/constants.h"
#include "errhandler/errhandler.h"
#include "mpi/f77/bindings.h"
#include "mpi/f77/strings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_TYPE_GET_NAME = mpi_type_get_name_f
#pragma weak pmpi_type_get_name = mpi_type_get_name_f
#pragma weak pmpi_type_get_name_ = mpi_type_get_name_f
#pragma weak pmpi_type_get_name__ = mpi_type_get_name_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_TYPE_GET_NAME,
                           pmpi_type_get_name,
                           pmpi_type_get_name_,
                           pmpi_type_get_name__,
                           pmpi_type_get_name_f,
                           (MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr),
                           (type, type_name, resultlen, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_TYPE_GET_NAME = mpi_type_get_name_f
#pragma weak mpi_type_get_name = mpi_type_get_name_f
#pragma weak mpi_type_get_name_ = mpi_type_get_name_f
#pragma weak mpi_type_get_name__ = mpi_type_get_name_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_TYPE_GET_NAME,
                           mpi_type_get_name,
                           mpi_type_get_name_,
                           mpi_type_get_name__,
                           mpi_type_get_name_f,
                           (MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr),
                           (type, type_name, resultlen, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/f77/profile/defines.h"
#endif

OMPI_EXPORT
void mpi_type_get_name_f(MPI_Fint *type, char *type_name, MPI_Fint *resultlen, MPI_Fint *ierr)
{
    int err, c_len;
    MPI_Datatype c_type = MPI_Type_f2c(*type);
    char c_name[MPI_MAX_OBJECT_NAME];

    err = MPI_Type_get_name(c_type, c_name, &c_len);
    if (MPI_SUCCESS == err) {
        ompi_fortran_string_c2f(c_name, type_name, 
                                OMPI_FINT_2_INT(*resultlen));
        *resultlen = OMPI_INT_2_FINT(c_len);
        *ierr = OMPI_INT_2_FINT(MPI_SUCCESS);
    } else {
        *ierr = OMPI_INT_2_FINT(err);
    }
}
