cmake_minimum_required(VERSION 3.17)
project(serialization_benchmarks)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/cereal)

add_executable(test_cereal test_cereal.cpp Utility.cpp)
target_link_libraries(test_cereal
    PUBLIC
        cereal::cereal
)
