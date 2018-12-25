Augen Light
===========

### About

This is an OpenGL 4.5 Core "hello triangle" code using GLFW (to open a window) and glad (to load opengl). It includes the typical folder layout and CMakeLists that I use for larger projects.

The C++ code of the program is located in [`src/AugenLight`](src/AugenLight) and in particular [`src/AugenLight/main.cpp`](src/AugenLight/main.cpp) is a fully commented sample explaining the use of a VBO, a VAO and a shader program to render a dynamic scene.

### Downloading

This repository contains *submodules* to include the GLFW and glad repositories, so **do not use** the *download zip* button provided by GitHub. If you want a zip, download the [last release](https://github.com/eliemichel/AugenLight/releases/download/lvl0-v1.0/AugenLight.zip). To clone the repository using git, run:

```
git clone --recurse-submodules https://github.com/eliemichel/AugenLight.git
```

If you forgot to use the `--recurse-submodules`, you can get submodules with:

```
git submodule update --init --recursive
```

### Dependencies

Before building, you may need some additional piece of software depending on your platform.

#### Windows

On Windows, you just need [cmake](https://cmake.org/).

#### Linux

You can install the required dependencies with the following `apt` command line, or something similar on other packet managers:

```
sudo apt-get install git cmake xorg-dev
```

#### OSX

You should only need [cmake](https://cmake.org/), but this has not been tested. Feel free to let me know if anything is missing.

### Building

This is a standard CMake project. Building it consits in running:

```
mkdir build
cd build
cmake ..
```

You can chose which compiler to use in the call to `cmake`, using the `-G` option. See for instance [`build-msvc15.bat`](build-msvc15.bat) to build for Visual Studio 15 (2017), or the other build files for `mingw` or `gcc`. More easily, you can also just run one of those scripts, it will even get git submodules.

Once CMake has run, you can build the project. You can do it in command line, from the `build` directory:

```
cmake --build .
```

Or you can use the build process of your compiler, for instance running `make` (for `gcc` projects) or openning the Visual Studio solution.

### Running

The executable file is created within the `build` directory, in `src/AugenLight` or `src/AugenLight/Debug` or `src/AugenLight/Release` or something similar depending on your compiler.

### Troubleshooting

When using Visual Studio, if you encounter this error dialog when trying to run the solution generated by CMake:

![Start up issue](doc/images/startup-issue.png)

Right click on *AugenLight* in the solution explorer (right panel, in default Visual Studio window layout) and chose "Set as StartUp Project".

### License

```
Copyright (c) 2017 - 2018 -- Élie Michel <elie.michel@exppad.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

The Software is provided “as is”, without warranty of any kind, express or
implied, including but not limited to the warranties of merchantability,
fitness for a particular purpose and non-infringement. In no event shall the
authors or copyright holders be liable for any claim, damages or other
liability, whether in an action of contract, tort or otherwise, arising
from, out of or in connection with the software or the use or other dealings
in the Software.
```
