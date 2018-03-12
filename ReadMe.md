### Introduce

This is a drawing board using OpenGL, allowing users to 
draw lines/rectangles/ellipse/polygons and drag them, 
offering saving/loading project function.

I would be very grateful it if you could offer some practical advice for this project.

### Compilation

First you need to install OpenGL, if you are compiling in Linux systems, install the following:
```
apt-get install freeglut3-dev
apt-get install libxi-dev
apt-get install libxmu-dev
```
then just simply clone this repo, using CLion to open its directory and build.

For Windows systems, install **freeglut**, modify CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.8)
project(Board)

set(CMAKE_CXX_STANDARD 11)

add_executable(Board main.cpp Board.cpp Board.h EShape.cpp EShape.h main.h drawtext.h ELine.cpp ELine.h ERect.cpp ERect.h EEllipse.cpp EEllipse.h EPolygon.cpp EPolygon.h)

target_link_libraries(Board freeglut glu32 opengl32)
```
and then run the building process.