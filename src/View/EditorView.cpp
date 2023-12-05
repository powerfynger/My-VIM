#include "EditorView.h"

EditorView::EditorView(EditorApp &buffer) : editorBuffer(buffer)
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
    for (const auto lineBuffer : *editorBuffer.returnText())
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
    auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

    newCords.y = _contentWindowCords.y;
    newCords.x = editorBuffer.findStartOfWordL(textLine, _contentWindowCords.x);

    if (newCords.x < 0)
    {
        if (_currentTextLine == 0)
            return;

        if (_contentWindowCords.y != 0)
        {

            decCurrentLine();
            auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine].length();

            newCords.x = editorBuffer.findStartOfWordL(textLine, _contentWindowCords.x);
            newCords.y -= 1;
        }
        else
        {
            handleScrollUp();
            auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine].length();

            newCords.x = editorBuffer.findStartOfWordL(textLine, _contentWindowCords.x);
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
    auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

    newCords.y = _contentWindowCords.y;
    newCords.x = editorBuffer.findStartOfWordR(textLine, _contentWindowCords.x);

    if (newCords.x >= textLine.length())
    {
        if (_currentTextLine >= editorBuffer.getLinesNumber() - 1)
            return;

        if (_contentWindowCords.y != _screenSizeY-2)
        {

            incCurrentLine();
            auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

            _contentWindowCords.x = 0;

            newCords.x = 0;
            newCords.y += 1;
        }
        else
        {
            handleScrollDown();
            auto textLine = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine];

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
        _contentWindowCords.x = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine].length();
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
    }
}

void EditorView::moveCursorLineNumber(int lineNumber)
{

}

void EditorView::moveCursorRight(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x += 1;
        auto line = (*editorBuffer.returnLine(_currentTextLine));
        if (_contentWindowCords.x > line[_currentSubtextLine].length())
        {
            if (_currentSubtextLine < line.size() - 1)
            {
                _contentWindowCords.x = _screenSizeX;
                incCurrentLine();
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
                decCurrentLine();
                _contentWindowCords.x = (*editorBuffer.returnLine(_currentTextLine))[_currentSubtextLine].length();
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
            handleScrollUp();
        }
        if (scrl == ScrollingCode::STAY)
        {
            decCurrentLine();
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
    auto lineCurr = (*editorBuffer.returnLine(_currentTextLine));
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
            handleScrollDown();
        }
        if (scrl == ScrollingCode::STAY)
        {
            incCurrentLine();
        }
        if (_fixCordXOutsideString()) ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    }
    else
    {
        _commandWindowCords.y += 1;
        scrl = ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

bool EditorView::handleScrollUp()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentTextLine -= 1;
        _currentSubtextLine = 0;
        if (_currentTextLine < 0)
        {
            _currentTextLine = 0;
            return true;
        }
    }
    ncurses.scrollWindowUp(getContentWindowId());
    auto textLine = (*editorBuffer.returnLine(_currentTextLine));
    ncurses.writeWindow(getContentWindowId(), textLine[_currentSubtextLine]);
    ncurses.refreshWindow(getContentWindowId());
    return false;
}

bool EditorView::handleScrollDown()
{
    if (_currentTextLine + 1 >= (editorBuffer.getLinesNumber()))
    {
        _currentSubtextLine = 0;
        return true;
    }

    auto textLine = (*editorBuffer.returnLine(_currentTextLine));
    _currentSubtextLine += 1;
    if (_currentSubtextLine == textLine.size())
    {
        _currentSubtextLine = 0;
        _currentTextLine += 1;
        textLine = (*editorBuffer.returnLine(_currentTextLine));
    }
    ncurses.writeAppendWindow(getContentWindowId(), textLine[_currentSubtextLine]);
    ncurses.refreshWindow(getContentWindowId());
    return false;
}

// True or false indicates whether changed real line or not
bool EditorView::incCurrentLine()
{
    auto textLine = (*editorBuffer.returnLine(_currentTextLine));
    _currentSubtextLine += 1;

    if (_currentSubtextLine >= textLine.size())
    {
        _currentSubtextLine = 0;
        _currentTextLine += 1;
        if (_currentTextLine >= editorBuffer.getLinesNumber())
            _currentTextLine -= 1;
        return true;
    }
    return false;
}

bool EditorView::decCurrentLine()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentTextLine -= 1;
        if (_currentTextLine < 0)
            _currentTextLine = 0;
        auto textLine = (*editorBuffer.returnLine(_currentTextLine));
        _currentSubtextLine = textLine.size() - 1;
        return true;
    }
    return false;
}