cmake_minimum_required(VERSION 3.10)
project(PokemonFangame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Fichiers sources
file(GLOB SOURCES "src/*.cpp")

# Exécutable
add_executable(pokemon ${SOURCES})

# Lier SFML
target_link_libraries(pokemon sfml-graphics sfml-window sfml-system)

# Inclure le dossier include/
target_include_directories(pokemon PRIVATE include)
