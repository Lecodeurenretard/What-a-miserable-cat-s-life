#pragma once
#include "Imports.hpp"

/**
 * A stream handler that prints warnings.
 */
class WarningStream {
private:
	std::ostream* stream;

	const WarningStream& operator=(const WarningStream&);
public:
	static const std::string heading;

	explicit WarningStream(const std::ostream& = std::cerr);
	WarningStream(const WarningStream&) = default;

	template<typename T>
	WarningStream& operator<<(const T& value) {
		*stream << STYLE_WARNING << WarningStream::heading << value << STYLE_RESET;
		return *this;
	}

	WarningStream& operator<<(ostream_manipulator);

};

/** The warning stream */
inline WarningStream wout;