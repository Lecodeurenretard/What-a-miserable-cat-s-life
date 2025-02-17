#include "../include/Dog.hpp"

const std::string Dog::spriteBase = Animal::spriteFolder + "dog";

void Dog::setToRandomSprite(void) noexcept(false){
	const auto mask = [](const fs::path& path){
		const std::string pathStr = path.string();

		std::cout << pathStr.substr(pathStr.length() - 10, pathStr.length() - 6);
		return fs::is_regular_file(path) &&
				pathStr
					.substr(pathStr.length() - 10, pathStr.length() - 6)
					.find("dog")
				!= pathStr.npos;
	};
	
	spritePath = getRandomPathFromMask(mask).string();
}


Dog::Dog(const Pos& p)
	: Animal(p, Dog::size, Dog::speed)
{
	setToRandomSprite();
}