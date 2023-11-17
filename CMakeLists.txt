cmake_minimum_required(VERSION 3.10)

project(TextEditor)

set(CMAKE_CXX_STANDARD 14)

add_executable(TextEditor
    src/main.cpp
    src/Controller/EditorController.cpp
    src/Model/Document.cpp
    src/View/EditorView.cpp
    src/Utils/FileIO.cpp
)

target_include_directories(TextEditor
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)