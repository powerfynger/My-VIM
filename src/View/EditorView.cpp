#include "EditorView.h"

EditorView::EditorView(Buffer &buffer) : editorBuffer(buffer)
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

void EditorView::moveCursorRight(bool isContent)
{
    if (isContent)
    {
        _contentWindowCords.x += 1;
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
            handleMoveUp();
        }
    }
    else
    {
        _commandWindowCords.y -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
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
            handleMoveDown();
        }
    }
    else
    {
        _commandWindowCords.y += 1;
        scrl = ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}

void EditorView::handleScrollUp()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentSubtextLine = 0;
        _currentTextLine -= 1;
        if (_currentTextLine < 0)
        {
            _currentTextLine = 0;
            return;
        }
    }
    ncurses.scrollWindowUp(getContentWindowId());
    auto textLine = (*editorBuffer.returnLine(_currentTextLine));
    ncurses.writeWindow(getContentWindowId(), textLine[_currentSubtextLine]);
    ncurses.refreshWindow(getContentWindowId());
}

void EditorView::handleScrollDown()
{
    if (_currentTextLine + 1 >= (editorBuffer.getLinesNumber()))
    {
        _currentSubtextLine = -1;
        return;
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
}

void EditorView::handleMoveDown()
{
    auto textLine = (*editorBuffer.returnLine(_currentTextLine));
    _currentSubtextLine += 1;
    if (_currentSubtextLine >= textLine.size())
    {
        _currentSubtextLine = 0;
        _currentTextLine += 1;
        if (_currentTextLine >= editorBuffer.getLinesNumber()) _currentTextLine -= 1;
    }
}

void EditorView::handleMoveUp()
{
    _currentSubtextLine -= 1;
    if (_currentSubtextLine < 0)
    {
        _currentTextLine -= 1;
        if (_currentTextLine < 0) _currentTextLine = 0;
        auto textLine = (*editorBuffer.returnLine(_currentTextLine));
        _currentSubtextLine = textLine.size() - 1;
    }
}
