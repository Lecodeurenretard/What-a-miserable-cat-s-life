#pragma once
#include "Imports.hpp"

/**
 * An error which has no other purpose than being displayed
 */
class Warning final : std::exception{
private:
	std::string msg;

public:
	Warning(std::string);
	const char* what(void) const noexcept;

	operator std::string();
	operator const char*();
};