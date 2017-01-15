cmake_minimum_required(VERSION 3.5)
project(arcadeui CXX)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
set(CMAKE_MODULE_PATH .)

set(SOURCES 
	src/main.cpp
	src/CGL.hpp
	src/pngloader.hpp
	src/pngloader.cpp
	src/Model.cpp
	src/Model.h
	src/GLVAO.cpp
	src/GLVAO.h)

find_package(glfw3)

add_executable(gltest ${SOURCES})

target_compile_features(gltest PRIVATE cxx_lambdas cxx_nullptr cxx_range_for cxx_uniform_initialization)

target_link_libraries(gltest ${GLFW3_LIBRARY} GL GLEW png)
