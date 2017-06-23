#include <iostream>
#include "BST.h"

using namespace std;

int main() {
    vector<int> arr;
    genData(4, arr);
    for (int i = 0; i < arr.size(); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    Node * root = makeBST(arr, 0, arr.size() - 1);
    printBST(root);
    int high = height(root);
    remove(root, -3);
    cout << "Update BST: " << endl;
    printBST(root);
    return 0;
}