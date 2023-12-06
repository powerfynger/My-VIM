#include "EditorView.h"

EditorView::EditorView(EditorApp &buffer) : _editorApp(buffer)
{
    ncurses.getSize(&_screenSizeX, &_screenSizeY);
    _contentWindowId = ncurses.addWindow(0, 0, _screenSizeY - 1, _screenSizeX, 0);
    _commandWindowId = ncurses.addWindow(_screenSizeY - 1, 0, 1, _screenSizeX, 1);
    _contentWindowCords.x = 0;
    _contentWindowCords.y = 0;
    _currentTextLine = 0;
    _currentSubtextLine = 0;
}

unsigned int EditorView::getScrSizeX()
{
    return _screenSizeX;
}

unsigned int EditorView::getScrSizeY()
{
    return _screenSizeY;
}

unsigned int EditorView::getCmdWindowId()
{
    return _commandWindowId;
}

unsigned int EditorView::getContentWindowId()
{
    return _contentWindowId;
}

void EditorView::displayAllText()
{
    bool f = true;
    for (const auto lineBuffer : *_editorApp.returnText())
    {
        for (const auto line : lineBuffer)
        {
            ncurses.writeAppendWindow(getContentWindowId(), line);
            ncurses.refreshWindow(getContentWindowId());
            _contentWindowCords.y += 1;
            if (_contentWindowCords.y + 1 >= _screenSizeY)
            {
                f = false;
                break;
            }
        }
        if (!f)
            break;
    }
    _contentWindowCords.y = 0;
    _contentWindowCords.x = 0;
    ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
}

void EditorView::moveCursorBegWord(bool isContent)
{
    WindowCords newCords;
    auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

    newCords.y = _contentWindowCords.y;
    newCords.x = _editorApp.findStartOfWordL(textLine, _contentWindowCords.x);

    if (newCords.x < 0)
    {
        if (_currentTextLine == 0)
            return;

        if (_contentWindowCords.y != 0)
        {

            _decCurrentLine();
            auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine].length();

            newCords.x = _editorApp.findStartOfWordL(textLine, _contentWindowCords.x);
            newCords.y -= 1;
        }
        else
        {
            _handleScrollUp();
            auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine].length();

            newCords.x = _editorApp.findStartOfWordL(textLine, _contentWindowCords.x);
            newCords.y -= 1;
            return;
        }
    }

    _contentWindowCords = newCords;
    ncurses.setCursor(getContentWindowId(), &newCords.y, &newCords.x);
}

void EditorView::moveCursorEndWord(bool isContent)
{
    WindowCords newCords;
    auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

    newCords.y = _contentWindowCords.y;
    newCords.x = _editorApp.findStartOfWordR(textLine, _contentWindowCords.x);

    if (newCords.x >= textLine.length())
    {
        if (_currentTextLine >= _editorApp.getRealLinesNumbers() - 1)
            return;

        if (_contentWindowCords.y != _screenSizeY-2)
        {

            _incCurrentLine();
            auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = 0;

            newCords.x = 0;
            newCords.y += 1;
        }
        else
        {
            _handleScrollDown();
            auto textLine = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = 0;

            newCords.x = 0;
            newCords.y += 1;
            return;
        }
    }

    _contentWindowCords = newCords;
    ncurses.setCursor(getContentWindowId(), &newCords.y, &newCords.x);
}

void EditorView::moveCursorStartLine(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x = 0;
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
    }
}

void EditorView::moveCursorEndLine(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine].length();
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
    }
}

void EditorView::moveCursorLineNumber(int lineNumber)
{
    int maxToDisplayLineNumber = _editorApp.getTextToDisplayLinesNumber() - 1;
    if (lineNumber > maxToDisplayLineNumber) lineNumber = maxToDisplayLineNumber;
    if (lineNumber < 0) lineNumber = 0;

    while (_currentTextLine + _currentSubtextLine != lineNumber)
    {

        if (lineNumber > _currentTextLine + _currentSubtextLine)
            moveCursorDown(true);
        else
            moveCursorUp(true);
        if (_currentTextLine + 1 == _editorApp.getRealLinesNumbers() && _currentSubtextLine + 1 == _editorApp.returnLine(_currentTextLine)->size()) break;
    }
}

void EditorView::moveCursorFirstPage()
{
    moveCursorLineNumber(0);
}

void EditorView::moveCursorLastPage()
{
    moveCursorLineNumber(_editorApp.getTextToDisplayLinesNumber());
}

void EditorView::moveCursorPageUp()
{
    moveCursorLineNumber(_currentSubtextLine + _currentTextLine - _screenSizeY-2);
}

void EditorView::moveCursorPageDown()
{   
    moveCursorLineNumber(_currentSubtextLine + _currentTextLine + _screenSizeY-2);
}

void EditorView::moveCursorRight(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x += 1;
        auto line = (*_editorApp.returnLine(_currentTextLine));
        if (_contentWindowCords.x > line[_currentSubtextLine].length())
        {
            if (_currentSubtextLine < line.size() - 1)
            {
                _contentWindowCords.x = _screenSizeX;
                _incCurrentLine();
            }
            else
            {
                _contentWindowCords.x -= 1;
                return;
            }
        }
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
        _commandWindowCords.x += 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

void EditorView::moveCursorLeft(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x -= 1;
        if (_contentWindowCords.x < 0)
        {
            if (_currentSubtextLine > 0)
            {
                _decCurrentLine();
                _contentWindowCords.x = (*_editorApp.returnLine(_currentTextLine))[_currentSubtextLine].length();
                _contentWindowCords.y -= 1;
            }
            else
            {
                _contentWindowCords.x += 1;
                return;
            }
        }
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
        _commandWindowCords.x -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

void EditorView::moveCursorUp(bool isContent)
{
    ScrollingCode scrl;
    if (isContent)
    {
        _contentWindowCords.y -= 1;
        scrl = ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
        if (scrl == ScrollingCode::SCROLL_UP)
        {
            _handleScrollUp();
        }
        if (scrl == ScrollingCode::STAY)
        {
            _decCurrentLine();
        }
        if (_fixCordXOutsideString()) ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);

    }
    else
    {
        _commandWindowCords.y -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

bool EditorView::_fixCordXOutsideString()
{
    auto lineCurr = (*_editorApp.returnLine(_currentTextLine));
    if (_contentWindowCords.x > lineCurr[_currentSubtextLine].length())
    {
        _contentWindowCords.x = lineCurr[_currentSubtextLine].length();
        return true;
    }
    return false;
}

void EditorView::moveCursorDown(bool isContent)
{
    ScrollingCode scrl;
    if (isContent)
    {
        _contentWindowCords.y += 1;
        scrl = ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
        if (scrl == ScrollingCode::SCROLL_DOWN)
        {
            _handleScrollDown();
        }
        if (scrl == ScrollingCode::STAY)
        {
            _incCurrentLine();
        }
        if (_fixCordXOutsideString()) ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
        _commandWindowCords.y += 1;
        scrl = ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

bool EditorView::_handleScrollUp()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentTextLine -= 1;
        if (_currentTextLine < 0)
        {
            _currentTextLine = 0;
            return true;
        }
        _currentSubtextLine = _editorApp.returnLine(_currentTextLine)->size()-1;
    }
    ncurses.scrollWindowUp(getContentWindowId());
    auto textLine = (*_editorApp.returnLine(_currentTextLine));
    ncurses.writeWindow(getContentWindowId(), textLine[_currentSubtextLine]);
    ncurses.refreshWindow(getContentWindowId());
    return false;
}

bool EditorView::_handleScrollDown()
{
    auto textLine = (*_editorApp.returnLine(_currentTextLine));
    if (_currentTextLine + 1 >= (_editorApp.getRealLinesNumbers()) && _currentSubtextLine + 1 >= textLine.size())
    {
        // _currentSubtextLine = 0;
        return true;
    }

    _currentSubtextLine += 1;
    if (_currentSubtextLine == textLine.size())
    {
        _currentSubtextLine = 0;
        _currentTextLine += 1;
        textLine = (*_editorApp.returnLine(_currentTextLine));
    }
    ncurses.writeAppendWindow(getContentWindowId(), textLine[_currentSubtextLine]);
    ncurses.refreshWindow(getContentWindowId());
    return false;
}

// True or false indicates whether changed real line or not
bool EditorView::_incCurrentLine()
{
    auto textLine = (*_editorApp.returnLine(_currentTextLine));
    _currentSubtextLine += 1;

    if (_currentSubtextLine >= textLine.size())
    {
        _currentSubtextLine = 0;
        _currentTextLine += 1;
        if (_currentTextLine >= _editorApp.getRealLinesNumbers())
            _currentTextLine -= 1;
        return true;
    }
    return false;
}

bool EditorView::_decCurrentLine()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentTextLine -= 1;
        if (_currentTextLine < 0)
            _currentTextLine = 0;
        auto textLine = (*_editorApp.returnLine(_currentTextLine));
        _currentSubtextLine = textLine.size() - 1;
        return true;
    }
    return false;
}