add_executable(triangle main.cpp)
target_link_libraries(triangle 
	PRIVATE VulkanRaytracing)

GENERATE_SHADERS(${CMAKE_CURRENT_SOURCE_DIR}/shaders triangle)