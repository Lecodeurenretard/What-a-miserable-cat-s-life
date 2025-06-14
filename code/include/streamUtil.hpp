//utility for streams
#pragma once
#include "Imports.hpp"

template<typename T>
concept Streamable = requires (const T& val, std::ostream& s) {
	{s << val} -> std::same_as<std::ostream&>;
};

typedef std::ostream& (*ostream_manipulator)(std::ostream&);