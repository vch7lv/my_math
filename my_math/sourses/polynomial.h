#pragma once
#include <vector>
#include <iostream>

template <typename T>
struct polynomial;

template<typename T>
std::ostream& operator<< (std::ostream& stream, const polynomial<T>& p);

template <typename T>
struct polynomial 
{
private:
	std::vector<T> coef;
public:
	polynomial() : coef(std::vector<T>({ 0 })) {}
	polynomial(T num) : coef(std::vector<T>({ num })) {}
	polynomial(const std::vector<T>& array) : coef(array) {}
	polynomial(const polynomial& p) = default;
	polynomial(std::initializer_list<T> list)
	{
		for (auto elem : list)
			coef.push_back(elem);
	}
	~polynomial() = default;


	T operator()(T value) const
	{
		long double out = coef[coef.size() - 1];
		for (int n = coef.size() - 2; n >= 0; --n)
		{
			out = coef[n] + out * value;
		}
		return out;
	}
	T& operator[](int i)
	{
		return coef[i];
	}
	T operator[](int i) const
	{
		return this->coef[i];
	}
	polynomial& operator=(const polynomial&) = default;
	polynomial operator-()
	{
		for (int n = 0; n < this->coef.size(); ++n)
			coef[n] *= -1;
		return *this;
	}


	int dim() const
	{
		return this->coef.size() - 1;
	}
	polynomial take_derivative() const
	{
		if (this->dim() == 0) return static_cast<T>(0);
		polynomial out(std::vector<T>(this->coef.size() - 1));
		for (int n = 0; n < this->dim(); ++n)
			out.coef[n] = this->coef[n + 1] * (n + 1);
		return out;
	}
	polynomial take_derivative(int n) const
	{
		if (n > this->dim()) return 0.0;
		polynomial out(std::vector<long double>(this->coef.size() - n));


		for (int i = 0; i < out.coef.size(); ++i)
		{
			out[i] = this->coef[n + i];
			for (int j = n + i; j > i; --j)
				out[i] *= j;
		}
		return out;
	}

	polynomial& operator+=(T num)
	{
		this->coef[0] += num;
		return *this;
	}
	polynomial& operator+=(const polynomial& p)
	{
		size_t first_size = this->coef.size();

		if (p.dim() == this->dim())
		{
			int size = p.coef.size();
			for (int i = p.coef.size() - 1; i > 0; --i)
			{
				if (p[i] + this->coef[i] == 0)
					--size;
				else
					break;
			}
			this->coef.resize(size);
		}
		else
			this->coef.resize(std::max(p.coef.size(), this->coef.size()));

		for (int i = 0; i < std::min(this->coef.size(), std::min(p.coef.size(), first_size)); ++i)
		{
			this->coef[i] += p.coef[i];
		}
		for (int i = std::min(this->coef.size(), std::min(p.coef.size(), first_size));
			i < std::max(first_size, std::max(p.coef.size(), this->coef.size())); ++i)
		{
			if (first_size < p.coef.size())
				this->coef[i] = p.coef[i];
		}
		return *this;
	}
	polynomial& operator-=(T num)
	{
		return (*this) += (-num);
	}
	polynomial& operator-=(const polynomial& p)
	{
		size_t first_size = this->coef.size();

		if (p.dim() == this->dim())
		{
			int size = p.coef.size();
			for (int i = p.coef.size() - 1; i > 0; --i)
			{
				if (-p[i] + this->coef[i] == 0)
					--size;
				else
					break;
			}
			this->coef.resize(size);
		}
		else
			this->coef.resize(std::max(p.coef.size(), this->coef.size()));

		for (int i = 0; i < std::min(this->coef.size(), std::min(p.coef.size(), first_size)); ++i)
		{
			this->coef[i] -= p.coef[i];
		}
		for (int i = std::min(this->coef.size(), std::min(p.coef.size(), first_size));
			i < std::max(first_size, std::max(p.coef.size(), this->coef.size())); ++i)
		{
			if (first_size < p.coef.size())
				this->coef[i] = -p.coef[i];
		}
		return *this;
	}
	polynomial& operator*=(T num)
	{
		if (num == 0)
		{
			this->coef.resize(1);
			this->coef[0] = 0;
		}
		else
		{
			for (int i = 0; i < this->coef.size(); ++i)
			{
				this->coef[i] *= num;
			}
		}
		return *this;
	}
	polynomial& operator*=(const polynomial& p)
	{
		std::vector<T> oT(this->coef);

		this->coef = std::vector<T>((this->dim() + p.dim() + 1), 0);

		for (size_t j = 0; j < this->coef.size(); ++j)
		{
			for (size_t i = 0; i < std::min(j + 1, oT.size()); ++i)
				if (j - i < p.coef.size())
					this->coef[j] += oT[i] * p.coef[j - i];
		}
		return *this;
	}

	friend std::ostream& operator<< <>(std::ostream& stream, const polynomial<T>& p);
};
	template<typename T>
	std::ostream& operator<< (std::ostream& stream, const polynomial<T>& p)
	{
		if (p.coef.size() == 1)
		{
			stream << p.coef[0];
			return stream;
		}
		for (int i = p.coef.size() - 1; i >= 0; --i)
		{
			if (p.coef[i] != 0)
			{
				if (p.coef[i] > 0 && i != p.coef.size() - 1)
					stream << " + ";
				if (p.coef[i] < 0 && p.coef[i] != 0)
					stream << " - ";
				if ((p.coef[i] != 1 && p.coef[i] != -1) || i == 0)
					stream << abs(p.coef[i]);
				if (i > 0)
					stream << 'X';
				if (i > 1)
					stream << "^" << i;
			}
		}
		return stream;
	}

	template<typename T>
	polynomial<T> operator+(const polynomial<T>& p1, const polynomial<T>& p2)
	{
		polynomial<T> answer = p1;
		return answer += p2;
	}

	template<typename T>
	polynomial<T> operator-(const polynomial<T>& p1, const polynomial<T>& p2)
	{
		polynomial<T> answer = p1;
		return answer -= p2;
	}

	template<typename T>
	polynomial<T> operator*(const polynomial<T>& p1, const polynomial<T>& p2)
	{
		polynomial<T> answer = p1;
		return answer *= p2;
	}

