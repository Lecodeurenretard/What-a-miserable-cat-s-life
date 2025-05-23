# What a miserable cat's life!
## General presentation
This project is an AI project (it doesn't look like it yet but it will). In the final version I'm planning to add:
+ ✅ More cats
+ ✅ Dogs
+ ✅ A health system (for cats)
+ ✅ Movement
+ ❌ A genetic algorithm
+ ❌ Better graphisms ?

This project is a improved copy of an [old scratch project](https://scratch.mit.edu/projects/713148064/) that I made (it' s really buggy and the cats don't move after the first gen) which is itself an implementation of one of [Laupok's example in his video on making an AI to play Mario World](https://youtu.be/F63GNXGHVwM?t=501s) (french warning) where he makes a brain that plays Mario World. I'm aware that he made a [video](https://www.youtube.com/watch?v=u5xCl1bSe6o&t=0s&ab_channel=Laupok) explaining his [code](https://pastebin.com/Jcvdqhqm) in details but I didn't watch it nor seen his code since I want to do this project my way.

## Usage
The program can take up to 6 arguments:
+ The first  one is **`--verbose`** (`-v` for short) in order to enable verbose.
+ The second one is **`--step`** (`-s` for short) for running the program frame by frame (press any key to play next frame next frame).
+ The third  one is **`--dbg-infos`** (`-d` for short) for showing destination and the speed vector of the animals.
+ The fourth one is **`--follow-mouse`** (`-m` for short), if on the animals will follow the mouse if it's in the window.
+ The fifth and sixth one are **`--catCount`** and **`--dogCount`**, they control the number of cats and dogs, it is bounded to stay between 1 and 255 included although it is advised to only display a total of 50 animals on screen to not hurt performances too much.

example input (main is the name of the executable):
```bash
./main -vm --catCount 8 -d --dogCount 3
```

## Description
### Short description of each class
+ **[Animal](code/include/Animal.hpp)**: The `Animal` class is the parent class of `Cat` and `Dog`.
+ **[Cat](code/include/Cat.hpp)**: Cats have to run away of the dogs.
+ **[Dog](code/include/Dog.hpp)**: Dogs walk randomly and are an obstacle to the cats.
+ **[Hitbox](code/include/Hitbox.hpp)**: A hitbox to detect collisions.
+ **[Pos](code/include/Pos.hpp)**: This is a simple struct to represent a position on the screen.
+ **[Vector](code/include/Vector.hpp)**: A vector in a 2D space.<!--duh-->
+ **[WarningStream](code/include/Warning.hpp)**: A stream to output warnings.
+ **[VerboseStream](code/include/Verbose.hpp)**: A stream to output verbose.

### Other files
Some files don't contain any class:
+ **[Color_SDL.hpp](code/include/Color_SDL.hpp)**: A set of macros reprensenting colors with the type `SDL_Color`.
+ **[Images.hpp](code/include/Images.hpp)**: Import the hex dumps of the images into the project because cmake can't do it.
+ **[Imports.hpp](code/include/Imports.hpp)**: It's just a file that group all imports so we can see them all at once.
+ **[Utilities.hpp](code/include/Utilities.hpp)**: Some functions that I wrote and have nowhere to put them.
+ **[main.cpp](code/src/main.cpp)**: The main program and a few functions that are only useful in the `main()` function.
+ **[imgToHex](sprites/imgToHex)**: A zsh script that convert images in hex dump with `xxd`.
+ **[test/*](test/)**: Some test to verify all is working as expected.

## Modding
This is not a tutorial in order to mod this project but a group of information that can be useful.

### Building from source
This project uses 4 external dependencies:
- SDL2
- SDL_ttf
- Argument-Parser
- Misc_modules

In order to install those, you can use the following commands (on Linux Mint/Ubuntu):
```bash
sudo apt-get install libsdl2-dev										# SDL2
sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0		# SDL_tff

git clone "https://github.com/Lecodeurenretard/What-a-miserable-cat-s-life.git"

# Argument-Parser and misc_modules are in the project as git submodules
cd "What-a-miserable-cat-s-life/code/include/utility libs"
./installSubmodules			# Execute the install scripts of submodules
```

For this project I used cmake so once the project is imported and you're at the root of the cloned repo, you just have to run those commands:
```bash
cd sprites
./imgToHex		# Transform .bmp files into C hexdumps, make sure to have zsh installed

mkdir ../build; cd ../build
cmake .. && cmake --build . && ../cmake-clean && ./main 
```

### Adding custom sprites
In order to add new sprites for a class, you have to put the image file in the corresponding subdirectory of [img](sprites/img/) ([other](sprites/img/other) is for the  sprites of the `Animal` class and the default ones for its children classes), name it following the name pattern. Make sure the image a `.bmp` file and has square dimensions (even if you have to fill with transparent pixels), in the first case the image won't be detected (and is incompatible with SDL anyway) and in the second, it will be strected/compressed.  
Then it's just creating your own class.


_________________________________
This project isn't even tested on Windows, only Linux Mint. I'll tackle down of compatibility issues at the 1.0 version of this project.
