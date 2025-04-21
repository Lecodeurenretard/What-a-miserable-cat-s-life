cmake_minimum_required(VERSION 3.6)
project("WaMCL test")

add_compile_options(-Wall -ggdb)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

set(
	HIT_LIBS
	"../code/src/Pos.cpp"
	"../code/src/Warning.cpp"
	"../code/src/Vector.cpp"
	"../code/src/Hitbox.cpp"
)
add_library(Hitbox STATIC ${HIT_LIBS})

add_executable(test_hit "hitbox.cpp")
target_compile_definitions(Hitbox PRIVATE NOT_INCLUDE_IMGS)		# Disable image inclusion
target_link_libraries(test_hit PUBLIC 
	${SDL2_LIBRARIES}
	Hitbox
)

set(
	LIB_IMAGE
	"../sprites/hexDump/images.cpp"
)
set(
	INIT_LIBS
	"../code/src/Animal.cpp"
	"../code/src/Dog.cpp"
	"../code/src/Cat.cpp"
	"../code/src/AlgorithmHandler.cpp"
	${HIT_LIBS}
)
add_library(InitAlgo STATIC ${INIT_LIBS})
add_library(Images STATIC ${LIB_IMAGE})

add_executable(initAlgo "initAlgo.cpp")
target_link_libraries(initAlgo PUBLIC
	${SDL2_LIBRARIES}
	${SDL2TTF_LIBRARIES}
	Images
	InitAlgo
w