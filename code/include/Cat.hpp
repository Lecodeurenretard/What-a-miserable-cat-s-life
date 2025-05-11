#include "Dog.hpp"
#define CATLIST_SIZE UINT8_MAX

/** Represents a unique ID */
typedef uint8_t ID;

/** The (future) prey and subject to evolution (not implemented yet) */
class Cat final : public Animal {
private:
	ID id = CATLIST_SIZE;
	bool collisionLastFrame = false;

	bool setSprite(uint8_t) noexcept;
	
	void setToRandomSprite(void)	noexcept;
	bool trySetLowestID(void)		noexcept;
	bool isHitByDog(void)			const noexcept;

	static ID getLowestID(void) noexcept;

	void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const;

public:
	Cat(void)						noexcept;
	explicit Cat(Pos)				noexcept;
	Cat(pos_t, pos_t)				noexcept;
	Cat(Pos, uint)					noexcept;
	Cat(Pos, uint, uint8_t);
	Cat(Pos, uint, uint, uint8_t);
	Cat(const Cat&)					= default;
	~Cat(void) noexcept;

	static Cat createUnlisted(Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);
	Cat copy(void)	const noexcept;

	ID getID(void)	const noexcept;

	void draw(SDL_Renderer*, TTF_Font* =nullptr, bool=false) const;
	std::string string(void) const noexcept;

	void handleCollisions(void) noexcept;

	static void generateCats(uint8_t, ID(*)[] = nullptr, Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);
	static void freeCatList(void) noexcept;

	/** The base to make spritePath. */
	inline static const std::string spriteBase = Animal::spriteFolder + "cat";

	/** The base to make a dead cat img sprite path */
	inline static const std::string spriteDeadBase = Animal::spriteFolder + "deadCat";

	/** The list contaning all Cat instances */
	inline static std::array<Cat*, CATLIST_SIZE> catList{};	//by default, all elements are initalized to nullptr
};
