cmake_minimum_required(VERSION 3.1)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(libate)
add_library(libate INTERFACE)

set(${PROJECT_NAME}_EXECUTABLES
	ticktock
	)

set(${PROJECT_NAME}_HEADERS
	timer
	)

foreach(executable ${${PROJECT_NAME}_EXECUTABLES})
	add_executable(${executable} ${executable}.cc)
	install(TARGETS ${executable} RUNTIME DESTINATION bin)
endforeach(executable)

foreach(header ${${PROJECT_NAME}_HEADERS})
	install(FILES ${header}.h DESTINATION "include/late")
endforeach(header)
