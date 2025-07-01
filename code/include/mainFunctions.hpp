#include "../include/Verbose.hpp"
#include "../include/Cat.hpp"

bool saveImageToFile(const char*, const unsigned char*, const unsigned int);
bool saveImageToFile(const char*, uint8_t);
void saveImgs(const char*, size_t, size_t);
void createSpriteDir(void);

void waitNextFrame(double);
void waitEvent(SDL_EventType,	SDL_Renderer* = nullptr, SDL_Window* = nullptr)	noexcept;
void waitKeyPress(SDL_KeyCode,	SDL_Renderer* = nullptr, SDL_Window* = nullptr)	noexcept;


void init(SDL_Window** win, SDL_Renderer** ren);
void quit(int = EXIT_SUCCESS, SDL_Renderer* = nullptr, SDL_Window* = nullptr) noexcept;