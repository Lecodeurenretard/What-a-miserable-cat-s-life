#include "Animal.hpp"

class Dog final : public Animal{
private:
	void setToRandomSprite(void) noexcept(false);

public:
	explicit Dog(const Pos&);
	Dog(const Dog&) = default;

	std::string string(void) const;

	//Those constructors give too much liberty
	Dog(pos_t, pos_t)				= delete;
	Dog(Pos, uint)					= delete;
	Dog(Pos, uint, uint)			= delete;
	Dog(Pos, uint, uint, uint8_t)	= delete;

	//Dogs' speeds and sizes are constant
	void increaseSize(uint)		= delete;
	void increaseSpeed(uint)	= delete;

	uint getSize(void) const	= delete;

	/** The size of all dogs in pixels*/
	static constexpr uint8_t size = 100;

	/** The speed of all dogs in pixels per frame */
	static constexpr uint8_t speed = 3;

	static const std::string spriteBase;
};