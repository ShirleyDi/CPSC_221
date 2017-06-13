#include "dynmatrix.h"
#include <iostream>

using namespace std;


void addCluster(ClusterNode *&head,ClusterNode *&tail,const std::string& name)
// adds a cluster (at the tail) and the corresponding row and column to data structure
// distance of all added DistanceNodes should be initialized to 0.0
// at the end, tail should point to the newly added ClusterNode
{
	ClusterNode * newCluster = new ClusterNode();
	newCluster -> name = name;
	if (head == NULL) {
		head = newCluster;
		tail = newCluster;
		DistanceNode * distanceNode = new DistanceNode();
		distanceNode -> distance = 0.0;
		head -> row = distanceNode;
		head -> column = distanceNode;
		return;
	} 
	tail -> next = newCluster;
	newCluster -> prev = tail;
	DistanceNode* tailDistance = tail -> column;
	newCluster -> column = new DistanceNode();
	DistanceNode* newTailDis = newCluster -> column;
	newTailDis -> distance = 0.0;
	while (tailDistance -> nextInColumn) {
		tailDistance -> nextInRow = newTailDis;
		tailDistance = tailDistance -> nextInColumn;
		newTailDis -> nextInColumn = new DistanceNode();
		newTailDis = newTailDis -> nextInColumn;
	}
	tailDistance -> nextInRow = newTailDis;
	tailDistance = tail -> row;
	newCluster -> row = new DistanceNode();
	newTailDis = newCluster -> row;
	newTailDis -> distance = 0.0;
	while (tailDistance -> nextInRow){
		tailDistance -> nextInColumn = newTailDis;
		tailDistance = tailDistance -> nextInRow;
		newTailDis -> nextInRow = new DistanceNode();
		newTailDis = newTailDis -> nextInRow;
	}
	tailDistance -> nextInColumn = newTailDis;
	tail = newCluster;
}


void removeCluster(ClusterNode *&head,ClusterNode *&tail,ClusterNode *toBeRemoved)
// removes a cluster pointed to by toBeRemoved and the corresponding row and column
// if toBeRemoved is the first or last cluster then head or tail needs to be updated
{
  // YOUR CODE HERE
	DistanceNode* gar = 0;
	if (toBeRemoved -> prev == NULL){
		head = head -> next;
		head -> prev = NULL;
		ClusterNode* temp = head;
		while (temp -> next) {
			gar = temp->row;
			temp -> row = temp -> row -> nextInRow;
			delete gar;
			gar = temp->column;
			temp -> column = temp -> column -> nextInColumn;
			delete gar;
			temp = temp -> next;
		}
		gar = temp -> row;
		temp -> row = temp -> row -> nextInRow;
		delete gar;
		gar = temp -> column;
		temp -> column = temp -> column -> nextInColumn;
		delete gar;

		delete toBeRemoved -> row;
		delete toBeRemoved;

		return;
	}

	if (toBeRemoved -> next == NULL){
		tail -> row = NULL;
		tail -> column = NULL;
		tail -> prev -> next = NULL;
		tail = tail -> prev;
		DistanceNode* tempRow = tail->row;
		while (tempRow -> nextInRow){
			gar = tempRow -> nextInColumn;
			tempRow -> nextInColumn = NULL;
			tempRow = tempRow -> nextInRow;
			delete gar;
		}
		gar = tempRow -> nextInColumn;
		tempRow -> nextInColumn = NULL;
		delete gar;

		DistanceNode* tempCol = tail -> column;
		while (tempCol -> nextInColumn){
			gar = tempCol -> nextInRow;
			tempCol -> nextInRow = NULL;
			tempCol = tempCol -> nextInColumn;
			delete gar;
		}
		gar = tempCol -> nextInRow;
		tempCol -> nextInRow = NULL;
		delete gar;

		delete toBeRemoved;

		return;

	} else {
		DistanceNode* temp = toBeRemoved -> prev -> column;
		DistanceNode* temp2 = toBeRemoved -> next -> column;

		while (temp){							// remove the whole column
			gar = temp -> nextInRow;
			temp -> nextInRow = temp2;
			temp = temp -> nextInColumn;
			temp2 = temp2 -> nextInColumn;
			delete gar;
		}
		DistanceNode* temp3 = toBeRemoved -> prev -> row;
		DistanceNode* temp4 = toBeRemoved -> next -> row;
		
		while(temp3){
			gar = temp3 -> nextInColumn;
			temp3 -> nextInColumn = temp4;
			temp4 = temp4 -> nextInRow;
			temp3 = temp3 -> nextInRow;
			delete gar;
			
		}
		toBeRemoved -> prev -> next = toBeRemoved -> next;
		toBeRemoved -> next -> prev = toBeRemoved ->prev;

		delete toBeRemoved;
		
		
	}

}


void findMinimum(ClusterNode *head,ClusterNode *&C,ClusterNode *&D)
// finds the minimum distance (between two different clusters) in the data structure 
// and returns the two clusters via C and D
{
	ClusterNode* temp = head;
	double minDis = 1000.0;
	int row = 0;
	int col = 0;
	int minRow = 0;
	int minCol = 0;
	DistanceNode* disNode = 0;
	temp = temp -> next;
	while (temp){
		disNode = temp -> row;
		row++;
		col = 0;
		for (int i = 0; i < row; i++){
			if ((disNode -> distance) < minDis && ((disNode -> distance) != 0) ){
				minDis = disNode -> distance;
				minRow = row;
				minCol = col;
			}
			col++;
			disNode = disNode -> nextInRow;
		}
		temp = temp -> next;
	}
	
	ClusterNode* ptC = head;
	ClusterNode* ptD = head;
	if ((minRow == 0)&&(minCol != 0)) {
		
		for (int i = 0;i < minCol;i++){
			ptD = ptD->next;
		}
		C = ptC;
		D = ptD;
	} else
	if ((minCol == 0)&&(minRow != 0)) {
		for (int i = 0;i < minRow;i++){
			ptC = ptC -> next;
		}
		C = ptC;
		D = ptD;
	} else {
		for (int i = 0;i < minRow;i++){
			ptC = ptC -> next;
			
		}
		for (int i = 0;i < minCol;i++){
			ptD = ptD -> next;
		}
		C = ptC;
		D = ptD;
		
	}
	
}

void printDynMatrix(ClusterNode *head, ClusterNode *tail){
	ClusterNode* temp2 = head;
	DistanceNode* temp = 0;
	while (temp2){
		temp = temp2 -> row;
		while (temp){
			cout<< "Entry:  " <<temp -> distance<< endl;
			temp = temp -> nextInRow;
		}
		cout << "nextRow:" <<endl;
		temp2 = temp2 -> next;
	}
}
