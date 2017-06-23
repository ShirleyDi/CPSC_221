#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "treap.h"

using namespace std;

// Replace all characters except letters (A-Z,a-z) in line with ' ',
// except keep '-' or '\'' if they are between letters.
void lowercaseWords(string & line) {
    for( string::iterator it = line.begin(); it != line.end(); ++it ) {
        if(!isalpha(*it)) {
            if((*it != '-' && *it != '\'') || it == line.begin() ||
                    it+1 == line.end() || !isalpha(*(it-1)) || !isalpha(*(it+1))) {
                *it = ' ';
            }
        } else {
            *it = tolower(*it);
        }
    }
}

//helper for splitting string and store splitted strings in a vector
vector<string> split(string str, char delimiter) {
    vector<string> internal;                // Turn the string into a stream.
    stringstream ss(str);
    string tok;
    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    return internal;
}

int main(int argc, char *argv[]) {
    Treap* t = new Treap();
    srand (time(0));

    if( argc < 2 || argc > 3 ) {
        cerr << "Usage: " << argv[0] << " n book.txt" << endl;
        exit(1);
    }

    int n = atoi(argv[1]);
  
    ifstream fin;
    fin.open(argv[2]);
    if( !fin.is_open() ) {
        cerr << "Unable to open " << argv[1] << endl;
        exit(2);
    }
    //Commented out the print function
    /*
    string line;
    string word;
    while(!fin.eof()) {
        getline(fin,line);
        lowercaseWords(line);
        istringstream iss(line, istringstream::in);
        while( iss >> word ) {
          cout <<word<<endl;
        }
    }
    */
    fin.close();

    //Define vectors to store words, all_word_rank, follower_ranks
    vector<string> wordv;
    vector<int> all_word_ranks;
    vector<vector<int> > follower_ranks;
    ifstream ifs("book.txt");                             //Convert input to string
    string input( (istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));

    lowercaseWords(input);
    vector<string> arr = split(input, ' ');
    // add elements into wordvector, remove duplicates
    for (int i = 0; i < arr.size(); ++i) {
        if((find(wordv.begin(), wordv.end(), arr[i]) == wordv.end()) && (arr[i].size() != 0)) {
            wordv.push_back(arr[i]);
        }
    }
  
    // put elements into treap
    for (int i = 0; i< wordv.size(); ++i){
        t->insert(wordv[i], i);
    }
  
    // put elements into all_word_ranks
    for (int i = 0; i < arr.size(); ++i) {
        for (int j = 0; j < wordv.size(); ++j) {
            if (wordv[j] == arr[i]) {
                all_word_ranks.push_back(j);
            }
        }
    }

    // put elements into follower_ranks
    for (int i = 0; i < wordv.size(); ++i) {
        vector<int> lead;
        follower_ranks.push_back(lead);
    }

    for (int i = 0; i < all_word_ranks.size() - 1; ++i) {
        follower_ranks[all_word_ranks[i]].push_back(all_word_ranks[i + 1]);
    }

    // Generate Text
    if (n == 0){                                                        //The case when n is 0
        for (int i = 0; i < wordv.size(); i++){
            cout << wordv[i] << ": ";
            for (int j = 0; j < follower_ranks[i].size(); j++){         //Use two for-loops to print out words and
                cout << wordv[follower_ranks[i][j]] << " ";             //their followers
            }
        cout << endl;
        }
    } else {                                                            //Cases when n > 0
        string output;
        int dictionarySize = wordv.size();
        int wordindex = rand()% dictionarySize;                         //Randomly generate a word index
        string append = wordv[wordindex];                               //set append to the word with the generated index
        output.append(append);                                          //Append the word to the empty output string
        int followerIndex;
        //Use a for loop to append random words from follower and generate new random words.
        for (int i = 0; i < n-1; i++){
            if (follower_ranks[wordindex].size() != 0){                 //The case when the word has follower
                followerIndex = rand() % (follower_ranks[wordindex].size());
                output.append(" ");
                output.append(wordv[follower_ranks[wordindex][followerIndex]]);
                wordindex = follower_ranks[wordindex][followerIndex];
            } else {
                wordindex = rand()% dictionarySize;
                output.append(" ");
                output.append(wordv[wordindex]);
            }
        }
        cout << output << endl;
    }
    // Helps visualize the treap, uncomment if needed.
//    t->printTree(t->root, 2);
    t->deleteTree(t->root);
    delete t;
    return 0;
}
