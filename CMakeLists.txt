cmake_minimum_required(VERSION 3.13)
project(codevs_reborn)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static -O2")

add_executable(codevs_reborn main.cpp strategy/randomStrategy.h common.h strategy/onlyChainStrategy.h strategy/randomStrategy.cpp strategy/onlyChainStrategy.cpp common.cpp strategy/prioritizeSkillStrategy.h strategy/prioritizeSkillStrategy.cpp strategy/antiSkillStrategy.cpp strategy/antiSkillStrategy.h)