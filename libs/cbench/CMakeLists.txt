cmake_minimum_required(VERSION 3.2)
project(cbench C)

option(BUILD_SHARED_LIBS "Build as a shared library." OFF)

include_directories(include)
install(FILES include/cbench.h DESTINATION include)
install(FILES include/hires-timer.h DESTINATION include)

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
