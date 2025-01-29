cmake_minimum_required(VERSION 3.6) 
project("What a miserable cat's life!" VERSION 0.1.1)

if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
	set(CMAKE_BUILD_TYPE Release)
endif()
message("Build type used: ${CMAKE_BUILD_TYPE}")

add_compile_options(-Wall -std=gnu++23)

# https://blog.shaduri.dev/easily-create-shared-libraries-with-cmake-part-1
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(
	LIB_SOURCES
	"code/src/Pos.cpp"
	"code/src/Animal.cpp"
	"code/src/Cat.cpp"
	"code/src/Dog.cpp"
	"code/src/Warning.cpp"
)

execute_process(COMMAND "sprites/imgToHex")   # Convert all the Images into HexDump
set(
	LIB_IMAGE
	"sprites/hexDump/images.cpp"
)


# Add a shared library target
add_library(ProjectSrc STATIC ${LIB_SOURCES})
add_library(Images SHARED ${LIB_IMAGE})
set_target_properties(Images PROPERTIES LINKER_LANGUAGE CXX)

# Make all non-exported symbols hidden by default
set_target_properties(ProjectSrc
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
)

# Treat the public symbols as exported
# (and not imported) by defining LIBRARY_BUILD
# when building the library.
target_compile_definitions(ProjectSrc
    PRIVATE
        LIBRARY_BUILD
)

# Export the library's public header path to dependent targets
target_include_directories(ProjectSrc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main "code/src/main.cpp")

target_link_libraries(main PRIVATE 
	ProjectSrc
	Images
	${SDL2_LIBRARIES}
)