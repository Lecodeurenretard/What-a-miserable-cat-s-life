#include "../include/Pos.hpp"

/**
 * `Pos(0, 0)`  
 * In SDL it represent the top left corner, if you want the actual center of the screen, use `Pos::SCREEN_CENTER`.
 */
const Pos Pos::ORIGIN = Pos(0, 0);

/** The center of the screen. */
Pos Pos::SCREEN_CENTER = Pos(WIN_WIDTH/2, WIN_HEIGHT/2);

[[ nodiscard ]] Pos::Pos(pos_t _x, pos_t _y) noexcept
	: x(_x), y(_y)
{}

[[ nodiscard ]] Pos::Pos(const Vector& v) noexcept
	: Pos(v.x, v.y)
{}

[[ nodiscard ]] Pos::Pos(const SDL_Point& p) noexcept
	: Pos(p.x, p.y)
{}

[[ nodiscard ]] Pos::Pos(const SDL_FPoint& p) noexcept
	: Pos(p.x, p.y)
{}

/**
 * If this object is in bounds (in the window).
 */
[[ nodiscard ]] bool Pos::isInBounds(void) {
	return Pos::isInBounds(x, y);
}

/**
 * If those coordonates are in bounds (in the window).
 */
[[ nodiscard ]] bool Pos::isInBounds(pos_t x, pos_t y) {
	return x <= WIN_WIDTH && y <= WIN_HEIGHT;		//Since `pos_t` is a `uint`, it can't be too high or too to the left since it can't get below 0.
}

/**
 * Returns `{}` if `_x` or `_y` is not in the screen else returns a new `Pos` object.
 */
[[ nodiscard ]] std::optional<Pos> Pos::createInBounds(pos_t x , pos_t y) {
	if(!isInBounds(x, y))
		return {};
	return Pos(x, y);
}

/*
 * Returns a human-readable string of `this` object.
 */
[[ nodiscard ]] std::string Pos::string(void) const {
	return "("+ std::to_string(x) + ", " + std::to_string(y) + ")";
}

/**
 * It shift this object of `x` pixels down and `y` pixels right. You can see it as `lerp(*this, Pos(x, y), 1)`.
 */
[[ nodiscard ]] Pos Pos::shift(pos_t _x, pos_t _y) const {
	//no bound checking, adding suffeciently large floats yields infinity (for result above ~std::numeric_limits<float>::max()*2.5 by testing)
	//that's way beyond the screen bottom right corner, so we can say infinity and this large number are, in this case equivalent
	return Pos(x + _x, y + _y);
}

/**
 * `Pos::shift()` but the results are applied to `this`.
 */
void Pos::shiftSelf(pos_t _x, pos_t _y) {
	operator=(shift(_x, _y));
}

/**
 * Draws a cross at `this` position in `r`.
 */
void Pos::draw(SDL_Renderer* r) const {
	constexpr uint lineLen = std::max(WIN_MIN/100, (uint)1);
	SDL_RenderDrawLine(r, x, y - lineLen/2, x, y + lineLen/2);	//vertical
	SDL_RenderDrawLine(r, x - lineLen/2, y, x + lineLen/2, y);	//horizontal
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go, if strictly above 1, it gets treated as 1.
 */
[[ nodiscard ]] Pos Pos::lerp(const Pos& from, const Pos& to, float t) {
	return Pos(Vector::lerp(from, to, t));
}


[[ nodiscard ]] Pos::operator Vector() const {
	return Vector{
		.x = x,
		.y = y
	};
}

[[ nodiscard ]] Pos::operator SDL_Point() const {
	return SDL_Point{
		.x = (int)std::round(x),
		.y = (int)std::round(y)
	};
}

[[ nodiscard ]] Pos::operator SDL_FPoint() const {
	return SDL_FPoint{
		.x = x,
		.y = y
	};
}

/**
 * A shorthand for rounding fields of a `Pos` object.
 */
[[ nodiscard ]] Pos round(const Pos& toRound) {
	return Pos(
		std::round(toRound.x),
		std::round(toRound.y)
	);
}