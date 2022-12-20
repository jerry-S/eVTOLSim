cmake_minimum_required(VERSION 3.5)
project(EVSim)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)


add_executable(EvSim main.cpp ../random/src/simpleRandomGen.cpp ../ev/src/ev.cpp)

# Unit tests
add_subdirectory(test)


