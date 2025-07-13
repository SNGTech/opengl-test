cmake_minimum_required(VERSION 3.16)
project(opengl_test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(opengl_test 
  src/main.cpp
  src/shader.cpp
  src/gl.c
)

target_link_libraries(opengl_test 
  ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
  OpenGL::OpenGL
)

set (DATA_SOURCE "${CMAKE_SOURCE_DIR}/src/data")
set (DATA_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/data")
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${DATA_SOURCE} ${DATA_DESTINATION}
  COMMENT "Symbolic linked data directory from ${DATA_SOURCE} to ${DATA_DESTINATION}"
)
