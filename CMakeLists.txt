cmake_minimum_required(VERSION 3.16)
project(opengl_test)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(opengl_test 
  src/main.cpp
  src/gl.c
)

target_link_libraries(opengl_test 
  ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
  OpenGL::OpenGL
)
