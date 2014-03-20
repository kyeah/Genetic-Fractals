#include "common.h"
#include <AntTweakBar.h>

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

#include <pthread.h>
#include <signal.h>

#include "fractal.h"
#include "rng.h"

int PRECISION_POINTS = 1000;
float preAlphaVal = 90000 / (float)PRECISION_POINTS;
float ALPHA = (preAlphaVal > 1 ? 1 : preAlphaVal);

void setPrecisionPoints(int points) {
  PRECISION_POINTS = points;
  preAlphaVal = 60000 / (float)PRECISION_POINTS;
  ALPHA = (preAlphaVal > 1 ? 1 : preAlphaVal);
}

void setAlpha(float a) {
  ALPHA = (a > 1 ? 1 : a);
}

void* AttractorFractal::calculateAsync(void* args) {
  AttractorFractal *f = (AttractorFractal*)args;
  double x, y, z;
  float r, g, b;
  x = y = z = 0.0;
  r = g = b = 1.0;

  vector<double> vals = {x, y, z, r, g, b};
  int pp = PRECISION_POINTS;
  float alpha = ALPHA;
  for (int i=0; i < pp; i++) {
    cout << "test" << endl;
    vals[0] = x;
    vals[1] = y;
    vals[2] = z;

    cout << "test exp x" << endl;
    x = f->expressionX->evaluate(vals);

    cout << "test exp y" << endl;
    y = f->expressionY->evaluate(vals);

    cout << "test exp z" << endl;
    if (f->expressionZ) {
      z = f->expressionZ->evaluate(vals);
    cout << "test minmaxz" << endl;
      if (z < f->minZ) f->minZ = z;
      if (z > f->maxZ) f->maxZ = z;
    }

    cout << "test minmaxx" << endl;
    if (x < f->minX) f->minX = x;
    if (x > f->maxX) f->maxX = x;

    cout << "test minmaxy" << endl;
    if (y < f->minY) f->minY = y;
    if (y > f->maxY) f->maxY = y;

    cout << "test minmaxrgb" << endl;
    if (f->expressionR)
      r = f->expressionR->evaluate(vals);

    if (f->expressionG)
      g = f->expressionG->evaluate(vals);

    if (f->expressionB)
      b = f->expressionB->evaluate(vals);


    Vec3f p = {(float)x, (float)y, (float)z};
    Vec4f c = {(float)r, (float)g, (float)b, alpha};

    cout << "test points" << endl;
    f->points.push_back(p);
    cout << "test colorss" << endl;
    f->colors.push_back(c);
    cout << "test pp" << endl;
  }

  f->bb.min = Vec3f::makeVec(f->minX, f->minY, f->minZ);
  f->bb.max = Vec3f::makeVec(f->maxX, f->maxY, f->maxZ);
  f->isCalculated = true;
  f->alive = false;
  waiting = false;
  glutPostRedisplay();
}

void AttractorFractal::calculate() {
  isCalculated = false;
  cout << "LIVE?" << endl;
  cout << alive << endl;
  if (!alive) {
    cout << "LIVEE" << endl;
    alive = true;
    pthread_create(&calcThread, NULL, calculateAsync, this);
  }
}

bool AttractorFractal::paint() {
  if (!isReady()) {
    calculate();
    return false;
  } else {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), points.data());
    glColorPointer(4, GL_FLOAT, sizeof(Vec4f), colors.data());
    
    glDrawArrays(GL_POINTS, 0, getNumPoints());
    
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    return true;
  }
}

void AttractorFractal::clear() {
  points.clear();
  colors.clear();
  minX = minY = INT_MAX;
  maxX = maxY = INT_MIN;
}

void AttractorFractal::saveToFile(string name) {
  string fn = name + ".info";
  std::ofstream out(fn);
  std::streambuf *coutbuf = cout.rdbuf();
  cout.rdbuf(out.rdbuf());

  printInfo();
  cout.rdbuf(coutbuf);
}

void AttractorFractal::printInfo() {
  cout << "ExpressionX: ";
  expressionX->printInfixString();
  expressionX->printConstants();
  cout << "ExpressionY: ";
  expressionY->printInfixString();
  expressionY->printConstants();
  if (expressionZ) {
    cout << "ExpressionZ: ";
    expressionZ->printInfixString();
    expressionZ->printConstants();
  }
  if (expressionR) {
    cout << "ExpressionR: ";
    expressionR->printInfixString();
    expressionR->printConstants();
  }
  if (expressionG) {
    cout << "ExpressionG: ";
    expressionG->printInfixString();
    expressionG->printConstants();
  }
  if (expressionB) {
    cout << "ExpressionB: ";
    expressionB->printInfixString();
    expressionB->printConstants();
  }
}

void CliffordAttractor::constructConstants() {
  //  while (1) {
  constVals.clear();
  expressionX->constVals.clear();
  expressionY->constVals.clear();
  if (expressionZ) expressionZ->constVals.clear();

  for (int i = 0; i < consts.size(); i++) {
    constVals.push_back(gen_random_float(-PI, 3*PI));
  }

  for (int i = 0; i < expressionX->numConsts; i++)
    expressionX->constVals.push_back( constVals[i] );

  for (int i = 0; i < expressionY->numConsts; i++)
    expressionY->constVals.push_back( constVals[i] );

  if (expressionZ) {
    for (int i = 0; i < expressionY->numConsts; i++)
      expressionZ->constVals.push_back( constVals[i] );
  }

  /* Constants evaluation
     double x = 0;
     double y = 0;

     int good = 100;
     for (int i = 0; i < 100; i++) {
     vector<double> vals{x,y};
     double nx = expressionX->evaluate(vals);
     double ny = expressionY->evaluate(vals);

     double d = pow(x-nx, 2) + pow(y-ny, 2);
     }*/
  //}
}

void CliffordAttractor::mutateConstants() {
  constructConstants();
  clear();
  calculate();
}
