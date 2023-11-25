cmake_minimum_required(VERSION 3.10)

project(TextEditor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(TextEditor
    src/main.cpp
    src/Controller/EditorController.cpp
    src/Model/EditorApp.cpp
    src/View/EditorView.cpp
    src/View/NcursesWrapper.cpp
    src/Utils/FileIO.cpp
    src/Utils/MyString.cpp
)
target_link_libraries(TextEditor ${CURSES_LIBRARIES})

target_include_directories(TextEditor
    PUBLIC
)