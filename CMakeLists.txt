cmake_minimum_required(VERSION 3.13)
project(codevs_reborn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static")

add_executable(codevs_reborn main.cpp strategy/randomStrategy.h common.h strategy/beamSearchStrategy.h strategy/randomStrategy.cpp strategy/beamSearchStrategy.cpp common.cpp)