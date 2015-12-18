cmake_minimum_required(VERSION 3.3)
project(mandelbrot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    generate.cpp
    mandel.cpp)
find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(mandelbrot ${SOURCE_FILES})
target_link_libraries(mandelbrot ${FLTK_LIBRARIES})
target_link_libraries(mandelbrot ${OPENGL_LIBRARIES})
target_link_libraries(mandelbrot ${CMAKE_DL_LIBS})
