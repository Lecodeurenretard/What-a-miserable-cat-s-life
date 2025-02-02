#include "../include/Vector.hpp"

const Vector Vector::ZERO = Vector(0, 0);

[[ nodiscard ]] Vector::Vector(uint _x, uint _y){
	x = _x;
	y = _y;
}

[[ nodiscard ]] Vector::Vector(const SDL_Point& from, const SDL_Point& to){
	x = to.x - from.x;
	y = to.y - from.y;
}

[[ nodiscard ]] std::string Vector::string(void) const{
	return "("+ std::to_string(x) + ", " + std::to_string(y) + ")";
}

/**
 * Calculates the LERP between `from` and `to`.
 * @param from The point from where we begin.
 * @param to The point to end.
 * @param t How far between two points we have to go, if strictly above 1, it gets treated as 1.
 */
Vector Vector::lerp(const Vector& from, const Vector& to, float t){
	if(t > 100)
		t = 100;
	return (1 - t) * from + t * to;
}



Vector Vector::operator+(const Vector& v) const{
	return Vector(x + v.x, y + v.y);
}

Vector Vector::operator-(const Vector& v) const{
	return Vector(x - v.x, y - v.y);
}

Vector Vector::operator*(uint k) const{
	return Vector(x * k, y * k);
}

Vector Vector::operator/(uint k) const{
	return Vector(x / k, y / k);
}

Vector operator*(uint k, const Vector& v){
	return v * k;
}

Vector operator/(uint k, const Vector& v){
	return v / k;
}
