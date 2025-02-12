#include "Imports.hpp"

class VerboseStream {
private:
	static bool enabled;

	std::ostream* stream;
	bool printHeading;

	VerboseStream& operator=(const VerboseStream&);

public:
	VerboseStream(const std::ostream& = std::cout);
	VerboseStream(const VerboseStream&) = default;

	static void setEnabled(int, char**);

	template<typename T>
	VerboseStream& operator<<(const T& value) {
		if (enabled) {
			*stream << STYLE_VERBOSE;
			if(printHeading){
				printHeading = false;
				operator<<(VerboseStream::heading);
			}
			*stream << value << COLOR_ALL_RESET;
		}
		return *this;
	}

	typedef std::ostream& (*ostream_manipulator)(std::ostream&);
	VerboseStream& operator<<(ostream_manipulator);

	typedef VerboseStream& (*verbose_manipulator)(VerboseStream&);
	VerboseStream& operator<<(verbose_manipulator);

	constexpr static std::string heading;
	static VerboseStream& noHeading(VerboseStream&);
	static VerboseStream& newLine(VerboseStream&);
};

/** The verbose stream. */
VerboseStream vout;