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
	src/GLVAO.h
	src/GLTexture.cpp
	src/GLTexture.h
	src/GLShader.cpp
	src/GLShader.h)

find_package(glfw3)
find_package(PNG)
find_package(OpenGL)

find_package(GLEW)

add_executable(gltest ${SOURCES})

include_directories(/usr/local/include ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})

target_compile_features(gltest PRIVATE cxx_lambdas cxx_nullptr cxx_range_for cxx_uniform_initialization)

if (DEFINED GLFW3_LIBRARY)
	message("Found GLFW3 Library")
else (DEFINED GLFW3_LIBRARY)
	message("Could not find GLFW3 Library, assuming it is just 'glfw'")
	set(GLFW3_LIBRARY glfw)
endif(DEFINED GLFW3_LIBRARY)

target_link_libraries(gltest ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${PNG_LIBRARIES})
