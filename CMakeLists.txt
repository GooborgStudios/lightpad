# Lightpad - Nightwave Studios

# Minimum version required for CMake
IF(APPLE)
	cmake_minimum_required(VERSION 3.6)
ELSE()
	cmake_minimum_required(VERSION 3.1) # XXX Validate
ENDIF()

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

# Project properties
project(lightpad)
set(TARGET_VERSION_MAJOR 0)
set(TARGET_VERSION_MINOR 0)
set(TARGET_VERSION_PATCH 1)
set(TARGET_COPYRIGHT "©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.")

set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE_FILES "")
aux_source_directory(src source_file_list)
file(GLOB RESOURCE_FILES "${CMAKE_SOURCE_DIR}/graphics/icons" "${CMAKE_SOURCE_DIR}/graphics/launchpad_display" "${CMAKE_SOURCE_DIR}/graphics/splash.png")
IF(APPLE)
	list(FILTER RESOURCE_FILES EXCLUDE REGEX ".*\.DS_Store")
ENDIF()

set(ROOT  ${CMAKE_SOURCE_DIR})
set(SRC   ${PROJECT_SOURCE_DIR})
set(BIN   ${CMAKE_BINARY_DIR})
set(INCL  ${ROOT}/include)
set(LIB   ${ROOT}/lib)
set(IMGS  ${ROOT}/graphics)
set(REFS  ${ROOT}/references)
set(BUILD ${PROJECT_BINARY_DIR})

find_package(GraphicsMagick REQUIRED)

add_definitions(-DRESOURCE_DIR="${IMGS}")
include_directories(${ROOT})
include_directories(${INCL})

include_directories(${MAGICK_INCLUDE_DIR})
include_directories(${MAGICK++_INCLUDE_DIR})
link_directories(${MAGICK_LIBRARY_DIR})
link_directories(${MAGICK++_LIBRARY_DIR})

# IF(LINUX)
# 	find_package(PkgConfig REQUIRED)
# 	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# 	include_directories(${GTK3_INCLUDE_DIRS})
# 	link_directories(${GTK3_LIBRARY_DIRS})
# 	add_definitions(${GTK3_CFLAGS_OTHER})
# ENDIF()

add_executable(lightpad
	${SRC}/main.cpp ${SRC}/ElementIDs.h
	${SRC}/DisplayPanel.cpp ${SRC}/DisplayPanel.h
	${SRC}/FilePanel.cpp ${SRC}/FilePanel.h
	${SRC}/GridRenderer.cpp ${SRC}/GridRenderer.h
	${SRC}/Launchpad.cpp ${SRC}/Launchpad.h
	${SRC}/MidiLayer.cpp ${SRC}/MidiLayer.h
	${SRC}/NoteHelpers.cpp ${SRC}/NoteHelpers.h
	${SRC}/LightpadProject.cpp ${SRC}/LightpadProject.h
	${SRC}/PropertiesPanel.cpp ${SRC}/PropertiesPanel.h
	${SRC}/TestProject.cpp ${SRC}/TestProject.h
)

add_executable(timeline
	${SRC}/timeline.cpp ${SRC}/ElementIDs.h
	${SRC}/NoteHelpers.cpp ${SRC}/NoteHelpers.h
	${SRC}/Launchpad.cpp ${SRC}/Launchpad.h
	${SRC}/MidiLayer.cpp ${SRC}/MidiLayer.h
	${SRC}/LightpadProject.cpp ${SRC}/LightpadProject.h
	${SRC}/TestProject.cpp ${SRC}/TestProject.h
)

add_executable(midiprobe
	${SRC}/midiprobe.cpp
	${SRC}/Launchpad.cpp ${SRC}/Launchpad.h
	${SRC}/NoteHelpers.cpp ${SRC}/NoteHelpers.h
)

add_executable(midiout
	${SRC}/midiout.cpp
	${SRC}/Launchpad.cpp ${SRC}/Launchpad.h
	${SRC}/NoteHelpers.cpp ${SRC}/NoteHelpers.h
)

add_executable(GenerateNoteButtonMap
	${SRC}/GenerateNoteButtonMap.cpp
)

set(CMAKE_CXX_COMPILER g++)
find_package(wxWidgets COMPONENTS html qa adv core xml net base REQUIRED)
execute_process(COMMAND "wx-config --cxxflags" OUTPUT_VARIABLE wxConfig_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${wxConfig_CXX_FLAGS}")

IF(APPLE)
	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.12")
	ENDIF()
	IF(${CMAKE_GENERATOR} MATCHES "Xcode")
		set(OSX_ICON_FILES ${IMGS}/icon.icns)
		target_compile_definitions(lightpad PUBLIC -DXCODE_BUNDLE)
		target_sources(lightpad PUBLIC ${RESOURCE_FILES})
		target_link_libraries(lightpad "-framework CoreFoundation")
		set_target_properties(lightpad PROPERTIES MACOSX_BUNDLE TRUE MACOSX_FRAMEWORK_IDENTIFIER co.nightwave.lightpad RESOURCE "${RESOURCE_FILES}" MACOSX_BUNDLE_INFO_PLIST ${ROOT}/mac_info.plist.in)
		target_compile_definitions(timeline PUBLIC -DXCODE_BUNDLE)
		target_sources(timeline PUBLIC ${RESOURCE_FILES})
		target_link_libraries(timeline "-framework CoreFoundation")
		set_target_properties(timeline PROPERTIES MACOSX_BUNDLE TRUE MACOSX_FRAMEWORK_IDENTIFIER co.nightwave.lightpad.timeline RESOURCE "${RESOURCE_FILES}" MACOSX_BUNDLE_INFO_PLIST ${ROOT}/mac_info.plist.in)
	ENDIF()
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D__NO_INLINE__)
	set_target_properties(lightpad PROPERTIES WIN32_EXECUTABLE TRUE)
	set_target_properties(timeline PROPERTIES WIN32_EXECUTABLE TRUE)
ENDIF()

target_link_libraries(lightpad "-L${LIB}")
target_link_libraries(timeline "-L${LIB}")
target_link_libraries(midiprobe "-L${LIB}")
target_link_libraries(midiout "-L${LIB}")

target_link_libraries(lightpad NightwaveCore)
target_link_libraries(timeline NightwaveCore)
target_link_libraries(midiprobe NightwaveCore)
target_link_libraries(midiout NightwaveCore)

target_link_libraries(lightpad HOWL)
target_link_libraries(timeline HOWL)

# IF(LINUX)
# 	target_link_libraries(lightpad ${GTK3_LIBRARIES})
# 	target_link_libraries(timeline ${GTK3_LIBRARIES})
# ENDIF()

include(${wxWidgets_USE_FILE})
target_link_libraries(lightpad ${wxWidgets_LIBRARIES})
target_link_libraries(timeline ${wxWidgets_LIBRARIES})
target_link_libraries(midiprobe ${wxWidgets_LIBRARIES})
target_link_libraries(midiout ${wxWidgets_LIBRARIES})

add_definitions(-DLIB_GRAPHICSMAGICK)
target_link_libraries(lightpad "GraphicsMagick++")
target_link_libraries(timeline "GraphicsMagick++")

find_package(LibMagic)
IF(LibMagic_FOUND)
	add_definitions(-DLIB_MAGIC)
	include_directories(${LibMagic_INCLUDE_DIR})
	target_link_libraries(lightpad "magic")
	target_link_libraries(timeline "magic")
ENDIF(LibMagic_FOUND)

find_package(Doxygen)
IF(DOXYGEN_FOUND)
	configure_file(${ROOT}/doxygen.conf.in ${BUILD}/doxygen.conf @ONLY)
	add_custom_target(docs ALL DEPENDS ${BUILD}/doxygen/)
	add_custom_command(OUTPUT ${BUILD}/doxygen/ COMMAND ${DOXYGEN_EXECUTABLE} ${BUILD}/doxygen.conf DEPENDS ${SRC}/* ${ROOT}/readme.md ${ROOT}/doxygen.conf.in VERBATIM) # COMMENT "Generating API documentation with Doxygen"
ENDIF(DOXYGEN_FOUND)

include_directories(${INCL}/rtmidi)
target_link_libraries(lightpad "rtmidi")
target_link_libraries(timeline "rtmidi")
target_link_libraries(midiprobe "rtmidi")
target_link_libraries(midiout "rtmidi")

include_directories(${INCL}/midifile)
target_link_libraries(lightpad "midifile")
target_link_libraries(timeline "midifile")
target_link_libraries(midiout "midifile")

IF(NOT ${CMAKE_GENERATOR} MATCHES "Xcode")
	FOREACH(f ${source_file_list})
		list(APPEND SOURCE_FILES "${ROOT}/${f}")
	ENDFOREACH()
	file(MAKE_DIRECTORY ${BUILD}/oclint)
	add_custom_target(oclint oclint -p ${BUILD} ${SOURCE_FILES} DEPENDS ${SRC}/* ${BUILD}/compile_commands.json WORKING_DIRECTORY ${BUILD}/oclint VERBATIM) # COMMENT "Generating cleanliness report with OCLint..."
ENDIF()
