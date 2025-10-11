



#define LENPARTYACUDA_BUILD

#include "Public/LenpartyaSimulationCUDA/icosphere.h"
#include <cuda_runtime.h>




__global__ void kernel_compute_3d_coordinate(Coord3D* arrOut)
{


	return;
}

LENPARTYACUDA_EXPORT void compute_3d_coordinate(SimulationData* data)
{
	// grid and block dimensions
	dim3 blocks(1,1);
	dim3 threads(1,1);

	
	kernel_compute_3d_coordinate <<< blocks, threads >>> ();
	//return 0;
}

LENPARTYACUDA_EXPORT SimulationData* generate_simulation_struct(
	uint32_t NUM_DIVISIONS, double RADIUS
)
{
	SimulationData* ret;
	ret = new SimulationData(NUM_DIVISIONS, RADIUS);
	return ret;
}