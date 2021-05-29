cmake_minimum_required(VERSION 3.10)
project(Bplustree)

set(CMAKE_CXX_STANDARD 14)

add_executable(Bplustree  bplustree.hpp
        utility.hpp vector.h
        exceptions.hpp
      testerase.cpp)