#include "../include/Verbose.hpp"

/**
 * If verbose is enabled by the user.
 */
bool VerboseStream::enabled(false);

/**
 * For now, it copies:
 * - printHeading
 * - stream
 */
const VerboseStream& VerboseStream::operator=(const VerboseStream& toCopy) noexcept {
	if(this == &toCopy)
		return *this;

	printHeading = toCopy.printHeading;
	stream = toCopy.stream;
	return *this;
}

VerboseStream::VerboseStream(std::ostream& out/* = std::cout*/) noexcept
	: printHeading(true)
{
	stream = &out;
}

/**
 * Verify if the user enabled verbose with `-v` or `--verbose`.
 * @param args The result of parsing the arguments with `Parser.parse()`.
 */
void VerboseStream::setEnabled(const cmd::Parser::parseReturn_t& args) {
	enabled = std::get<bool>(args.at("-v")) || std::get<bool>(args.at("--verbose"));
}

/**
 * @throw Throw an exception only if `foo()` throws one.
 */
VerboseStream& VerboseStream::operator<<(ostream_manipulator foo) {
	if (enabled && (foo == (ostream_manipulator)std::endl || foo == (ostream_manipulator)std::flush))
		printHeading = true;
	if (enabled)
		*stream << foo;
	return *this;
}

using v_manip=VerboseStream::verbose_manipulator;
/**
 * @throw Throw an exception only if `foo()` throws one.
 */
VerboseStream& VerboseStream::operator<<(v_manip foo) {
	operator=(foo(*this));
	return *this;
}

/**
 * What to print before every verbose message.
 */
const std::string VerboseStream::heading("Verbose: ");

/**
 * Forbid the display of the annoying `Verbose: `.
 */
VerboseStream& VerboseStream::noHeading(VerboseStream& ver) noexcept {
	ver.printHeading = false;
	return ver;
}

/**
 * Insert a new line, ignore the heading.
 */
VerboseStream& VerboseStream::newLine(VerboseStream& ver) noexcept {
	(*ver.stream) << "\r\n";	//preparing the windows port
	return ver;
}