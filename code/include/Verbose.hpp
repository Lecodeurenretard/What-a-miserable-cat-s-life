#include "Imports.hpp"

class VerboseStream {
private:
	static bool enabled;

	std::ostream stream;
	bool flushed;
public:
	VerboseStream(const std::ostream& = std::cout);

	static void setEnabled(int, char**);

	template<typename T>
	VerboseStream& operator<<(const T& value) {
		if (enabled) {
			stream << STYLE_VERBOSE;
			if(flushed){
				stream << "Verbose: ";
				flushed = false;
			}
			stream << value << COLOR_ALL_RESET;
		}
		return *this;
	}

	typedef std::ostream& (*ostream_manipulator)(std::ostream&);
	VerboseStream& operator<<(ostream_manipulator);
};