#include "Pos.hpp"

#define HITBOX_ALPHA 128
#define HITBOX_COLOR_INACTIVE SDL_Color{.r = 200, .g = 0, .b = 0, .a = HITBOX_ALPHA}
#define HITBOX_COLOR_ACTIVE   SDL_Color{.r = 255, .g = 0, .b = 0, .a = HITBOX_ALPHA}


SDL_Rect  rectFromVector(Vector, Pos)	noexcept;
SDL_FRect frectFromVector(Vector, Pos)	noexcept;

Vector rectToVector(SDL_Rect)	noexcept;
Vector rectToVector(SDL_FRect)	noexcept;
#define frectToVector(rect) rectToVector(rect)

#define vectorToRect(vec, pos) rectFromVector(vec, pos)
#define vectorToFRect(vec, pos) frectFromVector(vec, pos)

SDL_Rect getSquare(int, Pos=Pos::ORIGIN) noexcept;
SDL_FRect getSquare(double, Pos=Pos::ORIGIN) noexcept;


struct Hitbox {
	SDL_FRect zone;

	Hitbox(void)			noexcept;
	Hitbox(SDL_Rect)		noexcept;
	Hitbox(SDL_FRect)		noexcept;
	Hitbox(const Hitbox&) = default;

	operator SDL_Rect(void)  const noexcept;
	operator SDL_FRect(void) const noexcept;

	bool isInside(SDL_FPoint, bool=false)	const noexcept;
	bool isOverlapping(Hitbox)				const noexcept;

	Pos calcCenter(void)		const noexcept;
	Hitbox translate(Vector)	const noexcept;

	/**
	 * Expand the hitbox `times` times from the upper left corner.
	 */
	template<typename Num>
		requires std::is_convertible_v<Num, double>
	[[ nodiscard ]] Hitbox scale(Num times) const noexcept {
		return Hitbox(rectFromVector(
			rectToVector(zone) * (double)times,
			Pos(SDL_FPoint{
				.x = zone.x,
				.y = zone.y
			})
		));
	}

	/**
	 * Expand and move the hitbox such as it is as if the world was `times` times larger.
	 * @example if `times = 2` the sides of the hitbox will double in size but the hitbox will be at coordonates `(2x, 2y)`.
	 */
	template<typename Num>
		requires std::is_convertible_v<Num, double>
	[[ nodiscard ]] Hitbox resize_world(Num times) const noexcept {
		return Hitbox(rectFromVector(
			rectToVector(zone) * (double)times,
			Pos(SDL_FPoint{
				.x = times*zone.x,
				.y = times*zone.y
			})
		));
	}

	void draw(SDL_Renderer*, SDL_Color=HITBOX_COLOR_INACTIVE, Vector=Vec_ZERO) const;

	std::string string(void) const noexcept;
};