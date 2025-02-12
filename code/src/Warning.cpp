#include "../include/Warning.hpp"

/**
 * What to print before every warning message.
 */
const std::string WarningStream::heading("Warning: ");

/**
 * For now, it copies:
 * - stream
 */
const WarningStream& WarningStream::operator=(const WarningStream& toCopy){
	if(this == &toCopy)
		return *this;

	this->stream = toCopy.stream;
	return *this;
}

WarningStream::WarningStream(const std::ostream& out/* = std::cout*/){
	stream = new std::ostream(out.rdbuf());
}

WarningStream& WarningStream::operator<<(ostream_manipulator foo) {
	*stream << foo;
	return *this;
}