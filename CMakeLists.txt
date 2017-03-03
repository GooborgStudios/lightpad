# Lightpad - Nightwave Studios

# Minimum version required for CMake
cmake_minimum_required(VERSION 2.8)

# Project setup
project(lightpad)
set(lightpad_VERSION_MAJOR 0)
set(lightpad_VERSION_MINOR 0)
set(lightpad_VERSION_MICRO 1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(src source_file_list)
set(SOURCE_FILES "")

set(wxDRAW_BUTTONS 0)
if(wxDRAW_BUTTONS EQUAL 1)
	add_definitions(-DwxDRAW_BUTTONS)
endif(wxDRAW_BUTTONS EQUAL 1)

add_executable(lightpad src/main.cpp src/FilePanel.cpp src/DisplayPanel.cpp src/TimelinePanel.cpp src/PropertiesPanel.cpp src/GridRenderer.cpp src/Colors.cpp src/Helpers.cpp)
add_executable(midiprobe src/midiprobe.cpp src/Helpers.cpp)
add_executable(midiout src/midiout.cpp src/Helpers.cpp)
add_executable(GenerateNoteButtonMap src/GenerateNoteButtonMap.cpp)

add_custom_target(Colors ALL DEPENDS src/Colors.cpp)
add_custom_command(OUTPUT src/Colors.cpp COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/sample_colors.py" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS src/Colors.cpp.in references/rgbcolors1.png references/rgbcolors2.png sample_colors.py VERBATIM) # COMMENT "Generating color mapping with sample_colors.py"

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(doxygen.conf.in bin/doxygen.conf @ONLY)
	add_custom_target(doc ALL DEPENDS doxygen/)
	add_custom_command(OUTPUT doxygen/ COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bin/doxygen.conf DEPENDS src/* readme.md doxygen.conf.in WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} VERBATIM) # COMMENT "Generating API documentation with Doxygen"
endif(DOXYGEN_FOUND)

set(CMAKE_CXX_COMPILER g++)
execute_process(COMMAND "wx-config --cxxflags" OUTPUT_VARIABLE wxConfig_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.12 -std=c++11 -O3 ${wxConfig_CXX_FLAGS}")

add_dependencies(lightpad Colors)

target_link_libraries(lightpad "-L${CMAKE_CURRENT_SOURCE_DIR}/lib")
target_link_libraries(midiprobe "-L${CMAKE_CURRENT_SOURCE_DIR}/lib")
target_link_libraries(midiout "-L${CMAKE_CURRENT_SOURCE_DIR}/lib")

find_package(wxWidgets COMPONENTS html qa adv core xml net base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(lightpad ${wxWidgets_LIBRARIES})

find_package(GraphicsMagick)
if(MAGICK++_FOUND)
	add_definitions(-DLIB_GRAPHICSMAGICK)
	include_directories(${MAGICK++_INCLUDE_DIR})
	target_link_libraries(lightpad "graphicsmagick++")
endif(MAGICK++_FOUND)

find_package(LibMagic)
if(LibMagic_FOUND)
	add_definitions(-DLIB_MAGIC)
	include_directories(${LibMagic_INCLUDE_DIR})
	target_link_libraries(lightpad "magic")
endif(LibMagic_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/rtmidi)
target_link_libraries(lightpad "rtmidi")
target_link_libraries(midiprobe "rtmidi")
target_link_libraries(midiout "rtmidi")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/midifile)
target_link_libraries(lightpad "midifile")
target_link_libraries(midiout "midifile")

foreach(f ${source_file_list})
	list(APPEND SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
endforeach()
add_custom_target(oclint oclint -p ${CMAKE_CURRENT_SOURCE_DIR} -enable-clang-static-analyzer -enable-global-analysis -report-type html -o oclint_result.html -max-priority-1 1000 -max-priority-2 2000 -max-priority-3 3000 ${SOURCE_FILES} DEPENDS src/* compile_commands.json WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/oclint VERBATIM) # COMMENT "Generating cleanliness report with OCLint..."
