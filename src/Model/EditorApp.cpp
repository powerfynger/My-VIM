#include "EditorApp.h"

void Buffer::readText(std::fstream& file){
    MyString line;
    while(file >> line){
        _text.push_back(line);
    }
}

EditorApp::EditorApp(MyString &fileName){
    _fileDescr.open(fileName.c_str());
}

void EditorApp::readToBuffer(){
    _buf.readText(_fileDescr);
}

EditorApp::~EditorApp(){
    _fileDescr.close();

}