# Changelog
This file simply list the progression of patches in this minor version.

## 0.3.0:
+ **big changes**:
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

## 0.3.0 bis
+ The speed vector is now calculated with delta time.
+ Corrected the calculations of delta.
+ Added new global variable `deltaTime`.
+ Renamed `speed` member on animals to `speedPerSecond`.