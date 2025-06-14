#pragma once
#include "Hitbox.hpp"
#include "Images.hpp"

/** The size of the sprite of **all** `Animal` sprites and child classes. */
#define ANIMAL_SPRITE_SIZE 100

/** Allocates a `TTF_Font` which represents the default font to use when an Animal draws text. */
#define ANIMAL_ALLOCATE_DEFAULT_FONT(txtSize) TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf", txtSize)

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

	Animal(Pos, uint, uint, std::string)	noexcept;
	virtual bool setSprite(uint8_t)			noexcept;
	virtual void setToRandomSprite(void);

	void setDestRand(void)	noexcept;
	void setDestMouse(void)	noexcept;
	bool isAtDest(void)			const noexcept;
	Vector getSpeedVector(void) const noexcept;

	virtual void drawSprite(SDL_Renderer*) const;
	virtual void drawInfos(SDL_Renderer*, bool=false) const noexcept;
	virtual void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const = 0;

	static fs::path getRandomPathFromMask(mask_t, std::string=Animal::spriteFolder);

public:
	explicit Animal(Pos)				noexcept;
	Animal(pos_t, pos_t)				noexcept;
	Animal(Pos, uint)					noexcept;
	Animal(Pos, uint, uint)				noexcept;
	Animal(Pos, uint, uint, uint8_t);

	Animal(const Animal&) = default;
	virtual ~Animal(void) = default;

	void increaseSize(uint)		noexcept;
	void increaseSpeed(uint)	noexcept;

	void incrementHealth(void)							noexcept;
	void randomizeHealth(uint8_t=1, uint8_t=UINT8_MAX)	noexcept;

	void move(bool=false);
	void moveToDest(void);

	Hitbox getHitbox(void)	const noexcept;
	bool isDead(void)		const noexcept;

	virtual void draw(SDL_Renderer*, TTF_Font* =nullptr, bool=false) const;
	virtual std::string string(void) const noexcept;

	/** The folder where the sprites are located. */
	inline static const std::string spriteFolder = "sprites/";

	/** The base in order to make the full sprite path. */
	inline static const std::string spriteBase = Animal::spriteFolder + "other";

	/** The base in order to make the full sprite path of the dead animal. */
	inline static const std::string spriteBaseDead = Animal::spriteFolder + "deadOther";
};