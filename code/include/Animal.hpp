#pragma once
#include "Hitbox.hpp"
#include "Images.hpp"

/** The size of the sprite of **all** `Animal` sprites and child classes. */
#define ANIMAL_SPRITE_SIZE 100

/** Allocates a `TTF_Font` which represents the default font to use when an Animal draws text. */
#define ANIMAL_DEFAULT_FONT(size) TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf", size)

/**
 * Parent class of `Cat` and `Dog`.
 */
class Animal {
protected:
	Pos pos					= Pos::SCREEN_CENTER;
	Pos dest				= Pos::SCREEN_CENTER;
	uint size				= 0;
	uint speed				= 0;
	uint8_t health			= 1;

	std::string spritePath;
	std::string spritePathDead;
	Hitbox hitbox;

	Animal(Pos, uint, uint, std::string);
	virtual bool setSprite(uint8_t);
	virtual void setToRandomSprite(void) noexcept(false);

	void setDestRand(void);
	void setDestMouse(void);
	Vector getSpeedVector(void) const;
	bool isAtDest(void) const;

	virtual void drawSprite(SDL_Renderer*) const;
	virtual void drawInfos(SDL_Renderer*, bool=false) const;
	virtual void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const = 0;

	static fs::path getRandomPathFromMask(mask_t);

public:
	explicit Animal(Pos)				noexcept(false);
	Animal(pos_t, pos_t)				noexcept(false);
	Animal(Pos, uint)					noexcept(false);
	Animal(Pos, uint, uint)				noexcept(false);
	Animal(Pos, uint, uint, uint8_t)	noexcept(false);

	Animal(const Animal&) = default;
	virtual ~Animal(void) = default;

	void increaseSize(uint);
	void increaseSpeed(uint);
	void incrementHealth(void);

	void move(bool=false);
	void moveToDest(void);

	Hitbox getHitbox(void)	const;
	bool isDead(void)		const;

	virtual void draw(SDL_Renderer*, TTF_Font* =nullptr, bool=false) const noexcept(false);
	virtual std::string string(void) const;

	/** The folder where the sprites are located. */
	inline static const std::string spriteFolder = "sprites/";

	/** The base in order to make the full sprite path. */
	inline static const std::string spriteBase = Animal::spriteFolder + "other";

	/** The base in order to make the full sprite path of the dead animal. */
	inline static const std::string spriteBaseDead = Animal::spriteFolder + "deadOther";
};