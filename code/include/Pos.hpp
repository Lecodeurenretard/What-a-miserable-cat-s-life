#pragma once

#include "Vector.hpp"

/** The minimum value of `pos_t` */
#define POS_MIN FLT_MIN

/** The maximum value of `pos_t` */
#define POS_MAX FLT_MAX

/** 
 * The type for representing a position on 1 dimension.
 */
struct pos_t {
	double val;
	inline pos_t(double x) noexcept {val = std::abs(x);}

	inline operator double() const noexcept { return val; }

	inline pos_t operator+=(const pos_t& p) noexcept { val += p.val; return *this;}
	inline pos_t operator-=(const pos_t& p) noexcept { val -= p.val; return *this;}
	inline pos_t operator*=(const pos_t& p) noexcept { val *= p.val; return *this;}
	inline pos_t operator/=(const pos_t& p) noexcept { val /= p.val; return *this;}
};


/**
 * A position in a 2D space.
 */
struct Pos {
	pos_t x;
	pos_t y;

	Pos(pos_t, pos_t)	noexcept;
	Pos(const Vector&)	noexcept;
	Pos(const SDL_Point&)	noexcept;
	Pos(const SDL_FPoint&)	noexcept;

	Pos& operator=(const Pos&)	= default;
	Pos(const Pos&)			= default;
	~Pos(void)				= default;

	bool isInBounds(void) const noexcept;
	static bool isInBounds(pos_t, pos_t) noexcept;
	static std::optional<Pos> createInBounds(pos_t, pos_t) noexcept;

	std::string string(void)	const noexcept;
	Pos shift(pos_t, pos_t)		const noexcept;
	void shiftSelf(pos_t, pos_t) noexcept;

	void draw(SDL_Renderer*)	const;
	
	static Pos lerp(const Pos&, const Pos&, double) noexcept;

	bool operator==(const Pos&) const = default;
	bool operator!=(const Pos&) const = default;
	operator Vector()		const noexcept;
	operator SDL_Point()	const noexcept;
	operator SDL_FPoint()	const noexcept;

	static const Pos ORIGIN;
	static Pos SCREEN_CENTER;
};
Pos round(const Pos&) noexcept;