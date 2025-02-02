#include "../include/Warning.hpp"

/**
 * Initialize warning the instance.
 */
Warning::Warning(std::string errMsg)
	: msg(errMsg)
{}

/**
 * Get the msg.
 */
const char* Warning::what(void) const noexcept{
	return msg.c_str();
}

/**
 * Print the message.
 */
void Warning::print(bool flush /*= true*/, std::ostream& out /*= std::cerr*/) const{
	out << STYLE_WARNING << "Warning: " << msg << COLOR_ALL_RESET;
	
	if(flush)
		out << std::endl;
	else
		out << '\n';
}

/**
 * Converts to `std::string`
 */
Warning::operator std::string(){
	return what();
}

/**
 * Converts to C-String
 */
Warning::operator const char*(){
	return what();
}