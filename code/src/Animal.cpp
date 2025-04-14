#include "../include/Animal.hpp"
struct stat sb;

/** The folder where the sprites are located. */
const std::string Animal::spriteFolder = "sprites/";

/** The base in order to make the full sprite path. */
const std::string Animal::spriteBase = Animal::spriteFolder + "other";


/**
 * This constructor is only for path we are sur they exist. Directly set `spritePath` to `path` without any test.
 */
[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint _speed, std::string path) 
	: pos(_pos), dest(_pos), size(_size), speed(_speed), spritePath(path)
{}

/**
 * Set the current sprite at a random sprite.
 */
void Animal::setToRandomSprite(void) noexcept(false) {
	const auto mask = [](const fs::path& path){
		const std::string pathStr = path.string().replace(0, 8, "");	// Removing the leading "sprites/"
		return fs::is_regular_file(path) && pathStr.starts_with("other") && pathStr.ends_with(".bmp");
	};
	
	spritePath = Animal::getRandomPathFromMask(mask).string();
}

/**
 * Return a path to random sprite corresponding to the mask passed.
 * @param mask A function pointer pointing to a function taking a path in argument and returning `true` if this argument is a path to include in the search.
 */
[[ nodiscard ]] fs::path Animal::getRandomPathFromMask(mask_t mask) noexcept(false) {
	uint8_t limit = randInt(
		1,
		std::min(
			(size_t)UINT8_MAX,	//In case there's more than 256 files
			howManyFiles(
				spriteFolder,
				mask
			)
		)
	);
	if(limit == 0)
		throw std::runtime_error("There are no files matching the mask in `" + Animal::spriteFolder + "`!");
	limit--;

	const auto folderIt = fs::directory_iterator(Animal::spriteFolder);
	
	uint8_t found(0);			//The number of correct matches found
	for(const auto& file : folderIt)
		if(mask(file.path())) {
			if(found >= limit)
				return file.path();
		
			found++;
		}

	//if `limit` > nb correct files
	throw std::logic_error("The limit = " + std::to_string(limit) +" is aboves the number of regular files in Animal::getRandomPathFromMask().");
}

/**
 * Get the vector representing how much `this` animal will move this frame.
 */
[[ nodiscard ]] Vector Animal::getSpeedVector(void) const {
	return Vector::fromPoints(
		(SDL_FPoint)pos,
		(SDL_FPoint)dest
	).withNorm(speed);
}

/**
 * Set the `sprite` field, return `false` on failure.
 */
[[ nodiscard ]] bool Animal::setSprite(uint8_t spriteNum) {
	const std::string sprite(Animal::spriteBase + ((spriteNum < 10)? "0" : "") + std::to_string(spriteNum) + ".bmp");

	if (stat(sprite.c_str(), &sb) == 0) {
		spritePath = sprite;
		return true;
	}
	
	return false;
}

/**
 * Build the Animal with a random sprite
 */
[[ nodiscard ]] Animal::Animal(Pos _pos) noexcept(false)
	: pos(_pos), dest(_pos), size(0), speed(0)
{
	Animal::setToRandomSprite();
}

/**
 * Shorthand for `Animal(Pos(x, y))`
 */
[[ nodiscard ]] Animal::Animal(pos_t x, pos_t y)
	: Animal(Pos(x, y))
{}

[[ nodiscard ]] Animal::Animal(Pos p, uint _size) noexcept(false)
	: pos(p), dest(p), size(_size), speed(0)
{
	Animal::setToRandomSprite();
}

[[ nodiscard ]] Animal::Animal(Pos p, uint _size, uint velocity) noexcept(false)
	: pos(p), dest(p), size(_size), speed(velocity)
{
	Animal::setToRandomSprite();
}


[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint velocity, uint8_t spriteNum=0) noexcept(false)
	: pos(_pos), dest(_pos), size(_size), speed(velocity)
{
	if(spriteNum == 0) {
		setToRandomSprite();
		return;
	}
	if(!setSprite(spriteNum))
		throw std::invalid_argument("Couldn't set the sprite.\nMaybe `"+ Animal::spriteFolder + std::to_string(spriteNum) + ".bmp` is not a correct path?");
	
}

/**
 * Increase the size of by `by` pixels.
 */
void Animal::increaseSize(uint by) {
	if(size + by < size || size + by < by)		//interger overflow
		size = UINT32_MAX;

	size += by;
}

/**
 * Increase the speed of by `by` pixels per frame.
 */
void Animal::increaseSpeed(uint by) {
	if(speed + by < speed || speed + by < by)		//interger overflow
		speed = UINT32_MAX;

	speed += by;
}

/**
 * Check if the animal is at destination.
 */
bool Animal::isAtDest(void) const {
	const float threshold = speed + 0.01;	//make sure the animal can't jump behing the destination's hitbox.
	return 
		dest.x - threshold < pos.x && pos.x < dest.x + threshold &&
		dest.y - threshold < pos.y && pos.y < dest.y + threshold;
}

/**
 * Set the destination to a random point on the screen
 */
void Animal::setRandDest(void) {
	dest = Pos(
		randInt(0, WIN_WIDTH),
		randInt(0, WIN_HEIGHT)
	);
}

/**
 * Moves the animal toward its destination and defines a new one if the animal is at destination.
 */
void Animal::move(void) {
	if(isAtDest())
		setRandDest();
	pos = getSpeedVector().translate(pos);
}

/**
 * Moves the animal toward its destination.
 */
void Animal::moveToDest(void) {
	pos = getSpeedVector().translate(pos);
}

/**
 * Getter for `size`
 */
[[ nodiscard ]] uint Animal::getSize(void) const {
	return size;
}


/**
 * Display the Animal on screen
 */
void Animal::draw(SDL_Renderer* r, bool drawInfos /*=false*/) const noexcept(false) {
	if(size == 0)
		return;

	SDL_Surface* tmp = SDL_LoadBMP(&spritePath[0]);
	if(!tmp)
		throw std::runtime_error("Cannot load `" + std::string(spritePath) + "`, got this SDL error: " + std::string(SDL_GetError()) + ".");
	
	SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(r, tmp);
	SDL_FreeSurface(tmp);		tmp = nullptr;

	const SDL_Rect* finalDimensions = SDL_RectInit(pos.x - ANIMAL_SPRITE_SIZE/2, pos.y - ANIMAL_SPRITE_SIZE/2, size, size);	//Shifting the position to get the center of the rectangle at `pos`
	if(SDL_RenderCopy(r, spriteTexture, nullptr, finalDimensions) < 0)
		throw std::runtime_error("Cannot copy sprite into the renderer. SDL returned this error: " + std::string(SDL_GetError()) + ".");
	
	SDL_DestroyTexture(spriteTexture);
	
	if(drawInfos) {
		Uint8* alpha	= new Uint8(0);
		Uint8* red		= new Uint8(0);
		Uint8* green	= new Uint8(0);
		Uint8* blue		= new Uint8(0);

		SDL_GetRenderDrawColor(r, red, green, blue, alpha);
		SDL_SetRenderDrawColor(r, 0, 255, 255, 255);

		(getSpeedVector() * DESIRED_FPS/2).draw(r, (Vector)pos );	//This speed vector is for the next half-second
		dest.draw(r);

		SDL_SetRenderDrawColor(r, *red, *green, *blue, *alpha);
		
	}
}

/**
 * Returns a human-readable string of the instance
 */
[[ nodiscard ]] std::string Animal::string(void) const {
	return "Animal{ .pos=" + pos.string() + "; .spritePath=\"" + spritePath + "\"}";
}
