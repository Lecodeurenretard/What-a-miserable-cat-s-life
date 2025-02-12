#include "Imports.hpp"

/**
 * A class for outputting verbose.
 */
class VerboseStream {
private:
	static bool enabled;

	std::ostream* stream;
	bool printHeading;

	const VerboseStream& operator=(const VerboseStream&);

public:
	explicit VerboseStream(const std::ostream& = std::cout);
	VerboseStream(const VerboseStream&) = default;

	static void setEnabled(int, char**);

	template<typename T>
	VerboseStream& operator<<(const T& value) {
		if (enabled) {
			if(printHeading){
				printHeading = false;
				*stream << VerboseStream::heading;
			}
			*stream << STYLE_VERBOSE << value << COLOR_ALL_RESET;
		}
		return *this;
	}

	VerboseStream& operator<<(ostream_manipulator);

	typedef VerboseStream& (*verbose_manipulator)(VerboseStream&);
	VerboseStream& operator<<(verbose_manipulator);

	static const std::string heading;
	static VerboseStream& noHeading(VerboseStream&);
	static VerboseStream& newLine(VerboseStream&);
};

/** The verbose stream */
inline VerboseStream vout;