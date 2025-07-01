#include "streamUtil.hpp"

/**
 * A class for outputting verbose.
 */
class VerboseStream {
private:
	static bool enabled;

	std::ostream* stream;
	bool printHeading;

	VerboseStream(const VerboseStream&) 			= default;
	VerboseStream& operator=(const VerboseStream&)	= default;

public:
	explicit VerboseStream(std::ostream& = std::cout) noexcept;
	
	~VerboseStream(void)							= default;

	static void setEnabled(const cmd::Parser::parseReturn_t&);

	typedef VerboseStream& (*verbose_manipulator)(VerboseStream&) noexcept;

	/**
	 * @throw Throw an exception only if the inclusion in `stream` throws.
	 */
	template<typename T>
		requires Streamable<T>
	VerboseStream& operator<<(const T& value) {
		if (!enabled)
			return *this;
		
		if(printHeading) {
			printHeading = false;
			*stream << VerboseStream::heading;
		}
		
		//throws a -Waddress warning because GCC wants to.
		*stream << STYLE_VERBOSE << value << STYLE_RESET;
		return *this;
	}
	VerboseStream& operator<<(ostream_manipulator);
	VerboseStream& operator<<(verbose_manipulator);


	static const std::string heading;
	static VerboseStream& noHeading(VerboseStream&)	noexcept;
	static VerboseStream& newLine(VerboseStream&)	noexcept;
};

/** The verbose stream */
inline VerboseStream vout;