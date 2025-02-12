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
VerboseStream& VerboseStream::operator=(const VerboseStream& toCopy){
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
 * @param argc The number of argument the user passed. Must come from `argc` in the `main()` definition.
 * @param argc The values of the arguments passed by the user. Must come from `argv` in the `main()` definition.
 * @return If verbose will be displayed
 */
void VerboseStream::setEnabled(int argc, char** argv){	//Who needs safety?
	enabled = argc > 1 && (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--verbose");
	return enabled;
}

using os_manip=VerboseStream::ostream_manipulator;
VerboseStream& VerboseStream::operator<<(os_manip foo) {

	if (enabled && (foo == (os_manip)std::endl || foo == (os_manip)std::flush))
		printHeading = true;
	if (enabled)
		*stream << foo;
	return *this;
}

using v_manip=VerboseStream::verbose_manipulator;
VerboseStream& VerboseStream::operator<<(v_manip foo) {
	*this = foo(*this);
	return *this;
}

/**
 * What to print before verbose message.
 */
constexpr std::string VerboseStream::heading("Verbose: ");

/**
 * Forbid the display of the annoying `Verbose: `.
 */
VerboseStream& VerboseStream::noHeading(VerboseStream& ver){
	ver.printHeading = false;
	return ver;
}

/**
 * Insert a new line, ignore the heading.
 */
VerboseStream& VerboseStream::newLine(VerboseStream& ver){
	*(ver.stream) << "\r\n";	//preparing the windows port
	return ver;
}