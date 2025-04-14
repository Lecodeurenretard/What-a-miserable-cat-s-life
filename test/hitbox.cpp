#include <iostream>
#include "../code/include/Hitbox.hpp"

void waitKeyPress(void);
#define pause() waitKeyPress()


std::string strRepeat(const std::string&, size_t);
std::string to_string(bool);
void runTest(const char*, SDL_FRect, SDL_FRect, bool, size_t=1);
void runAndDisplay(const char*, SDL_FRect, SDL_FRect, bool, size_t=1, bool=false);
void drawTest(Hitbox, Hitbox, bool=false);

int main() {
	runAndDisplay("1. Full overlap"		,		{0 , 0 , 10  , 10	},	{2    , 2    , 5   , 5   },		true,	2);
	runAndDisplay("2. Partial overlap"	,		{0 , 0 , 10  , 10	},	{8    , 8    , 5   , 5   },		true,	2);
	runAndDisplay("3. Edge touch (right-left)",	{0 , 0 , 5   , 5	},	{5    , 0    , 5   , 5   },		false,	1);
	runAndDisplay("4. Corner touch",			{0 , 0 , 5   , 5	},	{5    , 5    , 5   , 5   },		false,	2);
	runAndDisplay("5. No overlap",				{0 , 0 , 5   , 5	},	{6    , 6    , 5   , 5   },		false,	3);
	runAndDisplay("6. A contains B",			{0 , 0 , 10  , 10	},	{1    , 1    , 2   , 2   },		true,	2);
	runAndDisplay("7. Tiny rectangles overlap",	{1 , 1 , 0.1f, 0.1f	},	{1.05f, 1.05f, 0.1f, 0.1f},		true,	1);
	runAndDisplay("8. Negative coords overlap",	{-5, -5, 10  , 10	},	{-1   , -1   , 2   , 2   },		true,	1);
	runAndDisplay("9. Same zone",				{2 , 2 , 4   , 4	},	{2    , 2    , 4   , 4   },		true,	3);
	runAndDisplay("10. Edge match, same size",	{0 , 0 , 5   , 5	},	{5    , 0    , 5   , 5   },		false,	1);

	return 0;
}

/**
 * Wait until the specified key is entered or quit if the corresponding event is fired.
 * @param key The key code (one of `SDLK_*` macros) of the key to wait.
 * @param r The renderer to free if the user quits.
 * @param win The window to free if the user quits
 */
void waitKeyPress(void){
	SDL_Event ev;
	SDL_PollEvent(&ev);

	while(ev.type != SDL_KEYDOWN)
		SDL_PollEvent(&ev);
}

std::string strRepeat(const std::string& r, size_t nb) {
	if(nb == 0)
		return "\0";

	std::string res = r;
	for(size_t i = 1; i < nb; i++)
		res += r;
	return res;
}

std::string to_string(bool b) {
	return b? "true" : "false";
}

void runTest(const char* desc, SDL_FRect a, SDL_FRect b, bool expected, size_t align/*= 1*/) {
	Hitbox ha(a), hb(b);
	bool result = ha.isOverlapping(hb);
	const char* tab = align == 0? " " : strRepeat("\t", align).c_str();

	std::cout	<< desc << ":" << tab << (
		result == expected ?
		COLOR_FG_GREEN  "PASS"  COLOR_FG_RESET
		: COLOR_FG_RED  "FAIL"  COLOR_FG_RESET " (got " COLOR_FG_BYELLOW + to_string(result) + COLOR_FG_RESET " but expected " COLOR_FG_BYELLOW + to_string(expected) + COLOR_FG_RESET ")"
	) << "\n";
}

void runAndDisplay(const char* desc, SDL_FRect a, SDL_FRect b, bool expected, size_t align/*= 1*/, bool pretty/*=false*/) {
	runTest(desc, a, b, expected, align);
	drawTest(a, b, pretty);
}

void drawTest(Hitbox h1, Hitbox h2, bool pretty/*=false*/) {
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_CreateWindowAndRenderer(500, 500, 0, &win, &ren);
	
	if(pretty){
		constexpr uint growth = 30;
		h1.expand(growth).draw(ren, HITBOX_COLOR_INACTIVE	, Vector::ZERO);
		h2.expand(growth).draw(ren, HITBOX_COLOR_ACTIVE		, {growth, growth});
	} else {
		h1.draw(ren, HITBOX_COLOR_INACTIVE);
		h2.draw(ren, HITBOX_COLOR_ACTIVE);
	}
	SDL_RenderPresent(ren);
	pause();

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
}