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

class Node {
 public:
  static const unordered_set<string> unary_ops;
  static const unordered_set<string> binary_ops;
  static const unordered_set<string> ternary_ops;

  // Set when calling evaluate to prevent recursive passing
  static vector<string> *vars;
  static vector<string> *consts;
  static vector<double> *values;
  static vector<double> *constVals;
  static int numVars;
  static int numConsts;

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
  double evaluate(int n, int nc, vector<string> *vars, vector<string> *consts, vector<double> *values, vector<double> *constVals);
  virtual double evalTree() = 0;
  virtual void printTree() = 0;
  virtual void printTreeRPN() = 0;
};

class UnaryNode : public Node {
 public:
 UnaryNode(string _val): Node(_val) {
    center = NULL;
    type = TYPE_UNARY;
  }

 private:
  Node *center;

 public:
  Node *setCenter(Node *newNode) {
    Node *prev = center;
    center = newNode;
    return center;
  }

  Node *getCenter() { return center; }

  double evalTree() {
    if (value == "sin")       return sin(center->evalTree());
    else if (value == "cos")  return cos(center->evalTree());
    else if (value == "tan")  return tan(center->evalTree());
    else if (value == "abs")  return abs(center->evalTree());
    else return 0.0;
  }

  void printTree() {
    std::cout << value << "(";
    center->printTree();
    std::cout << ") ";
  }

  void printTreeRPN() {
    center->printTreeRPN();
    std::cout << value << " ";
  }
};

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

  Node *getLeft() { return left; }
  Node *getRight() { return right; }

  double evalTree() {
    if (value == "+")      return left->evalTree() + right->evalTree();
    else if (value == "-") return left->evalTree() - right->evalTree();
    else if (value == "/") return left->evalTree() / right->evalTree();
    else if (value == "*") return left->evalTree() * right->evalTree();
    else if (value == "^") return pow(left->evalTree(), right->evalTree());
    else {
      return 0.0;
    }
  }

  void printTree() {
    left->printTree();
    cout << value << " ";
    right->printTree();
  }

  void printTreeRPN() {
    left->printTreeRPN();
    right->printTreeRPN();
    cout << value << " ";
  }
};

class VarNode : public Node {
 public:
 VarNode(string _val): Node(_val) {
    type = TYPE_VAR;
  }

  double evalTree() {
    for (int i = 0; i < numVars; i++) {
      if (value == (*vars)[i])
        return (*values)[i];
    }
    for (int i = 0; i < numConsts; i++) {
      if (value == (*consts)[i])
        return (*constVals)[i];
    }

    return 0.0;
  }

  void printTree() {
    cout << value << " ";
  }

  void printTreeRPN() {
    cout << value << " ";
  }
};

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
  
  double evalTree() {  
    return constval;
  }

  void printTree() {
    std::cout << value << " ";
  }

  void printTreeRPN() {
    std::cout << value << " ";
  }
};

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
  double evalTree(Node *n, vector<double> values);

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
