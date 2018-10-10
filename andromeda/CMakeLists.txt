# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++17")
endif (WIN32)

if (UNIX)
    set (CMAKE_CXX_STANDARD 17)
endif (UNIX)



add_subdirectory ("src")
add_subdirectory("tests")
