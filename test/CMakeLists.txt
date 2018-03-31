cmake_minimum_required(VERSION 3.0)
project(tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -std=c99") # c
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcomment -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_REENTRANT")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-write-strings")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/../libs/Modest/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/../libs/Modest/lib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/../src")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/../build")

# add_definitions(-DMODEST_HTML_DEBUG -DMODEST_HTML_USE_DMT)

message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE SOURCE "../src/*.c")

file(GLOB_RECURSE TESTS "html_*_test.c")
foreach(TEST ${TESTS})
  message("TEST = ${TEST}")
  get_filename_component(TEST_NAME ${TEST} NAME_WE)
  add_executable(${TEST_NAME} ${TEST})
    target_link_libraries(
    ${TEST_NAME} 
    libmodest_static.a
  )
  target_sources(
    ${TEST_NAME} 
    PUBLIC
    ${SOURCE}
    ../libs/vec/src/vec.c
    ../libs/dmt/src/dmt.c
  )
  target_compile_definitions(
    ${TEST_NAME} 
    PRIVATE 
    MODEST_HTML_DEBUG=1 
    MODEST_HTML_USE_DMT=1
  )
endforeach()

file(GLOB_RECURSE TESTS "example*_test.c")
foreach(TEST ${TESTS})
  message("TEST = ${TEST}")
  get_filename_component(TEST_NAME ${TEST} NAME_WE)
  add_executable(${TEST_NAME} ${TEST})
  target_link_libraries(
    ${TEST_NAME} 
    libmodest_static.a 
    libmodest_html.a
  )
endforeach()