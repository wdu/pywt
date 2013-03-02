# TODO:
# This cmake script needs work and will probably not work out of
# the box

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(PYWT)

SET(CMAKE_INCLUDE_DIRECTORIES_PROJECT_BEFORE true)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

INCLUDE(cmake/WtFindBoost.txt)


#For now, we want to be sure that we link against the right llvm with our
#custom modifications
IF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm")
  message(FATAL_ERROR "download llvm in directory pywt: git clone git@github.com:wdu/llvm.git")
ENDIF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm")
IF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm/tools/clang")
  message(FATAL_ERROR "download clang in directory pywt/llvm/tools: git clone git@github.com:wdu/clang.git")
ENDIF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm/tools/clang")

INCLUDE(ExternalProject)
ExternalProject_add(llvm
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/llvm"
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/install"
#          "-DBUILD_SHARED_LIBS=OFF"
)

IF (WIN32)
  SET(CLANG_LIBRARIES
    OPTIMIZED ${CMAKE_BINARY_DIR}/llvm-prefix/install/lib/clang.lib
    DEBUG ${CMAKE_BINARY_DIR}/llvm-prefix/install/lib/clangd.lib
  )
ELSE (WIN32)
  SET(CLANG_LIBRARIES
    ${CMAKE_BINARY_DIR}/llvm-prefix/install/lib/libclang.dylib
  )
ENDIF (WIN32)


#For now, we want to be sure that we link against the right wt with our
#custom modifications
IF (NOT EXISTS "${CMAKE_SOURCE_DIR}/wt")
  message(FATAL_ERROR "download wt in directory pywt")
ENDIF (NOT EXISTS "${CMAKE_SOURCE_DIR}/wt")

include(ExternalProject)
ExternalProject_add(wt
	SOURCE_DIR "${CMAKE_SOURCE_DIR}/wt"
	CMAKE_ARGS
          "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/install"
          "-DDESTDIR=<INSTALL_DIR>/install/"
# static libs = bad idea; nearly impossible to find out the correct
# dependencies of the .a file
#          "-DSHARED_LIBS=OFF"
#          "-DPOSITION_INDEPENDEND_CODE=ON"
)
IF (WIN32)
  SET(Wt_LIBRARY
    OPTIMIZED ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/wt.lib
    DEBUG ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/wtd.lib
  )
  SET(Wt_HTTP_LIBRARY
    OPTIMIZED ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/wthttp.lib
    DEBUG ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/wthttpd.lib
  )
ELSEIF (APPLE)
  SET(Wt_LIBRARY ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/libwt.dylib)
  SET(Wt_HTTP_LIBRARY ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/libwthttp.dylib)
ELSE (APPLE)
  SET(Wt_LIBRARY ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/libwt.so)
  SET(Wt_HTTP_LIBRARY ${CMAKE_BINARY_DIR}/wt-prefix/install/lib/libwthttp.so)
ENDIF (WIN32)

#link_directories(${CMAKE_BINARY_DIR}/wt-prefix/install/lib)

SUBDIRS(introspection)

SET(BOOST_INCLUDE_DIRS "/opt/local/include")

# Attention: dependency checking for python_dump will NOT be correct!!
ADD_CUSTOM_TARGET(run_python_dump
    python_dump
    "wt.cc"
    "-I${CMAKE_BINARY_DIR}/wt-prefix/install/include"
    "-I${BOOST_INCLUDE_DIRS}"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
ADD_DEPENDENCIES(run_python_dump wt python_dump)

# Attention: dependency checking?
ADD_CUSTOM_TARGET(clean_bindings
    ${CMAKE_SOURCE_DIR}/descr-cleanup.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
ADD_DEPENDENCIES(clean_bindings run_python_dump)

# Attention: dependency checking for binding generation will NOT be correct!!
ADD_CUSTOM_TARGET(generate_bindings
    python
    scripts/PyGenBindings.py
    .
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
ADD_DEPENDENCIES(generate_bindings clean_bindings)


IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 OLD)
ENDIF(COMMAND CMAKE_POLICY)

# Two lines below are required on recent linux to perform checks of missing
# symbols at compile time
#SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -Wl,-z,defs")
#SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,-z,defs")

SET(CMAKE_MODULE_PATH ${PYWT_SOURCE_DIR}/cmake)

#
# Various things that must be configured by the user or packager ...
#

SET(LIB_INSTALL_DIR "lib" CACHE STRING
  "Name for library directory within ${CMAKE_INSTALL_PREFIX}")

IF(WIN32)
  SET(USERLIB_PREFIX_DEFAULT "c:/libraries")
ELSE(WIN32)
  SET(USERLIB_PREFIX_DEFUALT "/usr")
ENDIF(WIN32)

SET(USERLIB_PREFIX ${USERLIB_PREFIX_DEFAULT} CACHE PATH
    "Installation prefix of dependency libraries")

ADD_DEFINITIONS(-DNDEBUG)

IF(WIN32)
  SET(BOOST_PREFIX_DEFAULT "c:/Program Files/Boost")
  OPTION(BOOST_DYNAMIC "Link to boost DLLs (OFF means static link)" OFF)
ELSE(WIN32)
  SET(BOOST_PREFIX_DEFAULT ${USERLIB_PREFIX})
ENDIF(WIN32)

SET(BOOST_PREFIX ${BOOST_PREFIX_DEFAULT} CACHE PATH
  "Installation prefix of boost libraries")

#FIND_PACKAGE(Wt REQUIRED)
INCLUDE(cmake/WtFindPython.txt)

INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${WT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/wt-prefix/install/include)
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

#LINK_DIRECTORIES("c:/python27/libs")
LINK_DIRECTORIES(${BOOST_LIB_DIRS})
IF(WIN32)
  IF(BOOST_DYNAMIC)
    ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
  ENDIF(BOOST_DYNAMIC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(WIN32)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Compile time constants & make sure our build finds it
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Wt)
SET(WCONFIG_H_PATH ${CMAKE_CURRENT_BINARY_DIR}/Wt/WConfig.h)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SUBDIRS(src)

IF( NOT DEFINED WT_CMAKE_FINDER_INSTALL_DIR )
  SET( WT_CMAKE_FINDER_INSTALL_DIR "share/cmake-2.4/Modules" )
ENDIF( NOT DEFINED WT_CMAKE_FINDER_INSTALL_DIR)

