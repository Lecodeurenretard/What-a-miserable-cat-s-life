#include "Animal.hpp"

#define DOGLIST_SIZE UINT8_MAX

typedef uint8_t ID;


class Dog final : public Animal {
private:
	ID index;	//The index in `dogList`

	void setToRandomSprite(void) noexcept(false);

	void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const;

	/** The size of all dogs in pixels*/
	static constexpr uint8_t size = 100;

	/** The speed of all dogs in pixels per frame */
	static constexpr uint8_t speed = 3;

public:
	explicit Dog(const Pos&);
	Dog(const Dog&);
	~Dog(void);

	std::string string(void) const;

	//Those constructors give too much liberty
	Dog(pos_t, pos_t)				= delete;
	Dog(Pos, uint)					= delete;
	Dog(Pos, uint, uint)			= delete;
	Dog(Pos, uint, uint, uint8_t)	= delete;

	static void generateDogs(uint8_t, ID(*)[] = nullptr, Pos=Pos::ORIGIN);

	//Dogs' speeds and sizes are constant
	void increaseSize(uint)		= delete;
	void increaseSpeed(uint)	= delete;

	uint getSize(void) const	= delete;

	static ID getLowestID(void);

	/** The base to set sprites. */
	inline static const std::string spriteBase = Animal::spriteFolder + "dog";

	/** A list containing all dog constructed. */
	inline static Dog* dogList[DOGLIST_SIZE] = {};
};