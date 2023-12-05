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
    EditorMode getMode();
    void setMode(EditorMode mode);
    void handleInput();

private:
    void handleNavigationInput();
    void handleWriteInput();
    void handleCommandInput();
    void handleFindInput();

    std::vector<int> _currentLineNumberToGo;
    std::vector<int> _previousKeys;
    const int _maxLineNumberLenght = 10;
    EditorMode _mode;
    EditorApp &_app;
    EditorView &_view;

    void _processKey(int c);
};

#endif