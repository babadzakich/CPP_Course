cmake_minimum_required(VERSION 3.30)
project(2Task)

set(CMAKE_CXX_STANDARD 23)

add_library(2Task STATIC library.cpp
        banklib/classes/Deposit.cpp
        banklib/classes/Deposit.h
        banklib/classes/Credit.cpp
        banklib/classes/Credit.h
        banklib/enums/Deposit_type.cpp
        banklib/enums/Credit_Type.cpp)

add_subdirectory(banklib)
