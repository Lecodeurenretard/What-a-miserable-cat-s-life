#include "Imports.hpp"

/** The type for representing a position on 1 dimension */
typedef uint pos_t;

/**
 * A position in space
 */
struct Pos {
	pos_t x;
	pos_t y;

	Pos(void);
	Pos(pos_t, pos_t);
	Pos(const Pos&)	= default;
	~Pos(void)		= default;
	
	std::string string(void) const;
};