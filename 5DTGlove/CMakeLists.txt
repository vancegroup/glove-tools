

###
# 5DTGlove Library
set(SOURCES
	CubeField.cpp
	GetVoxelLocations.cpp
	PointshellView.cpp
	VoxelizeVisitor.cpp
	VoxmapView.cpp)

set(API
	CubeField.h
	GetVoxelLocations.h
	OSGtoVPSMatrix.h
	PointshellView.h
	VoxelizeVisitor.h
	Voxelizer.h
	VoxmapView.h
	Voxode.h)

add_library(5DTGlove ${SOURCES} ${API})
set_target_properties(5DTGlove PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(5DTGlove ${EXTRA_LIBS})

stamp_target_with_version("${CPACK_PACKAGE_VERSION}" 5DTGlove)

add_cppcheck(5DTGlove)

###
# Install files
install(TARGETS 5DTGlove
	EXPORT vpstools-sdk
	RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
	LIBRARY DESTINATION ${LIB_DIR} COMPONENT sdk
	ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/5DTGlove COMPONENT sdk)
