#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

//Complex number class with our required functions.
class Complex
{
	long double r;
	long double i;

public:
	Complex(long double real, long double imag)
	{
		r = real;
		i = imag;
	}
	void add(Complex z) // adding 2 cmplx num's
	{
		r += z.getr();
		i += z.geti();
	}
	void mul(Complex z) // multiplying 2 cmplx num's
	{
		long double rt = r * z.getr() - i * z.geti();
		long double it = (r * z.geti()) + (i * z.getr());

		r = rt;
		i = it;
	}
	double getr() { return r; }
	double geti() { return i; }

	double sep(Complex z) //seperation b/w 2 complex numbers
	{
		Complex res = *this;
		z.mul(Complex(-1.00, 0));
		res.add(z);
		return res.mag();
	}
	double mag()  //magnitude of a cmplx number.
	{
		long double res = r * r + i * i;
		return sqrt(res);
	}
	sf::Color mand() //determining the state of the number in mandelbrot set.
	{

		int iterations = 50;
		Complex c = Complex(r, i);
		if( c.mag() > 2 ){

		}

		// eliminating the 'OVBIOUS' regions in the set.
		if (c.sep(Complex(-1, 0)) < 0.25)
			return sf::Color::Black;
		if (c.mag() < 0.25)
			return sf::Color::Black;
		if (r < 0 && c.mag() < 0.5)
			return sf::Color::Black;

		 //iterating 100 times and giving colors based on the iteration where loop breaks.
		for (int i = 0; i < iterations; i++)
		{
			c.mul(c);
			c.add(*this);
			// we do z_new --> z_old^2 + z.

			if (c.mag() > 2)
				return pixelcolor(i,iterations);
		}
		return sf::Color::Black;
	}

	sf::Color pixelcolor(int count,int precision) // giving color based on iterations.
	{

		    if (count < precision / 4.0f)
            {
                return sf::Color(count * 255.0f / (precision / 4.0f), 0, 0);
            }
            else if (count < precision / 2.0f)
            {
                return sf::Color(0, count * 255.0f / (precision / 2.0f), 0);
            }
            else if (count < precision)
            {
                return sf::Color(0, 0, count * 255.0f / precision);
            }

	}
};