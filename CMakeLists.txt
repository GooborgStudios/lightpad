# Lightpad - Nightwave Studios

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(lightpad main.cpp FilePanel.cpp DisplayPanel.cpp TimelinePanel.cpp PropertiesPanel.cpp GridRenderer.cpp Colors.cpp Helpers.cpp)
add_executable(midiprobe midiprobe.cpp Helpers.cpp)
add_executable(midiout midiout.cpp Helpers.cpp)
add_executable(GenerateNoteButtonMap GenerateNoteButtonMap.cpp)

set(CMAKE_CXX_COMPILER g++)
execute_process(COMMAND "wx-config --cxxflags" OUTPUT_VARIABLE wxConfig_CXX_FLAGS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.12 -std=c++11 -O3 ${wxConfig_CXX_FLAGS}")

add_custom_command(TARGET lightpad PRE_BUILD COMMAND "which" ARGS "python" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_command(TARGET lightpad PRE_BUILD COMMAND "/Library/Frameworks/Python.framework/Versions/2.7/bin/python" ARGS "${PROJECT_SOURCE_DIR}/sample_colors.py" WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

target_link_libraries(lightpad "-L${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(midiprobe "-L${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(midiout "-L${PROJECT_SOURCE_DIR}/lib")

find_package(wxWidgets COMPONENTS html qa adv core xml net base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(lightpad ${wxWidgets_LIBRARIES})

find_package(GraphicsMagick)
if(MAGICK++_FOUND)
	include_directories(${MAGICK++_INCLUDE_DIR})
	target_link_libraries(lightpad "graphicsmagick++")
endif(MAGICK++_FOUND)

find_package(LibMagic)
if(LibMagic_FOUND)
	include_directories(${LibMagic_INCLUDE_DIR})
	target_link_libraries(lightpad "magic")
endif(LibMagic_FOUND)

include_directories(${PROJECT_SOURCE_DIR}/include/rtmidi)
target_link_libraries(lightpad "rtmidi")
target_link_libraries(midiprobe "rtmidi")
target_link_libraries(midiout "rtmidi")

include_directories(${PROJECT_SOURCE_DIR}/include/midifile)
target_link_libraries(lightpad "midifile")
target_link_libraries(midiout "midifile")