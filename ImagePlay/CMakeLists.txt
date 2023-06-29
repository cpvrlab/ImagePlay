cmake_minimum_required(VERSION 3.1)

# Define project name
project(ImagePlay)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(${${TargetName}_COMPILE_OPTIONS} -g -O0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE QRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/res/*.qrc")

set(PLUGIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/media/plugin_development)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${PLUGIN_SRC}
                               ${PLUGIN_HEADERS} ${QRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH
                                                 TRUE)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
          "${CMAKE_CURRENT_SOURCE_DIR}/../IPL/include/"
          "${CMAKE_CURRENT_SOURCE_DIR}/../IPL/include/processes/"
          ${OpenCV_INCLUDE_DIRS})

target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE IMAGEPLAY_APPCAST_URL="http://cpvrlab.github.io/ImagePlay/Appcast.xml"
          IMAGEPLAY_URL="http://cpvrlab.github.io/ImagePlay")

find_package(
  Qt5
  COMPONENTS Core Widgets Gui
  REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

if(TEMPLATE___equals___app)
  target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE APP_COMPANY="BFH"
            APP_COPYRIGHT=" "
            APP_DESCRIPTION=" "
            APP_NAME="ImagePlay"
            APP_PRODUCT="ImagePlay"
            APP_VERSION="6.1.0"
            LINUX_PATH_PREFIX="/usr/share/imageplay/")
endif()

if(NOT TEMPLATE___equals___app)
  target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE APP_COMPANY="BFH"
            APP_COPYRIGHT=" "
            APP_DESCRIPTION=" "
            APP_NAME="ImagePlay"
            APP_PRODUCT="${PROJECT_NAME}"
            APP_VERSION="6.1.0"
            LINUX_PATH_PREFIX="/usr/share/${PROJECT_NAME}/")
endif()

if(LINUX)
  target_link_libraries(${PROJECT_NAME} PRIVATE IPL dl)
endif()

if(GCC AND NOT CLANG)
  target_link_libraries(${PROJECT_NAME} PRIVATE gomp)
endif()

if(_ss_USE_FERVOR_UPDATER)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_FERVOR_UPDATER)
endif()

install(
  TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Consider using qt_generate_deploy_app_script() for app deployment if the
# project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
