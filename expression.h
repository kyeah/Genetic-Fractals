#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

#define TYPE_UNARY 0
#define TYPE_BINARY 1
#define TYPE_NUM 2
#define TYPE_VAR 3

class Node {
 public:
 Node(string _val): value(_val) {
    static const unordered_set<string> unary_ops = {"sin", "cos", "tan", "abs"};
    static const unordered_set<string> binary_ops = {"+", "-", "*", "/", "^"};

    left = NULL;
    right = NULL;

    unary_ops.find(value);

    if (unary_ops.find(value) != unary_ops.end()) {
      type = TYPE_UNARY;
    } else if (binary_ops.find(value) != binary_ops.end()) {
      type = TYPE_BINARY;
    } else {
      char* fail;
      long l = strtol(value.c_str(), &fail, 10);
      type = (*fail ? TYPE_VAR : TYPE_NUM);
    }
  }
  
 private:
  string value;
  Node *left, *right;
  int type;
    
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

  string getValue() { return value; }
  Node *getLeft() { return left; }
  Node *getRight() { return right; }

  bool isLeaf() { return !left && !right; }
  bool isOp() { return isUnaryOp() || isBinaryOp(); }
  bool isUnaryOp() { return type == TYPE_UNARY; }
  bool isBinaryOp() { return type == TYPE_BINARY; }
};

class Expression {
 public:
  Expression(string infixExpression);
  
  Node *root;

  void print();
  void printRPN();
  void printTree(Node *n);
  void printTreeRPN(Node *n);

  void createTree(vector<string> tokens);
  double evaluate(double x, double y, double z = 0);
  double evalTree(Node *n, double x, double y, double z = 0);
};

#endif
