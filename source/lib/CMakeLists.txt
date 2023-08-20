project(${libname})

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
	${X_LIBRARIES}
	${PLATFORM_LIBRARIES}
	${cpp}
	)
set(shared_lib_dependency_public)

aux_source_directory(./mahjong/            LIB_SOURCE)

# static lib only
add_library(${libname} ${LIB_SOURCE})
set_target_properties(${libname} PROPERTIES VERSION 1.0)
target_link_libraries(${libname} PRIVATE ${shared_lib_dependency_private})
target_link_libraries(${libname} PUBLIC ${shared_lib_dependency_public})

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/../include/mahjong
	DESTINATION include
	FILES_MATCHING REGEX ".+\.(h|hpp)"
	)

install(TARGETS
	${libname}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	)

