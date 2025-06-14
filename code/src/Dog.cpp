#include "../include/Dog.hpp"

/**
 * Set a random sprite for the current dog.
 * @throw Exceptions are thrown by `getRandomPathFromMask()`.
 */
void Dog::setToRandomSprite(void) {
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
void Dog::drawSpecificities(SDL_Renderer* r, TTF_Font* font/*=nullptr*/) const noexcept {
	//do nothing
}

bool Dog::canBeListed(void) const noexcept {
	return index < DOGLIST_SIZE;
}

/**
 * @throw `setToRandomSprite()` may throw.
 */
[[ nodiscard ]] Dog::Dog(const Pos& p)
	: Animal(p, Dog::size, Dog::speed)
{
	setToRandomSprite();

	index = Dog::getLowestIndex();
	if(!canBeListed()) {
		wout << "Too many dogs are already present, the one being constructed will still be allocated but will not be in `dogList` (no hit detection, no drawing, etc..)." << std::endl;
		
		index = DOGLIST_SIZE;	//error value
	}
}

/**
 * Generates `howMany` dogs, their indexes are returned by the parameter `indexes`.
 * @throw The constructor may throw.
 */
void Dog::generateDogs(uint8_t howMany, std::vector<ID>* indexes /*= nullptr*/, Pos pos/*=Pos::ORIGIN*/) {
	if(indexes)
		indexes->reserve(howMany);
	for (double i = 0; i < howMany; i++) {
		const Pos position = static_cast<Vector>(pos) + Vector{.x = Dog::size * i, .y=0};	//shift the cats to they don't overlap

		std::unique_ptr<Dog> generated = std::make_unique<Dog>(position);
		generated->index = generated->getLowestIndex();
		
		if(!generated->canBeListed())
			continue;
		
		Dog::dogList[generated->index].reset(generated.get());
		if(indexes)
			indexes->push_back(generated->index);
		
		generated.release();
	}
}

/**
 * Free all dogs in `dogList`.
 */
void Dog::clearDogList(void) noexcept {
	for(auto& dogPtr : dogList)
		dogPtr.reset();
}

/**
 * Return a human-readable version of the dog.
 */
[[ nodiscard ]] std::string Dog::string(void) const noexcept {
	return "Dog{ index="+ std::to_string(index) +"; "+ Animal::string() +" }";
}

/**
 * Get the lowest ID available, return `DOGLIST_SIZE` if there isn't.
 */
ID Dog::getLowestIndex(void) noexcept {
	const auto lowestIt = std::find_if(
		dogList.begin(),
		dogList.end(),
		[](const std::unique_ptr<Dog>& ptr) {
			return ptr.get() == nullptr;	//searching for nullptr
		});

	if(lowestIt == dogList.end())
		return DOGLIST_SIZE;
	return std::distance(std::begin(dogList), lowestIt);	//iterator to index
}