#include "../include/Animal.hpp"
struct stat sb;

/**
 * This constructor is only for path we are sur they exist. Directly set `spritePath` to `path` without any test.
 */
[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, std::string path) 
	: pos(_pos), size(_size), spritePath(path), dest(_pos)
{}


/**
 * Set the current sprite at a random sprite
 */
void Animal::setToRandomSprite(void) noexcept(false){
	const auto mask = [](const fs::path& path){
		const std::string pathStr = path.string();

		return fs::is_regular_file(path) &&
				pathStr
					.substr(pathStr.length() - 10, pathStr.length() - 6)
					.find("other")
			!= pathStr.npos;
	};
	
	spritePath = Animal::getRandomPathFromMask(mask).string();
}

/**
 * Return a path to random sprite corresponding to the mask passed.
 * @param mask A function pointer pointing to a function taking a path in argument and returning `true` if this argument is a path to include in the search.
 */
[[ nodiscard ]] fs::path Animal::getRandomPathFromMask(bool (*mask)(const fs::path&)) noexcept(false){
	uint8_t limit = randInt(
		1,
		std::min(
			(size_t)UINT8_MAX,	//In case there's more than 256 files
			howManyFiles(
				Animal::getSpriteFolder(),
				mask
			)
		)
	);
	if(limit == 0)
		throw std::runtime_error("There are no files in `" + Animal::getSpriteFolder() + "`!");
	limit--;

	const auto folderIt = fs::directory_iterator(Animal::getSpriteFolder());
	
	uint8_t found(0);			//The number of correct matches found
	for(const auto& file : folderIt)
		if(mask(file.path()) && found >= limit){
			return file.path();
		}else if(mask(file.path())){
			found++;
		}
	
	//if `limit` > nb correct files
	throw std::logic_error("The limit = " + std::to_string(limit) +" is aboves the number of regular files =  in Animal::getRandomPathFromMask().");
}

/**
 * Set the `sprite` field, return `false` on failure.
 */
[[ nodiscard ]] bool Animal::setSprite(uint8_t spriteNum){
	const std::string sprite(Animal::getSpriteBase() + std::to_string(spriteNum) + ".bmp");

	if (stat(sprite.c_str(), &sb) == 0){
		spritePath = sprite;
		return true;
	}
	
	return false;
}

/**
 * Build the Animal with a random sprite
 */
[[ nodiscard ]] Animal::Animal(Pos _pos) noexcept(false)
	: pos(_pos), dest(_pos), size(0)
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
	: pos(p), dest(p), size(_size)
{
	Animal::setToRandomSprite();
}


[[ nodiscard ]] Animal::Animal(Pos _pos, uint _size, uint8_t spriteNum=0) noexcept(false)
	: pos(_pos), dest(_pos), size(_size)
{
	if(spriteNum == 0){
		setToRandomSprite();
		return;
	}
	if(!setSprite(spriteNum))
		throw std::invalid_argument("Couldn't set the sprite.\nMaybe `"+ Animal::getSpriteFolder() + std::to_string(spriteNum) + ".bmp` is not a correct path?");
	
}

/**
 * Setter for `size`
 */
void Animal::increaseSize(uint by){
	if(size + by < size || size + by < by)		//interger overflow
		size = UINT32_MAX;

	size += by;
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

void move(void) {
	
}

/**
 * Getter for `size`
 */
[[ nodiscard ]] uint Animal::getSize(void) const{
	return size;
}


/**
 * Display the Animal on screen
 */
void Animal::display(SDL_Renderer* r) const noexcept(false){
	if(size <= 0)
		return;

	SDL_Surface* tmp = SDL_LoadBMP(&spritePath[0]);
	if(!tmp)
		throw std::runtime_error("Cannot load `" + std::string(spritePath) + "`, got this SDL error: " + std::string(SDL_GetError()) + ".");
	
	SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(r, tmp);
	SDL_FreeSurface(tmp);		tmp = nullptr;

	if(SDL_RenderCopy(r, spriteTexture, nullptr, SDL_RectInit(pos.x, pos.y, size, size)) < 0)
		throw std::runtime_error("Cannot copy sprite into the renderer. SDL returned this error: " + std::string(SDL_GetError()));
	SDL_DestroyTexture(spriteTexture);
}

/**
 * Returns a human-readable string of the instance
 */
[[ nodiscard ]] std::string Animal::string(void) const{
	return "Animal{ .pos=" + pos.string() + "; .spritePath=\"" + spritePath + "\"}";
}

/**
 * Returns the folder where the sprites are located
 */
[[ nodiscard ]] std::string Animal::getSpriteFolder(void){
	return "sprites/";
}

/**
 * Returns the base in order to make the sprite path.
 */
[[ nodiscard ]] std::string Animal::getSpriteBase(void){
	return Animal::getSpriteFolder() + "other";
}