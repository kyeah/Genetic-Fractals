cmake_minimum_required(VERSION 3.8)
project(Genetic_Fractals)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        bb.h
        color.cpp
        common.h
        expression.cpp
        expression.h
        expressionParser.cpp
        expressionParser.h
        fbo.cpp
        fbo.h
        fractal.cpp
        fractal.h
        fractalEditor.cpp
        fractalEditor.h
        main.cpp
        mainWindow.cpp
        mainWindow.h
        Queue.h
        rng.h
        tw.h
        types.h
        vec.h)

add_executable(Genetic_Fractals_Source_Inspect ${SOURCE_FILES})
add_custom_target(Genetic_Fractals ALL COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
