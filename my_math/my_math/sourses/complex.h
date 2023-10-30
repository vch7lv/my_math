#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

template <typename T>

struct complex
{
	T Re;
	T Im;

	complex(T Re_) : Re(Re_), Im(0) {};
	complex(T Re_, T Im_) : Re(Re_), Im(Im_) {};
	~complex() = default;

	T abs() 
	{
		return sqrt(Re*Re + Im*Im);
	}
	T Arg()
	{
		return (Re >= 0 ? atan(Im / Re) : atan(Im / Re) + (Im >= 0 ? M_PI : -M_PI));
	}


	complex operator += (const complex& z)
	{
		Re += z.Re;
		Im += z.Im;
	}
	complex operator -= (const complex& z)
	{
		Re -= z.Re;
		Im -= z.Im;
	}
	complex operator *= (const complex& z)
	{
		Re = Re * z.Re - Im * z.Im;
		Im = Im * z.Re + Re * z.Im;
	}
	complex operator /= (const complex& z)
	{
		Re = (Re * z.Re + Im * z.Im) / (z.Re * z.Re + z.Im * z.Im);
		Im = (Im * z.Re - Re * z.Im) / (z.Re * z.Re + z.Im * z.Im);
	}
	

	bool operator > (complex z) 
	{
		return Re * Re + Im * Im > z.Re * z.Re + z.Im * z.Im;
	}

};



template <typename T>
complex<T> operator + (complex<T> a, complex<T> b)
{
	return a += b;
}

template <typename T>
complex<T> operator - (complex<T> a, complex<T> b)
{
	return a -= b;
}

template <typename T>
complex<T> operator * (complex<T> a, complex<T> b)
{
	return a *= b;
}

template <typename T>
complex<T> operator / (complex<T> a, complex<T> b)
{
	return a -= b;
}



template <typename T>
T abs(complex<T> num)
{
	return num.abs();
}



template <typename T>
std::ostream& operator << (std::ostream& stream, complex<T> num)
{
	if (num.Im != 0)
		stream << num.Re << (num.Im > 0 ? " + " : " - ") << "i*" << abs(num.Im);
	else
		stream << num.Re;
	return stream;
}

