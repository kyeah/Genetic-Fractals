#include <pthread.h>
#include <signal.h>

#include "common.h"
#include "fractal.h"
#include "mainWindow.h"
#include "rng.h"

int PRECISION_POINTS = 1000000;
float preAlphaVal = 90000 / (float)PRECISION_POINTS;
float ALPHA = (preAlphaVal > 1 ? 1 : preAlphaVal);

void setPrecisionPoints(int points) {
  PRECISION_POINTS = points;
  preAlphaVal = 40000 / (float)PRECISION_POINTS;
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
    vals[0] = x;
    vals[1] = y;
    vals[2] = z;

    x = f->expressionX->evaluate(vals);
    y = f->expressionY->evaluate(vals);

    if (f->expressionZ) {
      z = f->expressionZ->evaluate(vals);
      if (z < f->minZ) f->minZ = z;
      if (z > f->maxZ) f->maxZ = z;
    }

    if (x < f->minX) f->minX = x;
    if (x > f->maxX) f->maxX = x;

    if (y < f->minY) f->minY = y;
    if (y > f->maxY) f->maxY = y;

    if (f->expressionR)
      r = f->expressionR->evaluate(vals);

    if (f->expressionG)
      g = f->expressionG->evaluate(vals);

    if (f->expressionB)
      b = f->expressionB->evaluate(vals);


    Vec3f p = {(float)x, (float)y, (float)z};
    Vec4f c = {(float)r, (float)g, (float)b, alpha};
    f->points.push_back(p);
    f->colors.push_back(c);
  }

  f->bb.min = Vec3f::makeVec(f->minX, f->minY, f->minZ);
  f->bb.max = Vec3f::makeVec(f->maxX, f->maxY, f->maxZ);
  f->isCalculated = true;
  f->alive = false;
  waiting = false;

  if (rendering) {
    glutPostRedisplay();
  }
}

void AttractorFractal::calculate() {
  isCalculated = false;
  if (!alive) {
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
    glVertexPointer(3, GL_FLOAT, sizeof(Vec3f), points.data());

    if (grayscale) {
      glColor3f(1,1,1);
      glDrawArrays(GL_POINTS, 0, getNumPoints());
    } else {
      glEnableClientState(GL_COLOR_ARRAY);
      glColorPointer(4, GL_FLOAT, sizeof(Vec4f), colors.data());
      glDrawArrays(GL_TRIANGLES, 0, getNumPoints());
      glDisableClientState(GL_COLOR_ARRAY);
    }

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    return true;
  }
}

bool AttractorFractal::paintSpline() {
  static vector<int> starts;
  static vector<int> widths;
  static vector<int> delay_starts;
  static int delay = 80;
  static int order = 10;
  static float segments = 100.0;

  if (starts.size() == 0) {
    for (int i = 0; i < 1000; i++) {
      starts.push_back(rand() % (getNumPoints() / order));
      delay_starts.push_back(rand() % delay);
      widths.push_back((rand() % 5) + 2);
      widths.push_back((rand() % 5) + 2);
    }
  }

  if (!isReady()) {
    calculate();
    return false;
  } else {
    int max = getNumPoints() / order;
    for (int s = 0; s < starts.size(); s++)  {

      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, order, (float*)&points[(order-1)*starts[s]]);
      glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, order, (float*)&colors[(order-1)*starts[s]]);
      glEnable(GL_MAP1_COLOR_4);
      glEnable(GL_MAP1_VERTEX_3);

      glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
      glEnable(GL_LINE_SMOOTH);

      glLineWidth(widths[s * 2]);
      glBegin(GL_LINE_STRIP);
      for (int uInt = (delay_starts[s] * (segments/delay)); uInt <= segments; uInt++)
        {                                   
          GLfloat u = uInt/segments; 
          glEvalCoord1f(u);                 
        }                                   
      glEnd();

      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, order, (float*)&points[(order-1)*(starts[s]+1)]);
      glMap1f(GL_MAP1_COLOR_4, 0.0, 1.0, 4, order, (float*)&colors[(order-1)*(starts[s]+1)]);
      glEnable(GL_MAP1_COLOR_4);
      glEnable(GL_MAP1_VERTEX_3);
      
      glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
      glEnable(GL_POINT_SMOOTH);
      glLineWidth(widths[s * 2 + 1]);
      /*glBegin(GL_LINE_STRIP);
      
      for (int uInt = 0; uInt <= (delay_starts[s] * (segments/delay)); uInt++)
        {                                   
          GLfloat u = uInt/segments; 
          glEvalCoord1f(u);                 
        }                                   
      glEnd();*/
      glMapGrid1f(1000, 0.0, 1.0);
      glEvalMesh1(GL_POINT, 0, 1000);

      delay_starts[s]++;
      if (delay_starts[s] > delay) {
      delay_starts[s] = 0;
      starts[s] += 1;
      if (starts[s] > max - 100) {
        starts[s] = 0;
      }
      widths[2*s] = widths[2*s + 1];
      widths[2*s+1] = (rand() % 5) + 2;
      }
    }

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
