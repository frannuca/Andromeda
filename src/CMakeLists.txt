project(andromeda)
add_definitions(-DEXPMODULE)
message(STATUS "..................................." ${CMAKE_SOURCE_DIR}/inc/)
file(GLOB SOURCES 
				${CMAKE_SOURCE_DIR}/src/*.cpp 
				${CMAKE_SOURCE_DIR}/src/qtime/*.cpp
				${CMAKE_SOURCE_DIR}/src/yield/*.cpp
				${CMAKE_SOURCE_DIR}/src/instrument/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/inc/libdef.h
			      ${CMAKE_SOURCE_DIR}/src/*.h 
				  ${CMAKE_SOURCE_DIR}/src/qtime/*.h
				  ${CMAKE_SOURCE_DIR}/src/yield/*.h
				  ${CMAKE_SOURCE_DIR}/src/instrument/*.h)

message(STATUS "///////////CMAKE_SOURCE_DIR=" ${CMAKE_SOURCE_DIR})
message(STATUS "SOURCES")
foreach(testSrc ${SOURCES})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)
		message(STATUS "->" ${testName}.cpp)
endforeach(testSrc)
message(STATUS "HEADERS")
foreach(testSrc ${HEADERS})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)
		message(STATUS "->" ${testName}.h)
endforeach(testSrc)

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

find_package(Boost 1.68 COMPONENTS system filesystem  date_time REQUIRED)

if(NOT Boost_FOUND)
	error("BOOST LIBS not found")
endif()

message(STATUS "Boost_INCLUDE_DIRS")
message(STATUS ${Boost_INCLUDE_DIRS})

message(STATUS "Boost_LIBRARIES")
message(STATUS ${Boost_LIBRARIES})

message(STATUS "Boost_LIBRARY_DIRS")
message(STATUS ${Boost_LIBRARY_DIRS})

include_directories( ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/inc/  ${CMAKE_SOURCE_DIR}/src/ ${CMAKE_SOURCE_DIR}/inc/  ${ARMADILLO_INCLUDE} )

link_directories(${Boost_LIBRARY_DIRS} ${MKL_LIB})


add_library(
    andromeda
	STATIC	
        ${SOURCES} 
        ${HEADERS}
)
target_link_libraries(andromeda ${Boost_LIBRARIES} mkl_intel_ilp64_dll.lib mkl_sequential_dll.lib mkl_core_dll.lib )
message(STATUS "----------------------> andromeda lib imported into " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/andromeda.lib)
