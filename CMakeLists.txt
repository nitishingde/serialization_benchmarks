cmake_minimum_required(VERSION 3.17)
project(serialization_benchmarks)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/bitsery)
add_subdirectory(lib/cereal)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS serialization)
message(STATUS "Boost version: ${Boost_VERSION}")

add_executable(test_boost test_boost.cpp Utility.cpp)
target_link_libraries(test_boost
    PUBLIC
        ${Boost_LIBRARIES}
)

add_executable(test_bitsery test_bitsery.cpp Utility.cpp)
target_link_libraries(test_bitsery
    PUBLIC
        Bitsery::bitsery
)

add_executable(test_cereal test_cereal.cpp Utility.cpp)
target_link_libraries(test_cereal
    PUBLIC
        cereal::cereal
)
