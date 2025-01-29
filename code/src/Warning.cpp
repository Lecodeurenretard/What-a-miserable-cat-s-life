#include "../include/Warning.hpp"

Warning::Warning(std::string errMsg): msg(errMsg)
{}

const char* Warning::what(void) const noexcept{
	return msg.c_str();
}

Warning::operator std::string(){
	return what();
}

Warning::operator const char*(){
	return what();
}