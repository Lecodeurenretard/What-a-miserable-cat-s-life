#include "../include/Dog.hpp"

const std::string Dog::spriteBase = Animal::spriteFolder + "dog";

void Dog::setToRandomSprite(void) noexcept(false){
	const auto mask = [](const fs::path& path){
		const std::string pathStr = path.string().replace(0, 8, "");
		
		return fs::is_regular_file(path) && pathStr.starts_with("dog") && pathStr.ends_with(".bmp");
	};
	
	spritePath = getRandomPathFromMask(mask).string();
}


[[ nodiscard ]] Dog::Dog(const Pos& p)
	: Animal(p, Dog::size, Dog::speed)
{
	setToRandomSprite();
}

[[ nodiscard ]] std::string Dog::string(void) const {
	return "Dog{ "+ Animal::string() +" }";
}