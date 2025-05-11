#pragma once
#include "streamUtil.hpp"

/**
 * A stream handler that prints warnings.
 */
class WarningStream {
private:
	std::ostream* stream;
	bool printHeading = true;

	const WarningStream& operator=(const WarningStream&) noexcept;
public:
	static const std::string heading;

	explicit WarningStream(std::ostream& = std::cerr) noexcept;
	WarningStream(const WarningStream&) = default;

	template<typename T>
		requires Streamable<T>
	WarningStream& operator<<(const T& value) noexcept {
		*stream << STYLE_WARNING << (printHeading? WarningStream::heading : "\0") << value << STYLE_RESET;
		printHeading = false;
		return *this;
	}

	WarningStream& operator<<(ostream_manipulator);

	static WarningStream& end(WarningStream&);
};

/** The warning stream */
inline WarningStream wout;