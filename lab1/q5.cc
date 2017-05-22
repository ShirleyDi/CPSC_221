#include <iostream>
#include <cstdlib>
using namespace std;

void guessNumber(int guess, int key);
int main(void)
{
	srand(time(NULL));
	string str1;
	int key = rand()%100 +1; 
	cout << "Enter your guess here:" << endl;
	cin >> str1;
	int number = atoi(str1.c_str());
	guessNumber(number,key);
	return 0;
}

void guessNumber(int guess, int key){
	
	if (guess == key)
	{
		cout << "You win!" << endl;
		return;
	}
	if(guess > key)
	{
		cout << "Too large!" << endl;
		cout << "Enter your guess here:" << endl;
		string str2;
		cin >> str2;
		guessNumber(atoi(str2.c_str()),key);
	}
	else
	{
		cout << "Too small!" << endl;
		cout << "Enter your guess here:" << endl;
		string str3;
		cin >> str3;
		guessNumber(atoi(str3.c_str()),key);
	}
} 