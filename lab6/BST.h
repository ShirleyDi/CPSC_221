#ifndef ASSIGN_3_BST_H
#define ASSIGN_3_BST_H
#include <vector>
#include <iomanip>	// provides std::setw()

using namespace std;

class Node {
public:
    int value;
    Node * left;
    Node * right;
    Node(int v) {
        this->value = v;
        left = NULL;
        right = NULL;
    }
};

// Runtime O(n)
/**
 *  Give a number n and an array of list (implemented by std::vector),
 *  return a list of int by the requirement
 *  PRE: A non-negative int n and a vector of int
 *  POST: A vector of int
 */
vector<int> genData(int n, vector<int> & list) {
    for (int i = -n + 1; i < n; ++i) {
        list.push_back(i);
    }
    return list;
}

// Runtime O(n)
/**
 *  Create a binary search tree.
 *  PRE: Given a sorted array, and the first and last index of the array.
 *  POST: return a binary search tree.
 */
Node* makeBST(vector<int> arr, int start, int end) {
    if (start > end) {
        return NULL;
    }
    int mid = (start + end) / 2;
    Node * root = new Node(arr[mid]);
    root->left = makeBST(arr, start, mid-1);
    root->right = makeBST(arr, mid+1, end);
    return root;
}

// Runtime O(n)
/**
 *  Print the binary search tree by in-order travesal.
 *  PRE: A binary search tree and the initial depth of the node to the root.
 *  POST: None
 */
void printBST(Node * root, int d = 0) {
    if (root == NULL) {
        return;
    }
    printBST(root->left, d + 1);
    cout << setw(5 * d) << root->value << endl;
    printBST(root->right, d + 1);
}

// Runtime: O(lgn)
/**
 *  Return the hight of the binary search tree.
 *  PRE: Given a BST.
 *  POST: return the int of height.
 */
int height(Node * root) {
    if (root == NULL) {
        return -1;
    }
    if (!root->left && !root->right) {
        return 0;
    }
    if (height(root->left) > height(root->right)) {
        return 1 + height(root->left);
    } else {
        return 1 + height(root->right);
    }
}

// Runtime: O(lgn)
/**
 *  Return the specific node, assume the val is contained in the BST.
 *  PRE: The root of BST and target int value.
 *  POST: Return the specific node.
 */
Node *& find(Node *& root, int val) {
    if (root == NULL) return root;
    if (val < root->value)
        return find(root->left, val);
    if (val > root->value)
        return find(root->right, val);
    return root;
}

// This is help method in order to find the parent of the target node.
Node* find_parent(Node* root, Node* target) {
    if (root == NULL || root->value == target->value) {
        return NULL;
    }
    if (root->left) {
        if (root->left->value == target->value) {
            return root;
        }
    }
    if (root->right) {
        if (root->right->value == target->value) {
            return root;
        }
    }
    if (target->value < root->value) {
        return find_parent(root->left, target);
    } else {
        return find_parent(root->right, target);
    }
}

// This is help help method (the recursion implementation) for finding the predecessor of the node.
Node*& find_predecessor_helper(Node *& n){
    if (!(n->right))
        return n;
    return find_predecessor_helper(n->right);
}

// This is help method in order to find the predecessor of the node.
Node*& find_predecessor(Node *& n){
    return find_predecessor_helper(n->left);
}

// Runtime: O(lgn)
/**
 *  Remove the node with spefici value.
 *  PRE: Given the BST and the value of the node.
 *  POST: Update the BST.
 */
void remove(Node * root, int val) {
    Node * target = find(root, val);
    Node * parent = find_parent(root, target);
//    Case 1: The node is a leaf
    if (target->left == NULL && target->right == NULL) {
        if (parent != NULL) {
            if (parent->left == target)
                parent->left = NULL;
            else
                parent->right = NULL;
        }
        else
            root = NULL;
        delete target;
    } else if (target->left != NULL and target->right == NULL) {
//        Case 2: The node doesn't have its left child
        if (parent != NULL) {
            if (parent->left == target)
                parent->left = target->left;
            else {
                parent->right = target->left;
            }
        } else {
            root = target->left;
        }
        delete target;
    } else if (target->right != NULL and target->left == NULL) {
//        Case 3: The node doesn't have its right child
        if (parent != NULL) {
            if (parent->left == target)
                parent->left = target->right;
            else
                parent->right = target->right;
        } else {
            root = target->right;
        }
        delete target;
    } else {
//        Case 4: The node has two children
        if (parent != NULL) {
            Node* pre = find_predecessor(target);
            Node* parent_of_pre = find_parent(target, pre);

            target->value = pre->value;

            if (parent_of_pre->left == pre)
                parent_of_pre->left = pre->left;
            else
                parent_of_pre->right = pre->left;
            delete pre;
        } else {
            Node * pre = target->left;
            while (pre->right) {
                pre = pre->right;
            }
            if (pre->value == root->left->value) {
                target->value = target->right->value;
                target->right = NULL;
            } else {
                Node* parent_of_pre = find_parent(target, pre);
                target->value = pre->value;
                parent_of_pre->right = NULL;
                delete pre;
            }
        }
    }
}



#endif //ASSIGN_3_BST_H
