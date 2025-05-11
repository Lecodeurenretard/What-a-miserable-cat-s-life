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

template<typename T>
concept Number = std::is_arithmetic_v<T>;

/**
 * Return a double in the [`min`, `max`] range, undefined behaviour if max < min.
 */
inline double randDouble(int min=0, int max=RAND_MAX) noexcept {
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
 * Return an int in the [`min`, `max`] range
 */
inline int randInt(int min=0, int max=RAND_MAX) noexcept {
	return std::round(randDouble(min, max));
}

/**
 * Return the number of files the `dir` directory according to `fileMask`.
 * @throw Exceptions thrown come from `std::count_if()`, `fs::directory_iterator`'s constructor or `fileMask()`.
 */
inline size_t howManyFiles(fs::path dir, mask_t fileMask) {
	return std::count_if(
		fs::directory_iterator(dir),
		fs::directory_iterator{},
		fileMask
	);
}

/**
 * Return the number of regular files the `dir` directory.
 * @throw Exceptions come from `howManyFiles()` or `fs::is_regular_file()`.
 */
inline size_t howManyRegularFiles(fs::path dir) {
	return howManyFiles(dir, (mask_t)fs::is_regular_file);
}

/**
 * Check if an integer overflow occurs when `operation` is computed.
 * @param num1 The first int (its type must verify `std::is_integral_v<T>`).
 * @param num2 The second int (its type must verify `std::is_integral_v<T>`).
 * @param operation A pointer pointing to a function computing the wanted operation it is assumed that `operation(num1, num2) > num1` and `operation(num1, num2) > num2`.  
 * By default it is addition.
 * @param res The result of the operation, possible memory leak since the data at `res` is never freed.
 * @throw May throw an exception only if `operation()` throws one.
 */
template<typename T>
	requires Number<T>
inline constexpr bool checkIntegerOverflow(T num1, T num2, T* res = nullptr, T(*operation)(T, T) = [](T n1, T n2) -> T {return n1 + n2;}) {
	*res = operation(num1, num2);
	return *res < std::max(num1, num2);
}

/**
 * Check if an integer underflow occurs when `operation` is computed.
 * @param num1 The first int (its type must verify `std::is_integral_v<T>`).
 * @param num2 The second int (its type must verify `std::is_integral_v<T>`).
 * @param operation A pointer pointing to a function computing the wanted operation it is assumed that `operation(num1, num2) < num1` and `operation(num1, num2) < num2`.  
 * By default it is substraction.
 * @param res The result of the operation, possible memory leak since the data at `res` is never freed.
 * @throw May throw an exception only if `operation()` throws one.
 */
template<typename T>
	requires Number<T>
inline constexpr bool checkIntegerUnderflow(T num1, T num2, T* res = nullptr, T(*operation)(T, T) = [](T n1, T n2) -> T {return n1 - n2;}) {
	*res = operation(num1, num2);
	return *res > std::max(num1, num2);
}

/**
 * Converts degrees to radians.
 */
template<typename Num>
	requires Number<Num>
constexpr Num degToRadian(Num angle) noexcept {
	return angle * M_PI/180;
}

/**
 * Converts radians to degrees.
 */
template<typename Num>
	requires Number<Num>
constexpr Num radToDegree(Num angle) noexcept {
	return angle * 180/M_PI;
}