

CCL_NAMESPACE_BEGIN


ccl_device void kernel_subsurface_scatter(KernelGlobals *kg)
{
#ifdef __SUBSURFACE__

	ccl_local unsigned int local_queue_atomics;
	if(ccl_local_id(0) == 0 && ccl_local_id(1) == 0) {
		local_queue_atomics = 0;
	}
	ccl_barrier(CCL_LOCAL_MEM_FENCE);

	int ray_index = ccl_global_id(1) * ccl_global_size(0) + ccl_global_id(0);
	ray_index = get_ray_index(kg, ray_index,
	                          QUEUE_ACTIVE_AND_REGENERATED_RAYS,
	                          kernel_split_state.queue_data,
	                          kernel_split_params.queue_size,
	                          0);

#ifdef __COMPUTE_DEVICE_GPU__
	/* If we are executing on a GPU device, we exit all threads that are not
	 * required.
	 *
	 * If we are executing on a CPU device, then we need to keep all threads
	 * active since we have barrier() calls later in the kernel. CPU devices,
	 * expect all threads to execute barrier statement.
	 */
	if(ray_index == QUEUE_EMPTY_SLOT) {
		return;
	}
#endif

#ifndef __COMPUTE_DEVICE_GPU__
	if(ray_index != QUEUE_EMPTY_SLOT) {
#endif


	char enqueue_flag = 0;
	ccl_global char *ray_state = kernel_split_state.ray_state;
	ccl_global PathState *state = &kernel_split_state.path_state[ray_index];
	PathRadiance *L = &kernel_split_state.path_radiance[ray_index];
	ccl_global RNG *rng = &kernel_split_state.rng[ray_index];
	ccl_global Ray *ray = &kernel_split_state.ray[ray_index];
	ccl_global float3 *throughput = &kernel_split_state.throughput[ray_index];
	ccl_global SubsurfaceIndirectRays *ss_indirect = &kernel_split_state.ss_rays[ray_index];
	ShaderData *sd = &kernel_split_state.sd[ray_index];
	ShaderData *emission_sd = &kernel_split_state.sd_DL_shadow[ray_index];

	if(IS_STATE(ray_state, ray_index, RAY_ACTIVE)) {
		if(sd->flag & SD_BSSRDF) {
			if(kernel_path_subsurface_scatter(kg,
			                                  sd,
			                                  emission_sd,
			                                  L,
			                                  state,
			                                  rng,
			                                  ray,
			                                  throughput,
			                                  ss_indirect)) {
				ASSIGN_RAY_STATE(ray_state, ray_index, RAY_UPDATE_BUFFER);
				enqueue_flag = 1;
			}
		}
	}

#ifndef __COMPUTE_DEVICE_GPU__
	}
#endif

	/* Enqueue RAY_UPDATE_BUFFER rays. */
	enqueue_ray_index_local(ray_index,
	                        QUEUE_HITBG_BUFF_UPDATE_TOREGEN_RAYS,
	                        enqueue_flag,
	                        kernel_split_params.queue_size,
	                        &local_queue_atomics,
	                        kernel_split_state.queue_data,
	                        kernel_split_params.queue_index);

#endif  /* __SUBSURFACE__ */

}

CCL_NAMESPACE_END
