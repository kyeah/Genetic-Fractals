Genetic Fractals
=================

A research tool studying the generation and evolution of aesthetically-pleasing fractals. It uses crowd-sourced testing along with observational patterns in the mathematical structure of attractor fractals to find optimal constants and equations.

Dependencies
        
        sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev

Usage
============

        make

To open the single-fractal viewer with a randomized Clifford Attractor:

   ./aesthetics

Optional controls:
   f - Toggle Fullscreen
   s - Save image to test.ppm

You can also call the program to draw and save fractals offscreen with optional size parameters:
    
    .aesthetics -s <width> <height> <img name> <Expression X> <Expression Y>

-------------------------------

Visit https://bitbucket.org/terminex9/aestheticfractals for the most up-to-date version of our project, including the Java GUI.
Kevin Yeh and Tyler Yates © 2013 (UT Freshman Research Initiative: Computational Intelligence)