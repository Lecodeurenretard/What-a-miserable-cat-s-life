#include "Dog.hpp"
#define CATLIST_SIZE UINT8_MAX

/** Represents a unique ID */
typedef uint8_t ID;

/** The (future) prey and subject to evolution (not implemented yet) */
class Cat final : public Animal {
	friend class AlogrithmHandler;	//defined in `AlgorithmHandler.hpp`

private:
	ID id = CATLIST_SIZE;
	bool collisionLastFrame = false;

	bool setSprite(uint8_t);
	
	void setToRandomSprite(void)	noexcept(false);
	bool trySetLowestID(void)		noexcept(false);
	bool isHitByDog(void) const;

	static ID getLowestID(void);
	static fs::path getRandomPathFromMask(mask_t) noexcept(false);

	void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const;

public:
	Cat(void)						noexcept(false);
	explicit Cat(Pos)				noexcept(false);
	Cat(pos_t, pos_t)				noexcept(false);
	Cat(Pos, uint)					noexcept(false);
	Cat(Pos, uint, uint8_t)			noexcept(false);
	Cat(Pos, uint, uint, uint8_t)	noexcept(false);
	Cat(const Cat&)					= default;
	~Cat(void);

	static Cat createUnlisted(Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);
	Cat* copy()		const;

	ID getID(void)	const;

	void draw(SDL_Renderer*, TTF_Font* =nullptr, bool=false) const noexcept(false);
	std::string string(void) const;

	void handleCollisions(void);

	static void generateCats(uint8_t, ID(*)[] = nullptr, Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);

	/** The base to make spritePath. */
	inline static const std::string spriteBase = Animal::spriteFolder + "cat";

	/** The base to make a dead cat img sprite path */
	inline static const std::string spriteDeadBase = Animal::spriteFolder + "deadCat";

	/** The list contaning all Cat instances */
	inline static Cat* catList[CATLIST_SIZE];	//by default, all elements are initalized to 0
};
