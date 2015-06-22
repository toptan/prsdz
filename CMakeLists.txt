cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ttdzprs)
set(MINIMUM_BOOST_VERSION "1.54")
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

if(NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH 0)
endif(NOT DEFINED CMAKE_MACOSX_RPATH)

find_package(Boost ${MINIMUM_BOOST_VERSION} COMPONENTS program_options REQUIRED)

# Adding executable
add_definitions("-std=c++11 -Wall -Wextra -pedantic -Weffc++")
add_definitions("-Wmissing-braces -Wnon-virtual-dtor -Wshadow")
file(GLOB PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(ttdzprs ${PROJECT_SOURCES})
include_directories(ttdzprs ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(ttdzprs ${Boost_INCLUDE_DIRS})
target_link_libraries(ttdzprs ${Boost_LIBRARIES})


