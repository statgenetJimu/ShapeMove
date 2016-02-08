#!/bin/bash

# OpenGL
sudo apt-get install -y mesa-common-dev libglu1-mesa-dev freeglut3-dev

#build
cd ./spinxform_stl
make
cd ..
cd ./spinxform_opengl
make
cd ..
