#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__
#include "common.h"
#include "fractal.h"

static float fadeAlpha = 0.0;
static const float fadeSpeed = 0.07;

int createMainWindow(string _name, bool saving=false);
void Repaint();
void adjustBounds(AttractorFractal& f);
void registerCallbacks();
void resize(int w, int h);

void Reshape(int w, int h);
void resize();
void Idle();
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void Keyboard(unsigned char key, int x, int y);
Vec3f arcSnap(float x, float y);

#endif
