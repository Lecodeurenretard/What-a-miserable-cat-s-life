#pragma once
#include "Pos.hpp"

/** The size of the sprite of **all** `Animal` sprites and child classes. */
#define ANIMAL_SPRITE_SIZE 100

/**
 * Parent class of `Cat` and `Dog`.
 */
class Animal {
protected:
	Pos pos;
	Pos dest;
	uint size;
	uint speed;
	std::string spritePath;

	Animal(Pos, uint, uint, std::string);
	virtual bool setSprite(uint8_t);
	virtual void setToRandomSprite(void) noexcept(false);

	static fs::path getRandomPathFromMask(mask_t);

	Vector getSpeedVector(void) const;

public:
	Animal(void) = delete;
	explicit Animal(Pos);
	Animal(pos_t, pos_t);
	Animal(Pos, uint);
	Animal(Pos, uint, uint);
	Animal(Pos, uint, uint, uint8_t);

	Animal(const Animal&) = default;
	virtual ~Animal(void) = default;

	void increaseSize(uint);
	void increaseSpeed(uint);

	bool isAtDest(void) const;
	void setRandDest(void);
	void move(void);
	void moveToDest(void);

	uint getSize(void) const;

	
	virtual void draw(SDL_Renderer*, bool=false) const noexcept(false);
	virtual std::string string(void) const;

	static const std::string spriteFolder;
	static const std::string spriteBase;
};