#include <iostream>
using namespace std;
// prototype
int a[10];
void fill_array(int first_value, int increment);

int main(void){
	int first_value;
	int increment;
	cout<< "What is the first value?"<<endl;
	cin >> first_value;
	cout<< "What is the increment?"<<endl;
	cin >> increment;
	fill_array(first_value,increment);
	return 0;
}

void fill_array(int first_value, int increment){
	for(int i= 0; i<10; i++){
		a[i] = first_value + i * increment;
		cout << a[i] <<endl;
	}
	return;
}
