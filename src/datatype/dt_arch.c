#include "dt_arch.h"

int32_t ompi_arch_compute_local_id( uint32_t *me )
{
    ompi_arch_create_empty_id( me );

    /* Handle the size of long (can hold a pointer) */
    if( 8 == sizeof(long) )
        ompi_arch_setmask( me, OMPI_ARCH_LONGIS64 );

    /* Initialize the information regarding the long double */
    if( 12 == sizeof(long double) )
        ompi_arch_setmask( me, OMPI_ARCH_LONGDOUBLEIS96 );
    else if( 16 == sizeof(long double) )
        ompi_arch_setmask( me, OMPI_ARCH_LONGDOUBLEIS128 );

    /* Big endian or little endian ? That's the question */
    if( ompi_arch_isbigendian() )
        ompi_arch_setmask( me, OMPI_ARCH_ISBIGENDIAN );

    /* What's the maximum exponent ? */
    if ( LDBL_MAX_EXP == 16384 )
        ompi_arch_setmask( me, OMPI_ARCH_LDEXPSIZEIS15 );

    /* How about the length in bits of the mantissa */
    if ( LDBL_MANT_DIG == 64 )
        ompi_arch_setmask( me, OMPI_ARCH_LDMANTDIGIS64 );
    else if ( LDBL_MANT_DIG == 105 )
        ompi_arch_setmask( me, OMPI_ARCH_LDMANTDIGIS105 );
    else if ( LDBL_MANT_DIG == 106 )
        ompi_arch_setmask( me, OMPI_ARCH_LDMANTDIGIS106 );
    else if ( LDBL_MANT_DIG == 107 )
        ompi_arch_setmask( me, OMPI_ARCH_LDMANTDIGIS107 );
    else if ( LDBL_MANT_DIG == 113 )
        ompi_arch_setmask( me, OMPI_ARCH_LDMANTDIGIS113 );

    /* Intel data representation or Sparc ? */
    if( ompi_arch_ldisintel() )
        ompi_arch_setmask( me, OMPI_ARCH_LDISINTEL );

    return OMPI_SUCCESS;
}

int32_t ompi_arch_checkmask ( uint32_t *var, uint32_t mask )
{
    unsigned int tmpvar = *var;

    /* Check whether the headers are set correctly,
       or whether this is an erroneous integer */
    if( !((*var) & OMPI_ARCH_HEADERMASK) ) {
        if( (*var) & OMPI_ARCH_HEADERMASK2 ) {
            char* pcDest, *pcSrc;
            /* Both ends of this integer have the wrong settings,
               maybe its just the wrong endian-representation. Try
               to swap it and check again. If it looks now correct,
               keep this version of the variable
            */

            pcDest = (char *) &tmpvar;
            pcSrc  = (char *) var + 3;
            *pcDest++ = *pcSrc--;
            *pcDest++ = *pcSrc--;
            *pcDest++ = *pcSrc--;
            *pcDest++ = *pcSrc--;

            if( (tmpvar & OMPI_ARCH_HEADERMASK) && (!(tmpvar & OMPI_ARCH_HEADERMASK2)) ) {
                *var = tmpvar;
            } else
                return -1;
        } else
            return -1;
    }

    /* Here is the real evaluation of the bitmask */
    return ( ((*var) & mask) == mask );
}
