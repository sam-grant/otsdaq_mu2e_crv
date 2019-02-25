include_directories(${MPI_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR}/artdaq)
    
    
simple_plugin(ROCCosmicRayVetoInterface "interface"
	ROCCore
 )