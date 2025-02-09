# What a miserable cat's life!
## General presentation
This project is an AI project (it doesn't look like it yet but it will). In the final version I'm planning to add:
+ ❌ More cats
+ ❌ Dogs
+ ❌ A health system (for cats)
+ ❌ Movement
+ ❌ A genetic algorithm
+ ❌ Better graphisms ?

This project is an encore to an [old scratch project](https://scratch.mit.edu/projects/713148064/) that I made (it' s really buggy and the cats don't move after the first gen) which is an implementation of one of [Laupok's example in one of his videos](https://youtu.be/F63GNXGHVwM?t=352) (french warning) where he makes a brain that plays Mario World, I'm aware that he made a [video](https://www.youtube.com/watch?v=u5xCl1bSe6o&t=0s&ab_channel=Laupok) explaining his [code](https://pastebin.com/Jcvdqhqm) in details but I didn't watch it nor seen his code since I want to do this project my way.

## Description
### Short description of each class
+ **[Animal](code/include/Animal.hpp)**: The `Animal` class is the parent class of `Cat` and `Dog`.
+ **[Cat](code/include/Cat.hpp)**: In the final version, each cat will have to run away of the dogs.
+ **[Dog](code/include/Dog.hpp)**: In the final version, each dog will have to walk randomly and will be nothing but an obstacle to the cats.
+ **[Pos](code/include/Pos.hpp)**: This is a simple struct to represent a position on the screen.
+ **[Vector](code/include/Vector.hpp)**: A vector.
+ **[Warning](code/include/Warning.hpp)**: A warning class.

### Other files
Some files don't contain any class, for now there is only four of them:
+ **[Images.hpp](code/include/Images.hpp)**: Import the hex dumps of the images into the project because cmake can't do it.
+ **[Imports.hpp](code/include/Imports.hpp)**: It's just a file that group all imports so we can see all imports at once.
+ **[Utilities.hpp](code/include/Utilities.hpp)**: Some functions that I wrote and have nowhere to put them.
+ **[main.cpp](code/src/main.cpp)**: The main program and a few functions that are only useful in the `main()` function.

Well, I lied when I said there is only four files without classes, there is another one: **[imgToHex](sprites/imgToHex)**. This is a zsh script that convert images in hex dump with `xxd`.

## Building from source
For this project I used cmake so you just have to run those commands:
```bash
mkdir build; cd build
cmake .. && cmake --build . && ./main
```

This project isn't even tested on Windows, only Linux Mint. I'll take care of compatibility issues at the release of this project.	