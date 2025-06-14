#include "Dog.hpp"
#define CATLIST_SIZE UINT8_MAX

/** The (future) prey and subject to evolution (not implemented yet) */
class Cat final : public Animal {
private:
	ID id				= CATLIST_SIZE;

	bool collisionLastFrame = false;
	bool setSprite(uint8_t) noexcept;
	
	void setToRandomSprite(void)	noexcept;
	void setLowestID(void)			noexcept;
	
	bool canBeListed(void)	const noexcept;
	bool isHitByDog(void)	const noexcept;

	static ID getLowestID(void) noexcept;

	void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const;

	//Cats should be constructed by generateCats
	Cat(void)						noexcept;
	explicit Cat(Pos)				noexcept;
	Cat(pos_t, pos_t)				noexcept;
	Cat(Pos, uint)					noexcept;
	Cat(Pos, uint, uint8_t);
	Cat(Pos, uint, uint, uint8_t);

	//make_unique accesses constructors
	template< class _Tp, class... _Args >
	friend std::_MakeUniq<_Tp>::__single_object std::make_unique(_Args&& ...);

public:
	Cat(const Cat&)	= default;
	~Cat(void)		= default;

	static Cat createUnlisted(Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);
	Cat copy(void)	const noexcept;

	ID getID(void)	const noexcept;

	void draw(SDL_Renderer*, TTF_Font* =nullptr, bool=false) const;
	std::string string(void) const noexcept;

	void handleCollisions(void) noexcept;

	static void generateCats(uint8_t, std::vector<ID>* = nullptr, Pos=Pos::ORIGIN, uint=0, uint=0, uint8_t=0);
	static void clearCatList(void) noexcept;

	/** The base to make spritePath. */
	inline static const std::string spriteBase = Animal::spriteFolder + "cat";

	/** The base to make a dead cat img sprite path */
	inline static const std::string spriteDeadBase = Animal::spriteFolder + "deadCat";

	/** The list contaning all Cat instances */
	inline static std::array<std::unique_ptr<Cat>, CATLIST_SIZE> catList{};	//by default, all elements are initalized to nullptr
};
