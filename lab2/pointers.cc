#include <iostream>
int main () {

  int* p1;

  int* p2;		// value of x 			y 			p1 				p2 
  				//			5 			15 		   uninit 		  uninit

  int x = 5;

  int y = 15; 


  p1 = &x;     // 			5			15		address of x  	 	0
 
  p2 = &y;     // 			5			15		address of x    address of y
 
  *p1 = 6;     // 			6			15		address of x    address of y
 
  *p1 = *p2;   // 			15			15		address of x    address of y
 
  p2 = p1;     // 			15			15		address of x    address of x
 
  *p1 = *p2+10;  // 		25			15		address of x    address of x
 
  return 0;
}
