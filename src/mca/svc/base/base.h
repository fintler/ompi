/*
 * $HEADER$
 */

#ifndef MCA_SVC_BASE_H
#define MCA_SVC_BASE_H

#include "ompi_config.h"

#include "mca/mca.h"
#include "mca/svc/svc.h"


struct mca_svc_base_module_item_t {
  ompi_list_item_t super;
  mca_svc_base_component_t *svc_component;
  mca_svc_base_module_t *svc_module;
};
typedef struct mca_svc_base_module_item_t mca_svc_base_module_item_t;

OBJ_CLASS_DECLARATION(mca_svc_base_module_item_t);
                                                                                                                                

/*
 * Global functions for the SVC
 */

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif
  int mca_svc_base_open(void);
  int mca_svc_base_init(void);
  int mca_svc_base_close(void);
#if defined(c_plusplus) || defined(__cplusplus)
}
#endif


/*
 * Globals
 */
extern int mca_svc_base_output;
extern ompi_list_t mca_svc_base_components;
extern ompi_list_t mca_svc_base_modules;

#endif /* MCA_SVC_BASE_H */
