#include "bigint.h"
#include <iostream>


bigint::bigint(long long num) : sign(num >= 0)
{
	num = (num >= 0) ? num : (-1) * num;              //make positive;
	do {
		this->digits.push_back(num % base);	
		num /= base;								  //add digits;
	} 
	while (num != 0);
}
bigint::bigint(std::initializer_list<int> l): sign(*l.begin() >= 0)
{
	for (auto p = l.end()-1; p >= l.begin(); --p)
		if (p == l.begin())
			digits.push_back(abs(*p));
		else
			digits.push_back(*p);

}
bigint::bigint(const std::vector<int>& digits_): sign(digits_[0] >= 0)
{
	for (int i = digits_.size() - 1; i >= 0; --i)
		if (i == 0)
			digits.push_back(abs(digits_[i]));
		else
			digits.push_back(digits_[i]);
}
bigint::bigint(const std::string& str): sign(str[0] != '-')
{
	int bottom = (str[0] == '-') ? 1 : 0;

	for (int i = bottom; i < str.size(); ++i)
		if (str[i] < '0' || str[i] > '9')
			throw(std::exception());

	for (int i = str.size() - 1; i >= bottom; i -= 9)
		if (i >= 9 + bottom)
			digits.push_back(stoi(str.substr(i-8, 9)));
		else
			digits.push_back(stoi(str.substr(bottom, i - bottom + 1)));
}


bool bigint::operator<(const bigint& b) const
{
	if (b.sign != sign) return !sign;

	if (b.digits.size() != digits.size())
		return (b.digits.size() > digits.size() ? sign : !sign);
	else
		for (int i = digits.size() - 1; i >= 0; --i)
			if (b.digits[i] != digits[i])
				return (b.digits[i] > digits[i] ? sign : !sign);
	return false;
}
bool bigint::operator>(const bigint& b) const
{
	return b < *this;
}
bool bigint::operator==(const bigint& b) const
{
	if (b.digits.size() != digits.size()
		|| b.sign != sign) return false;
	for (int i = digits.size() - 1; i >= 0; --i)
		if (b.digits[i] != digits[i]) return false;
	return true;
}
bool bigint::operator>=(const bigint& b) const
{
	return !(b > *this);
}
bool bigint::operator<=(const bigint& b) const
{
	return !(*this > b);
}


bigint& bigint::operator+=(const bigint& b)
{
	int carry = 0;

	if (sign == b.sign)
	{
		for (int i = 0; i < std::max(b.digits.size(), digits.size()) || carry; ++i)
		{
			if (i >= digits.size()) digits.push_back(0);
			digits[i] += carry + (i >= b.digits.size() ? 0 : b.digits[i]);
			carry = digits[i] >= base;
			if (carry) digits[i] -= base;
		}
	}
	else
	{
		sign = !sign;                                             // to compare number modulo
		if (*this >= b && sign || *this < b && !sign)
		{
			for (int i = 0; i < b.digits.size() || carry; ++i)
			{
				digits[i] -= carry + (i < b.digits.size() ? b.digits[i] : 0);
				carry = digits[i] < 0;
				if (carry) digits[i] += base;
			}
			sign = !sign;
		} 
		else
		{
			for (int i = 0; i < b.digits.size() || carry; ++i)
			{
				if (i >= digits.size()) digits.push_back(0);
				digits[i] = b.digits[i] - digits[i] - carry;
				carry = digits[i] < 0;
				if (carry) digits[i] += base;
			}
		}
		for (int i = digits.size() - 1; i > 0; --i)
			if (digits[i] == 0) digits.pop_back();
			else break;
	}
	return *this;
}
bigint& bigint::operator-=(const bigint& b)
{
	int carry = 0;
	sign = !sign;
	if (sign == b.sign)
	{
		for (int i = 0; i < std::max(b.digits.size(), digits.size()) || carry; ++i)
		{
			if (i >= digits.size()) digits.push_back(0);
			digits[i] += carry + (i >= b.digits.size() ? 0 : b.digits[i]);
			carry = digits[i] >= base;
			if (carry) digits[i] -= base;
		}
	}
	else
	{
		sign = !sign;                                             // to compare number modulo
		if (*this >= b && sign || *this < b && !sign)
		{
			for (int i = 0; i < b.digits.size() || carry; ++i)
			{
				digits[i] -= carry + (i < b.digits.size() ? b.digits[i] : 0);
				carry = digits[i] < 0;
				if (carry) digits[i] += base;
			}
			sign = !sign;
		}
		else
		{
			for (int i = 0; i < b.digits.size() || carry; ++i)
			{
				if (i >= digits.size()) digits.push_back(0);
				digits[i] = b.digits[i] - digits[i] - carry;
				carry = digits[i] < 0;
				if (carry) digits[i] += base;
			}
		}
		for (size_t i = digits.size() - 1; i > 0; --i)
			if (digits[i] == 0) digits.pop_back();
			else break;
	}
	sign = !sign;
	return *this;
}
bigint& bigint::operator*=(const bigint& b)
{
	*this = *this * b;
	return *this;
}
bigint& bigint::operator++()
{
	return *this += 1;
}
bigint bigint::operator++(int)
{
	bigint out = *this;
	*this += 1;
	return out;
}
bigint& bigint::operator--()
{
	return *this -= 1;
}
bigint bigint::operator--(int)
{
	bigint out = *this;
	*this -= 1;
	return out;
}


inline int count_digits(int num)
{
	int sz = 1;
	while ( (num /= 10) != 0 )
		++sz;
	return sz;
}
std::ostream& operator<<(std::ostream& stream, const bigint& num)
{
	if (num.sign == false)
	{
		std::cout << '-';
	}

	for (int i = num.digits.size() - 1; i >= 0; --i)
	{
		if (i != num.digits.size() - 1)
			stream << '\'';

		if (count_digits(num.digits[i]) == 9 || i == num.digits.size() - 1)
			stream << num.digits[i];
		else
		{
			for (int j = 0; j < 9 - count_digits(num.digits[i]); ++j)
				stream << 0;
			stream << num.digits[i];
		}
	}
	return stream;
}
bigint operator""_bi(const char* s )
{
	return bigint(static_cast<std::string>(s));
}


bigint operator+(const bigint& operand1, const bigint& operand2)
{
	bigint out(operand1);
	out += operand2;
	return out;
}
bigint operator-(const bigint& operand1, const bigint& operand2)
{
	bigint out(operand1);
	out -= operand2;
	return out;
}
bigint operator*(const bigint& operand1, const bigint& operand2)
{
	bigint out;
	out.sign = operand1.sign == operand2.sign;
	for (int i = 0; i < operand1.digits.size(); ++i)
	{
		size_t carry = 0;
		for (int j = 0; j < operand2.digits.size() || carry; ++j)
		{
			if (i + j + 1 > out.digits.size()) out.digits.push_back(0);
			carry += ((i < operand1.digits.size() && j < operand2.digits.size()) ?
				static_cast<size_t>(operand1.digits[i]) * operand2.digits[j] : 0) + out.digits[i + j];
			out.digits[i + j] = carry % operand1.base;
			carry /= operand1.base;
		}
	}
	return out;
}
