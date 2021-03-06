#ifndef __FRACTAL_H__
#define __FRACTAL_H__

#include <climits>
#include <atomic>
#include <pthread.h>

#include "expression.h"
#include "vec.h"
#include "bb.h"

void setPrecisionPoints(int points);
void setAlpha(float a);

extern int PRECISION_POINTS;
extern float ALPHA;

class AttractorFractal {
 protected:
  string name;
  Expression *expressionX, *expressionY, *expressionZ;
  Expression *expressionR, *expressionG, *expressionB;

  double minX, minY, maxX, maxY, minZ, maxZ;
  BoundingBox bb;
  bool isCalculated;
  vector<Vec3f> points;
  vector<Vec4f> colors;
  pthread_t calcThread;

 public:
  atomic_bool alive;

 AttractorFractal(string _name) : name(_name), alive(false) { }

 AttractorFractal(string _name, string x, string y, string z = "0", string r = "1", string g = "1", string b = "1") : name(_name) {
    alive = false;
    vector<string> vars = {"x", "y", "z", "r", "g", "b"};

    expressionX = new Expression(x, vector<string>(), vars);
    expressionY = new Expression(y, vector<string>(), vars);
    expressionZ = new Expression(z, vector<string>(), vars);
    expressionR = new Expression(r, vector<string>(), vars);
    expressionG = new Expression(g, vector<string>(), vars);
    expressionB = new Expression(b, vector<string>(), vars);

    clear();
    calculate();
  }
  
 AttractorFractal(string _name, Expression* ex, Expression* ey, Expression* ez = 0, Expression* er = 0, Expression* eg = 0, Expression* eb = 0)
   : expressionX(ex), expressionY(ey), expressionZ(ez), expressionR(er), expressionG(eg), expressionB(eb), name(_name) {
    alive = false;
    clear();
    calculate();
  }

  static void TW_CALL setXStr(const void *value, void *clientData);
  static void TW_CALL setYStr(const void *value, void *clientData);
  static void TW_CALL setZStr(const void *value, void *clientData);
  static void TW_CALL setRStr(const void *value, void *clientData);
  static void TW_CALL setGStr(const void *value, void *clientData);
  static void TW_CALL setBStr(const void *value, void *clientData);

  static void TW_CALL getXStr(void *value, void *clientData);
  static void TW_CALL getYStr(void *value, void *clientData);
  static void TW_CALL getZStr(void *value, void *clientData);
  static void TW_CALL getRStr(void *value, void *clientData);
  static void TW_CALL getGStr(void *value, void *clientData);
  static void TW_CALL getBStr(void *value, void *clientData);

  BoundingBox getbb() { return bb; }
  bool isReady() { return isCalculated; } 
  int getNumPoints() { return points.size(); }
  string getName() { return name; }
  pthread_t getCalcThread() { return calcThread; }
  static void* calculateAsync(void*);
  void calculate();
  bool paint();
  void clear();
  void saveToFile(string name);
  void printInfo();
};

class CliffordAttractor : public AttractorFractal {  
 public:
  vector<string> consts;
  vector<float> constVals;

 CliffordAttractor(string _name, string x, string y, string z = "0", string r = "1", string g = "1", string b = "1") : AttractorFractal(_name) {

    consts = {"a", "b", "c", "d"};
    vector<string> vars = {"x", "y", "z", "r", "g", "b"};

    expressionX = new Expression(x, consts, vars);
    expressionY = new Expression(y, consts, vars);
    expressionZ = new Expression(z, consts, vars);
    expressionR = new Expression(r, consts, vars);
    expressionG = new Expression(g, consts, vars);
    expressionB = new Expression(b, consts, vars);

    clear();
    constructConstants();
    calculate();
  }

  void constructConstants();
  void mutateConstants();
};

#endif
