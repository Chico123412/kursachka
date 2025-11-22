cmake_minimum_required(VERSION 3.22)
project(Kursova)

set(CMAKE_CXX_STANDARD 17)

add_executable(Kursova
        main.cpp
        Utils.cpp
        TermBase.cpp
        Term.cpp
        PrimitiveTerm.cpp
        TermManager.cpp
        User.cpp
        UserManager.cpp
)
