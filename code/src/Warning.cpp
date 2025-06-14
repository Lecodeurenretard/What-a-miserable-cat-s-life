#include "../include/Warning.hpp"

/**
 * What to print before every warning message.
 */
const std::string WarningStream::heading("Warning: ");

/**
 * @throw `std::ios.rdbuf()` may throw implementation defined exception but it doesn't seem to on GCC. Also the `std::ostream` constructor is assumed to not throw.
 */
WarningStream::WarningStream(std::ostream& out/* = std::cerr*/) noexcept {
	stream = &out;
}

WarningStream& WarningStream::operator<<(ostream_manipulator foo) {
	if(foo == static_cast<ostream_manipulator>(std::endl))
		WarningStream::end(*this);

	*stream << foo;
	return *this;
}

/**
 * Indicates the end of an input, does not print anything nor flush.
 */
WarningStream& WarningStream::end(WarningStream& warn) {
	warn.printHeading = true;
	return warn;
}