# - Try to find the Kipi library
# Once done this will define
#
#  KIPI_FOUND - system has libexiv2
#  KIPI_INCLUDE_DIR - the libexiv2 include directory
#  KIPI_LIBRARIES - Link these to use libexiv2
#  KIPI_DEFINITIONS - Compiler switches required for using libexiv2
#

if (KIPI_INCLUDE_DIR AND KIPI_LIBRARIES)

  # in cache already
  SET(KIPI_FOUND TRUE)

else (KIPI_INCLUDE_DIR AND KIPI_LIBRARIES)

  # Check if library is not in local
  
  FIND_PATH(KIPI_INCLUDE_DIR version.h ${CMAKE_CURRENT_SOURCE_DIR}/libkipi/src)
  FIND_LIBRARY(KIPI_LIBRARIES NAMES kipi ${CMAKE_CURRENT_SOURCE_DIR}/libkipi/src)

  if (KIPI_INCLUDE_DIR AND KIPI_LIBRARIES)
    message(STATUS "Found Kipi library in local")
    set(KIPI_DEFINITIONS )
    set(KIPI_LOCAL_FOUND TRUE)
  endif (KIPI_INCLUDE_DIR AND KIPI_LIBRARIES)

  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  INCLUDE(UsePkgConfig)
  
  PKGCONFIG(exiv2 _EXIV2IncDir _EXIV2LinkDir _EXIV2LinkFlags _EXIV2Cflags)
  
  if(_EXIV2LinkFlags)
    # query pkg-config asking for a Exiv2 >= 0.12
    EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE} ARGS --atleast-version=0.12 exiv2 RETURN_VALUE _return_VALUE OUTPUT_VARIABLE _pkgconfigDevNull )
    if(_return_VALUE STREQUAL "0")
      message(STATUS "Found Exiv2 release >= 0.12")
      set(EXIV2_VERSION_GOOD_FOUND TRUE)
    else(_return_VALUE STREQUAL "0")
      message(FATAL_ERROR "Found Exiv2 release < 0.12")
    endif(_return_VALUE STREQUAL "0")
  endif(_EXIV2LinkFlags)

  if(EXIV2_VERSION_GOOD_FOUND)
     set(EXIV2_DEFINITIONS ${_EXIV2Cflags})
 
     FIND_PATH(EXIV2_INCLUDE_DIR exiv2/exif.hpp
       ${_EXIV2IncDir}
       /usr/include
       /usr/local/include
     )
  
     FIND_LIBRARY(EXIV2_LIBRARIES NAMES exiv2
       PATHS
       ${_EXIV2LinkDir}
       /usr/lib
       /usr/local/lib
     )
  
     if (EXIV2_INCLUDE_DIR AND EXIV2_LIBRARIES)
        set(EXIV2_FOUND TRUE)
     endif (EXIV2_INCLUDE_DIR AND EXIV2_LIBRARIES)
  
     if (EXIV2_FOUND)
        if (NOT Exiv2_FIND_QUIETLY)
         message(STATUS "Found Exiv2: ${EXIV2_LIBRARIES}")
        endif (NOT Exiv2_FIND_QUIETLY)
     else (EXIV2_FOUND)
       if (Exiv2_FIND_REQUIRED)
         if (NOT EXIV2_INCLUDE_DIR)
           message(FATAL_ERROR "Could NOT find Exiv2 header files")
         endif (NOT EXIV2_INCLUDE_DIR)
         if (NOT EXIV2_LIBRARIES)
           message(FATAL_ERROR "Could NOT find Exiv2 library")
         endif (NOT EXIV2_LIBRARIES)
       endif (Exiv2_FIND_REQUIRED)
     endif (EXIV2_FOUND)
  endif(EXIV2_VERSION_GOOD_FOUND)

  MARK_AS_ADVANCED(EXIV2_INCLUDE_DIR EXIV2_LIBRARIES)
  
endif (EXIV2_INCLUDE_DIR AND EXIV2_LIBRARIES)

