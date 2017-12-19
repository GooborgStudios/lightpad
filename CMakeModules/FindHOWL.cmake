# -*-cmake-*-
# 
# Nightwave Studios - FindHOWL.cmake
# https://www.nightwave.co
# 
# Search for HOWL library files
# Based off of https://github.com/buaazp/zimg/blob/master/src/CMakeModules/FindGraphicsMagick.cmake
#
# Once loaded this will define:
#   HOWL_FOUND       - system has HOWL
#   HOWL_INCLUDE_DIR - include directory for HOWL
#   HOWL_LIBRARY_DIR - library directory for HOWL
#   HOWL_LIBRARIES   - libraries you need to link to
#

SET(HOWL_FOUND "NO")

FIND_PATH(HOWL_INCLUDE_DIR
	NAMES "HOWL/Project.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\HOWL\\Current;BinPath]"
	"$ENV{HOWL_LOCATION}"
	"$ENV{HOWL_LOCATION}/include"
	"$ENV{HOWL_LOCATION}/include/HOWL"
	"$ENV{HOWL_HOME}/include"
	/usr/include/
	/usr/include/HOWL
	/usr/local/include
	/usr/local/include/HOWL
	/opt/local/include/HOWL
)

FIND_LIBRARY(HOWL
	NAMES libHOWL HOWL
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\HOWL\\Current;BinPath]"
	"$ENV{HOWL_LOCATION}/HOWL/.libs"
	"$ENV{HOWL_LOCATION}/lib"
	"$ENV{HOWL_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	DOC "HOWL library"
)

SET(HOWL_LIBRARIES ${HOWL})

IF (HOWL_INCLUDE_DIR)
	IF(HOWL_LIBRARIES)
		SET(HOWL_FOUND "YES")
		GET_FILENAME_COMPONENT(HOWL_LIBRARY_DIR ${HOWL} PATH)
	ENDIF(HOWL_LIBRARIES)
ENDIF(HOWL_INCLUDE_DIR)

IF(NOT HOWL_FOUND)
	# make FIND_PACKAGE friendly
	IF(NOT HOWL_FIND_QUIETLY)
		IF(HOWL_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "HOWL required, please specify it's location with HOWL_HOME or HOWL_LOCATION")
		ELSE(HOWL_FIND_REQUIRED)
			MESSAGE(STATUS "HOWL was not found.")
		ENDIF(HOWL_FIND_REQUIRED)
	ENDIF(NOT HOWL_FIND_QUIETLY)
ENDIF(NOT HOWL_FOUND)

#####