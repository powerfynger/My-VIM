#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include <fstream>

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    ~EditorApp();
    // EditorMode getMode();
    // void setMode(EditorMode mode);

private:
    std::ofstream _fileDescr;
};

#endif