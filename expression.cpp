#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <cmath>

#include "libs/expressionParser.h"
#include "expression.h"

using namespace std;

Expression::Expression(string infixExpression) {
  root = NULL;
  vector<string> rpnTokens;
  addVar("x");
  addVar("y");
  infixStringToRPN(infixExpression, &rpnTokens);
  createTree(rpnTokens);
}

void Expression::createTree(vector<string> tokens) {
  stack<Node*> stack;

  for (vector<string>::iterator itr = tokens.begin(); itr != tokens.end(); ++itr) {
    Node *n = new Node(*itr);
    if (n->isBinaryOp()) {
      Node *right = stack.top(); stack.pop();
      Node *left = stack.top(); stack.pop();
      n->setLeft(left);
      n->setRight(right);
    } else if (n->isUnaryOp()) {
      Node *right = stack.top(); stack.pop();
      n->setRight(right);
    }

    stack.push(n);
  }

  root = stack.top(); stack.pop();
}

double Expression::evaluate(double x, double y, double z) {
  return evalTree(root, x, y, z);
}

double Expression::evalTree(Node *n, double x, double y, double z) {
  if (!n) return -1;

  string token = n->getValue();
  if (n->isLeaf()) {
    if (token == "x")      return x;
    else if (token == "y") return y;
    else                   return stod(token);
  }
  
  if (n->isBinaryOp()) {
    if (token == "+")      return evalTree(n->getLeft(), x, y, z) + evalTree(n->getRight(), x, y, z);
    else if (token == "-") return evalTree(n->getLeft(), x, y, z) - evalTree(n->getRight(), x, y, z);
    else if (token == "/") return evalTree(n->getLeft(), x, y, z) / evalTree(n->getRight(), x, y, z);
    else if (token == "*") return evalTree(n->getLeft(), x, y, z) * evalTree(n->getRight(), x, y, z);
    else if (token == "^") return pow(evalTree(n->getLeft(), x, y, z), evalTree(n->getRight(), x, y, z));
  } else if (n->isUnaryOp()) {
    if (token == "sin")       return sin(evalTree(n->getRight(), x, y, z));
    else if (token == "cos")  return cos(evalTree(n->getRight(), x, y, z));
    else if (token == "tan")  return tan(evalTree(n->getRight(), x, y, z));
    else if (token == "abs")  return abs(evalTree(n->getRight(), x, y, z));
  }

  return 0.0;
}

void Expression::print() { printTree(root); cout << endl; }
void Expression::printRPN() { printTreeRPN(root); cout << endl; }

void Expression::printTree(Node *n) {
  if (!n) return;
  
  printTree(n->getLeft());
  cout << n->getValue() << " ";
  printTree(n->getRight());
}

void Expression::printTreeRPN(Node *n) {
  if (!n) return;
  
  printTreeRPN(n->getLeft());
  printTreeRPN(n->getRight());
  cout << n->getValue() << " ";
}
