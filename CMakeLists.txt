cmake_minimum_required(VERSION 3.16)
project(
  ImagePlayMain
  VERSION 1.0
  LANGUAGES C CXX)

include(GNUInstallDirs)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

add_subdirectory(IPL)
add_subdirectory(ImagePlay)
