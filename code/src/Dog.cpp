#include "../include/Dog.hpp"

void Dog::setToRandomSprite(void) noexcept(false) {
	const auto mask = [](const fs::path& path) {
		const std::string pathStr = path.string().replace(0, 8, "");
		
		return fs::is_regular_file(path) && pathStr.starts_with("dog") && pathStr.ends_with(".bmp");
	};
	
	spritePath = getRandomPathFromMask(mask).string();
}

/**
 * Do nothing.
 * This method is inherited from Animal, its goal is to let children classes draw elements with respect to their own members and methods.
 */
void Dog::drawSpecificities(SDL_Renderer* r, TTF_Font* font/*=nullptr*/) const {
	//do nothing
}

[[ nodiscard ]] Dog::Dog(const Pos& p)
	: Animal(p, Dog::size, Dog::speed)
{
	setToRandomSprite();

	const uint8_t lowestID = Dog::getLowestID();
	if(lowestID >= DOGLIST_SIZE) {
		wout << "Too many dogs are already present, the one being constructed will still be allocated but will not be in `dogList` (no hit detection, no drawing, etc..)." << std::endl;
		
		index = DOGLIST_SIZE;	//error value
		return;
	}
	
	dogList[lowestID] = this;
	index = lowestID;
}

[[ nodiscard ]] Dog::Dog(const Dog& dog)
	: Dog(dog.pos)
{}

Dog::~Dog(void) {
	if(index < DOGLIST_SIZE)		//error value indicating that the dg wasn't 
		dogList[index] = nullptr;
}

/**
 * Generates `howMany` dogs, their IDs are returned by the parameter `indexes`.
 */
void Dog::generateDogs(uint8_t howMany, ID (*indexes)[] /*= nullptr*/, Pos pos/*=Pos::ORIGIN*/) {
	for (uint8_t i = 0; i < howMany; i++) {
		Dog* generated = new Dog(
			(Vector)pos + Vector{.x = (float)size*i, .y=0}//shift the dogs to the don't overlap each other
		);

		if(indexes != nullptr)
			(*indexes)[i] = generated->index;
	}
}

[[ nodiscard ]] std::string Dog::string(void) const {
	return "Dog{ index="+ std::to_string(index) +"; "+ Animal::string() +" }";
}

/**
 * Get the lowest ID available, return `DOGLIST_SIZE` if there isn't.
 */
ID Dog::getLowestID(void) {
	for(ID i = 0;  i < DOGLIST_SIZE; i++)
		if(dogList[i] == nullptr)
			return i;
	return DOGLIST_SIZE;
}