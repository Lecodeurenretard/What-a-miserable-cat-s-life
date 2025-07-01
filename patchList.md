# Changelog
This file simply list the progression of patches in this minor version.

## 0.3.0:
+ **Big changes**:
	1. The number of cats and dogs is no more predetermined.
		- Added new arguments `--catCount` and `--dogCount`.
	2. Cats take damages and die.
	3. Completely automated the image extraction, it will no longer need any change in the future unless if `imgToHex` is modified.

+ changes to the `Animal` class:
	- Moved `health` from `Cat`
	- Added `spritePathDead` which is the sprite path when the animal is dead.
	- Changed `isAtDest()` to be private.
	- Split `draw()` to be in several methods: `drawSprite()`, `drawInfos()` and `drawSpecificities()`.
	- Removed parameter `isColliding()` to be computed inside `draw()`.
	- Removed getter `getSize()` since it can be deduced from the hitbox.
	- Added `isDead()`.

+ changes to the `Cat` class and header:
	- The header directly includes `Dog.hpp`.
	- Added member `collisionLastFrame`.
	- Changed `catList[]` to be public.
	- Added method `isHitByDog()`.
	- Removed getter `getSpeed()`.
	- Added static method `generateCats()` which construct cats with given attributes.
	- Added method `handleCollisions()`.

+ changes for the `Dog` class:
	- Changed `getLowestID()` to be public.
	- Added static method `generateDogs()` which has the same purpose as `generateCats()`.


+ other changes:
	- Added shorthand for `SDL_SetRenderDrawColor()` and `SDL_SetRenderDrawColor()` so they can take an `SDL_Color` in input.
	- Added `Hitbox::string()`.
	- Added sprites for dead animals and cats.
	- Fixed the lateness detection in `waitNextFrame()`.
	- Updated doc.
	- Added a space before `{` which begin a scope to make the code a little bit prettier.

## 0.3.1:
+ **Big changes**:
	- Better error propagation:
		* Removed all `noexcept(false)`, by default a function is considered potentionally throwing.
		* Added a `@throw` section in the documentation of potentially throwing functions which explain when the function throws.
		* Added `noexcept` for non-throwing functions.
	- `Vector` is now a literal type.
+ Changes for `Animal`:
	- Renamed `ANIMAL_DEFAULT_FONT` to `ANIMAL_ALLOCATE_DEFAULT_FONT`.
	- Added `nullptr` check for `spriteTexture` in `drawSprite()`.
	- Added a `folder` parameter in `getRandomPathFromMask()` to avoid duplicate code in child classes.
+ Changes for `Cat`:
	- `freeCatList()` which frees `catList[]`.
	- Changed `catList`'s type to be an `std::array`.
	- Removed override of `getRandomPathFromMask()` due to the original method's new parameter.
+ Changes for `Dog`:
	- Added static method `freeDogList()` which frees `dogList[]`.
	- Changed `dogList`'s type to be an `std::array`.
+ Changes for `Vector`:
	- Static constants have been moved out of the class and renamed with prefix `Vec_`.
	- Methods `norm()`, `dotProduct()` and arithmetical operators are now `constexpr`.
	- Added method `isNull()` and (non-member) unary `+` and `-` operators.
	- Changed `unit()` method to throw an `std::logic_error()` instead of just printing a warning.
	- Added `nullptr` checks in `draw()`.
+ Changes for `VerboseStream` and `WarningStream`:
	- Added concept `Streamable<>` which checks if the value can use the `<<` parameter on an `std::ostream` object.
	- The template parameter now have to satisfy concept `Streamable<>`.

+ other changes:
	- Added functions `checkIntegerOverflow()` and `checkIntegerUnderflow()`.
	- Added functions `degToRadian()` and `radToDegree()`.
	- All functions other that `main()` in [main.cpp](code/src/main.cpp) have been moved to [mainFunctions.cpp](code/src/mainFunctions.cpp).
	- Fixed argument checking & updated Argument-Parser.
	- Simplified end of frame logic to one `if` `else` block.

## 0.3.2:
+ **Big changes**:
	- Now coding in a more modern C++ (using smart pointers and standard arrays).
	- A few refactors
+ Changes for `Cat`:
	- `catList` now contains `unique_ptr` instead of raw pointers.
	- Split `trySetLowestID()` to `setLowestID()` and `canBeListed()`.
	- Constructors are private
	- Constructors are no longer in charge of indexing the cat in `catList`.
	- Defaulted destructor
	- Befriended `std::make_unique()` (now friend method).
	- Changed the type of `generateCats()`'s second argument to a pointer to vector.
	- Replaced `freeCatList()` by `clearCatList()` which unallocates each cat in it.
	- Fixed hitbox color when hit.
+ Changes for `Dog`:
	- `dogList` now contains `unique_ptr` instead of raw pointers.
	- Added method `canBeListed()`
	- The explicit constructor is now private
	- Befriended `std::make_unique()`.
	- Defaulted the copy constructor and the destructor.
	- Changed the type of `generateDogs()`'s second argument to a pointer to vector.
	- Renamed `getLowestID()` to `getLowestIndex()`
	- Replaced `freeDogList()` by `clearDogList()` which unallocates each dog in it.
+ Changes for `Hitbox`:
	- Added `noexcept`s where missing.
	- Replaced the `expand()` method by `scale()`
	- Added method `resize_world()` which scales and move the hitbox by a factor.
+ Changes for `Pos`:
	- Added defaulted `operator=()`, copy constructor and destructor.
	- Added `noexcept`s
	- Added checks for fails of SDL functions in `draw()`.
+ Changes for `Vector`:
	- Swapped `Vec_RIGHT` and `Vec_LEFT` so they represent the good vector.
	- Now ounding results.
+ Changes for streams:
	- Defaulted `=` operators.
+ other changes:
	- Replaced C-style casts to non-numeric types to `static_cast`.
	- Swapped the inclusion hierarchy for [Imports.hpp](code/include/Imports.hpp) and [Utilites.hpp](code/include/Utilities.hpp).
	- Added a header for [mainFunctions](code/include/mainFunctions.hpp)
	- Refactored into functions some parts of `main()`:
		* Added the `init()` function in [mainFunctions](code/src/mainFunctions.cpp) which launch libraries and creates the window and renderer.
		* Added the `createSpriteDir()` function which initialize the `sprite` directory by extracting images from `libImages.so`.
	- Better handling of floating-point arithmetic:
		* Added `areEqual()` function which check if two floating-point numbers are equal (`==` can fail)
		* Added rounding functions for decreasing the error factor.
	- Added some other angle calculations functions.
	- Added new tests.
	- Enhaced "pretty" mode for hitbox test.
	- Added verbose and aligned the verbose location.
	- Renamed the directory _utility libs_ to [submodules](code/include/submodules/)

## 3.2 bis
+ **Big changes**:
	- Reimplemented delta time (lost in merging).
+ Changes for `Animal`:
	- Changed the type of `size` and `speed` attributes to `double`.
	- Added maximum and minimum values for `health`, `size` and `speed`.
	- Improved the logic for `isAtDest()`.
	- Increased speeds for dogs and cats.
+ Other changes:
	- Respected [rule of zero](https://en.cppreference.com/w/cpp/language/rule_of_three.html#Rule_of_zero) and [rule of three](https://en.cppreference.com/w/cpp/language/rule_of_three.html#Rule_of_three).
	- `waitNextFrame()` now returns nothing and modify the `deltaTime` global variable.
	- Added `==` and `!=` operators to `pos_t`.
	- Changed `DESIRED_FPS` to be a macro.