#pragma once
#include "Hitbox.hpp"
#include "Images.hpp"

/** The size of the sprite of **all** `Animal` sprites and child classes. */
#define ANIMAL_SPRITE_SIZE 100

/**
 * Parent class of `Cat` and `Dog`.
 */
class Animal {
protected:
	Pos pos					= Pos::SCREEN_CENTER;
	Pos dest				= Pos::SCREEN_CENTER;
	uint size				= 0;
	uint speed				= 0;
	std::string spritePath;
	Hitbox hitbox;

	Animal(Pos, uint, uint, std::string);
	virtual bool setSprite(uint8_t);
	virtual void setToRandomSprite(void) noexcept(false);

	void setDestRand(void);
	void setDestMouse(void);

	static fs::path getRandomPathFromMask(mask_t);

	Vector getSpeedVector(void) const;

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

	bool isAtDest(void) const;

	void move(bool=false);
	void moveToDest(void);

	uint getSize(void) const;
	Hitbox getHitbox(void) const;

	
	virtual void draw(SDL_Renderer*, bool, bool=false) const noexcept(false);
	virtual std::string string(void) const;

	/** The folder where the sprites are located. */
	inline static const std::string spriteFolder = "sprites/";

	/** The base in order to make the full sprite path. */
	inline static const std::string spriteBase = Animal::spriteFolder + "other";
};