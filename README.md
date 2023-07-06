# 3DFileTool
The code in this repository is organized into several different files for which descriptions have been provided below. 

### 3d.h
This file was given by the professor and included all the headers for the methods that were required to be included in our library code.

### 3d.c
This file contains the code implementing the functions from 3d.h. It provides functions to create STL scenes and within these scenes triangles, as the filetype suggests. The library also contains functions to create a cuboid or pyramid constructed of several triangle faces. The code contains some basic comments but is fairly straightforward.

### generator.c
This file contains code that uses our created library to create a custom STL scene and save it as a file. The code is simple and shows how our functions implemented in 3d.c can be utilized to produce a complete STL file.

### generator.stl
Contains the output of generator.c as a text STL file.

### makefile
Contains the different build options used to compile this code.
