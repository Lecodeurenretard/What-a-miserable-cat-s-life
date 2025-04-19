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
const VerboseStream& VerboseStream::operator=(const VerboseStream& toCopy) {
	if(this == &toCopy)
		return *this;

	this->printHeading = toCopy.printHeading;
	this->stream = toCopy.stream;
	return *this;
}

VerboseStream::VerboseStream(const std::ostream& out/* = std::cout*/)
	: printHeading(true)
{
	stream = new std::ostream(out.rdbuf());
}

/**
 * Verify if the user enabled verbose with `-v` or `--verbose`.
 * @param args The result of parsing the arguments with `Parser.parse()`.
 */
void VerboseStream::setEnabled(cmd::Parser::parseReturn_t& args) {		//not const because the [] operator returns a mutable reference
	enabled = std::get<bool>(args["-v"]) || std::get<bool>(args["--verbose"]);
}

VerboseStream& VerboseStream::operator<<(ostream_manipulator foo) {
	if (enabled && (foo == (ostream_manipulator)std::endl || foo == (ostream_manipulator)std::flush))
		printHeading = true;
	if (enabled)
		*stream << foo;
	return *this;
}

using v_manip=VerboseStream::verbose_manipulator;
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
VerboseStream& VerboseStream::noHeading(VerboseStream& ver) {
	ver.printHeading = false;
	return ver;
}

/**
 * Insert a new line, ignore the heading.
 */
VerboseStream& VerboseStream::newLine(VerboseStream& ver) {
	*(ver.stream) << "\r\n";	//preparing the windows port
	return ver;
}