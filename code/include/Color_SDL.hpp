//Some color definitions for the SDL_Color type
#include <SDL2/SDL_pixels.h>	//the file containing `SDL_Color`

#define COL_WHITE		SDL_Color{.r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE}
#define COL_BLACK		SDL_Color{.r =   0, .g =   0, .b =   0, .a = SDL_ALPHA_OPAQUE}

#define COL_RED			SDL_Color{.r = 255, .g =   0, .b =   0, .a = SDL_ALPHA_OPAQUE}
#define COL_GREEN		SDL_Color{.r =   0, .g = 255, .b =   0, .a = SDL_ALPHA_OPAQUE}
#define COL_BLUE		SDL_Color{.r =   0, .g =   0, .b = 255, .a = SDL_ALPHA_OPAQUE}

#define COL_YELLOW		SDL_Color{.r = 255, .g = 255, .b =   0, .a = SDL_ALPHA_OPAQUE}
#define COL_MAUVE		SDL_Color{.r = 255, .g =   0, .b = 255, .a = SDL_ALPHA_OPAQUE}
#define COL_CYAN		SDL_Color{.r =   0, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE}

#define COL_PINK		SDL_Color{.r = 255, .g = 100, .b = 255, .a = SDL_ALPHA_OPAQUE}
#define COL_TURQUOISE	SDL_Color{.r =  70, .g = 130, .b = 180, .a = SDL_ALPHA_OPAQUE}
#define COL_NAVY		SDL_Color{.r =   0, .g =   0, .b = 128, .a = SDL_ALPHA_OPAQUE}
#define COL_FIREBRICK	SDL_Color{.r = 178, .g =  34, .b =  34, .a = SDL_ALPHA_OPAQUE}
#define COL_DARKCYAN	SDL_Color{.r =   0, .g = 139, .b = 139, .a = SDL_ALPHA_OPAQUE}