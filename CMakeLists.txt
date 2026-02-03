cmake_minimum_required(VERSION 3.16)
project(PokemonFangame VERSION 1.0.0 LANGUAGES CXX)

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
