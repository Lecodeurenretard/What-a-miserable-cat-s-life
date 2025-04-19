#include "../include/Vector.hpp"

/** The null vector */
const Vector Vector::ZERO	= {.x = 0, .y = 0};

/** The unit vector pointing right, in a (O, i, j) plane it would be $i$ */
const Vector Vector::RIGHT	= {.x = -1,.y =  0};		//Based on SDL's coordonates system

/** The unit vector pointing right, in a (O, i, j) plane it would be $-i$ */
const Vector Vector::LEFT	= {.x = 1, .y = 0};		//Based on SDL's coordonates system

/** The unit vector pointing up, in a (O, i, j) plane it would be $j$ */
const Vector Vector::UP		= {.x = 0, .y = -1};

/** The unit vector pointing up, in a (O, i, j) plane it would be $-j$ */
const Vector Vector::DOWN	= {.x = 0, .y = 1};

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(const SDL_Point& from, const SDL_Point& to) {
	return Vector{
		.x = static_cast<float>(to.x - from.x),		//implicit int to float throws a warning
		.y = static_cast<float>(to.y - from.y)
	};
}

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(const SDL_FPoint& from, const SDL_FPoint& to) {
	return Vector{
		.x = to.x - from.x,
		.y = to.y - from.y
	};
}

/**
 * Return the opposite of `this`.
 */
Vector Vector::opposite(void) const {
	return this->operator*(-1);
}

/**
 * Return the length of this vector.
 */
float Vector::norm(void) const {
	return std::sqrt(x*x + y*y);
}

/**
 * Scales the current vector to have a length of `newNorm` (`newNorm` will always be treated as positive).
 */
Vector Vector::withNorm(float newNorm) const {
	return unit() * abs(newNorm);
}

/**
 * Get corresponding unit vector.
 */
Vector Vector::unit(void) const {
	const float length(norm());
	if(length == 0)
		wout << "Division by zero in " + string() << ".unit()" << std::endl;
	return operator/(length);
}

/**
 * Draw the vector in the renderer, the tail of the vector is located at `start`.
 */
void Vector::draw(SDL_Renderer* const r, const Vector& start /*= Vector::ZERO*/) const {
	if(operator==(Vector::ZERO)) {
		SDL_RenderDrawPointF(r, start.x, start.y);
		return;
	}

	const Vector end = operator+(start);
	SDL_RenderDrawLineF(r, start.x, start.y, end.x, end.y);

	const Vector posHead1 = end + rotate(M_PI+M_PI/4).withNorm(norm()/5);	//rotate 135°
	const Vector posHead2 = end + rotate(M_PI-M_PI/4).withNorm(norm()/5);	//same but anti clockwise
	SDL_RenderDrawLineF(r, end.x, end.y, posHead1.x, posHead1.y);
	SDL_RenderDrawLineF(r, end.x, end.y, posHead2.x, posHead2.y);
}


/**
 * Output a human-readable representation of the vector.
 */
[[ nodiscard ]] std::string Vector::string(void) const {
	return "Vector{"+ std::to_string(x) + ", " + std::to_string(y) + "}";
}

/**
 * Rotate the vector around its tail by `angle` radians.
 */
[[ nodiscard ]] Vector Vector::rotate(float angle) const {
	return Vector{
		.x = x * std::cos(angle) - y * std::sin(angle),
		.y = x * std::sin(angle) + y * std::cos(angle)
	};
}

/**
 * Return the dot product between `v` and `u`.
 */
float Vector::dotProduct(const Vector& v, const Vector& u) {
	return v.x*u.x + v.x*u.y;
}


/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go, if not in [-1, 1], it gets treated as the closest bound (ie: if `-5`, gets treated as `-1`).
 */
[[ nodiscard ]] Vector Vector::lerp(const Vector& from, const Vector& to, float t) {
	if(t > 1)
		t = 1;
	else if(t < -1)
		t = -1;

	return lerpNoRestrict(from, to, t);
}

/**
 * Calculates the LERP between `from` and `to` with no restriction for `t`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go.
 */
[[ nodiscard ]] Vector Vector::lerpNoRestrict(const Vector& from, const Vector& to, float t) {
	return from + t*(to - from);
}



/**
 * Translate `p` by `this` vector
 */
SDL_FPoint Vector::translate(SDL_FPoint p) const {
	p.x += x;
	p.y += y;
	return p;
}


[[ nodiscard ]] Vector Vector::operator+(const Vector& v) const {
	return Vector{
		.x = x + v.x,
		.y = y + v.y
	};
}

[[ nodiscard ]] Vector Vector::operator-(const Vector& v) const {
	return Vector{
		.x = x - v.x,
		.y = y - v.y
	};
}

[[ nodiscard ]] Vector Vector::operator*(float k) const {
	return Vector{
		.x = x * k,
		.y = y * k
	};
}

[[ nodiscard ]] Vector Vector::operator/(float k) const {
	return Vector{
		.x = x / k,
		.y = y / k
	};
}

[[ nodiscard ]] Vector& Vector::operator+=(const Vector& v) {
	x += v.x;
	y += v.y;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator-=(const Vector& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator*=(float k) {
	x *= k;
	y *= k;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator/=(float k) {
	x /= k;
	y /= k;
	return *this;
}

[[ nodiscard ]] Vector operator*(float k, const Vector& v) {
	return v * k;
}

[[ nodiscard ]] Vector operator/(float k, const Vector& v) {
	return v / k;
}