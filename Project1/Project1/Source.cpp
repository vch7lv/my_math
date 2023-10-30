#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;


int main()
{
	int t;
	cin >> t;
	while (t--)
	{
		int n;
		cin >> n;
		vector<int> A(n);
		for (int i = 0; i < n; ++i)
			cin >> A[i];
		sort(A.begin(), A.end());


		if (n == 2) {
			cout << "YES" << endl;
			continue;
		}
		if (n == 3) {
			if (A[0] == A[1] || A[1] == A[2])
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
			continue;
		}

		int i = 0;
		while (i < n - 1 && A[i] == A[i + 1]) ++i;
		++i;
		if (!(i >= n / 2 && i <= (n + 1) / 2 || i == n))
		{
			cout << "NO" << endl;
			continue;
		}
		while (i < n - 1 && A[i] == A[i + 1]) ++i;

		if (i == n)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
}