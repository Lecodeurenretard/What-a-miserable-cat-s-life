#pragma once
#include "Imports.hpp"

/**
 * A 2D vector
 */
struct Vector {
	uint x;
	uint y;

	Vector(uint, uint);
	Vector(const SDL_Point&, const SDL_Point&);

	Vector(const Vector&)	= default;
	~Vector(void)			= default;

	
	std::string string(void) const;
	
	static Vector lerp(const Vector&, const Vector&, float);

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator*(uint) const;
	Vector operator/(uint) const;

	static const Vector ZERO;
};

Vector operator*(uint, const Vector&);
Vector operator/(uint, const Vector&);