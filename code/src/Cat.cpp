#include "../include/Cat.hpp"

Cat* Cat::catList[CATLIST_SIZE];	//by default, all elements are initalized to 0

/** The base to make spritePath. */
const std::string Cat::spriteBase = spriteFolder + "cat";

/**
 * Set the `sprite` field, return `false on failure`.
 */
[[ nodiscard ]] bool Cat::setSprite(uint8_t spriteNum) {
	const std::string sprite(Cat::spriteBase + std::string((spriteNum < 10)? "0" : "") + std::to_string(spriteNum) + std::string(".bmp"));
	struct stat sb;

	if (stat(sprite.c_str(), &sb) == 0) {
		spritePath = sprite;
		return true;
	}
	
	return false;
}

/**
 * Set the current sprite at a random sprite
 */
void Cat::setToRandomSprite(void) noexcept(false) {
	const auto mask = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");

		return fs::is_regular_file(path) && pathStr.starts_with("cat") && pathStr.ends_with(".bmp");
	};
	
	spritePath = getRandomPathFromMask(mask).string();
}


/**
 * Just a wrapper for constructors
 */
void Cat::trySetLowestID(void) {
	if(std::optional<ID> supposedID = getLowestID()) {
		catList[id] = this;
		id = supposedID.value();
		
		return;
	}

	wout << "Too many cats are already present." << std::endl;
	delete this;
}

/**
 * Delete `cat` from `catList`
 */
void Cat::eraseCat(Cat* cat) {
	if(cat) {		//Avoiding a double-free error
		catList[cat->getID()] = nullptr;
		delete cat;
	}
}

/**
 * Returns the lowest ID available. If there is more than 255 cats, returns null
 */
[[ nodiscard ]] std::optional<ID> Cat::getLowestID(void) {
	for(ID i = 0; i < CATLIST_SIZE; i++)
		if(catList[i] == nullptr)
			return i;
	
	return {};	
}

/**
 * Return a path to random sprite corresponding to the mask passed.
 * @param mask A function pointer pointing to a function taking a path in argument and returning `true` if this argument is a path to include in the search.
 */
[[ nodiscard ]] fs::path Cat::getRandomPathFromMask(mask_t mask) noexcept(false) {
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
 * Construct a new Cat obj with a random sprite and registers it in catList
 */
[[ nodiscard ]] Cat::Cat(Pos _pos) noexcept(false)
	: Cat(_pos, 0)
{}

/**
 * Construct a new Cat obj with a random sprite and registers it in catList
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size) noexcept(false)
	: Animal(_pos, _size), alive(true)
{
	trySetLowestID();
	setToRandomSprite();
}

/**
 * Shorthand for `Cat(Pos(x, y))`
 */
[[ nodiscard ]] Cat::Cat(pos_t x, pos_t y) noexcept(false)
	: Cat(Pos(x, y)) 
{}

/**
 * Construct a new Cat obj and registers it in `catList`
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size, uint8_t spriteNum) noexcept(false)
	: Animal(_pos, _size, spriteNum), alive(true)
{
	trySetLowestID();
	if(!setSprite(spriteNum))
		throw std::runtime_error("Couldn't set the sprite of a Cat instance.");
}

/**
 * Construct a new Cat obj and register it in `catList`.
 */
[[ nodiscard ]] Cat::Cat(Pos _pos, uint _size, uint velocity, uint8_t spriteNum) noexcept(false)
	: Animal(_pos, _size, spriteNum), alive(true)
{
	trySetLowestID();
	if(!setSprite(spriteNum))
		throw std::runtime_error("Couldn't set the sprite");
}

/**
 * Copy constructor, the copy will have a new id
 */
[[ nodiscard ]] Cat::Cat(const Cat& cat) noexcept(false)
	: Animal(cat.pos, cat.size, cat.speed, cat.spritePath), alive(cat.alive)
{
	trySetLowestID();
}

Cat::~Cat(void) {
	catList[id] = nullptr;
}

/**
 * Getter for `id`
 */
[[ nodiscard ]] ID Cat::getID(void) const {
	return id;
}

/**
 * Getter for `speed`
 */
[[ nodiscard ]] uint Cat::getSpeed(void) const {
	return speed;
}

/**
 * Returns a human-readable string representing `this` Cat
 */
std::string Cat::string(void) const {
	return "Cat{ .id="+ std::to_string(id) +"; .alive=" + std::to_string(alive) + "; "+ Animal::string() +" }";
}
