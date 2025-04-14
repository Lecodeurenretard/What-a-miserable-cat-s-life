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
	
	void setToRandomSprite(void)	noexcept(false);
	void trySetLowestID(void)		noexcept(false);

	static Cat* catList[CATLIST_SIZE];

	static void eraseCat(Cat*);
	static std::optional<ID> getLowestID(void);
	static fs::path getRandomPathFromMask(mask_t) noexcept(false);

public:
	explicit Cat(Pos);
	Cat(pos_t, pos_t)				noexcept(false);
	Cat(Pos, uint)					noexcept(false);
	Cat(Pos, uint, uint8_t)			noexcept(false);
	Cat(Pos, uint, uint, uint8_t)	noexcept(false);
	Cat(const Cat&)					noexcept(false);
	
	Cat& operator=(const Cat&)	= default;
	~Cat(void);

	ID getID(void)		const;
	uint getSpeed(void)	const;

	std::string string(void)	const;

	static const std::string spriteBase;
};