

###
# glove-tools Library
set(SOURCES
	FakeFlexingGloveHardware.cpp
	Glove.cpp
	GloveHardwareFactory.cpp
	GloveNode.cpp
	GloveToolsConfig.h.in
	GloveUpdater.cpp
	IGloveHardware.cpp
	MockGloveHardware.cpp)

set(API
	FakeFlexingGloveHardware.h
	Finger.h
	Glove.h
	GloveHardwareFactory.h
	GloveNode.h
	GlovePointerTypes.h
	GloveUpdater.h
	IGloveHardware.h
	MockGloveHardware.h)

if(BUILD_WITH_GLOVE5DT)
	list(APPEND
		SOURCES
		GloveHardware5DT.cpp
		GloveHardware5DT.h)
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file(GloveToolsConfig.h.in GloveToolsConfig.h)

add_library(glove-tools ${SOURCES} ${API})
set_target_properties(glove-tools PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(glove-tools ${EXTRA_LIBS})

add_cppcheck(glove-tools)

###
# Install files
install(TARGETS glove-tools
	EXPORT glove-tools-sdk
	RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
	LIBRARY DESTINATION ${LIB_DIR} COMPONENT sdk
	ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/glove-tools COMPONENT sdk)
