#include "../include/Pos.hpp"

/**
 * `Pos(0, 0)`  
 *  If you want the actual center of the screen, use `Pos::SCREEN_CENTER` (defined at tthe start of `main.cpp`).
 */
const Pos Pos::ORIGIN = Pos(0, 0);

/** The center of the screen. */
Pos Pos::SCREEN_CENTER = Pos(WIN_WIDTH/2, WIN_HEIGHT/2);

/**
 * Base constructor, just set `x` at `_x` and `y` at `_y`.
 */
[[ nodiscard ]] Pos::Pos(pos_t _x, pos_t _y)
	: x(_x), y(_y)
{}

/**
 * Returns `{}` if `_x` or `_y` is not in the screen else returns a new `Pos` object.
 */
[[ nodiscard ]] std::optional<Pos> Pos::inBounds(pos_t x , pos_t y){
	if(x > WIN_WIDTH || y > WIN_HEIGHT)
		return {};
	return Pos(x, y);
}

[[ nodiscard ]] std::string Pos::string(void) const{
	return "("+ std::to_string(x) + ", " + std::to_string(y) + ")";
}

[[ nodiscard ]] Pos Pos::shift(pos_t _x, pos_t _y) const {
	if((uint64_t)x + _x > POS_MAX || y + _y > POS_MAX)
		Warning("The sum of the parameter `_x` ("+ std::to_string(_x) +") and the field `x` "+ std::to_string(x) +" is greater than the maximum of `pos_t` (" + std::to_string(POS_MAX) + ").").print();
	if((uint64_t)y + _y > POS_MAX)
		Warning("The sum of the parameter `_y` ("+ std::to_string(_y) +") and the field `y` "+ std::to_string(y) +" is greater than the maximum of `pos_t` (" + std::to_string(POS_MAX) + ").").print();

	return Pos(x + _x, y + _y);
}

void Pos::shiftSelf(pos_t _x, pos_t _y) {
	if((uint64_t)x + _x > POS_MAX || y + _y > POS_MAX)
		Warning("The sum of the parameter `_x` ("+ std::to_string(_x) +") and the field `x` "+ std::to_string(x) +" is greater than the maximum of `pos_t` (" + std::to_string(POS_MAX) + ").").print();
	if((uint64_t)y + _y > POS_MAX)
		Warning("The sum of the parameter `_y` ("+ std::to_string(_y) +") and the field `y` "+ std::to_string(y) +" is greater than the maximum of `pos_t` (" + std::to_string(POS_MAX) + ").").print();

	x += _x;
	y += _y;
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go, if strictly above 1, it gets treated as 1.
 */
Pos Pos::lerp(const Pos& from, const Pos& to, float t){	//I made it an `uint8_t` because floats use 4 times more memory
	return vectorToPos(Vector::lerp(from, to, t));
}


Pos::operator Vector() const{
	return Vector(x, y);
}



Pos vectorToPos(const Vector& v){
	return Pos(v.x, v.y);
}