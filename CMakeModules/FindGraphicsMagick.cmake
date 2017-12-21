# -*-cmake-*-
#
# Nightwave Studios - FindGraphicsMagick.cmake
# https://www.nightwave.co
#
# Search for GraphicsMagick library files, unlike CMake's FindGraphicsMagick.cmake
# which tests for GraphicsMagick's binary utilities
#
# Once loaded this will define:
#  GraphicsMagick_FOUND    - system has GraphicsMagick
#  MAGICK_INCLUDE_DIR      - include directory for GraphicsMagick
#  MAGICK_LIBRARY_DIR      - library directory for GraphicsMagick
#  MAGICK_LIBRARIES        - libraries you need to link to
#
#  MAGICK++_INCLUDE_DIR    - include directory for GraphicsMagick C++
#  MAGICK++_LIBRARY_DIR    - library directory for GraphicsMagick C++
#  MAGICK++_LIBRARIES      - libraries you need to link to for C++

#  MAGICKWAND_INCLUDE_DIR  - include directory for GraphicsMagick Wand
#  MAGICKWAND_LIBRARY_DIR  - library directory for GraphicsMagick Wand
#  MAGICKWAND_LIBRARIES    - libraries you need to link to for Wand
#


# Find headers
FIND_PATH(MAGICK_INCLUDE_DIR
	NAMES "magick/magick.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]"
	"$ENV{MAGICK_LOCATION}"
	"$ENV{MAGICK_LOCATION}/include"
	"$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
	"$ENV{MAGICK_HOME}/include"
	/usr/include/
	/usr/include/GraphicsMagick
	/usr/local/include
	/usr/local/include/GraphicsMagick
	/opt/local/include/GraphicsMagick
)

FIND_PATH(MAGICK++_INCLUDE_DIR
	NAMES "Magick++.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]\\Magick++\\lib"
	"$ENV{MAGICK++_LOCATION}"
	"$ENV{MAGICK++_LOCATION}/include"
	"$ENV{MAGICK_LOCATION}"
	"$ENV{MAGICK_LOCATION}/include"
	"$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
	"$ENV{MAGICK_HOME}/include"
	/usr/include/
	/usr/include/GraphicsMagick
	/usr/local/include
	/usr/local/include/GraphicsMagick
	/opt/local/include/GraphicsMagick
)

FIND_PATH(MAGICKWAND_INCLUDE_DIR
	NAMES "wand/magick_wand.h" 
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]"
	"$ENV{MAGICKWAND_LOCATION}"
	"$ENV{MAGICKWAND_LOCATION}/include"
	"$ENV{MAGICK_LOCATION}"
	"$ENV{MAGICK_LOCATION}/include"
	"$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
	"$ENV{MAGICK_HOME}/include"
	/usr/include/
	/usr/include/GraphicsMagick
	/usr/local/include
	/usr/local/include/GraphicsMagick
	/opt/local/include/GraphicsMagick
)

# Find compiled library files
FIND_LIBRARY(MAGICK_LIBRARIES
	NAMES GraphicsMagick CORE_RL_magick_
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]"
	"$ENV{MAGICK_LOCATION}/magick/.libs"
	"$ENV{MAGICK_LOCATION}/lib"
	"$ENV{MAGICK_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	DOC   "GraphicsMagick Magick library"
)

FIND_LIBRARY(MAGICK++_LIBRARIES
	NAMES GraphicsMagick++ CORE_RL_Magick++_
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]"
	"$ENV{MAGICK++_LOCATION}/.libs"
	"$ENV{MAGICK_LOCATION}/.libs"
	"$ENV{MAGICK++_LOCATION}/lib"
	"$ENV{MAGICK_LOCATION}/lib"
	"$ENV{MAGICK_HOME}/lib"
	/opt/local/lib64
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib
	/usr/lib
	/usr/local/lib
	DOC   "GraphicsMagick Magick++ library"
)

FIND_LIBRARY(MAGICKWAND_LIBRARIES
	NAMES GraphicsMagickWand CORE_RL_MagickWand_
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]"
	"$ENV{MAGICKWAND_LOCATION}/.libs"
	"$ENV{MAGICK_LOCATION}/.libs"
	"$ENV{MAGICKWAND_LOCATION}/lib"
	"$ENV{MAGICK_LOCATION}/lib"
	"$ENV{MAGICK_HOME}/lib"
	/opt/local/lib64
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib
	/usr/lib
	/usr/local/lib
	DOC   "GraphicsMagick MagickWand library"
)

# Get paths to libraries
GET_FILENAME_COMPONENT(MAGICK_LIBRARY_DIR ${MAGICK} PATH)
GET_FILENAME_COMPONENT(MAGICK++_LIBRARY_DIR ${MAGICK++} PATH)
GET_FILENAME_COMPONENT(MAGICKWAND_LIBRARY_DIR ${MAGICKWAND} PATH)

# Set variables as advanced (hide from GUI unless "show advanced checked")
mark_as_advanced(MAGICK_LIBRARIES MAGICK_LIBRARY_DIR MAGICK_INCLUDE_DIR)
mark_as_advanced(MAGICK++_LIBRARIES MAGICK++_LIBRARY_DIR MAGICK++_INCLUDE_DIR)
mark_as_advanced(MAGICKWAND_LIBRARIES MAGICKWAND_LIBRARY_DIR MAGICKWAND_INCLUDE_DIR)

# Set GraphicsMagick_FOUND and print message
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GraphicsMagick
	DEFAULT_MSG

	MAGICK_LIBRARIES
	MAGICK_INCLUDE_DIR
	MAGICK++_LIBRARIES
	MAGICK++_INCLUDE_DIR
	MAGICKWAND_LIBRARIES
	MAGICKWAND_INCLUDE_DIR
)
