#include "../include/Vector.hpp"

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(SDL_Point from, SDL_Point to) noexcept {
	return Vector{
		.x = static_cast<float>(to.x - from.x),		//implicit int to float throws a warning
		.y = static_cast<float>(to.y - from.y)
	};
}

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(SDL_FPoint from, SDL_FPoint to) noexcept {
	return Vector{
		.x = to.x - from.x,
		.y = to.y - from.y
	};
}



/**
 * Scales the current vector to have a length of `newNorm` (`newNorm` will always be treated as positive).
 * @throw An `std::logic_error` if the vector has a norm of 0.
 */
Vector Vector::withNorm(float newNorm) const {
	return unit() * abs(newNorm);
}

/**
 * Get corresponding unit vector.
 * @throw An `std::logic_error` if the vector has a norm of 0.
 */
Vector Vector::unit(void) const {
	const float length(norm());
	if(length == 0)
		throw std::logic_error("Division by zero in " + string() +".unit(): the norm of the current vector is 0");
	return *this / length;
}

/**
 * Draw the vector in the renderer, the tail of the vector is located at `start`.
 * @throw If fail to draw a line, throw a `runtime_error`.
 */
void Vector::draw(SDL_Renderer* const r, const Vector& start /*= Vec_ZERO*/) const {
	if(isNull()) {
		SDL_RenderDrawPointF(r, start.x, start.y);
		return;
	}

	const Vector end = *this + start;
	if(SDL_RenderDrawLineF(r, start.x, start.y, end.x, end.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ start.string() +" to "+ end.string() +".\nLast SDL error: "+ SDL_GetError());

	const Vector posHead1 = end + rotate(degToRadian(+135)).withNorm(norm()/5);
	const Vector posHead2 = end + rotate(degToRadian(-135)).withNorm(norm()/5);

	if(SDL_RenderDrawLineF(r, end.x, end.y, posHead1.x, posHead1.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ end.string() +" to "+ posHead1.string() +".\nLast SDL error: "+ SDL_GetError());
	
	if(SDL_RenderDrawLineF(r, end.x, end.y, posHead2.x, posHead2.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ end.string() +" to "+ posHead2.string() +".\nLast SDL error: "+ SDL_GetError());

}


/**
 * Output a human-readable representation of the vector.
 */
[[ nodiscard ]] std::string Vector::string(void) const noexcept {
	return "Vector{"+ std::to_string(x) + ", " + std::to_string(y) + "}";
}

/**
 * Rotate the vector around its tail by `angle` radians.
 */
[[ nodiscard ]] Vector Vector::rotate(float angle) const noexcept {
	return Vector{
		.x = x * std::cos(angle) - y * std::sin(angle),
		.y = x * std::sin(angle) + y * std::cos(angle)
	};
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t The time value (How far between two points we have to go), if above 1 or below -1 is treated as the closest.
 * @throw We assume `std::min()` and `std::max()` won't throw.
 */
[[ nodiscard ]] Vector Vector::lerp(Vector from, Vector to, float t) noexcept {
	return lerpNoRestrict(from, to, std::max(-1.0f, std::min(1.0f, t)));
}

/**
 * Calculates the LERP between `from` and `to` with no restriction for `t`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go.
 */
[[ nodiscard ]] Vector Vector::lerpNoRestrict(Vector from, Vector to, float t) noexcept {
	return from + t*(to - from);
}



/**
 * Translate `p` by `this` vector
 */
SDL_FPoint Vector::translate(SDL_FPoint p) const noexcept {
	return SDL_FPoint{
		.x = x + p.x,
		.y = y + p.y
	};
}

[[ nodiscard ]] Vector& Vector::operator+=(const Vector& v) noexcept {
	x += v.x;
	y += v.y;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator-=(const Vector& v) noexcept {
	x -= v.x;
	y -= v.y;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator*=(float k) noexcept {
	x *= k;
	y *= k;
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator/=(float k) noexcept {
	x /= k;
	y /= k;
	return *this;
}