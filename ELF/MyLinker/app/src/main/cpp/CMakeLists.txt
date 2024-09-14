
cmake_minimum_required(VERSION 3.22.1)

project("mylinker")
include_directories(
        ./
        ./include
)

add_library( # Sets the name of the library.
        mylinker

        SHARED

        MyLoader.cpp
        native-lib.cpp)

find_library( # Sets the name of the path variable.
        log-lib


        log)


target_link_libraries( # Specifies the target library.
        mylinker
        android
        ${log-lib})