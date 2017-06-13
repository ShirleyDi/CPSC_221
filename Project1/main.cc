#include <iostream>
#include <fstream>
#include <algorithm>
#include "dynmatrix.h"

using namespace std;

// construct a string representation 

void usage() {
  cerr << "Usage: upgma species.txt" << endl;
}
void UPGMA(ClusterNode*& head, ClusterNode*& tail){
    ClusterNode* minimum1 = NULL;                             //make dummy clusterNodes to pass into findMinimum()
    ClusterNode* minimum2 = NULL;
    findMinimum(head, minimum1, minimum2);
    string species1 = minimum1 -> name;                       
    string species2 = minimum2 -> name;
    int s1 = count(species1.begin(),species1.end(),',') + 1;
    int s2 = count(species2.begin(),species2.end(),',') + 1;
    DistanceNode* temp1 = minimum1 -> row;
    DistanceNode* temp2 = minimum2 -> row;
    DistanceNode* temp3 = minimum2 -> column;
    minimum2->name = "(" + species2 + "," + species1 + ")";
    
    while (temp2) {
      if ((temp2-> nextInRow == temp3)|| (temp2 == head-> row)){
        temp2 -> distance = 0;
      } else {
        (temp2 -> distance) = ((s1 * (temp1 -> distance) + s2 * (temp2 -> distance))/(s1 + s2));
        temp3->distance = temp2 -> distance; 
      }
      temp2 = temp2 -> nextInRow;
      temp1 = temp1 -> nextInRow;
      temp3 = temp3 -> nextInColumn;
    }

    removeCluster(head, tail, minimum1);
    if ((head -> next) == tail) {
      head -> row -> distance = 0;
      tail -> row -> nextInRow -> distance = 0;
    }
}


void buildTree(TreeNode *& tree, string input) {
    TreeNode * localTree = tree;

    for (unsigned int i = 0; i < input.length(); i++) {
        if (input.at(i) == '(') {
            if (localTree->left == NULL) {
                localTree->left = new TreeNode();
                localTree->left->parent = localTree;
                localTree = localTree->left;
            }
        } else if (input.at(i) == ',') {

            if (localTree->parent->right == NULL) {
                localTree->parent->right = new TreeNode();
                localTree->parent->right->parent = localTree->parent;
                localTree = localTree->parent->right;
            }
        } else if (input.at(i) == ')') {
            localTree = localTree->parent;
        } else {
            localTree->value += input.at(i);
        }
    }
}
void appendTree(TreeNode * tree) {
    if (tree != NULL) {
        appendTree(tree->left);
        appendTree(tree->right);
        if (tree->value == "") {
            tree->value = " * ";
        }
    }
}
void printTree(TreeNode * tree, int space) {
    int i;
    string s = "    ";
    string halfSpace = "    ";
    if (tree != NULL) {
        printTree(tree->left, space + 2);
        if ((tree -> left ) && (tree -> right)){
          for (i = 0; i < (0.5* space); i++) {
              halfSpace += halfSpace;
          }
          for (i = 0; i< space; i++){
            s += s;
          }
          cout << halfSpace  <<"  /" << endl;
          cout << halfSpace << tree -> value << endl;
          cout << halfSpace << "  \\" << endl;
        }else {
          for (i = 0; i < space; i++) {
            cout << "    ";
          }
          cout  << tree->value << endl;
        }
        printTree(tree->right, space + 2);
    }

  }
void removeTree(TreeNode *& tree) {
    if (tree->left != NULL) {
        removeTree(tree->left);
    }
    if (tree->right != NULL) {
        removeTree(tree->right);
    }
    tree->parent = NULL;
    delete tree;
}
int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    usage();
    return 0;
  }

  // let's start with empty DynMatrix:
  ClusterNode* head = NULL; 
  ClusterNode* tail = NULL;

  int n = 0; // number of species
  ifstream fin( argv[1] );
  if( fin.is_open() ) {
    // read species:
    string species;
    while ((fin.peek() != '\n') && (fin >> species)) {
      n++;
      // YOUR CODE HERE: replace print statemnt by a code that insert a new species into DynMatrix
      addCluster(head, tail, species);
    }
    // read distances:
    for (int i=0; i<n && fin; i++)      //i = col;
      for (int j=0; j<n && fin; j++) {  //j = row;
        double d;
        fin >> d;
      // YOUR CODE HERE: replace print statemnt by a code that updates distance field of the corresponding DistanceNode
      DistanceNode* visitor = head -> row;
      int j_0 = j;
      while (j_0 != 0){
        visitor = visitor -> nextInRow;
        j_0 --;
      }
      int i_0 = i;
      while(i_0 != 0){
        visitor = visitor -> nextInColumn;
        i_0 --;
      }
      visitor -> distance = d;
    }
    fin.close();
  }

  // YOUR CODE HERE: implement UPGMA method
    while (head != tail) {
        UPGMA(head, tail);
    }

  // print the name of the last remaining cluster
  if (head)
    cout << head->name << endl;

    // BONUS (optional): print the tree in a nice way
    TreeNode * tree = new TreeNode();

    buildTree(tree, head->name);
    appendTree(tree);
    printTree(tree, 0);
    removeTree(tree);
    delete head -> row;
    delete head;
    return 0;
}
