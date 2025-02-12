#include "../include/Pos.hpp"

/**
 * `Pos(0, 0)`  
 * In SDL it represent the top left corner, if you want the actual center of the screen, use `Pos::SCREEN_CENTER`.
 */
const Pos Pos::ORIGIN = Pos(0, 0);

/** The center of the screen. */
Pos Pos::SCREEN_CENTER = Pos(WIN_WIDTH/2, WIN_HEIGHT/2);

[[ nodiscard ]] Pos::Pos(pos_t _x, pos_t _y)
	: x(_x), y(_y)
{}

[[ nodiscard ]] Pos::Pos(const Vector& v)
	: Pos(v.x, v.y)
{}

[[ nodiscard ]] Pos::Pos(const SDL_Point& p)
	: Pos(p.x, p.y)
{}

[[ nodiscard ]] Pos::Pos(const SDL_FPoint& p)
	: Pos(p.x, p.y)
{}

/**
 * If this object is in bounds (in the window).
 */
[[ nodiscard ]] bool Pos::isInBounds(void){
	return Pos::isInBounds(x, y);
}

/**
 * If those coordonates are in bounds (in the window).
 */
[[ nodiscard ]] bool Pos::isInBounds(pos_t x, pos_t y){
	return x <= WIN_WIDTH && y <= WIN_HEIGHT;		//Since `pos_t` is a `uint`, it can't be too high or too to the left since it can't get below 0.
}

/**
 * Returns `{}` if `_x` or `_y` is not in the screen else returns a new `Pos` object.
 */
[[ nodiscard ]] std::optional<Pos> Pos::createInBounds(pos_t x , pos_t y){
	if(!isInBounds(x, y))
		return {};
	return Pos(x, y);
}

[[ nodiscard ]] std::string Pos::string(void) const{
	return "("+ std::to_string(x) + ", " + std::to_string(y) + ")";
}

[[ nodiscard ]] Pos Pos::shift(pos_t _x, pos_t _y) const {
	if((uint64_t)x + _x > POS_MAX || y + _y > POS_MAX)
		wout << "The sum of the parameter `_x` (" << _x << ") and the field `x` " << x <<" is greater than the maximum of `pos_t` (" << POS_MAX << ").";
	if((uint64_t)y + _y > POS_MAX)
		wout << "The sum of the parameter `_y` (" << _y << ") and the field `y` " << y <<" is greater than the maximum of `pos_t` (" << POS_MAX << ").";

	return Pos(x + _x, y + _y);
}

void Pos::shiftSelf(pos_t _x, pos_t _y) {
	if((uint64_t)x + _x > POS_MAX || y + _y > POS_MAX)
		wout << "The sum of the parameter `_x` (" << _x << ") and the field `x` " << x <<" is greater than the maximum of `pos_t` (" << POS_MAX << ").";
	if((uint64_t)y + _y > POS_MAX)
		wout << "The sum of the parameter `_y` (" << _y << ") and the field `y` " << y <<" is greater than the maximum of `pos_t` (" << POS_MAX << ").";

	x += _x;
	y += _y;
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go, if strictly above 1, it gets treated as 1.
 */
[[ nodiscard ]] Pos Pos::lerp(const Pos& from, const Pos& to, float t){	//I made it an `uint8_t` because floats use 4 times more memory
	return Pos(Vector::lerp(from, to, t));
}


[[ nodiscard ]] Pos::operator Vector() const{
	return Vector(x, y);
}

[[ nodiscard ]] Pos::operator SDL_Point() const{
	SDL_Point res;
	res.x = x;
	res.y = y;
	return res;
}

[[ nodiscard ]] Pos::operator SDL_FPoint() const{
	SDL_FPoint res;
	res.x = x;
	res.y = y;
	return res;
}

/**
 * A shorthand to `Pos(std::round(toRound.x), std::round(toRound.y))`.
 */
[[ nodiscard ]] Pos round(const Pos& toRound){
	return Pos(std::round(toRound.x), std::round(toRound.y));
}