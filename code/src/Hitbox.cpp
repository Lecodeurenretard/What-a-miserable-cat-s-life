#include "../include/Hitbox.hpp"

[[ nodiscard ]] Hitbox::Hitbox(void) noexcept {
	zone.x = 0;
	zone.y = 0;
	zone.w = 0;
	zone.h = 0;
}

[[ nodiscard ]] Hitbox::Hitbox(SDL_Rect hit) noexcept {
	zone.x = hit.x;
	zone.y = hit.y;
	zone.w = hit.w;
	zone.h = hit.h;
}

[[ nodiscard ]] Hitbox::Hitbox(SDL_FRect hit) noexcept
	: zone(hit)
{}

[[ nodiscard ]] Hitbox::operator SDL_Rect(void) const noexcept {
	return SDL_Rect{
		.x = (int)std::round(zone.x),
		.y = (int)std::round(zone.y),
		.w = (int)std::round(zone.w),
		.h = (int)std::round(zone.h),
	};
}

[[ nodiscard ]] Hitbox::operator SDL_FRect(void) const noexcept {
	return zone;
}

/**
 * Check if the point `p` is inside the hitbox.
 */
[[ nodiscard ]] bool Hitbox::isInside(SDL_FPoint p, bool accountCorners /* = false */) const noexcept {
	return ((zone.x < p.x && p.x < zone.x + zone.w) || (accountCorners && (zone.x == p.x || zone.x + zone.w == p.x)))
		&& ((zone.y < p.y && p.y < zone.y + zone.h) || (accountCorners && (zone.y == p.y || zone.y + zone.h == p.y)));
}

/**
 * Check if this hitbox istersects with `other`.
 */
[[ nodiscard ]] bool Hitbox::isOverlapping(Hitbox other) const noexcept {
	return 
	   zone.x			< other.zone.x	+ other.zone.w
	&& zone.y			< other.zone.y	+ other.zone.h
	&& other.zone.x		< zone.x		+ zone.w
	&& other.zone.y		< zone.y		+ zone.h;
}

/**
 * Calculate the position of the center of the hitbox.
 */
[[ nodiscard ]] Pos Hitbox::calcCenter(void) const noexcept {
	return Pos(zone.x + zone.w/2, zone.y + zone.h/2);
}

/**
 * Return the current hitbox translated by `vec`.
 */
[[ nodiscard ]] Hitbox Hitbox::translate(Vector vec) const noexcept {
	return Hitbox(SDL_FRect{
		.x = zone.x + (float)vec.x,
		.y = zone.y + (float)vec.y,
		.w = zone.w,
		.h = zone.h,
	});
}

/**
 * Draw the hitbox in the `r` renderer with `col` as color (alpha value ignored and always set to `HITBOX_ALPHA`).
 */
void Hitbox::draw(SDL_Renderer* r, SDL_Color col/*= HITBOX_COLOR_INACTIVE*/, Vector offset/*= Vec_ZERO*/) const {
	SDL_Color old_color;
	SDL_GetRenderDrawColor(r, old_color);

	SDL_SetRenderDrawColor(r, col.r, col.g, col.b, HITBOX_ALPHA);

	const SDL_FRect toDraw = {
		.x = zone.x + (float)offset.x,
		.y = zone.y + (float)offset.y,
		.w = zone.w,
		.h = zone.h
	};
	SDL_RenderFillRectF(r, &toDraw);

	SDL_SetRenderDrawColor(r, old_color);
}

/**
 * Return a human-readable representation of this instance.
 */
[[ nodiscard ]] std::string Hitbox::string(void) const noexcept {
	return "Hitbox{ .x="+ std::to_string(zone.x)
		+ "; .y="+ std::to_string(zone.y)
		+ "; .w="+ std::to_string(zone.w)
		+ "; .h="+ std::to_string(zone.h)
		+" }";
}


/**
 * Create an `SDL_Rect` from a vector.
 * @param diag represents the diagonal from the upper left corner to the bottom right corner.
 * @param position The position of the upper left corner.
 */
[[ nodiscard ]] SDL_Rect rectFromVector(Vector diag, Pos position) noexcept {
	return SDL_Rect{
		.x = (int)std::round(position.x),
		.y = (int)std::round(position.y),

		.w = (int)std::round(diag.x),
		.h = (int)std::round(diag.y)
	};
}

/**
 * Create an `SDL_FRect` from a vector.
 * @param diag represents the diagonal from the upper left corner to the bottom right corner.
 * @param position The position of the upper left corner.
 */
[[ nodiscard ]] SDL_FRect frectFromVector(Vector diag, Pos position) noexcept {
	return SDL_FRect{
		.x = (float)position.x,
		.y = (float)position.y,

		.w = (float)diag.x,
		.h = (float)diag.y
	};
}

/**
 * Return the vector from the upper left corner to the bottom right corner of `rect`.
 */
[[ nodiscard ]] Vector rectToVector(SDL_Rect rect) noexcept {
	return Vector{
		.x = (double)rect.w,
		.y = (double)rect.h
	};
}

/**
 * Return the vector from the upper left corner to the bottom right corner of `rect`.
 */
[[ nodiscard ]] Vector rectToVector(SDL_FRect rect) noexcept {
	return Vector{
		.x = rect.w,
		.y = rect.h
	};
}

/**
 * Return a square with its upper left corner at `pos` and of side length of `size`.
 */
[[ nodiscard ]] SDL_Rect getSquare(int size, Pos pos/*=Pos::ORIGIN*/) noexcept {
	return SDL_Rect{
		.x = (int)std::round(pos.x),
		.y = (int)std::round(pos.y),
		.w = (int)std::round(size),
		.h = (int)std::round(size),
	};
}

/**
 * Return a square with its upper left corner at `pos` and of side length of `size`.
 */
[[ nodiscard ]] SDL_FRect getSquare(double size, Pos pos/*=Pos::ORIGIN*/) noexcept {
	return SDL_FRect{
		.x = (float)pos.x,
		.y = (float)pos.y,
		.w = (float)size,
		.h = (float)size,
	};
}