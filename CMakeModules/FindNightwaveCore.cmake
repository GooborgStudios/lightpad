# -*-cmake-*-
# 
# Nightwave Studios - FindNightwaveCore.cmake
# https://www.nightwave.co
# 
# Search for NightwaveCore library files
# Based off of https://github.com/buaazp/zimg/blob/master/src/CMakeModules/FindGraphicsMagick.cmake
#
# Once loaded this will define:
#   NIGHTWAVE_CORE_FOUND       - system has NightwaveCore
#   NIGHTWAVE_CORE_INCLUDE_DIR - include directory for NightwaveCore
#   NIGHTWAVE_CORE_LIBRARY_DIR - library directory for NightwaveCore
#   NIGHTWAVE_CORE_LIBRARIES   - libraries you need to link to
#

SET(NIGHTWAVE_CORE_FOUND "NO")

FIND_PATH(NIGHTWAVE_CORE_INCLUDE_DIR
	NAMES "NightwaveCore/NightwaveCore.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\NightwaveCore\\Current;BinPath]"
	"$ENV{NIGHTWAVE_CORE_LOCATION}"
	"$ENV{NIGHTWAVE_CORE_LOCATION}/include"
	"$ENV{NIGHTWAVE_CORE_LOCATION}/include/NightwaveCore"
	"$ENV{NIGHTWAVE_CORE_HOME}/include"
	/usr/include/
	/usr/include/NightwaveCore
	/usr/local/include
	/usr/local/include/NightwaveCore
	/opt/local/include/NightwaveCore
)

FIND_LIBRARY(NightwaveCore
	NAMES libNightwaveCore NightwaveCore
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\NightwaveCore\\Current;BinPath]"
	"$ENV{NIGHTWAVE_CORE_LOCATION}/NIGHTWAVE_CORE/.libs"
	"$ENV{NIGHTWAVE_CORE_LOCATION}/lib"
	"$ENV{NIGHTWAVE_CORE_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	DOC "NightwaveCore library"
)

SET(NIGHTWAVE_CORE_LIBRARIES ${NightwaveCore})

IF(NIGHTWAVE_CORE_INCLUDE_DIR)
	IF(NIGHTWAVE_CORE_LIBRARIES)
		SET(NIGHTWAVE_CORE_FOUND "YES")
		GET_FILENAME_COMPONENT(NIGHTWAVE_CORE_LIBRARY_DIR ${NightwaveCore} PATH)
	ENDIF(NIGHTWAVE_CORE_LIBRARIES)
ENDIF(NIGHTWAVE_CORE_INCLUDE_DIR)

IF(NOT NIGHTWAVE_CORE_FOUND)
	# make FIND_PACKAGE friendly
	IF(NOT NIGHTWAVE_CORE_FIND_QUIETLY)
		IF(NIGHTWAVE_CORE_FIND_REQUIRED)
			MESSAGE(FATAL_ERROR "NightwaveCore required, please specify it's location with NIGHTWAVE_CORE_HOME or NIGHTWAVE_CORE_LOCATION")
		ELSE(NIGHTWAVE_CORE_FIND_REQUIRED)
			MESSAGE(STATUS "NightwaveCore was not found.")
		ENDIF(NIGHTWAVE_CORE_FIND_REQUIRED)
	ENDIF(NOT NIGHTWAVE_CORE_FIND_QUIETLY)
ENDIF(NOT NIGHTWAVE_CORE_FOUND)
 
#####