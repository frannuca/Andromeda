# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(MKL_INCLUDE "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/mkl/include" )
set(MKL_LIB "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/mkl/lib/intel64_win" )
set(MKL_DLL "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/redist/intel64_win/mkl" )

set(ARMADILLO_INCLUDE "C:/armadillo-9.200.4/include/")
set(ARMADILLO_LIBS "C:/armadillo-9.200.4/examples/lib_win64/")

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc /std:c++17 /wd4244 /wd4267")
else()
    set (CMAKE_CXX_STANDARD 17)
endif()

add_subdirectory ("src")
add_subdirectory ("tests")

#C4244
