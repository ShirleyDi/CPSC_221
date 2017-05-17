#include <iostream>
using namespace std;

void guessNumber(int guess, int key);
int main(void)
{
	srand(time(NULL));
	int guess;
	int key = rand()%100 +1; 
	cout << "Enter your guess here:" << endl;
	cin >> guess;
	guessNumber(guess,key);
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
		cin >> guess;
		guessNumber(guess,key);
	}
	else
	{
		cout << "Too small!" << endl;
		cout << "Enter your guess here:" << endl;
		cin >> guess;
		guessNumber(guess,key);
	}
} 