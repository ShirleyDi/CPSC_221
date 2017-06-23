//
// Lab 8 
// AVL tree auto-balancing, and
// parallel calculation of word frequency statisticss
//
#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>	// provides setw()
#include <cstdlib>	// provides atoi()
#include <cassert>	// provides assert
#include <functional>	// provides std::function
#include <vector> // provides std::vector
#include <sstream>
#include <string> 
#include "Unit.h" 

#define REPETITIONS 10		// Number of times we repeat the task.

int SEQUENTIAL_CUTOFF = 11;	// Default height at which we switch from
                                // parallel to sequential code

typedef std::string KType;   // The datatype for the key of the AVL tree Node
typedef int VType;           // The datatype for the value of the AVL tree Node

Unit unit; // Unit testing framework 
std::vector<KType> visitor; 
std::vector<KType> erootpected; 



struct Node {        // A Node in an AVL tree.
  KType key;         // a word
  VType value;       // the number of times the word occurs in the input
  int height;        // height of the node
  Node * left;       // pointer to the left child of the node (or NULL)
  Node * right;      // pointer to the right child of the node (or NULL)
};


// Create a new Node with the given parameters.
Node* createNode( KType key, VType value, Node* left=NULL, Node* right=NULL ) {
  Node* result = new Node;
  if( result == NULL ) return (Node*) NULL;
  result->key = key;
  result->value = value;
  result->height = 0;
  result->left = left;
  result->right = right;
  return result;
}

// Delete all the nodes in the tree rooted at *root
void deleteTree( Node*& root ) {
  if ( root != NULL ) {
    deleteTree( root->left );
    deleteTree( root->right );
    delete root;
    root = NULL;
  }
}

// Return true iff the tree contains the given key.
bool contains( KType key, Node * root ) {
  if ( root == NULL ) {
    return false;
  }

  if ( key < root->key ) {
    return contains( key, root->left );
  }
  else if ( key > root->key ) {
    return contains( key, root->right );
  }
  else
    return true;
}

// Get height of a Node.
int getHeight( Node * node ) {
  return ( node != NULL ) ? (node->height) : -1;
}

// Calculate the height of a Node from its children's heights.
int calcHeight( Node * node ) {
  if ( node == NULL ) return -1;
  int left_height = getHeight( node->left );
  int right_height = getHeight( node->right );
  return ( left_height > right_height ) ? left_height + 1 : right_height + 1;
}

// Set the height of a Node.
void setHeight( Node * node, int height ) {
  if ( node == NULL ) return;
  if (node->height == height) return;
  node->height = height;
}

// Return the value of a Node.
VType getValue(Node* node) {
  assert( node );
  return node->value;
}

// Set the value of a Node.
void setValue(Node* node, VType value) {
  assert( node );
  node->value = value;
}

// "Rotate" the subtree to the left (counter-clockwise).
void rotateLeft( Node *& root ) {
  assert(root != NULL);
  assert(root->right != NULL);
  Node * temp = root->right;
  root->right = temp->left;
  temp->left = root;
  setHeight( root, calcHeight(root) );
  setHeight( temp, calcHeight(temp) );
  root = temp;
}

// "Rotate" the subtree to the right (clockwise).
void rotateRight( Node *& root ) {
  assert(root != NULL);
  assert(root->left != NULL);
  Node * temp = root->left;
  root->left = temp->right;
  temp->right = root;
  setHeight( root, calcHeight(root) );
  setHeight( temp, calcHeight(temp) );
  root = temp;
}

// doubleRotateLeft(root) is rotateRight(root->right) then rotateLeft(root).
void doubleRotateLeft( Node *& root ) {
  assert(root != NULL);
  assert(root->right != NULL);
  assert(root->right->left != NULL);
  rotateRight(root->right);
  rotateLeft(root);
}

// doubleRotateRight(root) is rotateLeft(root->left) then rotateRight(root).
void doubleRotateRight( Node *& root ) {
  assert(root != NULL);
  assert(root->left != NULL);
  assert(root->left->right != NULL);
  rotateLeft(root->left);
  rotateRight(root);
}

//
// Check if node root is unbalanced (i.e., the heights of its
// two children differ by more than one).  If it is, rebalance
// at root using one of rotateLeft, rotateRight, doubleRotateLeft,
// or doubleRotateRight, whichever is appropriate.
//
void balance( Node *& root ) {
   // ************ TODO: Implement this! *************
   int leftHeight;
   int rightHeight;
   if(!root->left){
     leftHeight = -1;
   }else{
     leftHeight = root->left->height;
   }
   if(!root->right){
     rightHeight = -1;
   }else{
     rightHeight = root->right->height;
   }
 
if(leftHeight - rightHeight <= 1 && leftHeight - rightHeight >= -1){
     return;
   }
 
   if(leftHeight > rightHeight){
     if(root->left->right){
       if(!root->left->left){
         //LR Case
         doubleRotateRight(root);
         return;
       }
       if(root->left->right->height > root->left->left->height){
         //LR Case
         doubleRotateRight(root);
         return;
       }
     }
     //LL Case
     rotateRight(root);
 
   }else{
     if(root->right->left){
       if(!root->right->right){
         //RL Case
         doubleRotateLeft(root);
         return;
       }
       if(root->right->left->height > root->right->right->height){
         //RL Case
         doubleRotateLeft(root);
         return;
       }
     }
     //RR Case
     rotateLeft(root);
 
   }
}

// Insert into tree.
void insert( KType key, VType value, Node *& root ) {
  if( root == NULL ) {  // base case
    root = createNode(key, value);
    return;
  }
  else if( key < root->key ) {
    insert( key, value, root->left );
  }
  else if( key > root->key ) {
    insert( key, value, root->right );
  }
  // update root->height if it's changed, and balance(root)
  int calcHeight_root = calcHeight( root );
  if( calcHeight_root != getHeight( root) ) {
    setHeight( root, calcHeight_root );
    balance(root);
  }
}

// Return a reference to a pointer to the Node in the tree with the given key.
Node*& findNode(KType key, Node*& root) {
  if( root == NULL ) {
    return root;
  }
  if( key < root->key ) {
    return findNode( key, root->left);
  }
  if( key > root->key ) {
    return findNode( key, root->right);
  }
  return root;
}

// Good for debugging.
// Helper to print the tree sideways.
void printTreeHelper( Node * root, int depth ) {
  if( root == NULL ) return;
  printTreeHelper( root->right, depth + 1 );
  std::cout << std::setw( 3 * depth ) << "";	// output 3 * depth spaces
  std::cout << root->key << std::endl;
  printTreeHelper( root->left, depth + 1 );
}

// Good for debugging.
// Print out the tree sideways.
void printTree( Node * root ) {
  printTreeHelper( root, 0 );
}

// Good for debugging.
// Return true iff *root is the root of a valid AVL tree.
bool isValidAVLTree( Node* root ){
  if ( root == NULL ) return true;
  if (isValidAVLTree( root->left ) && isValidAVLTree( root->right )) {
    if (abs(getHeight( root->left ) - getHeight( root->right )) > 1)
      return false;
    return true;
  }
  else return false;
}


// Calculate the stats for the whole tree given:
// the root's value and the stats for the root's left and right subtrees.
//
// This calculates the population variance.
//
// <see Wikipedia: Algorithms for calculating variance>
void updateStats( VType value,
                  double lavg, double lvar, int lcount,
                  double ravg, double rvar, int rcount,
                  double& avg, double& var, int& count ) {
  double delta, m;
  count = lcount + rcount;
  if( count > 0 ) {
    avg = (lcount * lavg + rcount * ravg)/(double)count;
    delta = lavg - ravg;
    double lm = lvar * lcount;
    double rm = rvar * rcount;
    m = lm + rm + delta * delta * lcount * rcount / (double)count;
  } else {
    avg = 0; var = 0; m = 0;
  }
  count++;
  delta = (double)(value - avg);
  avg += delta/(double)count;
  m += delta * (value - avg);
  var = m / (double)count;
}


// Calculate statistics for the subtree rooted at *root.
void findStatsSequential(Node * root, double &avg, double &var, int &count) {
  if (root == NULL) {
    avg = 0; var = 0; count = 0;
    return;
  }

  double lavg, lvar, ravg, rvar;
  int lcount, rcount;

  // Find stats in the left subtree.
  findStatsSequential(root->left, lavg, lvar, lcount);

  // Find stats in the right subtree.
  findStatsSequential(root->right, ravg, rvar, rcount);

  updateStats(root->value,
              lavg, lvar, lcount,
              ravg, rvar, rcount,
              avg, var, count);
}


// Helper for findStats.
//
// Fork off tasks to handle subtrees until you first can tell, by considering
// the height of the root, that the tree contains at most 1100 nodes
// at which point use the sequential version.
//
// Use findStatsSequential for inspirations of what you should do.
//
void findStatsHelper(Node * root, double &avg, double &var, int &count) {

  
  // ************ TODO: Implement this! *************
  // If you don't know where to start,
  // Please look into the lecture slides on Paralleism
   if (root == NULL) {
    avg = 0; var = 0; count = 0;
     return;
    }
  
    //node's height below cutoff switch to sequential
    if(root->height <= SEQUENTIAL_CUTOFF){
  
      double lavg, lvar, ravg, rvar;
      int lcount, rcount;
  
      // Find stats in the left subtree.
      findStatsSequential(root->left, lavg, lvar, lcount);
   
      // Find stats in the right subtree.
      findStatsSequential(root->right, ravg, rvar, rcount);
  
      updateStats(root->value,
              lavg, lvar, lcount,
                  ravg, rvar, rcount,
                  avg, var, count);
      return;
    }
   
    //compute in parallel
   double lavg, lvar, ravg, rvar;
   int lcount, rcount;
    //fork the left subtree out, but preserve parameters
    #pragma omp task untied shared(lavg, lvar, lcount)
      {
        findStatsHelper(root->left, lavg, lvar, lcount);
      }
      findStatsHelper(root->right, ravg, rvar, rcount);

  //done computing both subtree, update the total status
    #pragma omp taskwait
    updateStats(root->value,
                lavg, lvar, lcount,
                ravg, rvar, rcount,
                avg, var, count);
}

// Parallel version of findStatsSequential
void findStats(Node * root, double &avg, double &var, int &count) {
  // Declare OpenMP parallel region and start erootecution on one thread.
  // *untied* task creation (in findStatsHelper) will erootploit additional
  // threads.
#pragma omp parallel
  {
  #pragma omp single
    {
      findStatsHelper(root, avg, var, count);
    }
  }
}

// *******************************************
// records keys, heights, levels as tree is traversed
void visitTreeHelper( Node* root, int level, std::vector<KType>& visitor){
  if (! root) return; 
  visitTreeHelper( root->right, level + 1, visitor);
  visitor.push_back( root->key ); 
  visitor.push_back( std::to_string(root->height)); 
  visitor.push_back( std::to_string(level)); 
  visitTreeHelper( root->left, level + 1, visitor);
}  
void visitTree(Node* root, std::vector<KType>& visitor){ 
  visitTreeHelper( root, 0, visitor);
}

std::string to_string_helper(int i) {
	switch (i) {
		case 70: return "k";
		case 10: return "b";
		case 60: return "j";
		case 20: return "c";
		case 50: return "h";
		case 30: return "f";	
		case 40: return "g";	
		case 25: return "d";	
		case 27: return "e";	
		case 9: return "a";	
	}
	return std::to_string(i);
}

// values for default keys on tree traverals 
int traversalValues [] = { 70,0,0,
  70,1,0,10,0,1,
  70,0,1,60,1,0,10,0,1,
  70,0,1,60,2,0,20,0,2,10,1,1,
  70,0,1,60,2,0,50,0,2,20,1,1,10,0,2,
  70,0,2,60,1,1,50,2,0,30,0,2,20,1,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,20,2,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,25,0,3,20,2,1,10,0,2,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,27,0,3,25,2,1,20,1,2,10,0,3,
  70,0,2,60,1,1,50,3,0,40,0,3,30,1,2,27,0,3,25,2,1,20,0,3,10,1,2,9,0,3 };
void prepareErootpected(int num_to_skip, int num_to_take, std::vector<KType>& vv){
  vv.clear();
  for (int ii = num_to_skip; ii < num_to_skip + num_to_take; ++ ii ){ 
    vv.push_back( to_string_helper(traversalValues [ii]) ); 
  }
  return;
}

// Print a message on stdout about how to use this program.
void usage(const char* program) {
  std::cout << "Usage: " << program << " 1 "  << std::endl;
  std::cout << "\tTest the correctness of AVL tree implementation." << std::endl;
  std::cout << "Or: " << program << " 2 <filename> [cutoff]"  << std::endl;
  std::cout << "\tPrints the mean and std deviation of the distribution of words and the number of unique words in the named file."
            << std::endl;
}

// Run REPETITIONS trials of either sequential or parallel findStats
// and reports the time in seconds that this takes.
void findStatsAndReport(std::function<void(Node*, double&, double&, int&)> f,
                        Node * tree,
                        const char * type) {
  double avg, var;
  int count;

  double t1, t2;
  t1 = omp_get_wtime();
  for (int i = 0; i < REPETITIONS; i++) {
    f(tree, avg, var, count);
  }
  t2 = omp_get_wtime();

  std::cout << type << "\tmean:" << avg << "\tvar:" << var << "\tn:" << count << "\t";
  std::cout << t2 - t1 << " secs" << std::endl;
  std::cout.flush();
}

void avl_test() {
  const std::string keys[] = {"k", "b", "j", "c", "h", "f", "g", "d", "e", "a"};
  Node *T = NULL;
  int num_keys;

  num_keys = 10;

  int unitOffset = 0;
  for( int i=0; i < num_keys; ++i ) {
    std::cout << "Insert: " << keys[i] << std::endl;
    insert(keys[i], 0, T);
    std::cout << "Tree:" << std::endl;
    printTree(T);
    // use default keys for unit tests
    std::stringstream cmt;
    cmt << "contains( " << keys[i] << ") "; 
    unit.assertTrue(cmt.str(), contains( keys[i], T )); 
    visitor.clear();
    visitTree(T, visitor);
    prepareErootpected( 3 * unitOffset, 3 * (i + 1), erootpected ); 
    unitOffset += i + 1; 
    std::stringstream fmt;
    fmt << "Tree after insert key[" << i << "] == "<< keys[i] << " "; 
    unit.assertvectorEquals(fmt.str(), erootpected, visitor ); 
  }
  unit.printResults(); 


}

int main( int argc, char *argv[] ) {
  if (argc < 2 || argc > 4) {
    usage(argv[0]);
    return -1;
  }
  if (argc == 2) {
    if (std::atoi(argv[1]) != 1) {
      usage(argv[0]);
      return -1;
    }
    avl_test();
    return 0;
  }
  if (argc == 4) {
    if (std::atoi(argv[1]) != 2) {
      usage(argv[0]);
      return -1;
    }
    SEQUENTIAL_CUTOFF = std::atoi( argv[3] );
  }

  std::fstream infile;
  infile.open( argv[2], std::fstream::in );
  if (infile.rdstate()) { // some type of error
    std::cerr<< "Error opening " << argv[2] << std::endl;
    if ((infile.rdstate() &  std::ifstream::failbit) != 0 )
      std::cerr << " failbit";
    if ((infile.rdstate() &  std::ifstream::badbit) != 0 )
      std::cerr << " badbit";
    if ((infile.rdstate() &  std::ifstream::eofbit) != 0 )
      std::cerr << " eofbit";
    std::cerr << std::endl;
    return -1;
  }

  // Tally all the words.
  Node *avlTree_root = NULL;
  std::string word;

  // foreach word, attempt to find a Node with key==word already in tree
  // if none erootists, then make one, with value == 0
  // always increment the tally for the word by 1

  while (infile >> word) {
    Node* node = findNode( word, avlTree_root );
    if( node == NULL ) {
      insert( word, 1, avlTree_root );
    }
    else setValue( node, 1 + getValue(node) );
  }

  // Find stats for the word distribution, first sequential then parallel

  findStatsAndReport(&findStatsSequential, avlTree_root, "SEQUENTIAL");

  findStatsAndReport(&findStats, avlTree_root, "PARALLEL");
  std::cout << "Cutoff = " << SEQUENTIAL_CUTOFF << "\t";
  std::cout << "Number of processors = " << omp_get_num_procs() << std::endl;


  // Free up allocated memory
  deleteTree(avlTree_root);

  return 0;
}
