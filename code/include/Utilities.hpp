/** me-defined constants that need to be included in all files */
/** The width of the window */
constexpr uint WIN_WIDTH	= 1000;

/** The height of the window */
constexpr uint WIN_HEIGHT	= 1000;

/** The smallest dimension of the window */
constexpr uint WIN_MIN = std::min(WIN_HEIGHT, WIN_WIDTH);

/** The largest dimension of the window */
constexpr uint WIN_MAX = std::min(WIN_HEIGHT, WIN_WIDTH);

/** The framerate */
constexpr uint8_t DESIRED_FPS = 60;

/** The type of a path mask */
typedef bool (*mask_t)(const fs::path&);

/**
 * Returns a double in the [`min`, `max`] range, undefined behaviour if max < min.
 */
inline double randDouble(int min=0, int max=RAND_MAX) {
	return (double)std::rand()/RAND_MAX
		* (max - min)
		+ min;
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
inline int randInt(int min=0, int max=RAND_MAX) {
	return std::round(randDouble(min, max));
}

/**
 * Returns the number of files the `dir` directory according to `fileMask`
 */
inline size_t howManyFiles(fs::path dir, mask_t fileMask) {
	return std::count_if(
		fs::directory_iterator(dir),
		fs::directory_iterator{},
		fileMask
	);
}

/**
 * Returns the number of regular files the `dir` directory
 */
inline size_t howManyRegularFiles(fs::path dir) {
	return howManyFiles(dir, (mask_t)fs::is_regular_file);
}