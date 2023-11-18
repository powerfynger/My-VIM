#include "EditorController.h"

EditorController::EditorController(EditorApp &app, EditorView &view) : _app(app), _view(view)
{
    _mode = EditorMode::Navigation;
}

void EditorController::handleInput()
{
    switch (_mode)
    {
    case EditorMode::Navigation:
        handleNavigationInput();
        break;
    case EditorMode::Write:
        handleWriteInput();
        break;
    case EditorMode::Command:
        handleCommandInput();
        break;
    case EditorMode::Find:
        handleFindInput();
        break;
    default:
        break;
    }
}

void EditorController::handleNavigationInput()
{
}

void EditorController::handleWriteInput()
{
}

void EditorController::handleFindInput()
{
}

void EditorController::handleCommandInput()
{
}