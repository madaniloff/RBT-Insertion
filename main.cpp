//This program creates a red black tree, which can add or delete nodes either from input or from a file
//Author: Mark Daniloff
//Date: 5/11/2020

#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

//Struct for node
struct Node {
  Node* parent;
  Node* right;
  Node* left;
  int data;
  char color;
  //The following functions are from Geeks for Geeks
  //www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
  //Check if node is left child of parent
  bool isOnLeft() {
    return this == parent->left;
  }
  bool isOnRight() {
    return this == parent->right;
  }
  bool hasRedChild() {
      return (left != NULL and left->color == 'R') or
	(right != NULL and right->color == 'R');
    }
  //Returns pointer to sibling
  Node *sibling() {
    //If no parent sibling == NULL
    if (parent == NULL) {
      return NULL;
    }
    else if (isOnLeft()) {
      return parent->right;
    }
    else if (isOnRight()) {
      return parent->left;
    }
  }
  //Moves node down and moves given node in its place
  void moveDown(Node *nParent) {
    if (parent != NULL) {
      if (isOnLeft()) {
	parent->left = nParent;
      }
      else {
	parent->right = nParent;
      }
    }
    nParent->parent = parent;
    parent = nParent;
  }
};

//Struct for printing tree
//Same method from binary search tree, credit to techiedelight website for implementation
struct Trunk {
  Trunk *prev;
  char* str;

  Trunk(Trunk *prev, char *str) {
    this->prev = prev;
    this->str = str;
  }
};

//Function initializations
void manualInput(Node* &head, Node* &current, Node* &prev);
void fileInput(Node* &head, Node* &current, Node* &prev);
void Parse(Node* &head, Node* &current, Node* &prev, char* input, int* token, int* heaparr, int num, int count, int total, int exp, int j);
void printTree(Node* current, Trunk* prev, bool isLeft);
void showTrunks(Trunk *p);
void numInsert(Node* &head, Node* &current, Node* &prev, int num);
void fileInsert(Node* &head, Node* &current, Node* &prev, int* arr, int n);
void fixTree(Node* &head, Node* &current);
void rotateRight(Node* &head, Node* &current);
void rotateLeft(Node* &head, Node* &current);
bool Search(Node* current, int num);
void Remove(Node* &head, Node* &current, int num);
void fixDoubleBlack(Node* &head, Node* c);
void leftRotate(Node* &head, Node* c);
void rightRotate(Node* &head, Node* c);
void swapValues(Node* c, Node* s);
Node* BSTreplace(Node* c);
Node* successor(Node* c);

int main() {
  //Set nodes to NULL
  Node* head = NULL;
  Node* current = NULL;
  Node* prev = NULL;
  Node* right = NULL;
  Node* left = NULL;
  bool running = true;
  int input;
  //Ask user for input
  while (running == true) {
    cout << "Enter 1 to add a single number, 2 to read in from a file, 3 to display the tree, 4 to search the tree, 5 to delete a node, or 6 to quit" << endl;
    cin >> input;
    cin.ignore();
    //Single number
    if (input == 1) {
      manualInput(head, current, prev);
    }
    else if (input == 2) {
      fileInput(head, current, prev);
    }
    //Display
    else if (input == 3) {
      printTree(head, NULL, false);
    }
    //Search
    else if (input == 4) {
      //Ask for input
      cout << "Enter the number you want to search for" << endl;
      cin >> input;
      cin.ignore();
      //If node exists
      if (Search(head, input) == true) {
	cout << "This node exists!" << endl;
      }
      //If node doesn't exist
      else if (Search(head, input) == false) {
	cout << "This node does not exist!" << endl;
      }
    }
    //Remove
    else if (input == 5) {
      //Ask for input
      cout << "Enter the number you want to delete" << endl;
      cin >> input;
      cin.ignore();
      //If node exists
      if (Search(head,input) == true) {
	current = head;
	Remove(head, current, input);
      }
      //If node doesn't exist
      else if (Search(head, input) == false) {
	cout << "This node does not exist!" << endl;
      }
    }
    //Quit
    else if (input == 6) {
      running = false;
    }
  }
} 

//Enter a single number
void manualInput(Node* &head, Node* &current, Node* &prev) {
  //Prompt user
  int mInput;
  cout << "Enter a number: " << endl;
  cin >> mInput;
  cin.ignore();
  current = head;
  numInsert(head, current, prev, mInput);
}

//Read input from a file
//This is from my heap and binary search tree project
void fileInput(Node* &head, Node* &current, Node* &prev) {
  //Initialize variables
  char input[100];
  int token[100];
  int ftoken[100];
  int count = 0;
  int total = 0;
  int num = 0;
  int exp = 0;
  int j = 0;
  //Ask user for filename
  char filename[100];
  cout << "Enter the name of the file " << endl;
  cin.get(filename, 100);
  cin.ignore();
  //This code is from Stefan Ene, period 3, who in turn got it from Omar Nassar. This code reads in a file
  streampos size;
  //in opens the file, binary makes sure new line characters aren't translated,
  //and ate means that the output position starts at the end of the file
  ifstream file(filename, ios::in | ios::binary | ios::ate);
  //If the file exists
  if (file.is_open()) {
    //Get input position
    size = file.tellg();
    //Seek to 0 characters from the beginning of the file
    file.seekg(0, ios::beg);
    //Read the file and assign to input
    file.read(input, size);
    //Close the file
    file.close();
    //Parse the input
    Parse(head, current, prev, input, token, ftoken, num, count, total, exp, j);
  }
  //If the file can't be opened
  else {
    cout << "The file can't be opened" << endl;
  }
} 

//Parse input, reused from my binary search tree project
void Parse(Node* &head, Node* &current, Node* &prev, char* input, int* token, int* heaparr, int num, int count, int total, int exp, int j) {
    int outputarr[100];
  //Set the arrays to all zeros
  for (int i = 0; i < 100; i++) {
    heaparr[i] = 0;
    outputarr[i] = 0;
    token[i] = 0;
  }
  for (int i = 0; i < 100; i++) {
    //Null character
    if (input[i] == '\0') {
      i = 100;
    }
    //Single digit number
    else if (isdigit(input[i]) && !isdigit(input[i+1]) && input[i] != '\0') {
      token[i] = (int)input[i] - 48;
    }
    //Multiple digit number
    else if (isdigit(input[i]) && isdigit(input[i+1])) {
      //While input[i] isn't a space
      while (isdigit(input[i])) {
	count++;
	i++;
      }
      //If input[i] is a space
      if (!isdigit(input[i])) {
	for (int j = count; j > 0; j--) {
	  num = input[i-j] - 48;
	  exp = pow(10, j-1);
	  total += num * exp;
	}
      }
      token[i] = total;
      total = count = 0;
    }
  }
  heaparr[0] = 0;
  //Place parsed values into another array with no zeros
  for (int i = 0; i < 100; i++) {
    if (token[i] != 0) {
      heaparr[j] = token[i];
      j++;
      count++;
    }
  }
  fileInsert(head, current, prev, heaparr, count);
}

//Insert one number into tree
void numInsert(Node* &head, Node* &current, Node* &prev, int num) {
  //If head is NULL
  if (head == NULL) {
    //Create new node and set its color to black
    head = new Node();
    head->data = num;
    head->color = 'B';
  }
  //If head isn't NULL
  else if (head != NULL) {
    //If val is greater than or equal to current->data
    if (num >= current->data) {
      prev = current;
      current = current->right;
      //If current->right is NULL
      if (current == NULL) {
	//Create new node and set the color to red
	current = new Node();
	current->data = num;
	current->color = 'R';
	prev->right = current;
	current->parent = prev;
	//Balance the red black tree
	fixTree(head, current);
      }
      //If current->right != NULL
      else if (current != NULL) {
	numInsert(head, current, prev, num);
      }
    }
    //If val is less than current->data
    else if (num < current->data) {
      prev = current;
      current = current->left;
      //If current->left is NULL
      if (current == NULL) {
	//Create new node and set the color to red
	current = new Node();
	current->data = num;
	current->color = 'R';
	prev->left = current;
	current->parent = prev;
	//Balance the red black tree
	fixTree(head, current);
      }
      //If current->left isn't NULL
      else if (current != NULL) {
	numInsert(head, current, prev, num);
      }
    }
  }
}

//Insert numbers from a file
void fileInsert(Node* &head, Node* &current, Node* &prev, int* arr, int n) {
  for (int i = 0; i < n; i++) {
    current = head;
    numInsert(head, current, prev, arr[i]);
  }
}

//These print tree functions are partially from techiedelight with help from Stefan Ene, period 3
//I'm reusing these methods from my binary search tree project
//www.com/techiedelight.com/c-program-print-binary-tree
//www.com/github.com/Stefanene/Binary-Search-Tree/blob/master/main.cpp
//Print branches of the binary tree
void showTrunks(Trunk *p) {
  //If the trunk is empty
  if (p == NULL) {
    return;
  }
  //Recursively call the function
  showTrunks(p->prev);
  cout << p->str;
}

//Print the tree
void printTree(Node* current, Trunk* prev, bool isLeft) {
  //If the current is empty
  if (current == NULL) {
    //End the functiom
    return;
  }
  //Create a tab character
  char* prev_str = (char*)("    ");
  Trunk *trunk = new Trunk(prev, prev_str);
  printTree(current->left, trunk, true);
  //If prev isn't 0 but is false
  if (!prev) {
    trunk->str = (char*)("---");
  }
  else if (isLeft == true) {
    trunk->str = (char*)(".---");
    prev_str = (char*)("   |");
  }
  else {
    trunk->str = (char*)("`---");
    prev->str = prev_str;
  }
  showTrunks(trunk);
  cout << current->data << " " <<  current->color << endl;
  //If prev == true
  if (prev) {
    prev->str = prev_str;
  }
  trunk->str = (char*)("   |");
  printTree(current->right, trunk, false);
}

//Red Black Tree Balancing functions are partially from GeeksforGeeks
//www.geeksforgeeks.org/c-program-red-black-tree-insertion
//Fix red black tree insertion violations
void fixTree(Node* &head, Node* &current) {
  //Create nodes
  Node* nparent = NULL;
  Node* grandparent = NULL;
  while ((current != head) && (current->color != 'B') && (current->parent->color == 'R')) {
    nparent = current->parent;
    grandparent = current->parent->parent;
    //If Parent is left child of grandparent
    if (nparent == grandparent->left) {
      //Create uncle node
      Node* uncle = grandparent->right;
      //If uncle is red (recolor)
      if (uncle != NULL && uncle->color == 'R') {
	grandparent->color = 'R';
	nparent->color = 'B';
	uncle->color = 'B';
	current = grandparent;
      }
      else {
	//If current is right child of parent (left rotation)
	if (current == nparent->right) {
	  rotateLeft(head, nparent);
	  current = nparent;
	  nparent = current->parent;
	}
	//If current is left child of parent (right rotation)
	rotateRight(head, grandparent);
	//Swap the colors
	swap(nparent->color, grandparent->color);
	current = nparent;
      }
    }
    //If parent is right child of grandparent
    else {
      //Create uncle node
      Node* uncle = grandparent->left;
      //If uncle is red (recolor)
      if (uncle != NULL && uncle->color == 'R') {
	grandparent->color = 'R';
	nparent->color = 'B';
	uncle->color = 'B';
	current = grandparent;
      }
      else {
	//If current is left child of parent (right rotation)
	if (current == nparent->left) {
	  rotateRight(head, nparent);
	  current = nparent;
	  nparent = current->parent;
	}
	//If current is right child of parent (left rotation)
	rotateLeft(head, grandparent);
	swap(nparent->color, grandparent->color);
	current = nparent;
      }
    }
  }
  head->color = 'B';
}

//Rotate the tree right
void rotateRight(Node* &head, Node* &current) {
  Node* currentleft = current->left;
  current->left = currentleft->right;
  //If current's left isn't NULL
  if (current->left != NULL) {
    current->left->parent = current;
  }
  currentleft->parent = current->parent;
  //If current's parent is NULL
  if (current->parent == NULL) {
    head = currentleft;
  }
  //If current is equal to current's parent's left
  else if (current == current->parent->left) {
    current->parent->left = currentleft;
  }
  else {
    current->parent->right = currentleft;
  }
  currentleft->right = current;
  current->parent = currentleft;
}

//Rotate the tree left
void rotateLeft(Node* &head, Node* &current) {
  Node* currentright = current->right;
  current->right = currentright->left;
  //If current's right isn't NULL
  if (current->right != NULL) {
    current->right->parent = current;
  }
  currentright->parent = current->parent;
  //If current's parent is NULL
  if (current->parent == NULL) {
    head = currentright;
  }
  //If current is equal to current's parent's left
  else if (current == current->parent->left) {
    current->parent->left = currentright;
  }
  else {
    current->parent->right = currentright;
  }
  currentright->left = current;
  current->parent = currentright;
}

//Search the tree for a node
bool Search(Node* current, int num) {
  //If the number is found
  if (current->data == num) {
    return true;
  }
  //If the current data is greater than num
  else if (current->data > num) {
    if (current->left != NULL) {
      Search(current->left, num);
    }
    else {
      return false;
    }
  }
  //If the current data is less than num
  else if (current->data < num) {
    if (current->right != NULL) {
      Search(current->right, num);
    }
    else {
      return false;
    }
  }
  //If the number doesn't exist
  else {
    return false;
  }
}

//Delete a node
//The following functions are partially from Geeks for Geeks tutorial
//www.geeksforgeeks.org/red-black-tree-set-3-delete-2
void Remove(Node* &head, Node* &current, int num) {
  Node* u = BSTreplace(current);
  Node* parent = current->parent;
  //If num is the head data
  if (num == current->data) {
    //Red node 
    if (current->color == 'R') {
      //Leaf node
      if (current->left == NULL && current->right == NULL) {
	//If sibling isn't NULL color red
	if (current->sibling() != NULL) {
	  current->sibling()->color = 'R';
	}
	delete current;
	current = NULL;
      }
      //One child
      else if (current->left == NULL || current->right == NULL) {
	//Head
	if (current == head) {
	  current->data = u->data;
	  current->left = current->right = NULL;
	  delete u;
	  u = NULL;
	}
	//If not head
	else {
	  if (current->isOnLeft()) {
	    parent->left = u;
	  }
	  else {
	    parent->right = u;
	  }
	  delete current;
	  current = NULL;
	  u->parent = parent;
	  //If current and u are both black
	  if (current->color == 'B' && u->color == 'B') {
	    fixDoubleBlack(head, u);
	  }
	  //Current or u is black
	  else {
	    //Color u black
	    u->color = 'B';
	  }
	}
      }
      //Two children
      else if (current->left != NULL && current->right != NULL) {
	//Swap current with minimum value
	Node* temp = current->right;
	while (temp->left != NULL) {
	  temp = temp->left;
	}
	//Swap the values
	swapValues(temp, current);
	Remove(head, current->right, temp->data);
      }
    }
    //Black
    else if (current->color == 'B') {
      //Leaf
      if (current->left == NULL && current->right == NULL) {
	//Head
	if (current->data == head->data) {
	  delete head;
	  head = NULL;
	}
	else {
	  fixDoubleBlack(head, current);
	  delete current;
	  current = NULL;
	}
      }
      //One child
      else if (current->left == NULL || current->right == NULL) {
	//Head
	if (current == head) {
	  current->data = u->data;
	  current->left = current->right = NULL;
	  delete u;
	}
	//If not head
	else {
	  if (current->isOnLeft()) {
	    parent->left = u;
	  }
	  else {
	    parent->right = u;
	  }
	  delete current;
	  u->parent = parent;
	  //If current and u are both black
	  if (current->color == 'B' && u->color == 'B') {
	    fixDoubleBlack(head, u);
	  }
	  //Current or u is black
	  else {
	    //Color u black
	    u->color = 'B';
	  }
	}
      }
      //Two children
      else if (current->left != NULL && current->right != NULL) {
	//Swap current with minimum value
	Node* temp = current->right;
	while (temp->left != NULL) {
	  temp = temp->left;
	}
	//Swap the values
	swapValues(temp, current);
	Remove(head, current->right, temp->data);
      }
    }
  }
  //If num is greater than current data
  else if (num > current->data) {
    Remove(head, current->right, num);
  }
  //If num is less than current data
  else if (num < current->data) {
    Remove(head, current->left, num);
  }
}

//Fix double black violation
void fixDoubleBlack(Node* &head, Node* c) {
  if (c == head) {
    return;
  }
  else {
    Node* sibling = c->sibling();
    Node* parent = c->parent;
    //No siblings
    if (sibling == NULL) {
      //Push up to parent
      fixDoubleBlack(head, parent);
    }
    else {
      //Red sibling
      if (sibling->color == 'R') {
	parent->color = 'R';
	sibling->color = 'B';
	//Left case
	if (sibling->isOnLeft()) {
	  rightRotate(head, parent);
	}
	//Right case
	else {
	  leftRotate(head, parent);
	}
	fixDoubleBlack(head, c);
      }
      //Black sibling
      else  {
	//At least 1 red child
	if (sibling->hasRedChild()) {
	  //Red node is sibling's left
	  if (sibling->left != NULL && sibling->left->color == 'R') {
	    //Left left case: red node is sibling's left and sibling is parent's left
	    if (sibling->isOnLeft()) {
	      sibling->left->color = sibling->color;
	      sibling->color = parent->color;
	      rightRotate(head, parent);
	    }
	    //Right left case: red node is sibling's left and sibling is parent's right
	    else {
	      sibling->left->color = parent->color;
	      rightRotate(head, sibling);
	      leftRotate(head, parent);
	    }
	  }
	  //Red node is sibling's right
	  else {
	    //Left right case: red node is sibling's right and sibling is parent's left
	    if (sibling->isOnLeft()) {
	      sibling->right->color = parent->color;
	      leftRotate(head, sibling);
	      rightRotate(head, parent);
	    }
	    //Right right case: red node is sibling's right and sibling is parent's right
	    else {
	      sibling->right->color = sibling->color;
	      sibling->color = parent->color;
	      leftRotate(head, parent);
	    }
	  }
	  parent->color = 'B';
	}
	//Two black children
	else {
	  sibling->color = 'R';
	  if (parent->color == 'B') {
	    fixDoubleBlack(head, parent);
	  }
	  else {
	    parent->color = 'B';
	  }
	}
      }
    }
  }
}


//Left rotate the node
void leftRotate(Node* &head, Node *c) {
  //new parent will be node's right child
  Node* nParent = c->right;
  //Update head if c is head
  if (c == head) {
    head = nParent;
  }
  c->moveDown(nParent);
  //Connect c with nParent's left 
  c->right = nParent->left;
  //Connect nParent's left with node if it's not NULL
  if (nParent->left != NULL) {
    nParent->left->parent = c;
  }
  //Connect nParent with c
  nParent->left = c;
}

//Right rotate the node
void rightRotate(Node* &head, Node* c) {
  //New parent will be node's left child
  Node* nParent = c->left;
  //Update head if c is head
  if (c == head) {
    head = nParent;
  }
  c->moveDown(nParent);
  //Connect c with nParent's right
  c->left = nParent->right;
  //Connect nParent's right with node if it's not NULL
  if (nParent->right != NULL) {
    nParent->right->parent = c;
  }
  //Connect nParent with c
  nParent->right = c;
}

//Swaps values
 void swapValues(Node* c, Node* s) {
   int temp;
   temp = c->data;
   c->data = s->data;
   s->data = temp;
 }

//Finds node that replaces a deleted node in BST
Node* BSTreplace(Node* c){
  //Two children
  if (c->left != NULL && c->right != NULL) {
    return successor(c->right);
  }
  //Leaf
  if (c->left == NULL && c->right == NULL) {
    return NULL;
  }
  //Single child
  if (c->left != NULL) {
    return c->left;
  }
  else {
    return c->right;
  }
}

//Finds node that doesn't have a left child in the subtree of the given node
Node *successor(Node *c) {
  Node* temp = c;
  while (temp->left != NULL) {
    temp = temp->left;
  }
  return temp;
}
