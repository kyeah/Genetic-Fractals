#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stack>
#include <cmath>

#include <fcntl.h>

#include "expressionParser.h"
#include "expression.h"

using namespace std;

const unordered_set<string> Node::unary_ops = {"sin", "cos", "tan", "abs"};
const unordered_set<string> Node::binary_ops = {"+", "-", "*", "/", "^"};
const unordered_set<string> Node::ternary_ops = {"if"};

vector<string> * Node::vars;
vector<string> * Node::consts;
vector<double> * Node::values;
vector<double> * Node::constVals;
int Node::numVars;
int Node::numConsts;

/////////////////////////////////////////////////////////////////////////////////////

Node* createNode(string _val) {
  if (Node::unary_ops.find(_val) != Node::unary_ops.end()) {
    return new UnaryNode(_val);
  } else if (Node::binary_ops.find(_val) != Node::binary_ops.end()) {
    return new BinaryNode(_val);
  } else if (Node::ternary_ops.find(_val) != Node::ternary_ops.end()) {
    return new TernaryNode(_val);
  } else {
    char* fail;
    double constval = strtod(_val.c_str(), &fail);
    if (*fail != '\0')
      return new VarNode(_val);
    else
      return new NumNode(_val, constval);
  }
}

double Node::evaluate(int n, int nc, vector<string> *v, vector<string> *c, vector<double> *vals, vector<double> *cvals) {
  numVars = n;
  numConsts = nc;
  vars = v;
  consts = c;
  values = vals;
  constVals = cvals;
  return evalTree();
}

/////////////////////////////////////////////////////////////////////////////////////

double UnaryNode::evalTree() {
  if (value == "sin")       return sin(center->evalTree());
  else if (value == "cos")  return cos(center->evalTree());
  else if (value == "tan")  return tan(center->evalTree());
  else if (value == "abs")  return abs(center->evalTree());
  else return 0.0;
}

void UnaryNode::printTree() {
  std::cout << value << "(";
  center->printTree();
  std::cout << ") ";
}

void UnaryNode::printTreeRPN() {
  center->printTreeRPN();
  std::cout << value << " ";
}

//////////////////////////////////////////////////////////////////////////////////////

double BinaryNode::evalTree() {
  if (value == "+")      return left->evalTree() + right->evalTree();
  else if (value == "-") return left->evalTree() - right->evalTree();
  else if (value == "/") return left->evalTree() / right->evalTree();
  else if (value == "*") return left->evalTree() * right->evalTree();
  else if (value == "^") return pow(left->evalTree(), right->evalTree());
  else return 0.0;
}

void BinaryNode::printTree() {
  left->printTree();
  cout << value << " ";
  right->printTree();
}

void BinaryNode::printTreeRPN() {
  left->printTreeRPN();
  right->printTreeRPN();
  cout << value << " ";
}

//////////////////////////////////////////////////////////////////////////////////////

double TernaryNode::evalTree() {
  if (value == "if") return (left->evalTree() ? center->evalTree() : right->evalTree());
  else return 0.0;
}

void TernaryNode::printTree() {
  if (value == "if") {
    cout << "[if :: (";
    left->printTree();
    cout << ") then (";
    center->printTree();
    cout << ") else (";
    right->printTree();
    cout << ") ";
  }
}

void TernaryNode::printTreeRPN() {
  left->printTreeRPN();
  center->printTreeRPN():
  right->printTreeRPN();
  cout << value << " ";
}

//////////////////////////////////////////////////////////////////////////////////////

double VarNode::evalTree() {
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

void VarNode::printTree() {
  cout << value << " ";
}

void VarNode::printTreeRPN() {
  cout << value << " ";
}

//////////////////////////////////////////////////////////////////////////////////////

double NumNode::evalTree() {
  return constval;
}

void NumNode::printTree() {
  cout << value << " ";
}

void NumNode::printTreeRPN() {
  cout << value << " ";
}

//////////////////////////////////////////////////////////////////////////////////////

Expression::Expression(string infixExpression, vector<string> consts, vector<string> vars) {
  infixString = infixExpression;

  root = NULL;
  vector<string> rpnTokens;

  // Variables that will be defined iteratively e.g. x, y, z
  this->vars = vars;
  this->numVars = vars.size();
  for (string s: vars)
    addVar(s);

  // Ambiguous constants that will be calculated before evaluation e.g. a, b, c
  this->consts = consts;
  this->numConsts = consts.size();
  for (string s: consts)
    addVar(s);

  // Represent expression with a parse tree
  infixStringToRPN(infixExpression, &rpnTokens);
  createTree(rpnTokens);
}

void Expression::createTree(vector<string> tokens) {
  stack<Node*> stack;

  for (vector<string>::iterator itr = tokens.begin(); itr != tokens.end(); ++itr) {
    Node *n = createNode(*itr);
    if (n->isBinaryOp()) {
      Node *right = stack.top(); stack.pop();
      Node *left = stack.top(); stack.pop();
      BinaryNode *bn = (BinaryNode*)n;
      bn->setLeft(left);
      bn->setRight(right);
    } else if (n->isUnaryOp()) {
      Node *center = stack.top(); stack.pop();
      UnaryNode *un = (UnaryNode*)n;
      un->setCenter(center);
    }

    stack.push(n);
  }

  root = stack.top(); stack.pop();
}

double Expression::evaluate(vector<double> values) {
  if (!root) return -1;
  return root->evaluate(numVars, numConsts, &vars, &consts, &values, &constVals);
}

void Expression::printInfixString() {
  cout << infixString << endl;
}

void Expression::printConstants() {
  for (int i = 0; i < numConsts && i < constVals.size(); i++)  {

    cout << consts[i] << " = " << constVals[i] << endl;
  }
}

void Expression::print() {
  if (root)
    root->printTree();
  cout << endl;
  printConstants();
}
void Expression::printRPN() {
  if (root)
    root->printTreeRPN();
  cout << endl;
  printConstants();
}

void Expression::setString(string s) {
  infixString = s;

  root = NULL;
  vector<string> rpnTokens;

  // Represent expression with a parse tree
  infixStringToRPN(s, &rpnTokens);
  createTree(rpnTokens);
}

string Expression::getString() const {
  return infixString;
}
