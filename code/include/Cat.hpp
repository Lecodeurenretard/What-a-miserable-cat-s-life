#include "Animal.hpp"
#define CATLIST_SIZE 256

/** Represents a unique ID */
typedef uint8_t ID;

/** The (future) prey and subject to evolution (not implemented yet) */
class Cat final : public Animal {
private:
	bool alive;
	ID id;

	bool setSprite(uint8_t);
	
	void pushThis(void);
	void setToRandomSprite(void) noexcept(false);
	void trySetLowestID(void) noexcept(false);

	static Cat* catList[CATLIST_SIZE];

	static void pushCat(Cat*);
	static void eraseCat(Cat*);
	static std::optional<ID> getLowestID(void);
	static fs::path getRandomPathFromMask(bool (*mask)(const fs::path&)) noexcept(false);

public:
	explicit Cat(Pos);
	Cat(pos_t, pos_t);
	Cat(Pos, uint);
	Cat(Pos, uint, uint8_t);
	Cat(Pos, uint, uint, uint8_t);
	
	Cat(const Cat&);
	Cat& operator=(const Cat&)	= default;
	~Cat(void)					= default;

	ID getID(void) 		const;
	uint getSpeed(void) const;

	std::string string(void) 	const;

	static std::string getSpriteBase(void);
};