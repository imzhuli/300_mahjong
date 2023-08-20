project(samples)

# dependencies:

# enable specific macors by system type:
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	find_package(glfw3 CONFIG REQUIRED)
	set(PLATFORM_LIBRARIES)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(PLATFORM_LIBRARIES)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	find_library(COCOA_LIBRARY Cocoa REQUIRED)
	find_library(IOKIT_LIBRARY IOKit REQUIRED)
	set(PLATFORM_LIBRARIES ${IOKIT_LIBRARY} ${COCOA_LIBRARY})
endif()

# local dependencies
include_directories(
	../include/
	${X_INCLUDE_DIRS}
	)
link_directories(
	${X_LIBRARY_DIRS}
	)

set(shared_lib_dependency_private
	glfw3
	${X_LIBRARIES}
	${PLATFORM_LIBRARIES}
	${cpp}
	)
set(shared_lib_dependency_public)

function(add_sub exec_name sub_dir_name)
	aux_source_directory(${sub_dir_name} LOCAL_SOURCE_CODE)
	add_executable(${exec_name}  ${LOCAL_SOURCE_CODE})
	target_link_libraries(${exec_name} PRIVATE ${libname})
	target_link_libraries(${exec_name} PRIVATE ${shared_lib_dependency_private})
endfunction()

add_sub(01 01_test_9)
