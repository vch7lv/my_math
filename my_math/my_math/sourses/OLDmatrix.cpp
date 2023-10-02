using namespace std;
#include <vector>
#include <iostream>
#include <math.h>
typedef vector < vector<long double> > matrix;
matrix inverse(matrix A)
{
	matrix A_inverse;
	for (int j = 0; j < A[0].size(); ++j)
	{
		vector<long double> temp;
		for (int i = 0; i < A.size(); ++i)
			if (i == j) temp.push_back(1);
			else temp.push_back(0);
		A_inverse.push_back(temp);
	}

	for (int j = 0; j < A.size() - 1; ++j)
	{
		//find max
		long double max = 0 + j;
		for (int i = max + 1; i < A[j].size(); ++i)
			if (abs(A[j][i]) > abs(A[j][max])) max = i;

		//change order rows
		long double temp;
		for (int k = j; k < A[j].size(); ++k)
		{
			temp = A[k][j];
			A[k][j] = A[k][max];
			A[k][max] = temp;
			temp = A_inverse[k][j];
			A_inverse[k][j] = A_inverse[k][max];
			A_inverse[k][max] = temp;
		}
		max = A[j][j];

		//divide by max top row
		for (int k = j; k < A.size(); ++k)
		{
			A[k][j] /= max;
			A_inverse[k][j] /= max;
		}

		//transform every row
		for (int i = j + 1; i < A[j].size(); ++i)
		{
			temp = A[j][i];
			for (int k = j; k < A.size(); ++k)
			{
				A[k][i] -= temp * A[k][j];
				A_inverse[k][i] -= temp * A_inverse[k][j];
			}
		}
	}
	A_inverse[A.size() - 1][A.size() - 1] /= A[A.size() - 1][A.size() - 1];
	return A_inverse;
}
matrix t(matrix A)
{
	matrix A_T;
	for (int i = 0; i < A.size(); ++i)
	{
		vector<long double> temp;
		for (int j = 0; j < A.size(); ++j)
			temp.push_back(A[j][i]);
		A_T.push_back(temp);
	}
	return A_T;
}


long double norm2(vector<long double> x)
{
	long double out = 0;
	for (int i = 0; i < x.size(); ++i)
		out += x[i] * x[i];
	return pow(out, 0.5);
}
long double norm1(vector<long double> x)
{
	long double out = 0;
	for (int i = 0; i < x.size(); ++i)
		out += abs(x[i]);
	return out;
}
long double norm1(matrix A)
{
	long double max = 0;
	for (int j = 0; j < A.size(); ++j)
	{
		long double out = 0;
		for (int i = 0; i < A.size(); ++i)
			out += abs(A[j][i]);
		if (out > max) max = out;
	}
	return max;
}


vector<long double> multiply(matrix A, vector<long double> x)
{
	vector<long double> out;
	for (int i = 0; i < x.size(); ++i)
	{
		long double temp = 0;
		long double p;
		for (int j = 0; j < A.size(); ++j)
		{
			temp += A[j][i] * x[j];
			p = A[j][i] * x[j];
		}
		out.push_back(temp);
	}
	return out;
}
vector<long double> multiply(vector<long double> x, long double num)
{
	vector<long double> temp = x;
	for (int i = 0; i < x.size(); ++i)
		temp[i] *= num;
	return temp;
}


matrix multiply(matrix A, long double num)
{
	matrix Anum;
	for (int j = 0; j < A.size(); ++j)
		Anum.push_back(multiply(A[j], num));
	return Anum;
}
matrix multiply(matrix A, matrix B)
{
	matrix AB;
	for (int j = 0; j < B.size(); ++j)
		AB.push_back(multiply(A, B[j]));
	return AB;
}


vector<long double> sum(vector<long double> a, vector<long double> b)
{
	vector<long double> out;
	for (int i = 0; i < a.size(); ++i)
		out.push_back(a[i] + b[i]);
	return out;
}
matrix sum(matrix A, matrix B)
{
	matrix AB;
	for (int j = 0; j < A.size(); ++j)
	{
		vector<long double> temp;
		for (int i = 0; i < A[0].size(); ++i)
			temp.push_back(A[j][i] + B[j][i]);
		AB.push_back(temp);
	}
	return AB;
}


vector<long double> mpi(matrix& A, vector<long double>& b, int step)
{
	matrix B;
	for (int j = 0; j < A.size(); ++j)
	{
		vector<long double> temp;
		for (int i = 0; i < A.size(); ++i)
			if (i == j) temp.push_back(-A[i][j] + 1);
			else temp.push_back(-A[i][j]);
		B.push_back(temp);
	}
	cout << "cond. num of B - " << norm1(B) << endl;
	vector<long double> out = b;
	for (int i = 0; i < step; ++i)
	{
		out = sum(multiply(B, out), b);
	}
	return out;
}
vector<long double> gaussian(matrix A, vector<long double> B)
{
	vector<long double> X;
	for (int i = 0; i < A[0].size(); ++i)
		X.push_back(0);

	for (int j = 0; j < A.size() - 1; ++j)
	{
		//find max
		long double max = 0 + j;                        
		for (int i = max + 1; i < A[j].size(); ++i) 
			if (abs(A[j][i]) > abs(A[j][max])) max = i; 

		//change order rows
		long double temp;
		for (int k = j; k < A[j].size(); ++k)           
		{
			temp = A[k][j];
			A[k][j] = A[k][max];
			A[k][max] = temp;
		}												 
		temp = B[j];
		B[j] = B[max];
		B[max] = temp;
		max = A[j][j];

		//divide by max top row
		for (int k = j; k < A.size(); ++k)
			A[k][j] /= max;
		B[j] /= max;

		//transform every row
		for (int i = j + 1; i < A[j].size(); ++i)
		{
			temp = A[j][i];
			for (int k = j; k < A.size(); ++k)
				A[k][i] -= temp * A[k][j];

			B[i] -= B[j] * temp;
		}

	}
	//cout << B[B.size() - 1] << endl;
	//cout << A[A.size() - 1][A[0].size() - 1] << endl;
	X[B.size() - 1] = (B[B.size() - 1] / A[A.size() - 1][A[0].size() - 1]);

	long double temp;
	for (int i = A[0].size() - 2; i >= 0; --i)
	{
		temp = B[i];
		for (int j = i + 1; j < A.size(); ++j)
			temp -= A[j][i] * X[j];
		X[i] = temp;
	}

	return X;
}
long double condition_number(matrix A)
{
	return norm1(A) * norm1(inverse(A));
}
void test(matrix& A, vector<long double>& b, vector<long double> x)
{
	long double k;
	k = norm1(sum(multiply(A, x), multiply(b, -1)));
	cout << k << endl;
}
matrix vandermond(vector<long double> M)
{
	matrix V;
	for (size_t j = 0; j < M.size(); ++j)
	{
		vector<long double> temp;
		for (int i = 0; i < M.size(); ++i)
		{
			temp.push_back(pow(M[i], j));
		}
		V.push_back(temp);
	}
	return V;
}

