/** me-defined functions that need to be included in all files */


/**
 * Returns a double in the [`min`, `max`] range
 */
inline double randDouble(int min=0, int max=RAND_MAX){
	return std::round(
		(double)std::rand()/RAND_MAX
		* (max - min)
		+ min
	);
	/*
	 * `std::rand()` returns an int between `0` and `RAND_MAX` so 
	 * - we divide by `RAND_MAX` to get a value between `0` and `1`
	 * - we multiply by `max - min` to get a value between `0` and `max-min`
	 * - we finally add `min` to get a value between `min` and `max`
	 */
}

/**
 * Returns an int in the [`min`, `max`] range
 */
inline int randInt(int min=0, int max=RAND_MAX){
	return std::round(randDouble(min, max));
}

/**
 * Returns the number of files the `dir` directory according to to mask
 */
inline size_t howManyFiles(fs::path dir, bool (*fileMask)(const fs::path&)){
	return std::count_if(
		fs::directory_iterator((fs::path)dir),
		fs::directory_iterator{},
		fileMask
	);
}

/**
 * Returns the number of regular files the `dir` directory
 */
inline size_t howManyFiles(fs::path dir){
	using fp = bool (*)(const std::filesystem::path&);	//function pointer type
	return howManyFiles(dir, (fp)fs::is_regular_file);
}

/**
 * Initialize an SDL_Rect
 * @param x The x position of the rectangle
 * @param y The y position of the rectangle
 * @param w The width of the rectangle
 * @param h The height of the rectangle
 */
inline SDL_Rect* SDL_RectInit(int x, int y, int w, int h){
	SDL_Rect* res = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	res->x = x;
	res->y = y;
	res->w = w;
	res->h = h;

	return res;
}