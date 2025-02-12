#pragma once

#include "Imports.hpp"
#include "Warning.hpp"
#include "Vector.hpp"

/** The type for representing a position on 1 dimension */
struct pos_t {
private:
	float val;
public:
	inline pos_t(float x): val(x) {}
	inline operator float(){ return val; }
};

/** The minimum value of `pos_t` */
#define POS_MIN FLT_MIN

/** The maximum value of `pos_t` */
#define POS_MAX FLT_MAX

/**
 * A position in a 2D space.
 */
struct Pos {
	pos_t x;
	pos_t y;

	Pos(pos_t, pos_t);
	Pos(const Vector&);
	Pos(const SDL_Point&);
	Pos(const SDL_FPoint&);

	bool isInBounds(void);
	static bool isInBounds(pos_t, pos_t);
	static std::optional<Pos> createInBounds(pos_t, pos_t);

	Pos(const Pos&)	= default;
	~Pos(void)		= default;

	
	std::string string(void) const;
	Pos shift(pos_t, pos_t) const;
	void shiftSelf(pos_t, pos_t);
	
	static Pos lerp(const Pos&, const Pos&, float);

	bool operator==(const Pos&) const = default;
	bool operator!=(const Pos&) const = default;
	operator Vector() const;
	operator SDL_Point() const;
	operator SDL_FPoint() const;

	static const Pos ORIGIN;
	static Pos SCREEN_CENTER;
};
Pos round(const Pos&);