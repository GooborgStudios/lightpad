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

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10" CACHE STRING "Minimum macOS deployment version")

# Project properties
project(lightpad)
set(TARGET_VERSION_MAJOR 0)
set(TARGET_VERSION_MINOR 0)
set(TARGET_VERSION_PATCH 1)
set(TARGET_COPYRIGHT "©2017 Nightwave Studios: Vinyl Darkscratch, Light Apacha.")

set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

set(BU_CHMOD_BUNDLE_ITEMS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE_FILES "")
aux_source_directory(src source_file_list)
file(GLOB RESOURCE_FILES "${CMAKE_SOURCE_DIR}/graphics/icons" "${CMAKE_SOURCE_DIR}/graphics/launchpad_display" "${CMAKE_SOURCE_DIR}/graphics/splash.png")
IF(APPLE)
	list(FILTER RESOURCE_FILES EXCLUDE REGEX ".*\.DS_Store")
ENDIF()

set(ROOT  ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC   ${PROJECT_SOURCE_DIR})
set(BUILD ${CMAKE_CURRENT_BINARY_DIR})
set(IMGS  ${ROOT}/graphics)
set(REFS  ${ROOT}/references)

find_package(NightwaveCore REQUIRED)
include_directories(${NightwaveCore_INCLUDE_DIR})
link_directories(${NightwaveCore_LIBRARY_DIR})

find_package(HOWL REQUIRED)
include_directories(${HOWL_INCLUDE_DIR})
link_directories(${HOWL_LIBRARY_DIR})

add_definitions(-DRESOURCE_DIR="${IMGS}")
include_directories(${ROOT})

# IF(LINUX)
# 	find_package(PkgConfig REQUIRED)
# 	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# 	include_directories(${GTK3_INCLUDE_DIRS})
# 	link_directories(${GTK3_LIBRARY_DIRS})
# 	add_definitions(${GTK3_CFLAGS_OTHER})
# ENDIF()

add_executable(lightpad MACOSX_BUNDLE
	${SRC}/main.cpp ${SRC}/ElementIDs.h
	${SRC}/DisplayPanel.cpp ${SRC}/DisplayPanel.h
	${SRC}/GridRenderer.cpp ${SRC}/GridRenderer.h
	${SRC}/Launchpad.cpp ${SRC}/Launchpad.h
	${SRC}/MidiLayer.cpp ${SRC}/MidiLayer.h
	${SRC}/NoteHelpers.cpp ${SRC}/NoteHelpers.h
	${SRC}/LightpadProject.cpp ${SRC}/LightpadProject.h
	${SRC}/PropertiesPanel.cpp ${SRC}/PropertiesPanel.h
	${SRC}/StartupProject.cpp ${SRC}/StartupProject.h
	${SRC}/FileMetadata.cpp ${SRC}/FileMetadata.h
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
	set(OSX_ICON_FILES ${IMGS}/icon.icns)

	target_compile_definitions(lightpad PUBLIC -DOSX_BUNDLE)
	target_link_libraries(lightpad "-framework CoreFoundation")
	set_target_properties(lightpad PROPERTIES MACOSX_FRAMEWORK_IDENTIFIER co.nightwave.lightpad RESOURCE "${RESOURCE_FILES}" MACOSX_BUNDLE_INFO_PLIST ${ROOT}/mac_info.plist.in)
	target_sources(lightpad PUBLIC ${RESOURCE_FILES})
	
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.10")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D__NO_INLINE__)
	set_target_properties(lightpad PROPERTIES WIN32_EXECUTABLE TRUE)
ENDIF()

set(BUNDLE_INSTALL_TARGET "${BUILD}/${APP_BUNDLE_NAME}")
set(APPS ${BUNDLE_INSTALL_TARGET})

target_link_libraries(lightpad ${NightwaveCore_LIBRARIES})
target_link_libraries(midiprobe ${NightwaveCore_LIBRARIES})
target_link_libraries(midiout ${NightwaveCore_LIBRARIES})

target_link_libraries(lightpad ${HOWL_LIBRARIES})

# IF(LINUX)
# 	target_link_libraries(lightpad ${GTK3_LIBRARIES})
# ENDIF()

include(${wxWidgets_USE_FILE})
target_link_libraries(lightpad ${wxWidgets_LIBRARIES})
target_link_libraries(midiprobe ${wxWidgets_LIBRARIES})
target_link_libraries(midiout ${wxWidgets_LIBRARIES})

find_package(GraphicsMagick REQUIRED)
add_definitions(-DLIB_GRAPHICSMAGICK)
include_directories(${MAGICK_INCLUDE_DIR})
include_directories(${MAGICK++_INCLUDE_DIR})
link_directories(${MAGICK_LIBRARY_DIR})
link_directories(${MAGICK++_LIBRARY_DIR})
target_link_libraries(lightpad ${MAGICK_LIBRARIES})
target_link_libraries(lightpad ${MAGICK++_LIBRARIES})

find_package(LibMagic)
IF(LibMagic_FOUND)
	add_definitions(-DLIB_MAGIC)
	include_directories(${LibMagic_INCLUDE_DIR})
	target_link_libraries(lightpad ${LibMagic_LIBRARIES})
ENDIF(LibMagic_FOUND)

find_package(Doxygen)
IF(DOXYGEN_FOUND)
	configure_file(${ROOT}/doxygen.conf.in ${BUILD}/doxygen.conf @ONLY)
	add_custom_target(docs ALL DEPENDS ${BUILD}/doxygen/)
	add_custom_command(OUTPUT ${BUILD}/doxygen/ COMMAND ${DOXYGEN_EXECUTABLE} ${BUILD}/doxygen.conf DEPENDS ${SRC}/* ${ROOT}/README.md ${ROOT}/doxygen.conf.in VERBATIM) # COMMENT "Generating API documentation with Doxygen"
ENDIF(DOXYGEN_FOUND)

target_link_libraries(lightpad "rtmidi")
target_link_libraries(midiprobe "rtmidi")
target_link_libraries(midiout "rtmidi")

FIND_PATH(midifile_INCLUDE_DIR
	NAMES "MidiFile.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\midifile\\Current;BinPath]"
	"$ENV{midifile_LOCATION}"
	"$ENV{midifile_LOCATION}/include"
	"$ENV{midifile_LOCATION}/include/midifile"
	"$ENV{midifile_HOME}/include"
	/usr/include/
	/usr/include/midifile
	/usr/local/include
	/usr/local/include/midifile
	/opt/local/include/midifile
)

FIND_LIBRARY(midifile_LIBRARIES
	NAMES libmidifile midifile
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\midifile\\Current;BinPath]"
	"$ENV{midifile_LOCATION}/midifile/.libs"
	"$ENV{midifile_LOCATION}/lib"
	"$ENV{midifile_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
)

GET_FILENAME_COMPONENT(midifile_LIBRARY_DIR ${midifile_LIBRARIES} PATH)

include_directories(${midifile_INCLUDE_DIR})
link_directories(${midifile_LIBRARY_DIR})
target_link_libraries(lightpad ${midifile_LIBRARIES})
target_link_libraries(midiout ${midifile_LIBRARIES})

IF(NOT ${CMAKE_GENERATOR} MATCHES "Xcode") # Xcode doesn't generate the required compile_commands.json file that OCLint needs to function with CMake.  (This probably applies to Visual Studio as well.)
	FOREACH(f ${source_file_list})
		list(APPEND SOURCE_FILES "${ROOT}/${f}")
	ENDFOREACH()
	file(MAKE_DIRECTORY ${BUILD}/oclint)
	add_custom_target(oclint oclint -p ${BUILD} ${SOURCE_FILES} DEPENDS ${SRC}/* ${BUILD}/compile_commands.json WORKING_DIRECTORY ${BUILD}/oclint VERBATIM) # COMMENT "Generating cleanliness report with OCLint..."
ENDIF()

# IF(APPLE)
# 	# -------- Function to build OS X Stand Alone Bundles -----------------
# 	function(MakeOSXBundleApp target binary_dir)
# 		IF(${CMAKE_GENERATOR} MATCHES "Xcode")
# 			set(TARGET_FILE "${EXECUTABLE_OUTPUT_PATH}/Debug/${target}.app")
# 		ELSE()
# 			set(TARGET_FILE "${EXECUTABLE_OUTPUT_PATH}/${target}.app")
# 		ENDIF()

# 		SET(OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT "${binary_dir}/${target}_OSX_MakeStandAloneBundle.cmake")
# 		SET(OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT "${binary_dir}/${target}_OSX_MakeStandAloneBundle.sh")

# 		CONFIGURE_FILE("${ROOT}/scripts/CompleteBundle.cmake.in" "${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT}" @ONLY IMMEDIATE)
# 		CONFIGURE_FILE("${ROOT}/scripts/CreateBundle.sh.in" "${OSX_MAKE_STANDALONE_BUNDLE_BASH_SCRIPT}" @ONLY IMMEDIATE)

# 		add_custom_command(TARGET ${target} POST_BUILD COMMAND cmake -P ${OSX_MAKE_STANDALONE_BUNDLE_CMAKE_SCRIPT} VERBATIM)
# 	endfunction(MakeOSXBundleApp)

# 	MakeOSXBundleApp("lightpad" ${PROJECT_BINARY_DIR})
# ENDIF()