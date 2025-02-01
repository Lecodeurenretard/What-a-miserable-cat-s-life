#include "../include/Verbose.hpp"

bool VerboseStream::enabled(false);

VerboseStream::VerboseStream(const std::ostream& out/* = std::cout*/)
	: stream(out.rdbuf()), flushed(true)
{}

void VerboseStream::setEnabled(int argc, char** argv){	//Who needs safety?
	enabled = argc > 1 && (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--verbose");
}

using os_manip=VerboseStream::ostream_manipulator;
VerboseStream& VerboseStream::operator<<(os_manip foo) {

	if (enabled && (foo == (os_manip)std::endl || foo == (os_manip)std::flush))
		flushed = true;
	if (enabled)
		stream << foo;
	return *this;
}