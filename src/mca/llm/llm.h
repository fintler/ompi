/* -*- C -*-
 *
 * $HEADER$
 */
/**
 * @file
 *
 * Location List Manager (LLM)
 *
 * The Location List Manager (LLM) is an MCA component that provides
 * resource allocation for the Process Control Manager (PCM).  The LLM
 * is intended to be used by the PCM and should not be used outside of
 * the PCM framework.
 *
 */

#ifndef MCA_LLM_H
#define MCA_LLM_H

#include "ompi_config.h"
#include "mca/mca.h"
#include "class/ompi_list.h"
#include "runtime/runtime_types.h"

/*
 * MCA component management functions
 */

/**
 * LLM initialization function
 *
 * Called by the MCA framework to initialize the component.  Will
 * be called exactly once in the lifetime of the process.
 *
 * @param active_pcm (IN) Name of the currently active PCM module,
 *                       as it might be useful in determining
 *                       useability.
 * @param priority (OUT) Relative priority or ranking use by MCA to
 *                       select a module.
 * @param allow_multiple_user_threads (OUT) Whether this module can
 *                       run with multiple threads making calls into
 *                       the library (equivalent of MPI_THREAD_MULTIPLE 
 *                       from MPI-land).
 * @param have_hidden_threads (OUT) Whether this module needs to start
 *                       a background thread for operation.
 */
typedef struct mca_llm_base_module_1_0_0_t* 
(*mca_llm_base_component_init_fn_t)(const char *active_pcm,
                                    int *priority, 
                                    bool *allow_multiple_user_threads,
                                    bool *have_hidden_threads);


/**
 * LLM finalization function
 *
 * Called by the MCA framework to finalize the component.  Will be
 * called once per successful call to llm_base_compoenent_init.
 */
typedef int (*mca_llm_base_component_finalize_fn_t)(void);


/** 
 * LLM module version and interface functions
 *
 * \note the first two entries have type names that are a bit
 *  misleading.  The plan is to rename the mca_base_module_*
 * types in the future.
 */
struct mca_llm_base_component_1_0_0_t {
  /** component version */
  mca_base_component_t llm_version;
  /** component data */
  mca_base_component_data_1_0_0_t llm_data;
  /** Function called when component is initialized  */
  mca_llm_base_component_init_fn_t llm_init;
  /** Function called when component is finalized */
  mca_llm_base_component_finalize_fn_t llm_finalize;
};
/** shorten mca_llm_base_component_1_0_0_t declaration */
typedef struct mca_llm_base_component_1_0_0_t mca_llm_base_component_1_0_0_t;
/** shorten mca_llm_base_component_t declaration */
typedef mca_llm_base_component_1_0_0_t mca_llm_base_component_t;


/*
 * LLM interface functions
 */

/**
 * Allocate requested resources
 *
 * Allocate the specified nodes / processes for use in a new job.
 * Requires a jobid from the PCM interface.  The allocation returned
 * may be smaller than requested - it is up to the caller to proceed
 * as appropriate should this occur.  This function should only be
 * called once per jobid.
 *
 * @param jobid (IN) Jobid with which to associate the given resources.
 * @param nodes (IN) Number of ndoes to try to allocate.  If 0, the
 *                   allocator will try to allocate \c procs processes
 *                   on as many nodes as are needed.  If non-zero, 
 *                   will try to allocate \c procs process slots 
 *                   per node.
 * @param procs (IN) Number of processors to try to allocate.  See the note
 *                   for <code>nodes</code> for usage.
 * @param nodelist (OUT) List of <code>ompi_rte_node_allocation_t</code>s
 *                   describing the allocated resources.
 *
 */
typedef ompi_list_t *
(*mca_llm_base_allocate_resources_fn_t)(mca_ns_base_jobid_t jobid, 
                                        int nodes,int procs);


/**
 * Deallocate requested resources
 *
 * Return the resources for the given jobid to the system.
 *
 * @param jobid (IN) Jobid associated with the resources to be freed.
 * @param nodes (IN) Nodelist from associated allocate_resource call.
 *                   All associated memory will be freed as appropriate.
 */
typedef int (*mca_llm_base_deallocate_resources_fn_t)(mca_ns_base_jobid_t jobid,
                                                      ompi_list_t *nodelist);


/**
 * Base module structure for the LLM
 *
 * Base module structure for the LLM - presents the required function
 * pointers to the calling interface. 
 */
struct mca_llm_base_module_1_0_0_t {
    /** Function to be called on resource request */
    mca_llm_base_allocate_resources_fn_t llm_allocate_resources;
    /** Function to be called on resource return */ 
    mca_llm_base_deallocate_resources_fn_t llm_deallocate_resources;
};
/** shorten mca_llm_base_module_1_0_0_t declaration */
typedef struct mca_llm_base_module_1_0_0_t mca_llm_base_module_1_0_0_t;
/** shorten mca_llm_base_module_t declaration */
typedef struct mca_llm_base_module_1_0_0_t mca_llm_base_module_t;


/**
 * Macro for use in modules that are of type pml v1.0.0
 */
#define MCA_LLM_BASE_VERSION_1_0_0 \
  /* llm v1.0 is chained to MCA v1.0 */ \
  MCA_BASE_VERSION_1_0_0, \
  /* llm v1.0 */ \
  "llm", 1, 0, 0

#endif /* MCA_LLM_H */
