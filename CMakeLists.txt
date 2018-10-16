cmake_minimum_required(VERSION 2.6)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(MKL_INCLUDE "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/mkl/include" )
set(MKL_LIB "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/mkl/lib/intel64_win" )
set(MKL_DLL "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2019.0.117/windows/redist/intel64_win/mkl" )

set(ARMADILLO_INCLUDE "C:/Users/fran/source/github/armadillo-9.100.5/include")
set(ARMADILLO_LIBS "C:/Users/fran/source/github/armadillo-9.100.5/examples/lib_win64/")

add_subdirectory("andromeda")
