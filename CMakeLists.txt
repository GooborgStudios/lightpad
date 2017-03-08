# Lightpad - Nightwave Studios

# Minimum version required for CMake
cmake_minimum_required(VERSION 2.8)

# Project setup
project(lightpad)
set(lightpad_VERSION_MAJOR 0)
set(lightpad_VERSION_MINOR 0)
set(lightpad_VERSION_MICRO 1)

set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
aux_source_directory(src source_file_list)
set(SOURCE_FILES "")

set(ROOT ${CMAKE_SOURCE_DIR})
set(SRC ${PROJECT_SOURCE_DIR})
set(BIN ${CMAKE_BINARY_DIR})
set(INCL ${ROOT}/include)
set(LIB ${ROOT}/lib)
set(IMGS ${ROOT}/graphics)
set(REFS ${ROOT}/references)
set(BUILD ${PROJECT_BINARY_DIR})

include_directories(${ROOT})
include_directories(${INCL})

set(wxDRAW_BUTTONS 0)
if(wxDRAW_BUTTONS EQUAL 1)
	add_definitions(-DwxDRAW_BUTTONS)
endif(wxDRAW_BUTTONS EQUAL 1)

add_executable(lightpad ${SRC}/main.cpp ${SRC}/FilePanel.cpp ${SRC}/DisplayPanel.cpp ${SRC}/TimelinePanel.cpp ${SRC}/PropertiesPanel.cpp ${SRC}/GridRenderer.cpp ${SRC}/Colors.cpp ${SRC}/Helpers.cpp)
add_executable(midiprobe ${SRC}/midiprobe.cpp ${SRC}/Helpers.cpp)
add_executable(midiout ${SRC}/midiout.cpp ${SRC}/Helpers.cpp)
add_executable(GenerateNoteButtonMap ${SRC}/GenerateNoteButtonMap.cpp)

add_custom_target(Colors ALL DEPENDS ${SRC}/Colors.cpp)
add_custom_command(OUTPUT ${SRC}/Colors.cpp COMMAND python "${SRC}/sample_colors.py" WORKING_DIRECTORY ${ROOT} DEPENDS ${SRC}/Colors.cpp.in ${IMGS}/rgbcolors1.png ${IMGS}/rgbcolors2.png ${SRC}/sample_colors.py VERBATIM) # COMMENT "Generating color mapping with sample_colors.py"
add_dependencies(lightpad Colors)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${ROOT}/doxygen.conf.in ${BUILD}/doxygen.conf @ONLY)
	add_custom_target(doc ALL DEPENDS ${BUILD}/doxygen/)
	add_custom_command(OUTPUT ${BUILD}/doxygen/ COMMAND ${DOXYGEN_EXECUTABLE} ${BUILD}/doxygen.conf DEPENDS ${SRC}/* ${ROOT}/readme.md ${ROOT}/doxygen.conf.in VERBATIM) # COMMENT "Generating API documentation with Doxygen"
endif(DOXYGEN_FOUND)

set(CMAKE_CXX_COMPILER g++)
execute_process(COMMAND "wx-config --cxxflags" OUTPUT_VARIABLE wxConfig_CXX_FLAGS)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.12")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D__NO_INLINE__)
ENDIF()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 ${wxConfig_CXX_FLAGS}")

target_link_libraries(lightpad "-L${LIB}")
target_link_libraries(midiprobe "-L${LIB}")
target_link_libraries(midiout "-L${LIB}")

find_package(wxWidgets COMPONENTS html qa adv core xml net base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(lightpad ${wxWidgets_LIBRARIES})

find_package(GraphicsMagick)
if(MAGICK++_FOUND)
	add_definitions(-DLIB_GRAPHICSMAGICK)
	include_directories(${MAGICK_INCLUDE_DIR})
	include_directories(${MAGICK++_INCLUDE_DIR})
	target_link_libraries(lightpad "GraphicsMagick++")
endif(MAGICK++_FOUND)

find_package(LibMagic)
if(LibMagic_FOUND)
	add_definitions(-DLIB_MAGIC)
	include_directories(${LibMagic_INCLUDE_DIR})
	target_link_libraries(lightpad "magic")
endif(LibMagic_FOUND)

include_directories(${INCL}/rtmidi)
target_link_libraries(lightpad "rtmidi")
target_link_libraries(midiprobe "rtmidi")
target_link_libraries(midiout "rtmidi")

include_directories(${INCL}/midifile)
target_link_libraries(lightpad "midifile")
target_link_libraries(midiout "midifile")

foreach(f ${source_file_list})
	list(APPEND SOURCE_FILES "${ROOT}/${f}")
endforeach()
add_custom_target(oclint oclint -p ${ROOT} ${SOURCE_FILES} DEPENDS ${SRC}/* ${BUILD}/compile_commands.json WORKING_DIRECTORY ${BUILD}/oclint VERBATIM) # COMMENT "Generating cleanliness report with OCLint..."
