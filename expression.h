#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <unordered_set>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

#define TYPE_UNARY 0
#define TYPE_BINARY 1
#define TYPE_TERNARY 2
#define TYPE_NUM 3
#define TYPE_VAR 4

/*
  Node
  =====
  UnaryNode :: {"sin", "cos", "tan", "abs", "!"}
  BinaryNode :: {"+", "-", "*", "/", "^", "%"}
  TernaryNode :: {"if"}
  VarNode :: Variables (x,y,z) and unspecified constants (a,b,c,d)
  NumNode :: Specified constants ("1.5")

  Expression
  ===========
  Parse Tree Representation Class

*/

class Node {
 public:
  static const unordered_set<string> unary_ops;
  static const unordered_set<string> binary_ops;
  static const unordered_set<string> ternary_ops;

 public:
  Node() {}
 Node(string _val): value(_val) {}

 protected:
  string value;
  int type;

 public:
  string getValue() { return value; }
  bool isLeaf() { return !isOp(); }
  bool isVar() { return type == TYPE_VAR; }
  bool isNum() { return type == TYPE_NUM; }
  bool isOp() { return isUnaryOp() || isBinaryOp() || isTernaryOp(); }
  bool isUnaryOp() { return type == TYPE_UNARY; }
  bool isBinaryOp() { return type == TYPE_BINARY; }
  bool isTernaryOp() { return type == TYPE_TERNARY; }
  double evaluate(int n, int nc, vector<string> *vars, vector<string> *consts,
                  vector<double> *values, vector<double> *constVals);

  virtual double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals) = 0;
  virtual void printTree() = 0;
  virtual void printTreeRPN() = 0;
};

////////////////////////////////////////////////////////////////////////////////

class UnaryNode : public Node {
 public:
 UnaryNode(string _val): Node(_val) {
    center = NULL;
    type = TYPE_UNARY;
  }

 private:
  Node *center;

 public:
  Node *getCenter() { return center; }
  Node *setCenter(Node *newNode) {
    Node *prev = center;
    center = newNode;
    return center;
  }

  double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals);
  void printTree();
  void printTreeRPN();
};

////////////////////////////////////////////////////////////////////////////////

class BinaryNode : public Node {
 public:
 BinaryNode(string _val): Node(_val) {
    left = NULL;
    right = NULL;
    type = TYPE_BINARY;
  }

 private:
  Node *left, *right;

 public:
  Node *getLeft() { return left; }
  Node *getRight() { return right; }

  Node *setLeft(Node *newNode) {
    Node *prev = left;
    left = newNode;
    return prev;
  }
  Node *setRight(Node *newNode) {
    Node *prev = right;
    right = newNode;
    return prev;
  }

  double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals);
  void printTree();
  void printTreeRPN();
};

////////////////////////////////////////////////////////////////////////////////

class TernaryNode : public Node {
 public:
 TernaryNode(string _val): Node(_val) {
    left = NULL;
    center = NULL;
    right = NULL;
    type = TYPE_TERNARY;
  }

 private:
  Node *left, *center, *right;

 public:
  Node *getLeft() { return left; }
  Node *getCenter() { return center; }
  Node *getRight() { return right; }

  Node *setLeft(Node *newNode) {
    Node *prev = left;
    left = newNode;
    return prev;
  }
  Node *setCenter(Node *newNode) {
    Node *prev = center;
    center = newNode;
    return prev;
  }
  Node *setRight(Node *newNode) {
    Node *prev = right;
    right = newNode;
    return prev;
  }

  double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals);
  void printTree();
  void printTreeRPN();

};

////////////////////////////////////////////////////////////////////////////////

class VarNode : public Node {
 public:
 VarNode(string _val): Node(_val) {
    type = TYPE_VAR;
  }

  double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals);
  void printTree();
  void printTreeRPN();
};

////////////////////////////////////////////////////////////////////////////////

class NumNode : public Node {
 public:
 NumNode(string _val, double _cval): Node(_val), constval(_cval) {
    type = TYPE_NUM;
  }

 private:
  double constval;

 public:
  double setConstVal(double newVal) {
    double prev = constval;
    constval = prev;
    return prev;
  }

  double getConstVal() { return constval; }

  double evalTree(int numVars, int numConsts, vector<string> *vars, vector<string> *consts,
                          vector<double> *values, vector<double> *constVals);
  void printTree();
  void printTreeRPN();
};

////////////////////////////////////////////////////////////////////////////////

class Expression {
 public:
  Expression(string infixExpression, vector<string> consts, vector<string> vars);

  string infixString;

  Node *root;
  vector<string> vars;
  vector<string> consts;
  vector<double> constVals;
  int numVars;
  int numConsts;

  void print();
  void printRPN();
  void printConstants();
  void printInfixString();

  void setString(string s);
  string getString() const;

  void createTree(vector<string> tokens);
  double evaluate(vector<double> values);

  void constructConstants();
  void mutateConstants();
};

Node* randNode();
Node* randOp();
Node* randBinaryOp();
Node* randUnaryOp();
Node* randLeaf();
Expression randExpression();

Node* createNode(string _val);


#endif
