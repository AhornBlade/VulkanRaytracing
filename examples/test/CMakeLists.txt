add_executable(test main.cpp)
target_link_libraries(test
	PUBLIC Vulkan::Headers
	PUBLIC glm
	PUBLIC glfw)