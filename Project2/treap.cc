#include "treap.h"
#include <sstream>
using namespace std;


//Right-rotate function
TreapNode * Treap::rightRotate(TreapNode*& root){					
	TreapNode* temp = root -> left;
	root -> left = temp -> right;
	temp -> right = root;
	return temp;
}

//Left-rotate function
TreapNode * Treap::leftRotate(TreapNode*& root){					
	TreapNode* temp = root -> right;
	root -> right = temp -> left;
	temp -> left = root;
	return temp;
	
}

//Create a new node with random priority
TreapNode * Treap::newNode(const string& key, int value){
	TreapNode* temp = new TreapNode;					
	temp -> key = key;
	temp -> value = value;
	temp -> priority = rand();
	temp -> left = NULL;
	temp -> right = NULL;
	return temp;
}

// Return the value of the node with the key, -1 if not found
int Treap::find(const string& key){							
	if (findHelper(key,root)== NULL) {
		return -1;
	} else {		
		TreapNode * ret = findHelper(key, root); 	//set ret to be the found node	
		return ret->value;							//return the value of the node
	}
}

// A helper function to find the node that contains the key
TreapNode * Treap::findHelper(const string& key, TreapNode*& root){ 
	//When the treap is empty:		
	if (root == NULL)
		return NULL;
	// If we find the key:
	if (root -> key == key){
		return root;
	} else {
		// Recursively find the key in the treap
		if (root -> key < key)
			return findHelper(key, root -> right);
		else
			return findHelper(key, root -> left);
	}
}

// Insert function calling the helper
void Treap::insert(const string& key, int value){ 				
	insertHelper(root, key, value);
}

// A helper function for insert function with more variables passed in
void Treap::insertHelper(TreapNode* &root, const string& key, int value){
	//when the treap is empty or the root key is empty string
	if (root == NULL || root->key == "") {
		root = newNode(key, value);
	}
	//avoid duplicate keys	
	if (find(key) == -1) {
		if (key <= root->key) {
			insertHelper(root->left, key, value);
			if(root->left->priority < root->priority)	//fix minHeap property
				root = rightRotate(root);
		} else {
			insertHelper(root->right, key,value);
			if(root->right->priority < root->priority) //fix minHeap property
				root = leftRotate(root);
		}
	} else {
		// if the key is already in the treap, update the value
		TreapNode* temp = findHelper(key, root);
		temp->value = value;
	}
}

// Print treap function took from project1. Left child is on the top
void Treap::printTree(TreapNode* root, int blank) {
    if(root != NULL) {
        if(root->left) printTree(root->left, blank + 4);
        if (blank) {
            for (int i = 0; i < blank; i++) {
                cout << "   ";
            }
        }
        cout<< root->key << "\n ";
        if(root->right) printTree(root->right, blank + 4);
    }
}
void Treap::deleteTree(TreapNode *& treeNode) {
	if (treeNode == NULL) return;
	deleteTree(treeNode->left);
	deleteTree(treeNode->right);
	delete treeNode;
}
