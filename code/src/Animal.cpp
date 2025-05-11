#include "../include/Animal.hpp"
struct stat sb;

/**
 * This constructor is only for path we are sure the sprite exists. Directly set `spritePath` to `path` without any test.
 */
[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint _speed, std::string path) noexcept
	: pos(_pos), dest(_pos), size(_size), speed(_speed), spritePath(path)
{}

/**
 * Set `sprite` to a random sprite.
 * @throw `fs::is_regular_file()` errors are assumed to not occur. Else errors come from `getRandomPathFromMask`
 */
void Animal::setToRandomSprite(void) {
	const auto mask = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");	// Removing the leading "sprites/"
		return fs::is_regular_file(path) && pathStr.starts_with("other") && pathStr.ends_with(".bmp");
	};
	
	spritePath = Animal::getRandomPathFromMask(mask).string();
}


/**
 * Set the `sprite` field, return `false` on failure.
 */
[[ nodiscard ]] bool Animal::setSprite(uint8_t spriteNum) noexcept {
	const std::string spriteAlive(spriteBase + std::string((spriteNum		< 10)? "0" : "") + std::to_string(spriteNum) + ".bmp");

	const uint spriteDeadNum(spriteNum%deadOtherCount);
	const std::string spriteDead (spriteBase + std::string((spriteDeadNum	< 10)? "0" : "") + std::to_string(spriteDeadNum) + ".bmp");

	struct stat sb;

	if (stat(spriteAlive.c_str(), &sb))
		return false;

	spritePath = spriteAlive;

	if (stat(spriteDead.c_str(), &sb))
		return false;

	spritePathDead = spriteDead;
	return true;
}

/**
 * Draw only the sprite of the Animal.
 * @throw Throw a `std::runtime_error` if faild to load the sprite, create a texture from it or copying the texture into the renderer.
 */
void Animal::drawSprite(SDL_Renderer* r) const {
	const std::string rightPath = isDead()? spritePathDead : spritePath;

	SDL_Surface* tmp = SDL_LoadBMP(rightPath.c_str());
	if(!tmp)
		throw std::runtime_error("Cannot load `" + std::string(rightPath) + "`, got this SDL error: " + std::string(SDL_GetError()) + ".");
	
	SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(r, tmp);
	SDL_FreeSurface(tmp);		tmp = nullptr;
	if(spriteTexture == nullptr)
		throw std::runtime_error("Failed to create a texture.\nSDL's last error: " + std::string(SDL_GetError()));

	const SDL_Rect finalDimensions = getSquare(
		(int)size,
		Pos (
			pos.x - size/2,		//Shifting the position of the square to get its center at `pos`
			pos.y - size/2
		)
	);
	if(SDL_RenderCopy(r, spriteTexture, nullptr, &finalDimensions) < 0)
		throw std::runtime_error("Cannot copy sprite at `"+ rightPath +"` into the renderer. SDL returned this error: " + std::string(SDL_GetError()) + ".");
	
	SDL_DestroyTexture(spriteTexture);
}

/**
 * Draw additionals infos such as:
 * - The speed vector
 * - The destination
 * - The hitbox
 */
void Animal::drawInfos(SDL_Renderer* render, bool isColliding/*=false*/) const noexcept {
	SDL_Color oldColor;

	SDL_GetRenderDrawColor(render, oldColor);
	SDL_SetRenderDrawColor(render, COL_WHITE);

	(getSpeedVector() * DESIRED_FPS/4).draw(render, pos);	//This speed vector is for the next half-second
	dest.draw(render);
	hitbox.draw(render, isColliding? HITBOX_COLOR_ACTIVE : HITBOX_COLOR_INACTIVE);

	SDL_SetRenderDrawColor(render, oldColor);
}

/**
 * Return a path to random sprite corresponding to the mask passed.
 * @param mask A function pointer pointing to a function taking a path in argument and returning `true` if this argument is a path to include in the search.
 * @throw Throw a `std::invalid_argument` if no file match the `mask()` or an `std::logic_error` if the chosen file has been miscomputed.  
 * Other errors come from `howManyFiles()`, `fs::directory_iterator`'s constructor, the `mask()`.
 */
[[ nodiscard ]] fs::path Animal::getRandomPathFromMask(mask_t mask, std::string folder/*=Animal::spriteFolder*/) {
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
		throw std::invalid_argument("There are no files matching the mask in `" + folder + "`!");
	limit--;

	const auto folderIt = fs::directory_iterator(folder);
	
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

[[ nodiscard ]] Animal::Animal(Pos _pos) noexcept
	: Animal(_pos, 0, 0)
{}

[[ nodiscard ]] Animal::Animal(pos_t x, pos_t y) noexcept
	: Animal(Pos(x, y))
{}

[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size) noexcept
	: Animal(_pos, _size, 0)
{}

[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint velocity) noexcept
	: pos(_pos), dest(_pos), size(_size), speed(velocity)
{
	Animal::setToRandomSprite();

	const Pos hitboxPos = (Vector)_pos - Vector{.x = (float)_size, .y = (float)_size}/2;
	hitbox = getSquare((float)size, hitboxPos);
}


[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint velocity, uint8_t spriteNum)
	: pos(_pos), dest(_pos), size(_size), speed(velocity)
{
	const Pos hitboxPos = (Vector)_pos - Vector{.x = (float)_size, .y = (float)_size	}/2;
	hitbox = getSquare((float)size, hitboxPos);

	if(spriteNum != 0 && !setSprite(spriteNum))
		throw std::invalid_argument("Couldn't set the sprite.\nMaybe `"+ Animal::spriteFolder + std::to_string(spriteNum) + ".bmp` is not a correct path?");
	
	setToRandomSprite();
}

/**
 * Increase the size of by `by` pixels.
 */
void Animal::increaseSize(uint by) noexcept {
	if(checkIntegerOverflow(size, by, &size))		//if not overflowing, the result of the addition is strored in size
		size = UINT32_MAX;
}

/**
 * Increase the speed of by `by` pixels per frame.
 */
void Animal::increaseSpeed(uint by) noexcept {
	if(checkIntegerOverflow(speed, by, &speed))
		speed = UINT32_MAX;
}

/**
 * Increment the health of the animal.
 */
void Animal::incrementHealth(void) noexcept {
	if(checkIntegerOverflow(speed, 1u, &speed)) 
		health = UINT8_MAX;
}

/**
 * Set the destination to a random point on the screen
 */
void Animal::setDestRand(void) noexcept {
	dest = Pos(
		randInt(size/2, WIN_WIDTH),
		randInt(size/2, WIN_HEIGHT)
	);
}

/**
 * Set the destnation to the mouse's
 */
void Animal::setDestMouse(void) noexcept {
	int m_x, m_y;
	SDL_GetMouseState(&m_x, &m_y);
	
	dest.x = m_x;
	dest.y = m_y;
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
 * Check if the animal is at destination.
 */
bool Animal::isAtDest(void) const noexcept {
	const float threshold = speed + 0.01;	//making sure the animal can't jump behind the destination's bounding box.
	return 
		dest.x - threshold < pos.x && pos.x < dest.x + threshold &&		//dest.x - threshold < pos.x < dest.x + threshold
		dest.y - threshold < pos.y && pos.y < dest.y + threshold;		//dest.y - threshold < pos.y < dest.y + threshold
}

/**
 * Move the animal toward its destination, define a new one if the animal is already there.
 * @throw Execeptions are from `moveToDest()`.
 */
void Animal::move(bool followMouse/*=false*/) {
	if(isDead())
		return;

	if(followMouse)
		setDestMouse();
	else if(isAtDest())
		setDestRand();

	moveToDest();
}

/**
 * Move the animal toward its destination regardless of if the destination is reached or if the cat is is alive of dead.
 * @throw Execeptions come from `getVectorSpeed()`.
 */
void Animal::moveToDest(void) {
	pos = getSpeedVector().translate(pos);
	
	//snap the hitbox to the animal's position
	hitbox.zone.x = pos.x - size/2;
	hitbox.zone.y = pos.y - size/2;
}

/**
 * Getter for `hitbox`.
 */
[[ nodiscard ]] Hitbox Animal::getHitbox(void) const noexcept {
	return hitbox;
}

/**
 * Return if the animal is dead (health at 0).
 */
[[ nodiscard ]] bool Animal::isDead(void) const noexcept {
	return health <= 0;
}

/**
 * Display the Animal on screen.
 * @param r The renderer to draw onto.
 * @param font The font to use when drawing text/numbers, set to `nullptr` if you want to use the default one.
 * @param canDrawInfos If the method should also draw the speed vector, the destination and the hitbox.
 * @throw Exceptions are from `drawSprite()` and `drawSpecificities()` or `isDead()` or `drawInfos()` if they're overridden.
 */
void Animal::draw(SDL_Renderer* r, TTF_Font* font /*=nullptr*/, bool canDrawInfos /*=false*/) const {
	if(size == 0)
		return;

	drawSprite(r);
	drawSpecificities(r, font);

	if(canDrawInfos && !isDead())
		drawInfos(r);
}

/**
 * Returns a human-readable string of the instance
 */
[[ nodiscard ]] std::string Animal::string(void) const noexcept {
	return "Animal{ .pos=" + pos.string() 
		+ "; dest="+ dest.string()
		+ "; size="+ std::to_string(size)
		+ "; speed="+ std::to_string(speed)
		+ "; health="+ std::to_string(health)
		+ "; .spritePath=\""+ spritePath
		+ "\"; .spritePathDead=\""+ spritePathDead
		+ "\" }";
}
