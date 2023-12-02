#include "EditorController.h"

EditorController::EditorController(EditorApp &app, EditorView &view) : _app(app), _view(view)
{
    _mode = EditorMode::Navigation;
}

EditorMode EditorController::getMode()
{
    return _mode;
}

void EditorController::setMode(EditorMode mode)
{
    _mode = mode;
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
    int c = _view.ncurses.getInput();
    switch (c)
    {
    case KEY_UP:
        _view.moveCursorUp(true);
        break;
    case KEY_DOWN:
        _view.moveCursorDown(true);
        break;
    case KEY_LEFT:
        _view.moveCursorLeft(true);
        break;
    case KEY_RIGHT:
        _view.moveCursorRight(true);
        break;
    case '^':
        _view.moveCursorStartLine(true);
        break;
    case '&':
        _view.moveCursorEndLine(true);
        break;
    case 'r':
        /* code */
        break;
    case 'A':
        /* code */
        break;
    case 'S':
        /* code */
        break;
    case 'I':
        /* code */
        break;
    case 'i':
        /* code */
        break;
    case ':':
        /* code */
        break;
    case '?':
        /* code */
        break;
    case '/':
        /* code */
        break;
    default:
        // DEBUG PURPOSE ONLY
        _view.ncurses.endNcurses();
        exit(EXIT_SUCCESS);
        break;
    }
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