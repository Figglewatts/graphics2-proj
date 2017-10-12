#include "Color.h"

#include <cmath>

namespace Framework
{
	Color::Color()
	{
		this->_r = 1;
		this->_g = 1;
		this->_b = 1;
		this->_a = 1;
	}
	
	Color::Color(float r, float g, float b, float a)
	{
		float absR = std::abs(r);
		float absG = std::abs(g);
		float absB = std::abs(b);
		float absA = std::abs(a);
		this->_r = absR > 1 ? 1 : absR;
		this->_g = absG > 1 ? 1 : absG;
		this->_b = absB > 1 ? 1 : absB;
		this->_a = absA > 1 ? 1 : absA;
	}
	
	// multiplicative color combination
	Color Color::operator*(const Color& other)
	{
		return Color(this->_r * other._r,
				this->_g * other._g,
				this->_b * other._b,
				this->_a * other._a);
	}
	
	// additive color combination
	Color Color::operator+(const Color& other)
	{
		return Color(this->_r + other._r,
				this->_g + other._g,
				this->_b + other._b,
				this->_a + other._a);
	}
	
	const Color Color::WHITE = Color(1, 1, 1);
	const Color Color::BLACK = Color(0, 0, 0);
	const Color Color::RED = Color(1, 0, 0);
	const Color Color::GREEN = Color(0, 1, 0);
	const Color Color::BLUE = Color(0, 0, 1);
}