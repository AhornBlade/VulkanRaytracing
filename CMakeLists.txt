cmake_minimum_required(VERSION 3.10)

project(VulkanRaytracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#third_party
add_subdirectory(third_party)

#examples
add_subdirectory(examples)