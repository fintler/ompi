/*
 * $HEADER$
 */

extern const mca_base_component_t mca_allocator_bucket_component;
extern const mca_base_component_t mca_allocator_basic_component;

const mca_base_component_t *mca_allocator_base_static_components[] = {
  &mca_allocator_bucket_component, 
  &mca_allocator_basic_component, 
  NULL
};
