/* -*- Mode: C; c-basic-offset:4 ; -*- */

#ifndef DATATYPE_INTERNAL_H_HAS_BEEN_INCLUDED
#define DATATYPE_INTERNAL_H_HAS_BEEN_INCLUDED

#include "util/output.h"

extern int ompi_ddt_dfd;

#if defined(VERBOSE)
#  define DUMP_STACK( PSTACK, STACK_POS, PDESC, NAME ) \
     ompi_ddt_dump_stack( (PSTACK), (STACK_POS), (PDESC), (NAME) )
#  if defined(ACCEPT_C99)
#    define DUMP( ARGS... )          ompi_output(ompi_ddt_dfd, __VA_ARGS__)
#  else
#    if defined(__GNUC__) && !defined(__STDC__)
#      define DUMP(ARGS...)          ompi_output( ompi_ddt_dfd, ARGS)
#  else
static inline void DUMP( char* fmt, ... )
{
   va_list list;

   va_start( list, fmt );
   ompi_output( ompi_ddt_dfd, fmt, list );
   va_end( list );
}
#      define DUMP                   printf
#    endif  /* __GNUC__ && !__STDC__ */
#  endif  /* ACCEPT_C99 */
#else
#  define DUMP_STACK( PSTACK, STACK_POS, PDESC, NAME )
#  if defined(ACCEPT_C99)
#    define DUMP(ARGS...)
#  else
#    if defined(__GNUC__) && !defined(__STDC__)
#      define DUMP(ARGS...)
#    else
       static inline void DUMP( char* fmt, ...) { /* empty hopefully removed by the compiler */ }
#    endif  /* __GNUC__ && !__STDC__ */
#  endif  /* ACCEPT_C99 */
#endif  /* VERBOSE */

#define DT_LOOP                    0x00
#define DT_END_LOOP                0x01
#define DT_LB                      0x02
#define DT_UB                      0x03
#define DT_CHAR                    0x04
#define DT_CHARACTER               0x05
#define DT_UNSIGNED_CHAR           0x06
#define DT_BYTE                    0x07
#define DT_SHORT                   0x08
#define DT_UNSIGNED_SHORT          0x09
#define DT_INT                     0x0A
#define DT_UNSIGNED_INT            0x0B
#define DT_LONG                    0x0C
#define DT_UNSIGNED_LONG           0x0D
#define DT_LONG_LONG               0x0E
#define DT_LONG_LONG_INT           0x0F
#define DT_UNSIGNED_LONG_LONG      0x10
#define DT_FLOAT                   0x11
#define DT_DOUBLE                  0x12
#define DT_LONG_DOUBLE             0x13
#define DT_COMPLEX_FLOAT           0x14
#define DT_COMPLEX_DOUBLE          0x15
#define DT_COMPLEX_LONG_DOUBLE     0x16
#define DT_PACKED                  0x17
#define DT_LOGIC                   0x18
#define DT_FLOAT_INT               0x19
#define DT_DOUBLE_INT              0x1A
#define DT_LONG_DOUBLE_INT         0x1B
#define DT_LONG_INT                0x1C
#define DT_2INT                    0x1D
#define DT_SHORT_INT               0x1E
#define DT_INTEGER                 0x1F
#define DT_REAL                    0x20
#define DT_DBLPREC                 0x21
#define DT_2REAL                   0x22
#define DT_2DBLPREC                0x23
#define DT_2INTEGER                0x24
#define DT_WCHAR                   0x25
#define DT_2COMPLEX                0x26
#define DT_2DOUBLE_COMPLEX         0x27
#define DT_CXX_BOOL                0x28
#define DT_UNAVAILABLE             0x29
/* If the number of basic datatype should change update
 * DT_MAX_PREDEFINED in datatype.h
 */
#if DT_MAX_PREDEFINED <= DT_UNAVAILABLE
#error DT_MAX_PREDEFINED should be updated
#endif  /* safe check for max predefined datatypes. */

#define DT_INCREASE_STACK  32

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
struct __dt_stack {
    int32_t index;    /**< index in the element description */
    int32_t count;    /**< number of times we still have to do it */
    int32_t end_loop; /**< for loops the end of the loop, otherwise useless */
    long    disp;     /**< actual displacement depending on the count field */
};

/* These 2 typedefs are the same as the dt_elem_desc_t except
 * for the name of the fields.
 */
typedef struct __dt_loop_desc {
    u_int16_t flags;  /**< flags for the record */
    u_int16_t type;   /**< the basic data type id */
    u_int32_t loops;  /**< number of times the loop have to be done */
    long      items;  /**< number of items in the loop */
    u_int32_t extent; /**< extent of the whole loop */
} dt_loop_desc_t;

typedef struct __dt_endloop_desc {
    u_int16_t flags;        /**< flags for the record */
    u_int16_t type;         /**< the basic data type id */
    u_int32_t items;        /**< number of items in the loop */
    long      total_extent; /**< total extent of the loop taking in account the repetitions */
    u_int32_t size;         /**< real size of the data in the loop */
} dt_endloop_desc_t;

/* keep the last 16 bits free for data flags */
#define CONVERTOR_USELESS          0x00010000
#define CONVERTOR_RECV             0x00020000
#define CONVERTOR_SEND             0x00040000
#define CONVERTOR_HOMOGENEOUS      0x00080000
#define CONVERTOR_STATE_MASK       0xFF000000
#define CONVERTOR_STATE_START      0x01000000
#define CONVEROTR_STATE_COMPLETE   0x02000000
#define CONVERTOR_STATE_ALLOC      0x04000000

typedef struct {
    float r;
    float i;
} ompi_complex_float_t;

typedef struct {
    double r;
    double i;
} ompi_complex_double_t;

typedef struct {
    long double r;
    long double i;
} ompi_complex_long_double_t;

extern ompi_datatype_t* ompi_ddt_basicDatatypes[];

/* macros to play with the flags */
#define REMOVE_FLAG( INT_VALUE, FLAG )  (INT_VALUE) = (INT_VALUE) ^ (FLAG)
#define SET_FLAG( INT_VALUE, FLAG )     (INT_VALUE) = (INT_VALUE) | (FLAG)
#define UNSET_FLAG( INT_VALUE, FLAG)    (INT_VALUE) = (INT_VALUE) & (~(FLAG))

#define REMOVE_CONTIGUOUS_FLAG( INT_VALUE )  REMOVE_FLAG(INT_VALUE, DT_FLAG_CONTIGUOUS)
#define SET_CONTIGUOUS_FLAG( INT_VALUE )     SET_FLAG(INT_VALUE, DT_FLAG_CONTIGUOUS)
#define UNSET_CONTIGUOUS_FLAG( INT_VALUE )   UNSET_FLAG(INT_VALUE, DT_FLAG_CONTIGUOUS)

#if defined(__GNUC__) && !defined(__STDC__)
#define LMAX(A,B)  ({ long _a = (A), _b = (B); (_a < _b ? _b : _a) })
#define LMIN(A,B)  ({ long _a = (A), _b = (B); (_a < _b ? _a : _b); })
#define IMAX(A,B)  ({ int _a = (A), _b = (B); (_a < _b ? _b : _a); })
#define IMIN(A,B)  ({ int _a = (A), _b = (B); (_a < _b ? _a : _b); })
#else
static inline long LMAX( long a, long b ) { return ( a < b ? b : a ); }
static inline long LMIN( long a, long b ) { return ( a < b ? a : b ); }
static inline int  IMAX( int a, int b ) { return ( a < b ? b : a ); }
static inline int  IMIN( int a, int b ) { return ( a < b ? a : b ); }
#endif  /* __GNU__ */

extern conversion_fct_t ompi_ddt_copy_functions[DT_MAX_PREDEFINED];

extern void ompi_ddt_dump_stack( dt_stack_t* pStack, int stack_pos, dt_elem_desc_t* pDesc, char* name );
#define SAVE_STACK( PSTACK, INDEX, COUNT, DISP, END_LOOP) \
do { \
   (PSTACK)->index    = (INDEX); \
   (PSTACK)->count    = (COUNT); \
   (PSTACK)->disp     = (DISP); \
   (PSTACK)->end_loop = (END_LOOP); \
} while(0)

#define PUSH_STACK( PSTACK, STACK_POS, INDEX, COUNT, DISP, END_LOOP) \
do { \
   dt_stack_t* pTempStack = (PSTACK) + 1; \
   SAVE_STACK( pTempStack, (INDEX), (COUNT), (DISP), (END_LOOP) );  \
   (STACK_POS)++; \
   (PSTACK) = pTempStack; \
} while(0)

#define MEMCPY( DST, SRC, BLENGTH ) { \
    /*printf( "memcpy dest = %p src = %p length = %d\n", (void*)(DST), (void*)(SRC), (int)(BLENGTH) );*/ \
    memcpy( (DST), (SRC), (BLENGTH) ); }

#define OMPI_DDT_SAFEGUARD_POINTER( ACTPTR, LENGTH, INITPTR, PDATA, COUNT ) \
    ompi_ddt_safeguard_pointer( (ACTPTR), (LENGTH), (INITPTR), (PDATA), (COUNT) )

static inline void ompi_ddt_safeguard_pointer( void* actual_ptr, int length,
                                               void* initial_ptr,
                                               ompi_datatype_t* pData,
                                               int count )
{
    char* lower_bound = (char*)initial_ptr;
    char* upper_bound = (char*)initial_ptr;

    if( (length == 0) || (count == 0) ) return;
    lower_bound += pData->lb;
    upper_bound += pData->ub * (count - 1) + pData->true_ub;

    if( (char*)actual_ptr >= lower_bound )
        /* Im up from the lower bound */
        if( ((char*)actual_ptr + length) <= upper_bound )
            return;
    printf( "Pointer %p size %d is outside [%p,%p] for data \n", actual_ptr, length,
            lower_bound, upper_bound );
    ompi_ddt_dump( pData );
}

#ifdef USELESS
#define MEMCPY_LIMIT 1

#define MEMCPY( DST, SRC, BLENGTH ) \
do { \
   if( (BLENGTH) < (MEMCPY_LIMIT) ) { \
      long mask = sizeof(int) - 1; \
      char *dst = (char*)(DST), *src = (char*)(SRC); \
      int i; \
      if( ((long)(DST) & mask) == ((long)(SRC) & mask) ) { \
         int *idst = (int*)((long)(DST) & (~mask)); \
         int *isrc = (int*)((long)(SRC) & (~mask)); \
         for( i = 0; i < ((long)(DST) & mask); i++ ) { \
            *dst = *src; dst++; src++; \
         } \
         if( ((char*)idst) != dst ) { \
            idst++; isrc++; \
         } \
         for( i = 0; i < ((BLENGTH) >> 2); i++ ) { \
            *idst = *isrc; idst++; isrc++; \
         } \
      } else { \
         for( i = 0; i < (BLENGTH); i++ ) { \
            *dst = *src; dst++; src++; \
         } \
      } \
   } else \
      memcpy( (DST), (SRC), (BLENGTH) ); \
} while(0)
#endif  /* USELESS */

static inline int GET_FIRST_NON_LOOP( dt_elem_desc_t* _pElem )
{
    int index = 0;

    /* We dont have to check for the end as we always put an END_LOOP
     * at the end of all datatype descriptions.
     */
    while( _pElem->type == DT_LOOP ) {
        ++_pElem; index++;
    }
    return index;
}

static inline
int ompi_convertor_create_stack_at_begining( ompi_convertor_t* pConvertor, int* sizes )
{
    ompi_datatype_t* pData = pConvertor->pDesc;
    dt_elem_desc_t* pElems;
    int index;

    pConvertor->stack_pos = 1;
    /* Fill the first position on the stack. This one correspond to the
     * last fake DT_END_LOOP that we add to the data representation and
     * allow us to move quickly inside the datatype when we have a count.
     */
    pConvertor->pStack[0].index = -1;
    pConvertor->pStack[0].count = pConvertor->count;
    /* first here we should select which data representation will be used for
     * this operation: normal one or the optimized version ? */
    pElems = pData->desc.desc;
    pConvertor->pStack[0].end_loop = pData->desc.used;
    if( pConvertor->flags & CONVERTOR_HOMOGENEOUS ) {
        if( pData->opt_desc.used > 0 ) {
            pElems = pData->opt_desc.desc;
            pConvertor->pStack[0].end_loop = pData->opt_desc.used;
        }
    }
    index = GET_FIRST_NON_LOOP(pData->desc.desc);
    pConvertor->pStack[0].disp     = pElems[index].disp;
    pConvertor->pStack[1].index    = 0;
    pConvertor->pStack[1].count    = pElems->count;
    pConvertor->pStack[1].disp     = pConvertor->pStack[0].disp;
    pConvertor->pStack[1].end_loop = pConvertor->pStack[0].end_loop;
    /* And set the correct status */
    pConvertor->converted          = 0;
    pConvertor->bConverted         = 0;
    return OMPI_SUCCESS;
}
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif
#endif  /* DATATYPE_INTERNAL_H_HAS_BEEN_INCLUDED */
