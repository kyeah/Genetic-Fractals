#ifndef __FRACTAL_H__
#define __FRACTAL_H__

#include <climits>

#include "expression.h"
#include "vec.h"

/*
class Fractal {
 public:
  void paint();
  void paint(int x, int y, int width, int height);
  void mutate();
  void calculate();
  bool isCalculated();
  Fractal clone();
};
*/
class AttractorFractal {
 private:
  Expression *expressionX;
  Expression *expressionY;

  double minX, minY, maxX, maxY;  
  bool isCalculated;
  vector<Vec3f> points;
  //  Vec3f *points;

 public:
  AttractorFractal(string x, string y) {
    expressionX = new Expression(x);
    expressionY = new Expression(y);
    minX = minY = INT_MAX;
    maxX = maxY = INT_MIN;
    calculate();
  }
  
 AttractorFractal(Expression* ex, Expression* ey): expressionX(ex), expressionY(ey) {
    minX = minY = INT_MAX;
    maxX = maxY = INT_MIN;
    calculate();
  }
  
  Vec4d getBounds() { return Vec4d{minX, maxX, minY, maxY }; }
  bool isReady() { return isCalculated; } 
  int getNumPoints() { return points.size(); }
  void calculate();
  void paint();

};

#endif
