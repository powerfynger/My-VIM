#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include "../Model/EditorApp.h"
#include "../View/EditorView.h"

class EditorController{
public:
    explicit EditorController(EditorApp& app,  EditorView& view);
    
    void handleInput();
private:
    void handleInsertInput();
    void handleCommandInput();
    void handleFindInput();

    EditorApp& _app;
    EditorView& _view;


};

#endif