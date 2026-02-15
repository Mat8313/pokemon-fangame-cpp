# Configuration Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# Pour Windows : empêche gtest de forcer le runtime MT
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Active les tests CMake
enable_testing()

# Crée une bibliothèque avec ton code Entity (sans main.cpp)
add_library(pokemon_lib STATIC
    ../src/Entity/PokemonInstance.cpp
    # Ajoute ici les autres .cpp dont tu as besoin pour les tests
)

target_include_directories(pokemon_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(pokemon_lib PUBLIC
    # Pas besoin de SFML pour les tests Entity
)

# Exécutable de test
add_executable(pokemon_tests
    test_PokemonInstance.cpp
    # Ajoute d'autres fichiers de test ici
)

target_link_libraries(pokemon_tests PRIVATE
    pokemon_lib
    gtest_main
    gtest
)

# Déclare les tests pour CTest
include(GoogleTest)
gtest_discover_tests(pokemon_tests)


# Tests unitaires (optionnel, désactivable avec -DBUILD_TESTS=OFF)
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()