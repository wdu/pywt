cmake_minimum_required(VERSION 2.8)

# Do we need these or is this clang specific?
ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

INCLUDE_DIRECTORIES(
  llvm/install/include
)

LINK_DIRECTORIES(
  llvm/install/lib
)

ADD_EXECUTABLE(python_dump
  main.cc
)

target_link_libraries(python_dump
  clang
)
