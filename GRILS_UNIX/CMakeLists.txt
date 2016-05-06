cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(Grils CXX)

#Génération de la liste des fichiers sources

file(

        GLOB_RECURSE

        source_files

        sources/*

)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O3")        ## Optimize
endif()

#Déclaration de l'exécutable

add_executable(

        grils

        ${source_files}

)

