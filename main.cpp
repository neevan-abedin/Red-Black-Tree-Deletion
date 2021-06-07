/*
Red Black Tree Deletion by Nabhan Abedin
6.06.2021
*/

#include <iostream>
#include <cstring>
#include <fstream>
//set colors of tree nodes
//color codes from https://pkg.go.dev/github.com/whitedevops/colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREY    "\033[90m"

#include "node.h"

using namespace std;

//define trunks to represent each level of tree
struct Trunk {
  Trunk *prev;
  char* str;

  Trunk(Trunk *prev, char* str) {
    this->prev = prev;
    this->str = str;
  }
};

//for printing each level of tree
void showTrunks(Trunk *level) {
  if (level == NULL) {
    return;
  }

  showTrunks(level -> prev);

  cout << level -> str;
}

//function prototypes
void addNumber(Node* &head, Node* &curr, Node*& prev, int val);
void readFile(Node* &head);
void printTree(Node* root, Trunk *prev, bool isLeft);
void deleteNumber(Node* &head, Node* &v);
bool searchNumber(Node* curr, int val);
void parse(char* in, int* parsedInput, int &count);

//balancing prototypes
void balance(Node* &head, Node* &curr);
void rotateLeft(Node* &head, Node* &curr);
void rotateRight(Node* &head, Node* &curr);

//deletion prototypes
Node* getSibling(Node* &x);
Node* successor(Node* &x);
Node* replaceNode(Node* &x);
void balanceDoubleBlack(Node* &head, Node* &x);
bool hasRed(Node* &x);
void swapNodeValues(Node* &u, Node* &v);

int main() {
  char input[10]; //user input
  char read[10000];  //file input
  int parsedInput[100]; //parsed input for insertion
  Node* head = NULL;
  bool running = true;
  while (running) {
    cout << endl << "What do you want to do? ('add', 'read', 'print', 'search', 'delete', or 'quit')" << endl;
    cin.get(input, 10);
    cin.clear();
    cin.ignore(10000, '\n');
    if (strcmp(input, "add") == 0) {
      int val;
      cout << "Enter a value: ";
      cin >> val;
      cin.clear();
      cin.ignore(10000, '\n');
      Node* curr = head;
      Node* prev = NULL;
      addNumber(head, curr, prev, val);
      cout << endl << val << " was added." << endl;
    } else if (strcmp(input, "read") == 0) {
      readFile(head);
    } else if (strcmp(input, "print") == 0) {
      printTree(head, NULL, false);
    } else if (strcmp(input, "search") == 0) {
      int num;
      cout << "Input value to search for: ";
      cin >> num;
      cin.clear();
      cin.ignore(10000, '\n');
      bool s = searchNumber(head, num);
      if (s == true) {
	      cout << num << " is in the tree." << endl;
      } else {
	      cout << num << " is NOT in the tree." << endl;
      }
    } else if (strcmp(input, "delete") == 0) {
      while (true) 
      {
        int num = 0;
        cout << "Input value to delete: ";
        cin >> num;
        cin.clear();
        cin.ignore(10000, '\n');
        bool d = searchNumber(head, num);
        if (d == false) {
          cout << endl << "Value not found, try agian." << endl;
        } 
        else {
          //find node to be deleted
          Node* v = head;
          while (v->getData() != num) 
          {
            if (num < v->getData()) 
            {
              v = v->getLeft();
            } 
            else if (num > v->getData()) 
            {
              v = v->getRight();
            }
          }
          deleteNumber(head, v);
          cout << num << " was deleted." << endl;
          break;
        }
    }} else if (strcmp(input, "quit") == 0) {
      cout << endl << "Goodbye." << endl;
      running = false;
    } else {
      cout << endl << "Invalid input. Try again." << endl;
    }
  }
  return 0;
}

//balancing from https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
void balance(Node* &head, Node* &curr) {
  Node* parent = NULL;
  Node* grandparent = NULL;
  while ((curr != head) && (curr->getColor() != 0) && ((curr->getParent())->getColor() == 1)) {
    parent = curr->getParent();
    grandparent = parent->getParent();
    if (parent == grandparent->getLeft()) { //Case 1: parent is the left child of grandparent
      Node* uncle = grandparent->getRight();
      if (uncle != NULL && uncle->getColor() != 0) { //Case 1.1: uncle is red, then recolor
        grandparent->setColor(1);//red
        parent->setColor(0);//black
        uncle->setColor(0);//black
        curr = grandparent;
      } else { //Case 1.2: curr is the right child of parent, then rotate left
        if (curr == parent->getRight()) { //Case 1.3: curr is the left child of parent, then rotate right
            rotateLeft(head, parent);
            curr = parent;
            parent =  curr->getParent();
        }
        rotateRight(head, grandparent);
        //swap colors of parent and grandparent
        int tempC = parent->getColor();
        parent->setColor(grandparent->getColor());
        grandparent->setColor(tempC);
        curr = parent;
      }
    } else { //Case 2: parent is the right child of grandparent
      Node* uncle = grandparent->getLeft();
      if (uncle != NULL && uncle->getColor() != 0) { //Case 2.1: uncle is red, then recolor
        grandparent->setColor(1);//red
        parent->setColor(0);//black
        uncle->setColor(0);//black
        curr = grandparent;
      } else { //Case 2.2: curr is the left child of parent, then rotate right
        if (curr == parent->getLeft()) { //Case 2.3: curr is the right child of parent, then rotate left
            rotateRight(head, parent);
            curr = parent;
            parent = curr->getParent();
    	}
        rotateLeft(head, grandparent);
        //swap color of parent and grandparent
        int tempC = parent->getColor();
        parent->setColor(grandparent->getColor());
        grandparent->setColor(tempC);
        curr = parent;
      }
    }
  }
  head->setColor(0);
}

void rotateLeft(Node* &head, Node* &curr) {
  Node* rightPtr = curr->getRight(); 
  curr->setRight(rightPtr->getLeft()); 
  if (curr->getRight() != NULL) {
    (curr->getRight())->setParent(curr); 
  }
  rightPtr->setParent(curr->getParent());
  if (curr->getParent() == NULL) {
    head = rightPtr; 
  } else if (curr == (curr->getParent())->getLeft()) {
    (curr->getParent())->setLeft(rightPtr); 
  } else {
    (curr->getParent())->setRight(rightPtr); 
  }  
  rightPtr->setLeft(curr); 
  curr->setParent(rightPtr);
}

void rotateRight(Node* &head, Node* &curr) {
  Node *leftPtr = curr->getLeft(); 
  curr->setLeft(leftPtr->getRight()); 
  if (curr->getLeft() != NULL) {
    (curr->getLeft())->setParent(curr); 
  }  
  leftPtr->setParent(curr->getParent());
  if (curr->getParent() == NULL) {
    head = leftPtr; 
  } else if (curr == (curr->getParent())->getLeft()) {
    (curr->getParent())->setLeft(leftPtr); 
  } else {
    (curr->getParent())->setRight(leftPtr);
  }
  leftPtr->setRight(curr); 
  curr->setParent(leftPtr);
}

void addNumber(Node* &head, Node* &curr, Node* &prev, int val) {
  if (head == NULL) {
    head = new Node();
    curr = head;
    head->setData(val);
    head->setColor(0);  //head is black
    return;
  } else {
    if (val < curr->getData()) {  
      //lower value goes to left
      prev = curr;
      curr = curr->getLeft();
      if (curr == NULL) {  
        //if empty, insert value
        curr = new Node();
        curr->setData(val);
        prev->setLeft(curr);
        curr->setParent(prev);
        balance(head, curr);  //balance
        return;
      } else {  //if not empty, keep adding
	    addNumber(head, curr, prev, val);
      }
    } else {
      prev = curr;
      curr = curr->getRight();
        if (curr == NULL) {
        //if empty, insert value
        curr = new Node();
        curr->setData(val);
        prev->setRight(curr);
        curr->setParent(prev);
        balance(head, curr);  //balance
        return;
      } else {  //if not empty, keep adding
	    addNumber(head, curr, prev, val);
      }
    }
  }
}

void readFile(Node* &head) {
  char in[10000];
  char fileName[100];
  int parsedInput[100];
  int siz = 0;
  int count = 0;
  memset(in, 0, 10000);
  memset(fileName, 0, 100);
  memset(parsedInput, 0, 100);

  //get file name
  cout << endl << "What is the name of the file?" << endl;
  cin.get(fileName, 30);
  cin.clear();
  cin.ignore(100000, '\n');
  streampos size;
  ifstream file(fileName, ios::in | ios::binary | ios::ate);
  if (file.is_open()) {
    size = file.tellg();
    file.seekg(0, ios::beg);
    file.read(in, size);
    file.close();
    parse(in, parsedInput, count);  //parse input to modify array
    cout << "IN: ";
    for (int i = 0; i < 100; i++) {
      if (parsedInput[i] != 0) {
        cout << parsedInput[i] << " ";
        siz++;
      }
    }
    cout << endl << "Size: " << siz << endl;;
    //add to tree
    Node* curr = NULL;
    Node* prev = NULL;
    for (int i = 0; i < siz; i++) {
      if(parsedInput[i] == 0) {
        break;
      } 
      curr = head;
      addNumber(head, curr, prev, parsedInput[i]);
    }
  } 
}

//print tree
void printTree(Node* root, Trunk *prev, bool isLeft) {
  if (root == NULL) {
    return;
  }
  char* prev_str = (char*)("    ");
  Trunk *trunk = new Trunk(prev, prev_str);
  printTree(root->getLeft(), trunk, true);
  if (!prev) {
    trunk -> str = (char*)("---");
  } else if (isLeft) {
    trunk -> str = (char*)(".---");
    prev_str = (char*)("   |");
  } else {
    trunk -> str = (char*)("`---");
    prev -> str = prev_str;
  }
  showTrunks(trunk);
  if (root->getColor() == 0) {  
    //black nodes
    cout << GREY << root->getData() << RESET << endl;
  } else {  
    //red nodes
    cout << RED << root->getData() << RESET << endl;
  } if (prev) {
    prev -> str = prev_str;
  }
  trunk -> str = (char*)("   |");
  printTree(root->getRight(), trunk, false);
}

void parse(char* in, int* parsedInput, int &count) {
  int l = 0; //tracks how many digits each value is
  for (int i = 0; i < strlen(in); i++) {
    if (in[i] == ' ') {
      if (l == 1) {
        int temp = 0;
        temp = in[i-1] - '0';
        parsedInput[count] = temp;
        count++;
        l = 0;
      } else {
        int temp = 0;
        for (int a = 0; a < l; a++) {
          temp = 10 * temp + (in[i-l+a] - '0');
        }
        parsedInput[count] = temp;
        count++;
        l = 0;
      }
    } else {
      int temp = 0;
      l++;
      if (i == strlen(in) - 1) {  
        //last pair of chars
        for (int a = 0; a < l; a++) {
            temp = 10 * temp + (in[i+a+1-l] - '0');
        }
        parsedInput[count] = temp;
        count++;
      }
    }
  } 
}

bool searchNumber(Node* curr, int val) {
  if (val == curr->getData()) {
    return true;
  } else if (val < curr->getData()) {  //lower goes left
    if (curr->getLeft() != NULL) {
      searchNumber(curr->getLeft(), val);  //recursion
    } else {
      return false;
    }
  } else {  //higher goes right
    if (curr->getRight() != NULL) {
      searchNumber(curr->getRight(), val);  //recursion
    } else {
      return false;
    }
  }
}

void swapValue(Node* &u, Node* &v) { 
  //swap int values between the two given nodes
  int temp; 
  temp = u->getData(); 
  u->setData(v->getData()); 
  v->setData(temp); 
}

Node* getSibling(Node* &v) {
  if (v->getParent() == NULL) {
    return NULL;
  } if (v == v->getParent()->getLeft()) {
    return v->getParent()->getRight();
  } else {
    return v->getParent()->getLeft();
  }
}

Node* replaceNode(Node* &x) {
  //2 children
  if (x->getLeft() != NULL && x->getRight() != NULL) {
    Node* r = x->getRight();
    return successor(r); 
  }
  //no children
  else if (x->getLeft() == NULL && x->getRight() == NULL) 
  {
    return NULL;
  }
  //1 child 
  else {
    if (x->getLeft() != NULL) 
    { 
      return x->getLeft();
    } else {
      return x->getRight();
    }
  }
}

Node* successor(Node* &x) 
{
  //get leftmost value of right subtree
  Node* a = x;
  while (a->getLeft() != NULL) 
  {
    a = a->getLeft();
  }
  return a;
}

void deleteNumber(Node* &head, Node* &v) {
  Node* u = replaceNode(v);
  Node* parent = v->getParent();
  //track if there is doubleblack
  bool bothBlack = ((u==NULL || u->getColor()==0) && (v==NULL || v->getColor()==0));
  if (u == NULL) {
    //then v has no children
    if (v == head) {
      //make head null
      head = NULL;
    } else {
      if (bothBlack) {
      	balanceDoubleBlack(head, v);
      } else {
            if (getSibling(v) != NULL) {
              getSibling(v)->setColor(1); //set red
            }
        }
            //delete v
            if (v == parent->getLeft()) 
            {
              parent->setLeft(NULL);
            } 
            else 
            {
              parent->setRight(NULL);
            }
          }
          v->~Node();
    return;
  }

  //if v has 1 child
  if (v->getRight() == NULL || v->getLeft() == NULL) 
  {
    if (v == head) 
    {
      //replace u with v
      v->setData(u->getData());
      v->setLeft(NULL);
      v->setRight(NULL);
      //delete u
      u->~Node();
    } 
    else 
    {
      //detach v from tree and move u up
      if (v == parent->getLeft()) 
      {
	      parent->setLeft(u);
      } 
      else 
      {
	      parent->setRight(u);
      }
      //delete v
      v->~Node();
      u->setParent(parent);
      if(bothBlack) 
      {
	      balanceDoubleBlack(head, u);
      } 
      else 
      {
	      //if one is red, color u black
	      u->setColor(0);
      }
    }
    return;
  }

  //swap with successor and recurse
  swapValue(u, v);
  deleteNumber(head, u);
}

//doubleblack
void balanceDoubleBlack(Node* &head, Node* &x) {
  if (x == head)
    return;

  Node* sibling = getSibling(x);
  Node* parent = x->getParent();

  if (sibling == NULL) 
  {
    //if no sibling, push doubleblack up
    balanceDoubleBlack(head, parent);
  } 
  else 
  {
    if (sibling->getColor() == 1) 
    {
      //red sibling
      parent->setColor(1); //set red
      sibling->setColor(0); //set black
      if (sibling == parent->getLeft()) 
      {
	      rotateRight(head, parent);
      } 
      else 
      {
	      rotateLeft(head, parent);
      }
      balanceDoubleBlack(head, x);
    }  
    else 
    {
      //black sibling
      if (hasRed(sibling)) 
      {
        //has at least 1 red child
        if (sibling->getLeft() != NULL && sibling->getLeft()->getColor() == 1) 
        {
          //sibling's left child is red
          if (sibling == parent->getLeft()) 
          {
            //left left
            sibling->getLeft()->setColor(sibling->getColor());
            sibling->setColor(parent->getColor());
            rotateRight(head, parent);
          } 
          else
          {
            //right left
            sibling->getLeft()->setColor(parent->getColor());
            rotateRight(head, sibling);
            rotateLeft(head, parent);
          }
        } 
        else 
        {
          //sibling's right child is red
          if (sibling == parent->getLeft()) 
          {
            //left right
            sibling->getRight()->setColor(parent->getColor());
            rotateLeft(head, sibling);
            rotateRight(head, parent);
          } 
          else 
          {
            //right right
            sibling->getRight()->setColor(sibling->getColor());
            sibling->setColor(parent->getColor());
            rotateLeft(head, parent);
          }
        }
        parent->setColor(0); //set black
      } 
      else 
      {
	    //two black children
        sibling->setColor(1); //set red
        if (parent->getColor() == 0) 
        {
          balanceDoubleBlack(head, parent); //recursion
        } 
        else 
        {
          parent->setColor(0); //set black
        }
      }
    }
  }
}

bool hasRed(Node* &x) { 
  if (x->getLeft() != NULL && x->getLeft()->getColor() == 1) 
  {
    return true;
  }
   else if (x->getRight() != NULL && x->getRight()->getColor() == 1) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}