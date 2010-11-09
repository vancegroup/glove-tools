# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6.2)

# Set package properties
project(glove-tools)

set(CPACK_PACKAGE_VENDOR "Iowa State University")
set(CPACK_PACKAGE_CONTACT "Ryan Pavlik <rpavlik@iastate.edu>")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION
	"${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-src")

###
# Set up options
###

option(BUILD_WITH_GLOVE5DT "Build with a 5DT Glove driver?" off)

set(BIN_DIR bin)
set(INCLUDE_DIR include)
set(ARCH_DIR lib)
set(DATA_DIR share/glove-tools)
if(WIN32)
	set(LIB_DIR bin)
	set(EXPORT_DIR cmake)
else()
	set(LIB_DIR lib)
	set(EXPORT_DIR ${LIB_DIR}/glove-tools)
endif()

###
# End options
###

###
# Perform build configuration of dependencies
###

# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)
include(CppcheckTargets)
include(DoxygenTargets)
include(GetDirectoryList)
include(EnableExtraCompilerWarnings)

include(CTest)
include(CreateDashboardScripts)

set(EXTRA_LIBS)
include(SearchProgramFilesForOpenSceneGraph)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil)
list(APPEND EXTRA_LIBS ${OPENSCENEGRAPH_LIBRARIES})
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

get_directory_list(OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_LIBRARIES})
list(APPEND RUNTIME_LIBRARY_DIRS ${OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS})
if(WIN32)
    foreach(_dir ${OPENSCENEGRAPH_RUNTIME_LIBRARY_DIRS})
        list(APPEND RUNTIME_LIBRARY_DIRS ${_dir}/../bin)
    endforeach()
endif()

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Glove5DT)
if(BUILD_WITH_GLOVE5DT)
	include_directories(${GLOVE5DT_INCLUDE_DIRS})
	list(APPEND EXTRA_LIBS ${GLOVE5DT_LIBRARIES})
endif()

###
# End build configuration
###

###
# Build the project
###

set(CMAKE_DEBUG_POSTFIX  "_d")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(glove-tools)

add_subdirectory(applications)

add_doxygen(Doxyfile)

if(BUILD_TESTING)
	include(BoostTestTargets)
	add_subdirectory(tests)
endif()

create_dashboard_scripts("DashboardBuildInitialCache.cmake.in")

include(CreateLaunchers)
create_default_target_launcher(dummyHand
	FORWARD_ARGS
	RUNTIME_LIBRARY_DIRS
	${RUNTIME_LIBRARY_DIRS})

###
# Create config files to allow easy use of this library
###

# Set up use from build tree
set(BUILD_TREE_TARGETS glovetools)

configure_file(glovetools-config-build-tree.cmake.in glovetoolsConfig.cmake @ONLY)
export(TARGETS ${BUILD_TREE_TARGETS}
	NAMESPACE glovetools_imported_
	FILE glovetools-targets.cmake)
export(PACKAGE glovetools)

# Set up use from install tree
install(EXPORT glovetools-sdk
	DESTINATION ${EXPORT_DIR}
	NAMESPACE glovetools_imported_
	FILE glovetools-targets.cmake)
configure_file(glovetools-config.cmake.in glovetools-config-for-install.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/glovetools-config-for-install.cmake
	RENAME glovetools-config.cmake
	DESTINATION ${EXPORT_DIR})

install(FILES hand-structured.osg
	DESTINATION ${DATA_DIR})

###
# Set packaging options (for CPack)
###

# Choose desired package generators
if(APPLE)
	set(CPACK_GENERATOR DragNDrop)
	set(CPACK_SOURCE_GENERATOR ZIP)
elseif(WIN32)
	set(CPACK_SOURCE_GENERATOR ZIP)
else()
	set(CPACK_SOURCE_GENERATOR TARGZ)
endif()

# Include the packaging system now that we have it all set up
include(CPack)

###
# End Packaging
###
