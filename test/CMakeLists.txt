cmake_minimum_required(VERSION 3.6)
project("WaMCL test")

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Release"))
	set(CMAKE_BUILD_TYPE "Debug")
	set(
		DBG_FLAGS
		"-fsanitize=address"
		"-fsanitize=leak"
		"-fsanitize=undefined"
	)

	add_compile_options(-ggdb -Wextra ${DBG_FLAGS})
	add_link_options(${DBG_FLAGS})
endif()

add_compile_options(-Wall)
set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})




set(
	VEC_LIBS
	"../code/src/Warning.cpp"
	"../code/src/Vector.cpp"
)
add_library(Vector STATIC ${VEC_LIBS})

add_executable(test_vec "vector.cpp")
target_link_libraries(test_vec PUBLIC 
	${SDL2_LIBRARIES}
	Vector
)

add_executable(test_ang "angles.cpp")
target_link_libraries(test_ang PUBLIC 
	${SDL2_LIBRARIES}
	Vector
)

set(
	HIT_LIBS
	${VEC_LIBS}
	"../code/src/Pos.cpp"
	"../code/src/Hitbox.cpp"
)
add_library(Hitbox STATIC ${HIT_LIBS})

add_executable(test_hit "hitbox.cpp")
target_link_libraries(test_hit PUBLIC 
	${SDL2_LIBRARIES}
	Hitbox
)
