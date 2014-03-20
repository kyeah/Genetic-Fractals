#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>
#include <AntTweakBar.h>
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
//#include <GLFW/glfw3.h>
#endif

#include "fractal.h"
#include "Queue.h"
#pragma hdrstop

struct Color {
  Color() {}
  Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a=1.0f) {
    c[0] = _r;
    c[1] = _g;
    c[2] = _b;
    c[3] = _a;
  }
  operator GLfloat*() { return c; }
  GLfloat c[4];
};

extern Color kRed;
extern Color kGreen;
extern Color kBlue;
extern Color kYellow;
extern Color kViolet;
extern Color kBrown;
extern Color kOrange;
extern Color kBlack;
extern Color kWhite;
extern Color clearColor;

extern bool rendering;
extern int window_width, window_height;
extern int PRECISION_POINTS;
extern Queue<CliffordAttractor> fractals;
extern CliffordAttractor *mainFractal;

#endif
