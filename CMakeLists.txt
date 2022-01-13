cmake_minimum_required(VERSION 3.21)
project(Basecode C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Basecode
        IA.c
        IA.h
        Jeu.c
        Jeu.h
        main.c Erreurs.c Erreurs.h Multijoueur/Client.c Multijoueur/Client.h)
