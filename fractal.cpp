#include "fractal.h"

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __MAC__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

const int PRECISION_POINTS = 1000000;
const float preAlphaVal = 60000 / (float)PRECISION_POINTS;
const float ALPHA = (preAlphaVal > 1 ? 1 : preAlphaVal);

void AttractorFractal::calculate() {
  isCalculated = false;

  double x, y;
  x = y = 0.0;
  
  vector<double> vals = {x, y};
  for (int i=0; i < PRECISION_POINTS; i++) {
    vals[0] = x;
    vals[1] = y;

    x = expressionX->evaluate(vals);
    y = expressionY->evaluate(vals);
    
    if (x < minX) minX = x;
    if (x > maxX) maxX = x;

    if (y < minY) minY = y;
    if (y > maxY) maxY = y;

    Vec3f p = {(float)x, (float)y, 0};
    
    points.push_back(p);
  }

  isCalculated = true;
}

void AttractorFractal::paint() {
  if (!isReady())
    calculate();
  
  glColor4f(1,1,1, ALPHA); 
    
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), points.data());
  glDrawArrays(GL_POINTS, 0, getNumPoints());

  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);
}
