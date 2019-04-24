cmake_minimum_required(VERSION 3.13)
project(codevs_reborn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static -O2")

add_executable(codevs_reborn main.cpp strategy/randomStrategy.h common.h strategy/chainStrategy.h strategy/randomStrategy.cpp strategy/chainStrategy.cpp common.cpp strategy/skillStrategy.h strategy/skillStrategy.cpp strategy/antiSkillStrategy.cpp strategy/antiSkillStrategy.h strategy/switchableStrategy.h strategy/switchableStrategy.cpp)