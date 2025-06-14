#pragma once
#include "Warning.hpp"
#include "Utilities.hpp"

/** The null vector, it has coordonates (0, 0). */
#define Vec_ZERO	Vector{.x = 0, .y =  0}

/** The unit vector pointing right, in a (O, i, j) plane it would be $i$ */
#define Vec_RIGHT	Vector{.x = 1,.y =  0}

/** The unit vector pointing left, in a (O, i, j) plane it would be $-i$ */
#define Vec_LEFT	Vector{.x = -1, .y =  0}

/** The unit vector pointing up, in a (O, i, j) plane it would be $j$ */
#define Vec_UP		Vector{.x = 0, .y = -1}		//Based on SDL's coordonates system

/** The unit vector pointing down, in a (O, i, j) plane it would be $-j$ */
#define Vec_DOWN	Vector{.x = 0, .y =  1}

/**
 * A 2D vector.  
 * Aggregate type
 * Literal type
 */
struct Vector {
	double x;
	double y;

	static Vector fromPoints(SDL_Point, SDL_Point)	 noexcept;
	static Vector fromPoints(SDL_FPoint, SDL_FPoint) noexcept;
	
	~Vector(void) = default;

	/** Return the length of this vector.*/
	constexpr double norm(void) const noexcept {
		return roundDouble(std::sqrt(x*x + y*y));
	}

	Vector withNorm(double)	const;
	Vector unit(void)		const;

	void draw(SDL_Renderer* const, const Vector& = Vec_ZERO) const;
	std::string string(void) const noexcept;
	
	Vector rotate(double) const noexcept;

	/**
	 * Return the dot product between `v` and `u`.
	 */
	static constexpr double dotProduct(Vector v, Vector u) noexcept {
		return v.x*u.x + v.x*u.y;
	}

	static Vector lerp(Vector from, Vector to, double t)			noexcept;
	static Vector lerpNoRestrict(Vector from, Vector to, double t)	noexcept;

	SDL_FPoint translate(SDL_FPoint) const noexcept;

	bool operator==(const Vector&) const = default;
	bool operator!=(const Vector&) const = default;

	constexpr bool isNull(void) const noexcept {
		return x == 0 && y == 0;
	}

	Vector& operator+=(const Vector&) noexcept;
	Vector& operator-=(const Vector&) noexcept;
	Vector& operator*=(double) noexcept;
	Vector& operator/=(double) noexcept;

};





//unary + and minus
[[ nodiscard ]] constexpr Vector operator+(const Vector& v) noexcept {
	return v;
}

[[ nodiscard ]] constexpr Vector operator-(const Vector& v) noexcept {
	return Vector{
		.x = -v.x,
		.y = -v.y
	};
}

//binary addition and substraction
[[ nodiscard ]] constexpr Vector operator+(const Vector& v, const Vector& u) noexcept {
	return Vector{
		.x = roundDouble(u.x + v.x),
		.y = roundDouble(u.y + v.y)
	};
}

[[ nodiscard ]] constexpr Vector operator-(const Vector& v, const Vector& u) noexcept {
	return Vector{
		.x = roundDouble(u.x - v.x),
		.y = roundDouble(u.y - v.y)
	};
}

// multiplication and division by scalars
[[ nodiscard ]] constexpr Vector operator*(const Vector& v, double k) noexcept {
	return Vector{
		.x = roundDouble(v.x * k),
		.y = roundDouble(v.y * k)
	};
}

[[ nodiscard ]] constexpr Vector operator/(const Vector& v, double k) noexcept {
	return Vector{
		.x = roundDouble(v.x / k),
		.y = roundDouble(v.y / k)
	};
}

[[ nodiscard ]] constexpr Vector operator*(double k, const Vector& v) noexcept {
	return v * k;
}

[[ nodiscard ]] constexpr Vector operator/(double k, const Vector& v) noexcept {
	return v / k;
}
