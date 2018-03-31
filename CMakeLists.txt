cmake_minimum_required(VERSION 3.0)
project(modest_html)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -std=c99") # c
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcomment -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-write-strings")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/libs/Modest/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/libs/Modest/lib")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/libs/vec/src")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/libs/dmt/src")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")

message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_LINK_EXECUTABLE} -lmodest_static -lpthread")

file(GLOB_RECURSE MODEST_HTML "modest_html/*.c")

add_library(
  ${PROJECT_NAME}
  STATIC
  ${MODEST_HTML}
  libs/vec/src/vec.c
  libs/dmt/src/dmt.c
)

# add target tests

add_subdirectory(test)
enable_testing()
file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c")
foreach(TEST ${TESTS})
  get_filename_component(TEST ${TEST} NAME_WE)
  add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()