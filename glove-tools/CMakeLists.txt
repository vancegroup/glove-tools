

###
# glove-tools Library
set(SOURCES
	AvailableHardware.h.in
	Calibration.cpp
	Glove.cpp
	Glove_KalmanFilter.cpp
	GloveHardwareFactory.cpp
	GloveNode.cpp
	GloveToolsConfig.h.in
	GloveUpdater.cpp
	IGloveHardware.cpp
	MockGloveHardware.cpp)

set(API
	Calibration.h
	Finger.h
	Glove.h
	GloveHardwareFactory.h
	GloveNode.h
	GlovePointerTypes.h
	GloveUpdater.h
	Handedness.h
	IGloveHardware.h
	MockGloveHardware.h)

macro(add_driver _name)
	list(APPEND SOURCES ${_name}/${_name}.cpp ${_name}/${_name}.h)
	source_group("${_name}" FILES ${_name}/${_name}.cpp ${_name}/${_name}.h)
endmacro()

add_driver(FakeFlexingGloveHardware)
if(BUILD_WITH_GLOVE5DT)
	add_driver(GloveHardware5DT)
endif()
if(BUILD_WITH_VRPN)
	add_driver(GloveHardwareVRPN)
endif()

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
configure_file(GloveToolsConfig.h.in GloveToolsConfig.h)
configure_file(AvailableHardware.h.in AvailableHardware.h)

add_library(glovetools ${SOURCES} ${API})
set_target_properties(glovetools PROPERTIES PUBLIC_HEADER "${API}")
target_link_libraries(glovetools ${EXTRA_LIBS})

add_cppcheck(glovetools)

###
# Install files
install(TARGETS glovetools
	EXPORT glovetools-sdk
	RUNTIME DESTINATION ${BIN_DIR} COMPONENT runtime
	LIBRARY DESTINATION ${LIB_DIR} COMPONENT sdk
	ARCHIVE DESTINATION ${ARCH_DIR} COMPONENT sdk
	PUBLIC_HEADER DESTINATION ${INCLUDE_DIR}/glove-tools COMPONENT sdk)
