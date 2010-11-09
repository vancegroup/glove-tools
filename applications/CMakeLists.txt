# CMake cross-platform build system
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgUtil osgViewer osgGA)

if(OPENSCENEGRAPH_FOUND)
	include(CopyResourcesToBuildTree)
	set(RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../hand-structured.osg")
	add_executable(dummyHand dummyHand.cpp ${RESOURCES})
	set_target_properties(dummyHand PROPERTIES RESOURCES "${RESOURCES}")
	target_link_libraries(dummyHand glovetools ${OPENSCENEGRAPH_LIBRARIES})
	copy_resources_to_build_tree(dummyHand)
	
	add_cppcheck(dummyHand)
endif()
