#ifndef EDITORAPP_H
#define EDITORAPP_H

enum class EditorMode{
    Navigation,
    Write,
    Command,
    Find
};

class EditorApp {
public:

private:
    EditorMode _mode;
};

#endif