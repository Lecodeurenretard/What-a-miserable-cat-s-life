#include "../code/include/Vector.hpp"
#include <cassert>

int main(void) {
	SDL_Window* win;
	SDL_Renderer* ren;
	SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, 0, &win, &ren);

	std::cout << std::setprecision(17) << std::numeric_limits<double>::digits10 << ';' << std::numeric_limits<double>::digits10 << '\n';
	std::cout << std::numeric_limits<float>::epsilon() << ';' << std::numeric_limits<double>::epsilon() << '\n';
	for(double i = -360; i <= 360; i++) {
		assert(areApproxEqual(degToRadian(radToDegree(i), true), i, 3));	//values outide of [-360; 360] may need a less precise approximation
		assert(areApproxEqual(radToDegree(degToRadian(i), true), i, 3));

		std::cout << "Rendering the vector turned at " << degToRadian(i) << "rad (" << i << "deg).\n";

		SDL_SetRenderDrawColor(ren, SDL_Color{255, 255, 255, 255});
		(Vec_UP * 100).rotate(degToRadian(i)).draw(ren, Vector{WIN_WIDTH, WIN_HEIGHT}/2);

		SDL_RenderPresent(ren);
		SDL_Delay(10);

		SDL_SetRenderDrawColor(ren, SDL_Color{0, 0, 0, 0});
		SDL_RenderClear(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}