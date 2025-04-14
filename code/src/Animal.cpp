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


[[ nodiscard ]] Animal::Animal(Pos _pos) noexcept(false)
	: Animal(_pos, 0, 0)
{}

[[ nodiscard ]] Animal::Animal(pos_t x, pos_t y) noexcept(false)
	: Animal(Pos(x, y))
{}

[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size) noexcept(false)
	: Animal(_pos, _size, 0)
{}

[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint velocity) noexcept(false)
	: pos(_pos), dest(_pos), size(_size), speed(velocity)
{
	Animal::setToRandomSprite();

	const Pos hitboxPos = (Vector)_pos - Vector{.x = (float)_size, .y = (float)_size}/2;
	hitbox = getSquare((float)size, hitboxPos);
}


[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint velocity, uint8_t spriteNum) noexcept(false)
	: pos(_pos), dest(_pos), size(_size), speed(velocity)
{
	if(spriteNum == 0) {
		setToRandomSprite();
		return;
	}
	if(!setSprite(spriteNum))
		throw std::invalid_argument("Couldn't set the sprite.\nMaybe `"+ Animal::spriteFolder + std::to_string(spriteNum) + ".bmp` is not a correct path?");
	
	const Pos hitboxPos = (Vector)_pos - Vector{.x = (float)_size, .y = (float)_size	}/2;
	hitbox = getSquare((float)size, hitboxPos);
}

/**
 * Increase the size of by `by` pixels.
 */
void Animal::increaseSize(uint by) {
	if(size + by < size || size + by < by) {		//interger overflow
		size = UINT32_MAX;
		return;
	}

	size += by;
}

/**
 * Increase the speed of by `by` pixels per frame.
 */
void Animal::increaseSpeed(uint by) {
	if(speed + by < speed || speed + by < by){
		speed = UINT32_MAX;
		return;
	}

	speed += by;
}

/**
 * Check if the animal is at destination.
 */
bool Animal::isAtDest(void) const {
	const float threshold = speed + 0.01;	//making sure the animal can't jump behind the destination's bounding box.
	return 
		dest.x - threshold < pos.x && pos.x < dest.x + threshold &&		//dest.x - threshold < pos.x < dest.x + threshold
		dest.y - threshold < pos.y && pos.y < dest.y + threshold;		//dest.y - threshold < pos.y < dest.y + threshold
}

/**
 * Set the destination to a random point on the screen
 */
void Animal::setDestRand(void) {
	dest = Pos(
		randInt(size/2, WIN_WIDTH),
		randInt(size/2, WIN_HEIGHT)
	);
}

/**
 * Set the destnation to the mouse's
 */
void Animal::setDestMouse(void) {
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);
	
	dest.x = m_x;
	dest.y = m_y;
}

/**
 * Move the animal toward its destination, define a new one if the animal is already there.
 */
void Animal::move(bool followMouse/*=false*/) {
	if(followMouse)
		setDestMouse();
	else if(isAtDest())
		setDestRand();
	moveToDest();
}

/**
 * Move the animal toward its destination.
 */
void Animal::moveToDest(void) {
	pos = getSpeedVector().translate(pos);
	
	hitbox.zone.x = pos.x - size/2;
	hitbox.zone.y = pos.y - size/2;
}

/**
 * Getter for `size`
 */
[[ nodiscard ]] uint Animal::getSize(void) const {
	return size;
}

/**
 * Getter for `hitbox`
 */
[[ nodiscard ]] Hitbox Animal::getHitbox(void) const {
	return hitbox;
}

/**
 * Display the Animal on screen.
 * @param r the renderer to draw onto.
 * @param isColliding If the hitbox of this Animal is colling with another animal's
 * @param drawInfos If the method should also draw the speed vector, the destination and the hitbox.
 */
void Animal::draw(SDL_Renderer* r, bool isColliding, bool drawInfos /*=false*/) const noexcept(false) {
	if(size == 0)
		return;

	SDL_Surface* tmp = SDL_LoadBMP(spritePath.c_str());
	if(!tmp)
		throw std::runtime_error("Cannot load `" + std::string(spritePath) + "`, got this SDL error: " + std::string(SDL_GetError()) + ".");
	
	SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(r, tmp);
	SDL_FreeSurface(tmp);		tmp = nullptr;

	const SDL_Rect finalDimensions = getSquare(
		(int)size,
		Pos(
			pos.x - size/2,		//Shifting the position of the square to get its center at `pos`
			pos.y - size/2
		)
	);
	if(SDL_RenderCopy(r, spriteTexture, nullptr, &finalDimensions) < 0)
		throw std::runtime_error("Cannot copy sprite at `"+ spritePath +"` into the renderer. SDL returned this error: " + std::string(SDL_GetError()) + ".");
	
	SDL_DestroyTexture(spriteTexture);
	
	if(drawInfos) {
		Uint8* alpha	= new Uint8(0);
		Uint8* red		= new Uint8(0);
		Uint8* green	= new Uint8(0);
		Uint8* blue		= new Uint8(0);

		SDL_GetRenderDrawColor(r, red, green, blue, alpha);
		SDL_SetRenderDrawColor(r, 0, 255, 255, 255);

		(getSpeedVector() * DESIRED_FPS/4).draw(r, pos);	//This speed vector is for the next half-second
		dest.draw(r);
		hitbox.draw(r, isColliding? HITBOX_COLOR_ACTIVE : HITBOX_COLOR_INACTIVE);

		SDL_SetRenderDrawColor(r, *red, *green, *blue, *alpha);

		delete alpha;
		delete red;
		delete green;
		delete blue;
	}
}

/**
 * Returns a human-readable string of the instance
 */
[[ nodiscard ]] std::string Animal::string(void) const {
	return "Animal{ .pos=" + pos.string() + "; dest="+ dest.string() +"; size="+ std::to_string(size) +"; speed="+ std::to_string(speed) +"; .spritePath=\"" + spritePath + "\"}";
}
