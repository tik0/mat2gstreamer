cmake_minimum_required (VERSION 2.8)

# Set the project name to the name of the folder
string (REGEX MATCH "[^/]+$" PROJECT_NAME "${CMAKE_CURRENT_BINARY_DIR}")
message (STATUS "Set PROJECT_NAME to ${PROJECT_NAME}")

project ("${PROJECT_NAME}")

find_package(OpenCV 3.1)


# Set the include path
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)

add_executable ("${PROJECT_NAME}" 
    main.cpp
)
    

target_link_libraries (
    "${PROJECT_NAME}"
    ${OpenCV_LIBS}
)

SET (
   CMAKE_CXX_FLAGS 
   "-Wno-deprecated-declarations -Wno-write-strings -O2 -std=c++11"
)

SET (
   CMAKE_CXX_FLAGS_DEBUG 
   "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -g -std=c++11"
)

SET (
   CMAKE_CXX_FLAGS_RELEASE 
   "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2 -std=c++11"
)