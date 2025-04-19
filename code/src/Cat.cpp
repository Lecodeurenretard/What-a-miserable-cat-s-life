#include "../include/Cat.hpp"

/**
 * Set the `sprite` field, return `false` on failure.
 */
[[ nodiscard ]] bool Cat::setSprite(uint8_t spriteNum) {
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
 */
void Cat::setToRandomSprite(void) noexcept(false) {
	const auto maskAlive = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");

		return fs::is_regular_file(path) && pathStr.starts_with("cat") && pathStr.ends_with(".bmp");
	};
	const auto maskDead = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");

		return fs::is_regular_file(path) && pathStr.starts_with("deadCat") && pathStr.ends_with(".bmp");
	};
	
	spritePath = getRandomPathFromMask(maskAlive).string();
	spritePathDead = getRandomPathFromMask(maskDead).string();
}


/**
 * Set `id` to the smallest available, return if it was a success.
 */
[[ nodiscard ]] bool Cat::trySetLowestID(void) {
	id = getLowestID();
	
	if(id >= CATLIST_SIZE)
		return false;
	
	catList[id] = this;
	return true;
}

/**
 * Check if this instance collides with a dog from `dogList`.
 */
[[ nodiscard ]] bool Cat::isHitByDog(void) const {
	for(size_t i = 0; i < Dog::getLowestID(); i++)		//Avoid iterating through all `dogList` and getting bad values
		if(hitbox.isOverlapping(Dog::dogList[i]->getHitbox()))
			return true;
	return false;
}

/**
 * Returns the lowest ID available. If there is more than `CATLIST_SIZE` cats, returns `CATLIST_SIZE`.
 */
[[ nodiscard ]] ID Cat::getLowestID(void) {
	for(ID i = 0; i < CATLIST_SIZE; i++)
		if(catList[i] == nullptr)
			return i;
	
	return CATLIST_SIZE;
}

/**
 * Return a path to random sprite corresponding to the mask passed.
 * @param mask A function pointer pointing to a function taking a path in argument and returning `true` if this argument is a path to include in the search.
 */
[[ nodiscard ]] fs::path Cat::getRandomPathFromMask(mask_t mask) noexcept(false) {
	uint8_t limit = randInt(
		1,
		std::min(
			(size_t)UINT8_MAX,	//In case there's more than 255 files
			howManyFiles(
				spriteFolder,
				mask
			)
		)
	);
	if(limit == 0)
		throw std::runtime_error("There are no files in `" + Cat::spriteFolder + "`!");
	limit--;

	const auto folderIt = fs::directory_iterator(spriteFolder);
	
	uint8_t found(0);			//The number of correct matches found
	for(const auto& file : folderIt)
		if(mask(file.path()) && found >= limit) {
			return file.path();
		}else if(mask(file.path())) {
			found++;
		}
	
	//if `limit` > nb correct files
	throw std::logic_error("The limit = " + std::to_string(limit) +" is aboves the number of regular files =  in Animal::getRandomPathFromMask().");
}

/**
 * Draw the health of the cat.  
 * This method is inherited from Animal, its goal is to let children classes draw elements with respect to their own members and methods.
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
		font = ANIMAL_DEFAULT_FONT(35);
	SDL_Surface* surf = TTF_RenderText_Solid(font, std::to_string(health).c_str(), COL_WHITE);

	if(
		SDL_RenderCopy(
			r,
			SDL_CreateTextureFromSurface(r, surf),
			NULL,
			&fontRect
		) < 0
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
[[ nodiscard ]] Cat::Cat(void) noexcept(false)
	: Animal(Pos::ORIGIN)
{}

/**
 * Construct a new Cat obj with a random sprite and registers it in catList
 */
[[ nodiscard ]] Cat::Cat(Pos _pos) noexcept(false)
	: Cat(_pos, 0)
{}

/**
 * Construct a new Cat obj with a random sprite and registers it in catList
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size) noexcept(false)
	: Animal(_pos, _size)
{
	setToRandomSprite();
	if(!trySetLowestID())
		wout << "Too many cats are already present.";
}

/**
 * Shorthand for `Cat(Pos(x, y))`
 */
[[ nodiscard ]] Cat::Cat(pos_t x, pos_t y) noexcept(false)
	: Cat(Pos(x, y), 0) 
{}

/**
 * Construct a new Cat obj and registers it in `catList`
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size, uint8_t spriteNum) noexcept(false)
	: Cat(_pos, _size, 0, spriteNum)
{}

/**
 * Construct a new Cat obj and register it in `catList`.
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size, uint velocity, uint8_t spriteNum) noexcept(false)
	: Animal(_pos, _size, velocity, spriteNum)
{
	if(!trySetLowestID())
		wout << "Too many cats are already present." << std::endl;

	if(spriteNum == 0) {
		setToRandomSprite();
		return;
	}
	if(!setSprite(spriteNum))
		throw std::runtime_error("Couldn't set the sprite number "+ std::to_string(spriteNum) +" in constructor of Cat.");
}

Cat::~Cat(void) {
	if(id < CATLIST_SIZE)
		catList[id] = nullptr;
}

/**
 * Create a cat that is not listed in catList.
 */
[[ nodiscard ]] Cat Cat::createUnlisted(Pos _pos/*=Pos::ORIGIN*/, uint _size/*=0*/, uint velocity/*=0*/, uint8_t spriteNum/*=0*/) {
	Cat res;
	res.pos = _pos;
	res.size = _size;
	res.speed = velocity;
	
	if(!res.setSprite(spriteNum))
		throw std::runtime_error("Couldn't set the sprite of a new cat instance.");

	return res;
}

/**
 * Copies this instance and register the copy into the catList. The pointer is created with `new`.
 */
[[ nodiscard ]] Cat* Cat::copy(void) const {
	Cat* copy = new Cat(*this);
	
	if(!copy->trySetLowestID())
		wout << "There is no more space to create another listed Cat.";
	return copy;
}


/**
 * Getter for `id`
 */
[[ nodiscard ]] ID Cat::getID(void) const {
	return id;
}

/**
 * Display the Cat on screen.
 * @param r The renderer to draw onto.
 * @param font The font to use when drawing text/numbers, set to `nullptr` if you want to use the default one.
 * @param canDrawInfos If the method should also draw the speed vector, the destination and the hitbox.
 */
void Cat::draw(SDL_Renderer* r, TTF_Font* font /*=nullptr*/, bool canDrawInfos /*=false*/) const noexcept(false) {
	if(size == 0)
		return;

	drawSprite(r);
	drawSpecificities(r, font);

	if(canDrawInfos && !isDead())
		drawInfos(r);
}

/**
 * Returns a human-readable string representing `this` Cat
 */
std::string Cat::string(void) const {
	return "Cat{ .id="+ std::to_string(id) +"; "+ Animal::string() +" }";
}

/**
 * Check hits took and handle health.
 */
void Cat::handleCollisions(void) {
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
 * Generates `howMany` cats, their IDs are returned by the parameter `ids`.
 */
void Cat::generateCats(uint8_t howMany, ID (*ids)[] /*= nullptr*/, Pos pos/*=Pos::ORIGIN*/, uint size/*=0*/, uint speed/*=0*/, uint8_t spriteNum/*=0*/) {
	for (uint8_t i = 0; i < howMany; i++) {
		const Vector position = (Vector)pos + Vector{.x = (float)size*i, .y=0};	//shift the cats to the don't overlap each other
		Cat* generated = new Cat(position, size, speed, spriteNum);

		if(ids != nullptr)
			(*ids)[i] = generated->id;
	}
}