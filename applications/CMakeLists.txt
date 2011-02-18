# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil osgViewer osgGA)

if(OPENSCENEGRAPH_FOUND)
	include(CopyResourcesToBuildTree)
	set(RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../hand-structured.osg")
	add_executable(dummyHand dummyHand.cpp ${RESOURCE})
	set_target_properties(dummyHand PROPERTIES RESOURCE "${RESOURCE}")
	target_link_libraries(dummyHand glovetools ${OPENSCENEGRAPH_LIBRARIES})
	copy_resources_to_build_tree(dummyHand)
	
	add_cppcheck(dummyHand)
endif()

add_executable(logger logger.cpp)
target_link_libraries(logger glovetools ${OPENSCENEGRAPH_LIBRARIES})
add_cppcheck(logger)
