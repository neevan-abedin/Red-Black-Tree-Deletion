#include <iostream>

#include "node.h"

using namespace std;

//constructor
Node::Node() 
{
  data = 0;
  right = NULL;
  left = NULL;
  color = 1; //start with red
  parent = NULL;
}

//destructor
Node::~Node() 
{
  right = NULL;
  left = NULL;
  parent = NULL;
}

//setters
void Node::setData(int newData) 
{
  data = newData;
}

void Node::setRight(Node* newRight) 
{
  right = newRight;
}

void Node::setLeft(Node* newLeft) 
{
  left = newLeft;
}

void Node::setColor(int newColor) 
{
  color = newColor;
}

void Node::setParent(Node* newParent) 
{
  parent = newParent;
}

//getters
int Node::getData() 
{
  return data;
}

Node* Node::getRight() 
{
  return right;
}

Node* Node::getLeft() 
{
  return left;
}

int Node::getColor() 
{
  return color;
}

Node* Node::getParent() 
{
  return parent;
}