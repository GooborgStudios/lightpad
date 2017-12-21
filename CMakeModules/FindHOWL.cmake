# -*-cmake-*-
#
# Nightwave Studios - FindHOWL.cmake
# https://www.nightwave.co
#
# Search for HOWL library files
#
# Once loaded this will define:
#   HOWL_FOUND       - system has HOWL
#   HOWL_INCLUDE_DIR - include directory for HOWL
#   HOWL_LIBRARY_DIR - library directory for HOWL
#   HOWL_LIBRARIES   - libraries you need to link to



# Find headers
find_path(HOWL_INCLUDE_DIR
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

# Find compiled library file
find_library(HOWL_LIBRARIES
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

# Get path to library
get_filename_component(HOWL_LIBRARY_DIR ${HOWL_LIBRARIES} PATH)

# Set variables as advanced (hide from GUI unless "show advanced checked")
mark_as_advanced(HOWL_LIBRARIES HOWL_LIBRARY_DIR HOWL_INCLUDE_DIR)

# Set HOWL_FOUND and print message
INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	HOWL
	DEFAULT_MSG

	HOWL_LIBRARIES
	HOWL_INCLUDE_DIR
)
