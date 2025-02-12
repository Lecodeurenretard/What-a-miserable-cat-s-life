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
+ **[WarningStream](code/include/Warning.hpp)**: A stream to output a warning.
+ **[VerboseStream](code/include/Verbose.hpp)**: A stream to output verbose.

### Other files
Some files don't contain any class, for now there is only four of them:
+ **[Images.hpp](code/include/Images.hpp)**: Import the hex dumps of the images into the project because cmake can't do it.
+ **[Imports.hpp](code/include/Imports.hpp)**: It's just a file that group all imports so we can see all imports at once.
+ **[Utilities.hpp](code/include/Utilities.hpp)**: Some functions that I wrote and have nowhere to put them.
+ **[main.cpp](code/src/main.cpp)**: The main program and a few functions that are only useful in the `main()` function.

Well, I lied when I said there is only four files without classes, there is another one: **[imgToHex](sprites/imgToHex)**. This is a zsh script that convert images in hex dump with `xxd`.

## Modding
This is not a tutorial in order to mod this project but a group of information that can be useful.

### Building from source
For this project I used cmake so you just have to run those commands:
```bash
mkdir build; cd build
cmake .. && cmake --build . && ./main
```

### Adding custom sprites
In order to add new sprites for a class in particular, you have to put the image file in the corresponding subdirectory of [img](sprites/img/) ([other](sprites/img/other) is for the default sprites of the `Animal` class or its children classes), name it following the name pattern. Make sure the image is in the `.bmp` format and it exactly has `100x100` dimensions (even if you have to fill with transparent pixels).

Once you did this, you can go to [Image.hpp](code/include/Images.hpp) and declare the variable containing the hex dump of you sprite (located in [hexDump](sprites/hexDump)), follow this syntax:
```C++
extern unsigned char hex_name;		//Where `hex_name` is the name of your hex dump
extern unsigned int hex_name_len;
```

Don't forget to add those variables at the end of the `allDump[]` and `allDumpLen[]` arrays and to modify their sizes.

_________________________________
This project isn't even tested on Windows, only Linux Mint. I'll take care of compatibility issues at the 1.0 version of this project of this project.