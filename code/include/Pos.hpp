#pragma once

#include "Imports.hpp"
#include "Warning.hpp"
#include "Vector.hpp"

/** The type for representing a position on 1 dimension */
typedef uint pos_t;

/** The minimum value of `pos_t` */
#define POS_MIN UINT32_MIN

/** The maximum value of `pos_t` */
#define POS_MAX UINT32_MAX

/**
 * A position in a 2D space.
 */
struct Pos {
	pos_t x;
	pos_t y;

	Pos(void);
	Pos(pos_t, pos_t);
	static std::optional<Pos> inBounds(pos_t, pos_t);

	Pos(const Pos&)	= default;
	~Pos(void)		= default;

	
	std::string string(void) const;
	Pos shift(pos_t, pos_t) const;
	void shiftSelf(pos_t, pos_t);
	
	static Pos lerp(const Pos&, const Pos&, float);

	operator Vector() const;

	static const Pos ORIGIN;
	static Pos SCREEN_CENTER;
};

Pos vectorToPos(const Vector& v);