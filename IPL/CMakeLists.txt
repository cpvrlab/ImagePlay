cmake_minimum_required(VERSION 3.1)

# Define project name
project(IPL)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(${${TargetName}_COMPILE_OPTIONS} -g -O0)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED TRUE)
set(CXX_EXTENSIONS FALSE)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

find_package(OpenCV 4 REQUIRED)
message(STATUS " OpenCV library status: ")
message(STATUS " config: ${OpenCV_DIR} ")
message(STATUS " version: ${OpenCV_VERSION} ")
message(STATUS " libraries: ${OpenCV_LIBS} ")
message(STATUS " include path: ${OpenCV_INCLUDE_DIRS} ")

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" ${OpenCV_INCLUDE_DIRS}
         "${CMAKE_CURRENT_SOURCE_DIR}/include/processes")

target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_LIBRARY)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} freeimage)

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH
                                                 TRUE)

if(CLANG)
  target_compile_options(${PROJECT_NAME} -openmp)
endif()

if(GCC AND NOT CLANG)
  target_link_libraries(${PROJECT_NAME} PUBLIC gomp)

  target_compile_options(${PROJECT_NAME} -fopenmp)
endif()

install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
