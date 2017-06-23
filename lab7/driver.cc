#include "Hashtable.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;


void usage(char* argv[]) {
	cerr << "Usage: " << argv[0] << " scheme numberOfKeys sizeOfTable" << endl;
	exit(-1);
}

int main(int argc, char *argv[]) {
    
	int n, m;
	srand(time(0));

	if (argc == 1) {
		cout << "Running your test code..." << endl;
		/* ADD YOUR TEST CODE HERE */
        Hashtable H1(1000000);
        for (int i = 0; i < 400000; i++) {
            H1.qinsert(rand() + 1);
        }
        cout << "Quadratic with load factor " << double (400000.0/1000003.0) << ": ";
        H1.printStats();
        
        Hashtable H2(1000000);
        for (int i = 0; i < 400000; i++) {
            H2.linsert(rand() + 1);
        }
        cout << "Linear with load factor " << double (400000.0/1000003.0) << ": ";
        H2.printStats();
        
        Hashtable H3(1000000);
        for (int i = 0; i < 900000; i++) {
            H3.dinsert(rand() + 1);
        }
        cout << "Double with load factor " << double (300000.0/1000003.0) << ": ";
        H3.printStats();

		return 0;
	}
	
	if (argc != 4) {
		usage(argv);
	}
	n = atoi(argv[2]);
	m = atoi(argv[3]);
	Hashtable H(m);
	
	switch(argv[1][0]) {
	case 'l':
		for (int i=0; i<n; ++i) {
			H.linsert(rand() + 1);
		}
		//    H.print();
		cout << "Linear: ";  
		H.printStats();
		break;
	case 'q':
		for (int i=0; i<n; ++i) {
			H.qinsert(rand() + 1);
		}
		//    H.print();
		cout << "Quadratic: ";
		H.printStats();
		break;
	case 'd':
		for (int i=0; i<n; ++i) {
			H.dinsert(rand() + 1);
		}
		//    H.print();
		cout << "Double Hashing: "; 
		H.printStats();
		break;
	default:
		usage(argv);
            
	}
    
    
}

