#ifndef COLOR_H
#define COLOR_H
#pragma once

namespace Framework
{
    class Color {
	private:
		float _r = 1;
		float _g = 1;
		float _b = 1;
		float _a = 1;    
	public:
		Color();
		Color(float r, float g, float b, float a = 1);

		Color operator*(const Color& other);
		Color operator+(const Color& other);

		inline float get_r() const { return _r; }
		inline float get_g() const { return _g; }
		inline float get_b() const { return _b; }
		inline float get_a() const { return _a; }
		
		const static Color WHITE;
		const static Color BLACK;
		const static Color RED;
		const static Color GREEN;
		const static Color BLUE;
    };
}

#endif // COLOR_H