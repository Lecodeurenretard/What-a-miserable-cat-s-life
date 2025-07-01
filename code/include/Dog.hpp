#include "Animal.hpp"

#define DOGLIST_SIZE UINT8_MAX

/** Represents a unique ID */
typedef uint8_t ID;

class Dog final : public Animal {
private:
	ID index;	//The index in `dogList`

	void setToRandomSprite(void);

	void drawSpecificities(SDL_Renderer*, TTF_Font* =nullptr) const noexcept;

	bool canBeListed(void) const noexcept;

	/** The size of all dogs in pixels*/
	static constexpr double size = 100;

	/** The speed of all dogs in pixels per second */
	static constexpr double speed = 400;
	
	explicit Dog(const Pos&);

	//make_unique accesses constructors
	template< class _Tp, class... _Args >
	friend std::_MakeUniq<_Tp>::__single_object std::make_unique(_Args&& ...);
public:
	Dog(const Dog&)			= default;
	Dog& operator=(const Dog&)	= default;
	~Dog(void)				= default;

	//Those constructors give too much liberty
	Dog(pos_t, pos_t)					= delete;
	Dog(Pos, double)					= delete;
	Dog(Pos, double, double)			= delete;
	Dog(Pos, double, double, uint8_t)	= delete;

	static void generateDogs(uint8_t, std::vector<ID>* = nullptr, Pos=Pos::ORIGIN);
	static void clearDogList(void) noexcept;

	std::string string(void) const noexcept;

	//Dogs' speeds and sizes are constant
	void increaseSize(uint)		= delete;
	void increaseSpeed(uint)	= delete;
	uint getSize(void) const	= delete;

	static ID getLowestIndex(void) noexcept;

	/** The base to set sprites. */
	inline static const std::string spriteBase = Animal::spriteFolder + "dog";

	/** A list containing all dog constructed. */
	typedef std::array<std::unique_ptr<Dog>, DOGLIST_SIZE> dogList_t;
	inline static dogList_t dogList{};
};