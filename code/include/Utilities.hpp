#include "Imports.hpp"

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

struct pos_t;	//defined in Pos.hpp
template<typename T>
concept Number = (std::is_arithmetic_v<T> || std::is_convertible_v<T, double>);

/**
 * Safely check if floating-point numbers are equal.
 */
template<typename Num>
	requires std::is_floating_point_v<Num>
constexpr bool areEqual(Num f1, Num f2) noexcept {
	return -std::numeric_limits<Num>::epsilon() < f1 - f2 &&  f1 - f2 < std::numeric_limits<Num>::epsilon();	//if -epsilon < f1 - f2 < epsilon, f1 == f2
}

/**
 * Safely check if floating-point numbers are approximally equals equal to the `n`th power of ten.
 */
template<typename Num>
	requires std::is_floating_point_v<Num>
constexpr bool areApproxEqual(Num f1, Num f2, uint n) noexcept {
	return -std::numeric_limits<Num>::epsilon() * std::pow(10, n) < f1 - f2 &&  f1 - f2 < std::numeric_limits<Num>::epsilon() * std::pow(10, n);
}

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

//Integer convertions
/**
 * Converts degrees to radians.
 */
template<typename Num>
	requires (Number<Num> && !std::is_floating_point_v<Num>)
constexpr Num degToRadian(Num angle) noexcept {
	return angle * M_PI/180;
}

/**
 * Converts radians to degrees.
 */
template<typename Num>
	requires (Number<Num> && !std::is_floating_point_v<Num>)
constexpr Num radToDegree(Num angle) noexcept {
	return angle * 180/M_PI;
}

//floating point convertions, (rounding an integer is unnecessary work)
/**
 * Converts degrees to radians.
 */
template<typename Num>
	requires std::is_floating_point_v<Num>
constexpr Num degToRadian(Num angle, bool round = false) noexcept {
	const Num res(angle * M_PI/180.0);

	if(round)
		return roundTo<Num>(res, std::numeric_limits<Num>::digits10);
	return res;
}

/**
 * Converts radians to degrees.
 */
template<typename Num>
	requires std::is_floating_point_v<Num>
constexpr Num radToDegree(Num angle, bool round = false) noexcept {
	const Num res(angle * 180/M_PI);

	if(round)
		return roundTo<Num>(res, std::numeric_limits<Num>::digits10);
	return res;
}

/**
 * Round up to the `n`th power of ten.
 * @param floor Floor (truncate) instead of rounding to the nearest.
 * @example roundTo(2.3445, 2) == 2.35, roundTo(234.45, 2) == 200.0
 * @throw The convertion from `Input` to `Output` may throw.
 */
template<typename Input, typename Output = Input>
	requires (
		std::is_floating_point_v<Input> 
		&& std::is_floating_point_v<Output>
		&& std::is_convertible_v<Input, Output>
	)
constexpr Output roundTo(Input number, int n, bool floor = false) {
	Output res = number * std::pow(10, n);
	res = floor? std::floor(res) : std::round(res);
	return res * std::pow(10, -n);
}
static_assert(roundTo(2.3445, 2) == 2.34);
static_assert(roundTo(294.45, -2) == 300.0);

/**
 * Round `number` so it is not more precise than `Num`'s epsilon (`std::numeric_limits<Num>::epsilon()`).
 * This function reduces imprecisions in floating-point computations.
 * @throw The convertion from `Input` to `Output` may throw.
 */
template<typename Input, typename Output = Input>
	requires (
		std::is_floating_point_v<Input> 
		&& std::is_floating_point_v<Output>
		&& std::is_convertible_v<Input, Output>
	)
constexpr Output roundType(Input number) {
	return roundTo<Input, Output>(number, std::numeric_limits<Output>::digits10, true);
}

/** Rounds to 6 digits (float precision), help with floating point imprecisions */
#define roundFloat(flt) roundType<float>(flt)

/** Rounds to 15 digits (double precision), help with floating point imprecisions */
#define roundDouble(dbl) roundType<double>(dbl)


/**
 * Get the principal value of `angle` in radians (ie: the equivalent value in the interval `(-π ;π]`))
 */
template<typename Num>
	requires (Number<Num> && std::is_nothrow_convertible_v<double, Num>)
constexpr Num anglePrincipalValue(Num angle, bool round = false) noexcept {
	using namespace std::numbers;
	
	
	Num res = fmod(angle, (Num)(2*pi) - pi);	//fmod -> floating point modulo
	return round? roundType<Num>(res) : res;
}