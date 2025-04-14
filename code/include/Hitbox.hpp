#include "Pos.hpp"

#define HITBOX_ALPHA 128
#define HITBOX_COLOR_INACTIVE SDL_Color{.r = 200, .g = 0, .b = 0, .a = HITBOX_ALPHA}
#define HITBOX_COLOR_ACTIVE   SDL_Color{.r = 255, .g = 0, .b = 0, .a = HITBOX_ALPHA}


struct Hitbox {
	SDL_FRect zone;

	Hitbox(SDL_Rect);
	Hitbox(SDL_FRect);
	Hitbox(const Hitbox&) = default;

	operator SDL_Rect(void)  const;
	operator SDL_FRect(void) const;

	bool isInside(SDL_FPoint, bool=false)	const;
	bool isOverlapping(Hitbox)				const;

	Pos calcPos(void) const;

	Hitbox expand(uint) const;

	void draw(SDL_Renderer*, SDL_Color=HITBOX_COLOR_INACTIVE, Vector=Vector::ZERO) const;
};

SDL_Rect  rectFromVector(Vector, Pos);
SDL_FRect frectFromVector(Vector, Pos);

Vector rectToVector(SDL_Rect);
Vector rectToVector(SDL_FRect);
#define frectToVector(rect) rectToVector(rect)