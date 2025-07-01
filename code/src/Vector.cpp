#include "../include/Vector.hpp"

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(SDL_Point from, SDL_Point to) noexcept {
	return Vector{
		.x = static_cast<double>(to.x - from.x),		//implicit int to double throws a warning
		.y = static_cast<double>(to.y - from.y)
	};
}

/**
 * Return the vector from point `from` to the point `to`.
 */
[[ nodiscard ]] Vector Vector::fromPoints(SDL_FPoint from, SDL_FPoint to) noexcept {
	return Vector{
		.x = roundDouble(to.x - from.x),
		.y = roundDouble(to.y - from.y)
	};
}



/**
 * Scales the current vector to have a length of `newNorm` (`newNorm` will always be treated as positive).
 * @throw An `std::logic_error` if the vector has a norm of 0.
 */
Vector Vector::withNorm(double newNorm) const {
	return unit() * abs(newNorm);
}

/**
 * Get corresponding unit vector.
 * @throw An `std::logic_error` if the vector has a norm of 0.
 */
Vector Vector::unit(void) const {
	const double length(norm());
	if(length == 0)
		throw std::logic_error("Division by zero in " + string() +".unit(): the norm of the current vector is 0");
	return *this / length;
}

/**
 * Draw the vector in the renderer, the tail of the vector is located at `startPos`.
 * @throw If fail to draw a line, throw a `runtime_error`.
 */
void Vector::draw(SDL_Renderer* r, const Vector& startPos /*= Vec_ZERO*/) const {
	if(isNull()) {
		SDL_RenderDrawPointF(r, startPos.x, startPos.y);
		return;
	}

	const Vector endPos = *this + startPos;
	if(SDL_RenderDrawLineF(r, startPos.x, startPos.y, endPos.x, endPos.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ startPos.string() +" to "+ endPos.string() +".\nLast SDL error: "+ SDL_GetError());

	const Vector posHead1 = endPos + rotate(degToRadian(+135.0)).withNorm(norm()/5.0);
	const Vector posHead2 = endPos + rotate(degToRadian(-135.0)).withNorm(norm()/5.0);

	if(SDL_RenderDrawLineF(r, endPos.x, endPos.y, posHead1.x, posHead1.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ endPos.string() +" to "+ posHead1.string() +".\nLast SDL error: "+ SDL_GetError());
	
	if(SDL_RenderDrawLineF(r, endPos.x, endPos.y, posHead2.x, posHead2.y) < 0)
		throw std::runtime_error("Failed to draw line from "+ endPos.string() +" to "+ posHead2.string() +".\nLast SDL error: "+ SDL_GetError());
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
[[ nodiscard ]] Vector Vector::rotate(double angle) const noexcept {
	return Vector{
		.x = roundDouble(x * std::cos(angle)) - roundDouble(y * std::sin(angle)),
		.y = roundDouble(x * std::sin(angle)) + roundDouble(y * std::cos(angle))
	};
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t The time value (How far between two points we have to go), if above 1 or below -1 is treated as the closest.
 * @throw We assume `std::min()` and `std::max()` won't throw.
 */
[[ nodiscard ]] Vector Vector::lerp(Vector from, Vector to, double t) noexcept {
	return lerpNoRestrict(from, to, std::max(-1.0, std::min(1.0, t)));
}

/**
 * Calculates the LERP between `from` and `to` with no restriction for `t`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go.
 */
[[ nodiscard ]] Vector Vector::lerpNoRestrict(Vector from, Vector to, double t) noexcept {
	return from + t*(to - from);
}



/**
 * Translate `p` by `this` vector
 */
SDL_FPoint Vector::translate(SDL_FPoint p) const noexcept {
	return SDL_FPoint{
		.x = roundType<double, float>(x + p.x),
		.y = roundType<double, float>(y + p.y)
	};
}

[[ nodiscard ]] Vector& Vector::operator+=(const Vector& v) noexcept {
	x = roundType<double, float>(x + v.x);
	y = roundType<double, float>(y + v.y);
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator-=(const Vector& v) noexcept {
	x = roundType<double, float>(x - v.x);
	y = roundType<double, float>(y - v.y);
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator*=(double k) noexcept {
	x = roundType<double, float>(x * k);
	y = roundType<double, float>(y * k);
	return *this;
}

[[ nodiscard ]] Vector& Vector::operator/=(double k) noexcept {
	x = roundType<double, float>(x / k);
	y = roundType<double, float>(y / k);
	return *this;
}