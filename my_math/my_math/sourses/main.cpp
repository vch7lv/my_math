#include "headers.h"

using namespace std;
typedef matrix<long double> m;
typedef vec<long double> v;
typedef polynomial<long double> pol;


int main()
{
	pol a {1, 4, 6, 4, 1};

	cout << "a: " << a << endl;

	pol b { 1, 2, 4 };
	cout << "b: " << b << endl;


	cout << "a / b:    " << a / b << endl;
	cout << "a % b:   " << a % b << endl;
}
