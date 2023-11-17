cmake_minimum_required(VERSION 3.10)

project(TextEditor)

set(CMAKE_CXX_STANDARD 17)

add_executable(TextEditor
    src/main.cpp
    src/Controller/EditorController.cpp
    src/Model/EditorApp.cpp
    src/View/EditorView.cpp
    src/Utils/FileIO.cpp
    src/Utils/MyString.cpp
)

target_include_directories(TextEditor
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)