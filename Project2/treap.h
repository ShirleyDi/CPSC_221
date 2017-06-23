#ifndef __TREAP
#define __TREAP
#include <string>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

struct TreapNode {
    string key;
    int value;
    int priority;
    TreapNode* left;
    TreapNode* right;
};

class Treap {
public:
    //Some helper functions
    TreapNode *root;
    TreapNode * rightRotate(TreapNode*& root);
    TreapNode * leftRotate(TreapNode*& root);
    TreapNode * newNode(const string& key, int value);
    TreapNode * findHelper(const string& key, TreapNode*& root);
    void insertHelper(TreapNode*& root, const string& key, int value);
    void printTree(TreapNode* tree, int space);

    Treap() { // constructor
      srand(0);
      root = new TreapNode();
      root->priority = 0;
    }
    void insert(const string& key, int value);
    int find(const string& key);
    void printTreap(TreapNode* root);
    void deleteTree(TreapNode *& treeNode);
    // return value for the key if found
    // if not found returns -1
};
#endif
