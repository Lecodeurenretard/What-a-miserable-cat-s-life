#pragma once
#include "Imports.hpp"

/**
 * A 2D vector
 */
struct Vector{
	float x;
	float y;

	Vector(float, float);
	Vector(const SDL_Point&, const SDL_Point&);
	Vector(const SDL_FPoint&, const SDL_FPoint&);
	
	Vector(const Vector&)	= default;
	~Vector(void)			= default;

	Vector opposite(void) const;

	float norm(void) const;
	Vector withNorm(float) const;
	Vector unit(void) const;
	void draw(SDL_Renderer* const, const Vector& = Vector::ZERO) const;
	std::string string(void) const;
	
	Vector rotate(float) const;

	static float dotProduct(const Vector&, const Vector&);

	static Vector lerp(const Vector&, const Vector&, float);
	static Vector lerpNoRestrict(const Vector&, const Vector&, float);

	SDL_FPoint translate(SDL_FPoint) const;

	Vector operator+(const Vector&) const;
	Vector operator-(const Vector&) const;
	Vector operator*(float) const;
	Vector operator/(float) const;

	Vector& operator+=(const Vector&);
	Vector& operator-=(const Vector&);
	Vector& operator*=(float);
	Vector& operator/=(float);

	static const Vector ZERO;
	static const Vector UP;
	static const Vector RIGHT;
};
Vector operator*(float, const Vector&);
Vector operator/(float, const Vector&);
