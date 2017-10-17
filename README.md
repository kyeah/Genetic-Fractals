Genetic Fractals
=================

A research tool studying the generation and evolution of aesthetically-pleasing fractals. It uses crowd-sourced testing along with observational patterns in the mathematical structure of attractor fractals to find optimal constants and equations.

This program is meant to be used in conjunction with Tyler Yates' [Java Evolutionary GUI](https://github.com/Tyler-Yates/AestheticFractals).

![Genetic Fractals](http://giant.gfycat.com/RadiantDeliciousDuckbillcat.gif)

### Dependencies

#### Linux

        sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev libpng-dev

#### OSX

        brew install freeglut3 glew libpng

You'll also need to install XQuartz if you're on a newer OSX version to pull in the X11 dependency.

### Building

        run 'make' in root folder

If you have issues linking the AntTweakBar library on compilation, follow these steps:

For Linux:

        1. Manually run 'make' in libs/AntTweakBar/src/
        2. Add the libs/AntTweakBar/lib folder to your LD_LIBRARY_PATH

For OSX:

        1. Manually run `make -f Makefile.osx` in libs/AntTweakBar/src/

Usage
============

### Rendering Mode

To open the single-fractal viewer with a randomized Clifford Attractor:

        ./aesthetics

You can also provide optional infix equation representations with vars {x,y,z}, consts {a,b,c,d}.

        ./aesthetics [ -p PRECISION POINTS] [EXPR_X EXPR_Y EXPR_Z EXPR_R EXPR_G EXPR_B]
        ./aesthetics -p 100000 "sin(-1.4 * y) + cos(-1.4 * x)" "sin(1.6 * x) + 0.7 * cos(1.6 * y)" "x" "x" "y" "z"

Controls:
* f - Toggle Fullscreen
* s - Save image to test.ppm

### Image Save Mode

In addition to rendering mode, you can call the program to draw and save fractals offscreen with optional parameters:

        ./aesthetics -save [ -p PRECISION_POINTS ] [ -s WIDTH HEIGHT ] IMG_NAME EXPR_X EXPR_Y EXPR_Z EXPR_R EXPR_G EXPR_B

-------------------------------

Kevin Yeh and Tyler Yates © 2013 (UT Freshman Research Initiative: Computational Intelligence)
