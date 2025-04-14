cmake_minimum_required(VERSION 3.6)
project("WaMCL test")

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(CMAKE_BUILD_TYPE "Release")
else()
	add_compile_options(-ggdb)
endif()

add_compile_options(-Wall)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


set(
	HIT_LIBS
	"../code/src/Pos.cpp"
	"../code/src/Warning.cpp"
	"../code/src/Vector.cpp"
	"../code/src/Hitbox.cpp"
)
add_library(Hitbox STATIC ${HIT_LIBS})

add_executable(test_hit "hitbox.cpp")
target_link_libraries(test_hit PUBLIC 
	${SDL2_LIBRARIES}
	Hitbox
)