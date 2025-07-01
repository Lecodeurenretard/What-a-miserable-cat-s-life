#include "../include/Cat.hpp"

/**
 * Set the `sprite` field, return `false` on failure.
 */
[[ nodiscard ]] bool Cat::setSprite(uint8_t spriteNum) noexcept {
	const std::string spriteAlive(Cat::spriteBase		+ std::string((spriteNum		< 10)? "0" : "") + std::to_string(spriteNum) + std::string(".bmp"));

	const uint spriteDeadNum(spriteNum%deadCatCount);
	const std::string spriteDead (Cat::spriteDeadBase	+ std::string((spriteDeadNum	< 10)? "0" : "") + std::to_string(spriteDeadNum) + std::string(".bmp"));

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
 * Set the current sprite at a random sprite
 * @throw `fs::is_regular_file()` errors are assumed to not occur.
 */
void Cat::setToRandomSprite(void) noexcept {
	const auto maskAlive = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");

		return fs::is_regular_file(path) && pathStr.starts_with("cat") && pathStr.ends_with(".bmp");
	};
	const auto maskDead = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");

		return fs::is_regular_file(path) && pathStr.starts_with("deadCat") && pathStr.ends_with(".bmp");
	};
	
	spritePath = getRandomPathFromMask(maskAlive, Cat::spriteFolder);
	spritePathDead = getRandomPathFromMask(maskDead, Cat::spriteFolder);
}

/**
 * Set the ID of the cat to the lowest possible or `CATLIST_SIZE` if there is no IDs left.
 */
void Cat::setLowestID(void) noexcept {
	id = getLowestID();
}

/**
 * Check if this cat is listed.
 */
[[ nodiscard ]] bool Cat::canBeListed(void) const noexcept {
	return id < CATLIST_SIZE;
}

/**
 * Check if this instance collides with a dog from `dogList`.
 */
[[ nodiscard ]] bool Cat::isHitByDog(void) const noexcept {
	for(size_t i = 0; i < Dog::getLowestIndex(); i++)		//Avoid iterating through all `dogList` and getting bad values
		if(hitbox.isOverlapping(Dog::dogList[i]->getHitbox()))
			return true;
	return false;
}

/**
 * Returns the lowest ID available. If there is more than `CATLIST_SIZE` cats, returns `CATLIST_SIZE`.
 */
[[ nodiscard ]] ID Cat::getLowestID(void) noexcept {
	for(ID i = 0; i < CATLIST_SIZE; i++)
		if(catList[i].get() == nullptr)
			return i;
	
	return CATLIST_SIZE;
}

/**
 * Draw the health of the cat.  
 * This method is inherited from Animal, its goal is to let children classes draw elements with respect to their own members and methods.
 * @throw `std::round()` may throw. If fail to render health to surface, throw an `std::runtime_error`.
 */
void Cat::drawSpecificities(SDL_Renderer* r, TTF_Font* font/*=nullptr*/) const {
	const uint8_t digitInHP = (health >= 100) + (health >= 10) + 1;	//how many digits there are in health (health can't go over 255 so we only need to test for those)
	const int fontWidth(digitInHP * 10);

	const auto fontRect = SDL_Rect{
		.x = (int)std::round(hitbox.zone.x + hitbox.zone.w/2 - fontWidth/2),
		.y = (int)std::round(hitbox.zone.y + hitbox.zone.h),
		.w = fontWidth,
		.h = 20
	};

	const bool useDefFont(font == nullptr);		//if the default font should be used
	if(useDefFont)
		font = ANIMAL_ALLOCATE_DEFAULT_FONT(35);

	SDL_Surface* surf = TTF_RenderText_Solid(font, std::to_string(health).c_str(), COL_WHITE);
	if(surf == nullptr)
		throw std::runtime_error("Couldn't render font to the screen.\nLast SDL_ttf error: " + std::string(TTF_GetError()));

	if(
		SDL_RenderCopy(r, SDL_CreateTextureFromSurface(r, surf), NULL, &fontRect) < 0
	) {
		wout << "Couldn't draw the HP of the object " << string() << ".\n"
			<< "Last SDL error: " << SDL_GetError() << '\n'
			<< "Last SDL_ttf error: " << TTF_GetError() << '\n' << std::endl;
	}

	SDL_FreeSurface(surf);
	if(useDefFont)
		TTF_CloseFont(font);
}

/**
 * Create an UNLISTED instance with all values to default.
 */
[[ nodiscard ]] Cat::Cat(void) noexcept
	: Animal(Pos::ORIGIN)
{}

/**
 * Shorthand for `Cat(_pos, 0)`.
 */
[[ nodiscard ]] Cat::Cat(Pos _pos) noexcept
	: Cat(_pos, 0)
{}

/**
 * Construct a new Cat obj with a random sprite and registers it in catList
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, double _size) noexcept
	: Animal(_pos, _size)
{
	setToRandomSprite();
}

/**
 * Shorthand for `Cat(Pos(x, y), 0)`
 */
[[ nodiscard ]] Cat::Cat(pos_t x, pos_t y) noexcept
	: Cat(Pos(x, y), 0) 
{}

/**
 * Shorthand for `Cat(_pos, _size, 0, spriteNum)`
 * @throw Throw an `std::runtime_error` if failed to set the sprite.
 */

[[ nodiscard ]] Cat::Cat(Pos _pos, double _size, uint8_t spriteNum)
	: Cat(_pos, _size, 0, spriteNum)
{}

/**
 * Construct a new Cat obj and register it in `catList`.
 * @throw Throw an `std::runtime_error` if failed to set the sprite.
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, double _size, double velocity, uint8_t spriteNum)
	: Animal(_pos, _size, velocity, spriteNum)
{
	if(spriteNum == 0) {
		setToRandomSprite();
		return;
	}
	if(!setSprite(spriteNum))
		throw std::runtime_error("Couldn't set the sprite number "+ std::to_string(spriteNum) +" in constructor of Cat.");
}

/**
 * Create a cat that is not listed in catList.
 * @throw Throw an `std::runtime_error` if failed to set the sprite.
 */
[[ nodiscard ]] Cat Cat::createUnlisted(Pos _pos/*=Pos::ORIGIN*/, double _size/*=0*/, double velocity/*=0*/, uint8_t spriteNum/*=0*/) {
	return Cat(_pos, _size, velocity, spriteNum);
}

/**
 * Copy this instance and register the copy into `catList` if there is space left.
 */
[[ nodiscard ]] Cat Cat::copy(void) const noexcept {
	Cat copy;
	copy.setLowestID();

	if(!copy.canBeListed()) {
		wout << "There is no more space to create another listed Cat.";
		
		copy = *this;
		copy.id = CATLIST_SIZE;
		return copy;
	}
	
	std::unique_ptr<Cat>& copyPtr = catList[copy.id];	//alias
	
	copyPtr = std::make_unique<Cat>(*this);
	copyPtr->id = copy.id;
	
	return *copyPtr;
}


/**
 * Getter for `id`
 */
[[ nodiscard ]] ID Cat::getID(void) const noexcept {
	return id;
}

/**
 * Display the Cat on screen.
 * @param r The renderer to draw onto.
 * @param font The font to use when drawing text/numbers, set to `nullptr` if you want to use the default one.
 * @param canDrawInfos If the method should also draw the speed vector, the destination and the hitbox.
 * @throw `runtime_error`s can be thrown by `drawSprite()` and `drawSpecifities().`
 */
void Cat::draw(SDL_Renderer* r, TTF_Font* font /*=nullptr*/, bool canDrawInfos /*=false*/) const {
	if(size == 0)
		return;

	drawSprite(r);
	drawSpecificities(r, font);

	if(canDrawInfos && !isDead())
		drawInfos(r, collisionLastFrame);
}

/**
 * Returns a human-readable string representing `this` Cat
 */
std::string Cat::string(void) const noexcept {
	return "Cat{ .id="+ std::to_string(id) +"; "+ Animal::string() +" }";
}

/**
 * Check hits took and handle health.
 */
void Cat::handleCollisions(void) noexcept {
	if(isDead())
		return;

	if(!isHitByDog()) {
		collisionLastFrame = false;
		return;
	}
	if(collisionLastFrame)
		return;

	health--;
	collisionLastFrame = true;
}

/**
 * Generates `howMany` cats.
 * @param howMany How many cats to produce, if there is too much cats in `catList` the remaining cats will discarded.
 * @param IDs A vector that is modified to contain the IDs of generated cats (`CATLIST_SIZE` for unlisted cat).
 * @throw The Cat constructor may throw an exception.
 */
void Cat::generateCats(uint8_t howMany, std::vector<ID>* IDs /*= nullptr*/, Pos pos/*=Pos::ORIGIN*/, double size/*=0*/, double speed/*=0*/, uint8_t spriteNum/*=0*/) {
	if(IDs)
		IDs->reserve(howMany);
	for (double i = 0; i < howMany; i++) {
		const Pos position = static_cast<Vector>(pos) + Vector{.x = size*i, .y=0};	//shift the cats to they don't overlap

		std::unique_ptr<Cat> generated = std::make_unique<Cat>(position, size, speed, spriteNum);
		generated->setLowestID();
		
		if(!generated->canBeListed())
			continue;
		
		Cat::catList[generated->id].reset(generated.get());	//Reset the pointer to the generated Cat
		if(IDs)
			IDs->push_back(generated->id);
		
		generated.release();
	}
}

/**
 * Free all listed cats
 */
void Cat::clearCatList(void) noexcept {
	//I don't think std::for_each() lets us call methods
	for(auto& catPtr : catList)
		catPtr.reset();
}