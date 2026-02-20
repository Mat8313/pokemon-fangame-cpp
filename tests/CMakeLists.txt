include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(pokemon_lib STATIC
    ../src/Entity/PokemonInstance.cpp
    # ajoute ici les autres .cpp nécessaires si besoin
)

target_include_directories(pokemon_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_executable(test_basic
    test_basic.cpp
)

target_include_directories(test_basic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(test_basic PRIVATE
    pokemon_lib
    gtest_main
    gtest
)

include(GoogleTest)
gtest_discover_tests(test_basic)
