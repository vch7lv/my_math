#pragma once

#include <vector>
#include <string>


struct bigint
{
	static const int base = 1000'000'000;
	bool sign = true;
	std::vector<int> digits; 

	// bigint stores number as vector of digits by base (in reverse order)


	bigint() = default;
	bigint(long long number);
	bigint(std::initializer_list<int>);
	bigint(const std::vector<int>&);
	bigint(const std::string& number);
	bigint(const bigint&) = default;
	~bigint() = default;
	bigint& operator = (const bigint&) = default;

	bool operator < (const bigint&) const;
	bool operator > (const bigint&) const;
	bool operator == (const bigint&) const;
	bool operator >= (const bigint&) const;
	bool operator <= (const bigint&) const;


	bigint& operator += (const bigint&);
	bigint& operator -= (const bigint&);
	bigint& operator *= (const bigint&);
//	bigint& operator /= (const bigint&);
//	bigint& operator %= (const bigint&);
	bigint& operator ++ ();
	bigint operator ++ (int);
	bigint& operator -- ();
	bigint operator -- (int);
};

std::ostream& operator << (std::ostream& stream, const bigint&);
bigint abs(const bigint&);

bigint operator ""_bi(const char*);
bigint operator + (const bigint& operand1, const bigint& operand2);
bigint operator - (const bigint& operand1, const bigint& operand2);
bigint operator * (const bigint& operand1, const bigint& operand2);
//bigint operator / (const bigint& operand1, const bigint& operand2);
//bigint operator % (const bigint& operand1, const bigint& operand2);