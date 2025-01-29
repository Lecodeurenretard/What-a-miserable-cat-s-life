// Following the following question, I wrote a wrapper file
// https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

//TODO: use enums instead of macros

//reset
#define COLOR_ALL_RESET		"\033[0m"
#define COLOR_FG_RESET		"\033[39m"
#define COLOR_BG_RESET		"\033[49m"

//Classic foreground colors
#define COLOR_FG_BLACK		"\x1B[30m"
#define COLOR_FG_RED		"\x1B[31m"
#define COLOR_FG_GREEN		"\x1B[32m"
#define COLOR_FG_YELLOW		"\x1B[33m"
#define COLOR_FG_BLUE		"\x1B[34m"
#define COLOR_FG_MAGENTA	"\x1B[35m"
#define COLOR_FG_CYAN		"\x1B[36m"
#define COLOR_FG_WHITE		"\x1B[37m"

//Bright foreground colors
#define COLOR_FG_BBLACK		"\x1B[90m"
#define COLOR_FG_BRED		"\x1B[91m"
#define COLOR_FG_BGREEN		"\x1B[92m"
#define COLOR_FG_BYELLOW	"\x1B[93m"
#define COLOR_FG_BBLUE		"\x1B[94m"
#define COLOR_FG_BMAGENTA	"\x1B[95m"
#define COLOR_FG_BCYAN		"\x1B[96m"
#define COLOR_FG_BWHITE		"\x1B[97m"


//Classic background colors
#define COLOR_BG_BLACK		"\x1B[40m"
#define COLOR_BG_RED		"\x1B[41m"
#define COLOR_BG_GREEN		"\x1B[42m"
#define COLOR_BG_YELLOW		"\x1B[43m"
#define COLOR_BG_BLUE		"\x1B[44m"
#define COLOR_BG_MAGENTA	"\x1B[45m"
#define COLOR_BG_CYAN		"\x1B[46m"
#define COLOR_BG_WHITE		"\x1B[47m"

//Bright background colors
#define COLOR_BG_BBLACK		"\x1B[100m"
#define COLOR_BG_BRED		"\x1B[101m"
#define COLOR_BG_BGREEN		"\x1B[102m"
#define COLOR_BG_BYELLOW	"\x1B[103m"
#define COLOR_BG_BBLUE		"\x1B[104m"
#define COLOR_BG_BMAGENTA	"\x1B[105m"
#define COLOR_BG_BCYAN		"\x1B[106m"
#define COLOR_BG_BWHITE		"\x1B[107m"

//ostream << operator
#include <iostream>

#define STYLE_OS_VERBOSE PART_OPEN << PART_ITALIC << PART_CLOSE << COLOR_FG_MAGENTA

//If the user wants more control over styles
#define PART_ESCAPE		"\e"
#define PART_OPEN		"\e["
#define PART_CLOSE		"m"

#define PART_RESET		"0"
#define PART_BOLD		"1"
#define PART_FAINT		"2"
#define PART_ITALIC		"3"
#define PART_UNDERLINE	"4"
#define PART_BLINK		"5"		//slow blink

//PART_FAST_BLINK not implemented
//PART_INVERT not implemented
//PART_HIDE not implemented

#define PART_STRIKE "9"

// PART 10~21 not implemented

#define PART_NORMAL_INTENSITY	"22"
#define PART_NUNDERLINE			"23"
#define PART_NBLINK				"24"

// PART_Proportional_spacing not implemented

#define PART_NREVERSED			"24"
#define PART_REVEAL				"25"

//PART_NHIDE not implemented

#define PART_COLOR_BLACK		"0"
#define PART_COLOR_RED			"1"
#define PART_COLOR_GREEN		"2"
#define PART_COLOR_YELLOW		"3"
#define PART_COLOR_BLUE			"4"
#define PART_COLOR_MAGENTA		"5"
#define PART_COLOR_CYAN			"6"
#define PART_COLOR_WHITE		"7"

#define PART_FG					"3"
#define PART_BFG				"9"
#define PART_BG					"4"
#define PART_BBG				"10"

#define PART_FRAMED				"50"
#define PART_CIRCLED			"51"
#define PART_OVERLINED			"52"
#define PART_NCIRCLED_NFRAMED	"53"
#define PART_NFRAMED_NCIRCLED	PART_NCIRCLED_NFRAMED
#define PART_NOVERLINED			"55"

//the rest is not standar of not implemented

//example: ``td::cout << PART_OPEN << PART_BLINK << PART_CLOSE << "Blinkin'" << COLOR_ALL_RESET << std::endl;``