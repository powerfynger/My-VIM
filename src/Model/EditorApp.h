#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"

enum class EditorMode{
    Navigation,
    Write,
    Command,
    Find
};

class EditorApp {
public:
    explicit EditorApp(const MyString& fileName);
    ~EditorApp();
    EditorMode getMode();
    void setMode(EditorMode mode);

private:
    EditorMode _mode;
};

#endif