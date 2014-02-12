#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__
#include "common.h"
#include "fractal.h"

int createMainWindow(char* _name);
void Repaint();
void adjustBounds(AttractorFractal f);
void registerCallbacks();
void resize(int w, int h);

char* name;
int windowID;

void Reshape(int w, int h);
void resize();
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
Vec3f arcSnap(float x, float y);

vector<AttractorFractal> *fractals;

int window_width, window_height;
float window_aspect;
bool fullScreen=false;

// GLUT Utility Variables
float zoom = 1;
float mouse_x, mouse_y;
float arcmouse_x, arcmouse_y, arcmouse_z;

bool right_mouse_button= false;
bool left_mouse_button = false;

GLfloat rot_matrix[16] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};

#endif
