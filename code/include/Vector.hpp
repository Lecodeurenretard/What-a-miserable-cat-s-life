#pragma once
#include "Imports.hpp"

/**
 * A 2D vector
 */
struct Vector {
	float x;
	float y;

	static Vector fromPoints(const SDL_Point&, const SDL_Point&);
	static Vector fromPoints(const SDL_FPoint&, const SDL_FPoint&);
	
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

	bool operator==(const Vector&) const = default;
	bool operator!=(const Vector&) const = default;

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
	static const Vector DOWN;
	static const Vector RIGHT;
	static const Vector LEFT;
};
Vector operator*(float, const Vector&);
Vector operator/(float, const Vector&);
