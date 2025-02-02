#pragma once
#include "Pos.hpp"

/**
 * Parent class of `Cat` and `Dog`
 */
class Animal {
protected:
	Pos pos;
	Pos dest;
	uint size;
	std::string spritePath;

	Animal(Pos, uint, std::string);
	virtual bool setSprite(uint8_t);
	virtual void setToRandomSprite(void) noexcept(false);

	static fs::path getRandomPathFromMask(bool (*mask)(const fs::path&));

public:
	Animal(void) = delete;
	explicit Animal(Pos);
	Animal(pos_t, pos_t);
	Animal(Pos, uint);
	Animal(Pos, uint, uint8_t);

	Animal(const Animal&) = default;
	virtual ~Animal(void) = default;

	void increaseSize(uint);
	void setRandDest(void);
	void move(void);

	uint getSize(void) const;

	
	virtual void display(SDL_Renderer*) const noexcept(false);
	virtual std::string string(void) 	const;

	static std::string getSpriteFolder(void);
	static std::string getSpriteBase(void);
};