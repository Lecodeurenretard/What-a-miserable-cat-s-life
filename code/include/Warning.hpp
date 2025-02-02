#pragma once
#include "Imports.hpp"

/**
 * An error which has no other purpose than being displayed
 */
class Warning {
private:
	std::string msg;

public:
	Warning(std::string);
	const char* what(void) const noexcept;
	void print(bool = true, std::ostream& = std::cerr) const;

	operator std::string();
	operator const char*();
};