#include <iostream>
using namespace std;
// prototype
int a[10];
void fill_array(int first_value, int increment);

int main(void){
	fill_array(0,5);
	return 0;
}

void fill_array(int first_value, int increment){
	for(int i= 0; i<10; i++){
		a[i] = first_value + i * increment;
		cout << a[i] <<endl;
	}
	return;
}
