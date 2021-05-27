cmake_minimum_required(VERSION 3.17)
project(statistical_robust)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2 -march=native -Wall -Wconversion -pedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused")
#set(CMAKE_CXX_FLAGS "-O0 -g3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fsanitize=address -fsanitize=leak -g")

include_directories(lib)

add_executable(TIMatrix tests/main.cpp lib/Slices.h)
