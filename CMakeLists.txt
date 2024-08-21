if (WIN32)
    set(CMAKE_GENERATOR "MinGW Makefiles")    
    set(CMAKE_CXX_COMPILER g++)
else()
    set(CMAKE_CXX_COMPILER gcc)
endif()

cmake_minimum_required(VERSION 3.20.0)

project(darwin VERSION 1.0)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/build/${PROJECT_NAME}")

add_subdirectory(cpp)

install(TARGETS darwin DESTINATION dll)
