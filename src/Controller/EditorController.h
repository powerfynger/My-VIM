#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include "../Model/EditorApp.h"
#include "../View/EditorView.h"

enum class EditorMode
{
    Navigation,
    Write,
    Command,
    Find
};

class EditorController
{
public:
    explicit EditorController(EditorApp &app, EditorView &view);

    void handleInput();

private:
    void handleWriteInput();
    void handleCommandInput();
    void handleFindInput();
    EditorMode &_mode;
    EditorApp &_app;
    EditorView &_view;
};

#endif