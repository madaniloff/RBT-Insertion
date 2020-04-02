//This program creates a binary search tree which can add, remove, and search nodes
//Author: Mark Daniloff
//Date: 3/18/2020

#include <iostream>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

//Struct for node
struct Node {
  Node* right;
  Node* left;
  Node* next;
  int data;
};

//Struct for printing the tree
//Credit to Stefan Ene and techiedelight website for helping with the implementation
struct Trunk {
  Trunk *prev;
  char* str;

  Trunk(Trunk *prev, char* str) {
    this->prev = prev;
    this->str = str;
  }
};
//Function initializations
void manualInput(Node* &head);
void fileInput(Node* &head);
void parseInput(Node* &head, char* input, int* token, int* heaparr, int num, int count, int total, int exp, int j);
void insert(int* arr, int n, Node* &head);
void buildTree(int val, Node* &head, Node* current);
void showTrunks(Trunk *p);
void printTree(Node* current, Trunk *prev, bool isLeft);
bool searchTree(Node* current, int val);
void remove(Node* &current, int val);

int main() {
  //Declare nodes to null
  Node* head = NULL;
  Node* right = NULL;
  Node* left = NULL;
  //The first half of this project is basically the same as heap
  bool running = true;
  int mainInput;
  int mainInput2;
  int mainInput3;
  int mainInput4;
  //Ask user for input
  cout << "Enter 1 to enter through keyboard, and 2 to enter through a file" << endl;
  cin >> mainInput;
  cin.ignore();
  //Manual input
  if (mainInput == 1) {
    manualInput(head);
  }
  //File input
  if (mainInput == 2) {
    fileInput(head);
  }
  //Ask user if they want to add, remove, search, display, or quit
  while (running == true) {
    cout << "Enter 1 to add, 2 to remove, 3 to search, 4 to display, or 5 to quit" << endl;
    cin >> mainInput2;
    cin.ignore();
    //Add
    if (mainInput2 == 1) {
      manualInput(head);
    }
    //Remove
    if (mainInput2 == 2) {
      //Prompt user for input
      cout << "Enter the number you would like to delete: " << endl;
      cin >> mainInput4;
      cin.ignore();
      remove(head, mainInput4);
    }
    //Search
    if (mainInput2 == 3) {
	//Prompt user for input
	cout << "Enter a number to search for: " << endl;
	cin >> mainInput3;
	cin.ignore();
	//If the number exists
	if (searchTree(head, mainInput3) == true) {
	    cout << "This number exists!" << endl;	
	}
	//If the number doesn't exist
	if (searchTree(head, mainInput3) == false) {
	     cout << "This number does not exist!" << endl;	
	}
    }
    //Display
    if (mainInput2 == 4) {
      printTree(head, NULL, false);
    }
    //Quit
    if (mainInput2 == 5) {
      running = false;
    }
  }
}

//Keyboard entry
void manualInput(Node* &head) {
  char input[100];
  int token[100];
  int ftoken[100];
  //Ask for input
  cout << "Enter a series of numbers separated by spaces" << endl;
  cin.get(input, 100);
  int count = 0;
  int total = 0;
  int num = 0;
  int exp = 0;
  int j = 0;
  //Parse the input
  parseInput(head, input, token, ftoken, num, count, total, exp, j);
}

//File entry
void fileInput(Node* &head) {
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
    parseInput(head, input, token, ftoken, num, count, total, exp, j);
  }
  //If the file can't be opened
  else {
    cout << "The file can't be opened" << endl;
  }
}

//Parse the input
//I used the same parsing method here that I did in Heap
void parseInput(Node* &head, char* input, int* token, int* heaparr, int num, int count, int total, int exp, int j) {
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
  insert(heaparr, count, head);
  printTree(head, NULL, false);
}

//Insert nodes into tree
void insert(int* heaparr, int n, Node* &head) {
  for (int i = 0; i < n; i++) {
    buildTree(heaparr[i], head, head);
  }
}
//Build tree
void buildTree(int val, Node* &head, Node* current) {
  //If the head is NULL
  if (head == NULL) {
    //Set the head to the first index in the array
    head = new Node();
    head->data = val;
  }
  //If the head isn't NULL
  else if (head != NULL) {
    //If val is greater than or equal to current->data
    if (val >= current->data) {
      //If current->right is NULL
      if (current->right == NULL) {
	current->right = new Node();
	current->right->data = val;
      }
      //If current->right isn't NULL
      else if (current->right != NULL) {
	buildTree(val, head, current->right);
      }
    }
    //If val is less than current->data
    else if (val < current->data) {
      //If current->left is NULL
      if (current->left == NULL) {
	current->left = new Node();
	current->left->data = val;
      }
      //If current->left isn't NULL
      else if (current->left != NULL) {
	buildTree(val, head, current->left);
      }
    }
  }
}
//Search the tree
bool searchTree(Node* current, int val) {
  //If the val is equal to current->data
  if (val == current->data) {
    return true;
  }
  //If the val is greater than or equal to current->data
  else if (val >= current->data) {
    //If current->right isn't NULL
    if (current->right != NULL) {
      searchTree(current->right, val);
    }
    else {
      return false;
    }
  }
  //If the val is less than current->data
  else if (val < current->data) {
    //If current->left isn't NULL
    if (current->left != NULL) {
      searchTree(current->left, val);
    }
    else {
      return false;
    }
  }
}

//Remove a node from the tree
void remove(Node* &current, int val) {
  //Check if the value exists in the tree
  if (searchTree(current, val) == false) {
    cout << "This value doesn't exist!" << endl;
  }
  //If the value does exist
  else {
    if (val == current->data) {
      //If the node is a leaf node
      if (current->left == NULL && current->right == NULL) {
	delete current;
	current = NULL;
      }
      //If the node has one child
      else if ((current->right == NULL && current->left != NULL) || (current->left == NULL && current->right != NULL)) {
	//Right child exists
	if (current->left == NULL) {
	  Node* temp = current;
	  current = current->right;
	  delete temp;
	  temp = NULL;
	}
	//Left child exists
	else if (current->right == NULL) {
	  Node* temp = current;
	  current = current->left;
	  delete temp;
	  temp = NULL;
	}
      }
      //If the node has two children
      else if (current->right != NULL && current->left != NULL) {
	//Find the minimum value in the right subtree
	Node* temp = current->right;
	//While the right tree doesn't have a left value (so that its the minimum value)
	while (temp->left != NULL) {
	  temp = temp->left;
	}
	current->data = temp->data;
	//Call the function again to delete the duplicate node
	remove(current->right, temp->data);
      }
    }
    //If the val is greater than or equal to current->data
    else if (val >= current->data) {
      remove(current->right, val);
    }
    //If the val is less than current->data
    else if (val < current->data) {
      remove(current->left, val);
    }
  }
}

//These print tree functions are partially from techiedelight with help from Stefan Ene, period 3
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
  cout << current->data << endl;
  //If prev == true
  if (prev) {
    prev->str = prev_str;
  }
  trunk->str = (char*)("   |");
  printTree(current->right, trunk, false);
} 
