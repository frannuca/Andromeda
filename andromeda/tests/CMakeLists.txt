project(tests)
add_definitions(-DARMA_USE_BLAS)
add_definitions(-DARMA_USE_LAPACK)

file(GLOB SOURCES  ${CMAKE_SOURCE_DIR}/andromeda/tests/*.cpp )
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/andromeda/inc/libdef.h
			      ${CMAKE_SOURCE_DIR}/andromeda/src/*.h 
				  ${CMAKE_SOURCE_DIR}/andromeda/src/qtime/*.h
				   ${CMAKE_SOURCE_DIR}/andromeda/src/yield/*.h
				   ${CMAKE_SOURCE_DIR}/andromeda/src/instrument/*.h)

message(STATUS "/////////////////////////" ${SOURCES})
message(STATUS "/////////////////////////" ${HEADERS})

if(WIN32)
	message(STATUS "------------------:configuring windows")
	set(BOOST_LIBRARYDIR "C:/Boost/lib")
	set(BOOST_ROOT "C:/Boost/include/boost-1_68")
elseif (UNIX && APPLE)
	message(STATUS "------------------:configuring apple")
	set(BOOST_LIBRARYDIR "/usr/local/lib")
	set(BOOST_ROOT "/usr/local/include")
elseif(UNIX)
	message(STATUS "------------------:configuring unix")
	set(BOOST_LIBRARYDIR "/opt/dev/lib")
	set(BOOST_ROOT "/opt/dev/include")
else()
	error("OS not detected")
endif()



set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

#Setup CMake to run tests
enable_testing()

find_package(Boost 1.68 COMPONENTS system filesystem unit_test_framework date_time REQUIRED)

if(NOT Boost_FOUND)
	error("BOOST LIBS not found")
endif()

message(STATUS "Boost_INCLUDE_DIRS")
message(STATUS ${Boost_INCLUDE_DIRS})

message(STATUS "Boost_LIBRARIES")
message(STATUS ${Boost_LIBRARIES})

message(STATUS "Boost_LIBRARY_DIRS")
message(STATUS ${Boost_LIBRARY_DIRS})

include_directories( ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/andromeda/inc ${ARMADILLO_INCLUDE} ${MKL_INCLUDE})
link_directories(${Boost_LIBRARY_DIRS} ${MKL_LIB} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}  ${ARMADILLO_LIBS})


foreach(testSrc ${SOURCES})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)
		
	
        #Add compile target
        add_executable(${testName} ${testSrc})
		
        #link to Boost libraries AND your targets and dependencies
        #target_link_libraries(${testName} ${Boost_LIBRARIES} mkl_intel_ilp64_dll.lib mkl_sequential_dll.lib mkl_core_dll.lib blas_win64_MT.lib lapack_win64_MT.lib andromeda)
   	    target_link_libraries(${testName} ${Boost_LIBRARIES} blas_win64_MT.lib lapack_win64_MT.lib andromeda)
        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                 WORKING_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
                 COMMAND ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${testName} )

	#	add_custom_command(TARGET ${testName} POST_BUILD        # Adds a post-build event to MyTest
	#		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
    #    "${MKL_DLL}/mkl_sequential.dll"      # <--this is in-file
    #    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})                 # <--this is out-file path
	#
	#	
	#
	#	add_custom_command(TARGET ${testName} POST_BUILD        # Adds a post-build event to MyTest
	#		COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
    #    "${MKL_DLL}/mkl_core.dll"      # <--this is in-file
    #    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})                 # <--this is out-file path

		add_custom_command(TARGET ${testName} POST_BUILD        # Adds a post-build event to MyTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${ARMADILLO_LIBS}/lapack_win64_MT.dll"      # <--this is in-file
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})                 # <--this is out-file path

		add_custom_command(TARGET ${testName} POST_BUILD        # Adds a post-build event to MyTest
			COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${ARMADILLO_LIBS}/blas_win64_MT.dll"      # <--this is in-file
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})                 # <--this is out-file path


		message(STATUS "------------------------------>TEST ADDED")

endforeach(testSrc)
