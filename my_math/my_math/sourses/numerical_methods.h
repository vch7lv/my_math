#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <cmath>
#include "polynomial.h"
#include <vector>
#include "numerical_methods.h"
#include <functional>
using namespace std;

vector<long double> chebyshev_nodes(long double left, long double right, long double n)
{
	vector<long double> out;
	for (int k = 1; k <= n; ++k)
		out.push_back(0.5 * (left + right) + 0.5 * (right - left) * cos((2 * k - 1) * M_PI / (2 * n)));
	return out;
}

template <typename T>
polynomial<T> Lagrange_approximation(const vector<pair<long double, long double>>& table)
{
	polynomial<T> out(0);

	for (int i = 0; i < table.size(); ++i)
	{
		polynomial<T> l(1);
		long double A = 1;
		for (int j = 0; j < table.size(); ++j)
			if (j != i)
			{
				l *= polynomial<T>({ -(table[j]).first, 1 });
				A /= (table[i].first - table[j].first);
			}
		out += l * A * table[i].second;
	}

	return out;
}

template <typename T>
polynomial<T> Newton_approximation(const vector<pair<long double, long double>>& table)
{
	polynomial<T> out(table[0].second);
	polynomial<T> p = 1;
	long double a;
	for (int i = 1; i < table.size(); ++i)
	{
		p *= polynomial<T>(vector<long double>({ -(table[i - 1].first), 1 }));
		a = (table[i].second - out(table[i].first)) / p(table[i].first);
		out += a * p;
	}
	return out;
}

template <typename T>
polynomial<T> Newton_approximation_add_node(const polynomial<T>& polinom, const vector<pair<long double, long double>>& table)
{
	polynomial<T> p = 1;
	for (int i = 0; i < table.size() - 1; ++i)
	{
		p *= polynomial<T>(vector<long double>({ -(table[i].first), 1 }));
	}
	long double a = (table[table.size() - 1].second - polinom(table[table.size() - 1].first)) / p(table[table.size() - 1].first);
	return polinom + a * p;
}

long double rectangle_method(long double a, long double b, std::function<long double(long double)> f, long double n)
{
	// calculates the integral by rectangle_method
	long double out = 0;
	for (long double x = a; x < b; x += (b - a) / n)
	{
		out += f(x);
	}
	out *= (b - a) / n;
	return out;
}
long double trapezoidal_method(long double a, long double b, std::function<long double(long double)> f, long double n)
{
	// calculates the integral by trapezoidal_method
	long double out = 0;
	for (long double x = a; x < b; x += (b - a) / n)
	{
		out += (f(x) + f(x + (b - a) / n)) / 2;
	}
	out *= (b - a) / n;
	return out;
}
/*
template <typename T>
vec<T> gauss(matrix<T> A, vec<T> b)
{
	long long m = A.row_size();
	long long n = A.col_size();

	for (int i = 0; i < m; ++i)
	{
		int max = i;										  //index of "max" row
		for (int j = i; j < n; ++j)
			if (abs(A.cols[i][max]) < abs(A.cols[i][j])) max = j; //find max elem in column

		if (A.cols[i][max] == 0) break;

		if (max != i)
		{
			for (int j = i; j < n; ++j)					//swap rows
				swap(m.cols[j][i], m.cols[j][max]);
			out.swap_rows(i, max);
		}

		for (int k = 0; k < n; ++k)					//divide on max
			out.cols[k][i] /= m.cols[i][i];
		for (int k = n - 1; k >= i; --k)
			m.cols[k][i] /= m.cols[i][i];

		for (int j = 0; j < n; ++j)
		{
			if (j != i)
			{
				for (int k = 0; k < n; ++k)
					out.cols[k][j] -= out.cols[k][i] * m.cols[i][j];
				for (int k = n - 1; k > i; --k)
					m.cols[k][j] -= m.cols[k][i] * m.cols[i][j];
				m.cols[i][j] = 0;
			}
		}
	}
	return out;
}*/