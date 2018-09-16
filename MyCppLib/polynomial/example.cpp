#include "poly.h"
#include <iostream>
using namespace std;
using namespace _201700800404;
int main()
{
	polynomial_N_index p1, p2;
	p1 = { {"1/4",2},{"3",0} };  //p1^=(x^2)/4 + 3
	cout << p1 << endl;        
	cout << p1.format() << endl;
	cout << p1.value_at("4/3") << endl;  //f(4/3)=31/9
	cout << p1.derivative() << endl;  // (1/2)x
	p1 += p1.derivative(); // p1= (x^2)/4 + (1/2)x + 3
	cout<< p1 << endl; // (3/4)(x^2) + 3
	p1 *= p1.derivative(); // p1= (1/8)*x^3 + (3/8)*x^2 + (7/4)*x + 3/2
	cout << p1 << endl << endl;
	p1 -= { {"3", 2}};
	cout << p1 << endl;

	p1 = { {"-5",2},{"0",0} };
	cout << p1 << endl;
	p1 = { {"0",0} };
	cout << p1 << endl;
	
	system("pause");
	return 0;
}