#include "EditorController.h"

EditorController::EditorController(EditorApp &app, EditorView &view) : _app(app), _view(view)
{
    _mode = EditorMode::Navigation;
    _previousKeys.push_back('w');
    _previousKeys.push_back('w');
    _previousKeys.push_back('w');
}

EditorMode EditorController::getMode()
{
    return _mode;
}

void EditorController::_setMode(EditorMode mode)
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
    case 'b':
        _view.moveCursorBegWord(true);
        break;
    case 'w':
        if(_previousKeys[2] == 'y')
        {   
            _app.copyCurrentWordToBuffer();
            break;
        }
        if (_previousKeys[1] == 'd' && _previousKeys[2] == 'i')
        {
            _app.deleteWordAfterCursor();
            break;
        }
        _view.moveCursorEndWord(true);
        break;
    case 'G':
        if (_currentLineNumberToGo.empty())
        {
            _view.moveCursorLastPage();
            break;
        }
        _view.moveCursorLineNumber(_app.vectorOfIntsToInt(_currentLineNumberToGo));
        _currentLineNumberToGo.clear();
        break;
    case 'g':
        if (_previousKeys[2] == 'g') _view.moveCursorFirstPage();
        break;
    case KEY_PPAGE: // Page Up
        _view.moveCursorPageUp();
        break;
    case KEY_NPAGE: // Page down
        _view.moveCursorPageDown();
        break;
    case 'r':
        // Debug purpose
        _app.rebalanceLine(_view.getCurrentTextLine());
        break;
    case 'x':
        _app.deleteCharAfterCursor();
        break;
    case 'y':
        _app.copyCurrentLineToBuffer();
        break;
    case 'p':
        _app.pasteUserBuffer();
        break;
    case 'd':
        if (_previousKeys[2] == 'd')
        {
            _app.copyCurrentLineToBuffer();
            _app.deleteCurrentLine();
        }
        break;
    case 'i':
        _setMode(EditorMode::Write);
        break;
    case '?':
        /* code */
        break;
    case '/':
        /* code */
        break;
    case 'c':
        _view.ncurses.endNcurses();
        exit(EXIT_SUCCESS);
        break;
    default:
        // Needed by the command go to line number; Probably need to change all number of line types to unsigned long long
        if (_currentLineNumberToGo.size() <= _maxLineNumberLenght && (c >= '0' && c <= '9'))
            _currentLineNumberToGo.push_back(c);
        break;
    }
    _processKey(c);
}

void EditorController::handleWriteInput()
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
    // ESQ     
    case 27:
        _setMode(EditorMode::Navigation);
        return;
    default:
        _app.insertCharAfterCursor(c);
        break;
    }


}

void EditorController::handleFindInput()
{
}

void EditorController::handleCommandInput()
{
}

void EditorController::_processKey(int c)
{
    _previousKeys.push_back(c);
    _previousKeys[0] = _previousKeys[1]; _previousKeys[1] = _previousKeys[2]; _previousKeys[2] = _previousKeys[3];
    _previousKeys.pop_back();
}