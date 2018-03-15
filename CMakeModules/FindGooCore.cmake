# -*-cmake-*-
#
# Gooborg Studios - FindGooCore.cmake
# http://www.gooborg.com
#
# Search for GooCore library files
#
# Once loaded this will define:
#   GooCore_FOUND       - system has GooCore
#   GooCore_INCLUDE_DIR - include directory for GooCore
#   GooCore_LIBRARY_DIR - library directory for GooCore
#   GooCore_LIBRARIES   - libraries you need to link to



# Find headers
find_path(GooCore_INCLUDE_DIR
	NAMES "GooCore/GooCore.h"
	PATHS
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GooCore\\Current;BinPath]"
	"$ENV{GooCore_LOCATION}"
	"$ENV{GooCore_LOCATION}/include"
	"$ENV{GooCore_LOCATION}/include/GooCore"
	"$ENV{GooCore_HOME}/include"
	/usr/include/
	/usr/include/GooCore
	/usr/local/include
	/usr/local/include/GooCore
	/opt/local/include/GooCore
)

# Find compiled library file
find_library(GooCore_LIBRARIES
	NAMES libGooCore GooCore
	PATHS 
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\GooCore\\Current;BinPath]"
	"$ENV{GooCore_LOCATION}/GooCore/.libs"
	"$ENV{GooCore_LOCATION}/lib"
	"$ENV{GooCore_HOME}/lib"
	/usr/lib64
	/usr/local/lib64
	/opt/local/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	DOC "GooCore library"
)

# Get path to library
get_filename_component(GooCore_LIBRARY_DIR ${GooCore_LIBRARIES} PATH)

# Set variables as advanced (hide from GUI unless "show advanced checked")
mark_as_advanced(GooCore_LIBRARIES GooCore_LIBRARY_DIR GooCore_INCLUDE_DIR)

# Set GooCore_FOUND and print message
INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	GooCore
	DEFAULT_MSG

	GooCore_LIBRARIES
	GooCore_INCLUDE_DIR
)
