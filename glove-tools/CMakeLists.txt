

###
# glove-tools Library
set(SOURCES
	Glove.cpp
	GloveNode.cpp
	GloveUpdater.cpp
	IGloveHardware.cpp)

set(API
	Finger.h
	Glove.h
	GloveNode.h
	GlovePointerTypes.h
	GloveUpdater.h
	IGloveHardware.h)

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
