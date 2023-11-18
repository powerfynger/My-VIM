#include "EditorApp.h"

EditorApp::EditorApp(MyString &fileName){
    _fileDescr.open(fileName.c_str());
}
EditorApp::~EditorApp(){

}