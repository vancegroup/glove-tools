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

set(BIN_DIR bin)
set(INCLUDE_DIR include)
set(ARCH_DIR lib)
if(WIN32)
	set(LIB_DIR bin)
	set(EXPORT_DIR cmake)
else()
	set(LIB_DIR lib)
	set(EXPORT_DIR ${LIB_DIR}/5DTGlove)
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
include(StampSourcesWithVersion)

include(CTest)
include(CreateDashboardScripts)

set(EXTRA_LIBS)
include(SearchProgramFilesForOpenSceneGraph)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil)
list(APPEND EXTRA_LIBS ${OPENSCENEGRAPH_LIBRARIES})


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

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

###
# Create config files to allow easy use of this library
###

# Set up use from build tree
set(BUILD_TREE_TARGETS 5DTGlove)

configure_file(vpstools-config-build-tree.cmake.in VPSToolsConfig.cmake @ONLY)
export(TARGETS ${BUILD_TREE_TARGETS}
	NAMESPACE vpstools_buildtree_
	FILE vpstools-targets.cmake)
export(PACKAGE VPSTools)

# Set up use from install tree
install(EXPORT vpstools-sdk
	DESTINATION ${EXPORT_DIR}
	NAMESPACE vpstools_exported_
	FILE vpstools-targets.cmake)
configure_file(vpstools-config.cmake.in vpstools-config-for-install.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/vpstools-config-for-install.cmake
	RENAME vpstools-config.cmake
	DESTINATION ${EXPORT_DIR})


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
