#For now, we want to be sure that we link against the right llvm with our
#custom modifications
#IF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm")
#  message(FATAL_ERROR "download llvm in directory pywt: git clone git@github.com:wdu/llvm.git")
#ENDIF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm")
#IF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm/tools/clang")
#  message(FATAL_ERROR "download clang in directory pywt/llvm/tools: git clone git@github.com:wdu/clang.git")
#ENDIF (NOT EXISTS "${CMAKE_SOURCE_DIR}/llvm/tools/clang")
#
#INCLUDE(ExternalProject)
#ExternalProject_add(llvm
#        SOURCE_DIR "${CMAKE_SOURCE_DIR}/llvm"
#	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/install"
#)

# Do we need these or is this clang specific?
#ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}/llvm-prefix/install/include
)

ADD_EXECUTABLE(python_dump
  main.cc
)
target_link_libraries(python_dump
  ${CLANG_LIBRARIES}
)
SET_TARGET_PROPERTIES(python_dump
  PROPERTIES
    SKIP_BUILD_RPATH OFF
)

add_dependencies(python_dump llvm)
