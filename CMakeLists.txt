cmake_minimum_required(VERSION 3.16)
project(PokemonFangame VERSION 1.0.0 LANGUAGES CXX)

# GoogleTest nécessite C++14 minimum, mais C++17 c'est mieux
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Pour Windows/WSL
FetchContent_MakeAvailable(googletest)


include(CTest)
if(BUILD_TESTING)
  enable_testing()
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Fichiers sources - LISTE EXPLICITE
set(SOURCES
    src/main.cpp
    
    # Core
    src/Core/Game.cpp
    src/Core/GameState.cpp
    
    # Entity
    src/Entity/Entity.cpp
    src/Entity/Character.cpp
    src/Entity/Player.cpp
    src/Entity/PokemonInstance.cpp
    
    # World
    src/World/Map.cpp
    src/World/MapRenderer.cpp
    src/World/Tile.cpp
    src/World/TiledMapLoader.cpp
    
    # UI
    src/UI/MainMenuScene.cpp
    src/UI/SelectGameScene.cpp
    src/UI/Button.cpp
    
    # Utils
    src/Utils/SaveManager.cpp
)

# Exécutable
add_executable(pokemon ${SOURCES})

# Includes
target_include_directories(pokemon PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# SFML linking
target_link_libraries(pokemon PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Flags optionnels (pour plus de robustesse)
if(MSVC)
    target_compile_options(pokemon PRIVATE /W4)
else()
    target_compile_options(pokemon PRIVATE -Wall -Wextra -pedantic)
endif()

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()


