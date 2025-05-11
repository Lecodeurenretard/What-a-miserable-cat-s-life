#include "streamUtil.hpp"

/**
 * A class for outputting verbose.
 */
class VerboseStream {
private:
	static bool enabled;

	std::ostream* stream;
	bool printHeading;

	const VerboseStream& operator=(const VerboseStream&) noexcept;

public:
	explicit VerboseStream(std::ostream& = std::cout) noexcept;
	VerboseStream(const VerboseStream&) = default;

	static void setEnabled(const cmd::Parser::parseReturn_t&);

	template<typename T>
		requires Streamable<T>
	VerboseStream& operator<<(const T& value) noexcept {
		if (enabled) {
			if(printHeading) {
				printHeading = false;
				*stream << VerboseStream::heading;
			}
			//throws a -Waddress warning because GCC wants to.
			*stream << STYLE_VERBOSE << value << STYLE_RESET;
		}
		return *this;
	}

	VerboseStream& operator<<(ostream_manipulator);

	typedef VerboseStream& (*verbose_manipulator)(VerboseStream&);
	VerboseStream& operator<<(verbose_manipulator);

	static const std::string heading;
	static VerboseStream& noHeading(VerboseStream&)	noexcept;
	static VerboseStream& newLine(VerboseStream&)	noexcept;
};

/** The verbose stream */
inline VerboseStream vout;