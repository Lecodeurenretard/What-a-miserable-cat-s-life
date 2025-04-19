#pragma once
#include "Imports.hpp"

/**
 * A stream handler that prints warnings.
 */
class WarningStream {
private:
	std::ostream* stream;
	bool printHeading = true;

	const WarningStream& operator=(const WarningStream&);
public:
	static const std::string heading;

	explicit WarningStream(const std::ostream& = std::cerr);
	WarningStream(const WarningStream&) = default;

	template<typename T>
	WarningStream& operator<<(const T& value) {
		*stream << STYLE_WARNING << (printHeading? WarningStream::heading : "\0") << value << STYLE_RESET;
		printHeading = false;
		return *this;
	}

	WarningStream& operator<<(ostream_manipulator);

	static WarningStream& end(WarningStream&);
};

/** The warning stream */
inline WarningStream wout;