cmake_minimum_required(VERSION 3.9)
project(Board)
set(CMAKE_CXX_STANDARD 11)

add_executable(Board main.cpp Board.cpp Board.h EShape.cpp EShape.h main.h drawtext.h ELine.cpp ELine.h ERect.cpp ERect.h EEllipse.cpp EEllipse.h EPolygon.cpp EPolygon.h)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(Board ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})