#include "../include/Pos.hpp"

/**
 * Just set `x` and `y` at `0`.
 */
[[nodiscard]] Pos::Pos(void)
	:x(0), y(0)
{}

/**
 * Base constructor, just set `x` at `_x` and `y` at `_y`.
 */
[[nodiscard]] Pos::Pos(pos_t _x, pos_t _y)
	: x(_x), y(_y)
{}

/**
 * Returns a printable string of the object.
 */
[[nodiscard]] std::string Pos::string(void) const{
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
