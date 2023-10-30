#pragma once
#include "matrix.h" 
#include <vector>
#include <iostream>

typedef long double T;
using namespace std;

template<typename T>
struct vec
{
	vector<T> col;
	vec(vector<T> v) : col(v) {};
	vec(std::initializer_list<T> list) : col(list) {};
	T& operator [] (size_t index)
	{
		return col[index];
	}
	T operator [] (size_t index) const
	{
		return col[index];
	}
	vec& operator *= (T num)
	{
		for (auto el : col)
			el *= num;
		return *this;
	}
	vec operator * (T num)
	{
		vec out(*this);
		out *= num;
		return out;
	}
	vec& operator += (const vec& v2)
	{
		if (v2.col.size() != col.size())
		{
			throw(std::exception("wrong dim while sum vec"));
			return *this;
		}
		for (int i = 0; i < col.size(); ++i)
			col[i] += v2[i];
		return *this;
	}
	vec operator + (const vec& v2)
	{
		vec out(*this);
		out += v2;
		return out;
	}
	vec& operator -= (const vec& v2)
	{
		if (v2.col.size() != col.size())
		{
			throw(std::exception("wrong dim while sum vec"));
			return *this;
		}
		for (int i = 0; i < col.size(); ++i)
			col[i] -= v2[i];
		return *this;
	}
	vec operator - (const vec& v2)
	{
		vec out(*this);
		out -= v2;
		return out;
	}
};

template <typename T>
struct matrix
{
	std::vector<vector<T>> cols;

	matrix(vector <vector<T>> v) : cols(v) {};
	matrix(std::initializer_list<std::initializer_list<T>> cols_)
	{
		size_t n = (*cols_.begin()).size();
		for (auto c : cols_)
		{
			if (c.size() != n) throw(exception("incorrect input in matrix"));
			cols.push_back(c);
		}
	}
	matrix(T number, int dim) : cols(std::vector<std::vector<T>>(dim, std::vector<T>(dim)))
	{
		for (int i = 0; i < dim; ++i)
			cols[i][i] = number;
	}

	size_t col_size() const
	{
		return cols[0].size();
	}
	size_t row_size() const
	{
		return cols.size();
	}
	bool is_square() const 
	{
		return (col_size() == row_size());
	}
	void swap_cols(int i, int j)
	{
		std::swap(cols[i], cols[j]);
	}
	void swap_rows(int i, int j)
	{
		if (i >= col_size() ||
			j >= col_size() ||
			i < 0 ||
			j < 0)
		{
			throw(std::exception("wrong number of rows while swap"));
			return;
		}
		for (int k = 0; k < row_size(); ++k)
			std::swap(cols[k][i], cols[k][j]);
	}

	matrix& operator+= (const matrix& m)
	{
		if (this->cols_size() != m.cols_size() ||
			this->rows_size() != m.rows_size()) throw(std::exception("wrong dim while sum"));

		for (int i = 0; i < row_size(); ++i)
			for (int j = 0; j < col_size(); ++j)
				cols[i][j] += m.cols[i][j];
		return *this;
	}
	matrix& operator-=(const matrix& m)
	{
		if (this->col_size() != m.col_size() ||
			this->row_size() != m.row_size()) throw(std::exception("wrong dim while sum"));

		for (int i = 0; i < row_size(); ++i)
			for (int j = 0; j < col_size(); ++j)
				cols[i][j] -= m.cols[i][j];
		return *this;
	}
	matrix operator * (const matrix& m) const
	{
		if (cols.size() != m.cols[0].size()) throw(std::exception("wrong dim while multi matrix"));
		vector<vector<T>> cols_(m.cols.size(), vector<T>(cols[0].size(), 0));
		for (int j = 0; j < cols_.size(); ++j)
			for (int i = 0; i < cols_[0].size(); ++i)
			{
				for (int k = 0; k < cols.size(); ++k)
					cols_[j][i] += cols[k][i] * m.cols[j][k];
			}
		return matrix(cols_);
	}
	matrix& operator *= (const T num)
	{
		for (auto col : cols)
			for (auto el : col)
				el *= num;
		return *this;
	}
	matrix operator * (const T num) const
	{
		matrix out(*this);
		for (auto col : out.cols)
			for (auto el : col)
				el *= num;
		return out;
	}

	matrix transpose() const
	{
		if (!is_square()) throw(std::exception("wrong dim while transpose"));
		matrix out(vector<vector<T>>(cols.size(), vector<T>(cols.size())));
		for (int j = 0; j < cols.size(); ++j)
			for (int i = 0; i < cols[0].size(); ++i)
				out.cols[i][j] = cols[j][i];
		return out;
	}
	T sp() const {
		if (!is_square()) throw(std::exception("wrong dim while count sp"));
		T out = 0;
		for (int i = 0; i < cols.size(); ++i)
			out += cols[i][i];
		return out;
	}
	T det() const
	{
		if (!is_square()) throw(std::exception("wrong dim while calculating det"));

		matrix m(*this);
		T out = 1;
		int n = col_size();

		for (int i = 0; i < n-1; ++i)
		{
			int max = i;											  //index of "max" row
			for (int j = i; j < n; ++j)
				if (abs(m.cols[i][max]) < abs(m.cols[i][j])) max = j; //find max elem in column

			if (m.cols[i][max] == 0) return 0;

			if (max != i)
			{
				for (int j = i; j < n; ++j)							  //swap rows
					swap(m.cols[j][i], m.cols[j][max]);
				out *= -1;
			}

			for (int j = i + 1; j < n; ++j)						
			{
				T dif = (m.cols[i][j] / m.cols[i][i]);			  
				for (int k = i + 1; k < n; ++k)
					m.cols[k][j] -= m.cols[k][i] * dif;
				m.cols[i][j] = 0;
			}
		}

		for (int i = 0; i < n; ++i) out *= m.cols[i][i];
		return out;
	}
	matrix inverse() const {
		if (!is_square()) throw(std::exception("wrong dim while inverse"));

		long long n = col_size();
		matrix m(*this);
		matrix out(1,n);

		for (int i = 0; i < n; ++i)
		{
			int max = i;										  //index of "max" row
			for (int j = i; j < n; ++j)
				if (abs(m.cols[i][max]) < abs(m.cols[i][j])) max = j; //find max elem in column

			if (m.cols[i][max] == 0)
			{
				throw(std::exception("det of matrix = 0 while inverse"));
				return *this;
			}
			
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
	}
};


template<typename T>
vec<T> operator * (matrix<T> m, vec<T> v)
{
	vec<T> out(vector<T>(m.cols.size(), 0));
	for (int i = 0; i < m.cols.size(); ++i)
	{
		for (int j = 0; j < out.col.size(); ++j)
		{
			out.col[i] += m.cols[i][j] * v[j];
		}
	}
	return out;
}

ostream& operator << (ostream& stream, const vec<long double> v)
{
	stream.setf(ios::scientific);
	std::streamsize old_precision = stream.precision();
	stream.precision(2);

	stream << '(';
	for (auto el : v.col)
	{
		stream << el << ' ';
	}
	stream << ')' << endl;

	stream.unsetf(ios::scientific);
	stream.precision(old_precision);
	return stream;
}
ostream& operator << (ostream& stream, const matrix<long double>& m)
{
	stream.setf(ios::scientific);
	std::streamsize old_precision = stream.precision();
	stream.precision(2);

	for (int j = 0; j < m.col_size(); ++j)
	{
		stream << '[';
		for (int i = 0; i < m.row_size(); ++i)
		{
			stream << m.cols[i][j];
			if (m.cols[i][j] >= 0)
				stream << ' ';
			if (i != m.row_size()-1) stream << ' ';
		}
		stream << ']' << endl;
	}

	stream.unsetf(ios::scientific);
	stream.precision(old_precision);
	return stream;
}