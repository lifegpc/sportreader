# - Find pcre
# Find the native PCRE headers and libraries.
#
# PCRE_INCLUDE_DIRS	- where to find pcre.h, etc.
# PCRE_LIBRARIES	- List of libraries when using pcre.
# PCRE_FOUND	- True if pcre found.

# Look for the header file.
FIND_PATH(PCRE_INCLUDE_DIR NAMES pcre.h)

# Look for the library.
FIND_LIBRARY(PCRE_LIBRARY NAMES pcre)

# Handle the QUIETLY and REQUIRED arguments and set PCRE_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCRE DEFAULT_MSG PCRE_LIBRARY PCRE_INCLUDE_DIR)

# Copy the results to the output variables.
IF(PCRE_FOUND)
	SET(PCRE_LIBRARIES ${PCRE_LIBRARY})
	SET(PCRE_INCLUDE_DIRS ${PCRE_INCLUDE_DIR})
ELSE(PCRE_FOUND)
	SET(PCRE_LIBRARIES)
	SET(PCRE_INCLUDE_DIRS)
ENDIF(PCRE_FOUND)

MARK_AS_ADVANCED(PCRE_INCLUDE_DIRS PCRE_LIBRARIES)

if(PCRE_FOUND AND NOT TARGET Pcre::Pcre)
  add_library(Pcre::Pcre UNKNOWN IMPORTED)
  set_target_properties(Pcre::Pcre PROPERTIES
    IMPORTED_LOCATION "${PCRE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${PCRE_INCLUDE_DIR}"
  )
endif()
