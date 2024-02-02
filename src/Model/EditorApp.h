#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include "../View/EditorView.h"

#include <fstream>
#include <vector>
#include <limits>
#include <sstream>


class EditorView;

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    void readToBuffer(unsigned int maxLineLen);
    int vectorOfIntsToInt(const std::vector<int>& vector);
    std::vector<std::vector<MyString>>* returnText();
    MyString returnCommand();
    void clearCommand();
    std::vector<MyString>* returnLine(unsigned int lineIndex);

    int findStartOfWordL(MyString& line, int startIndex);
    int findStartOfWordR(MyString& line, int startIndex);

    int rebalanceLine(int lineNumber);
    
    void deleteCharAfterCursor(bool isContent);
    void deleteCharBeforeCursor(bool isContent);
    void insertCharAfterCursor(int, bool isContent);
    void insertEmptyLine();
    void deleteWordAfterCursor();

    void copyCurrentLineToBuffer();
    void copyCurrentWordToBuffer();
    void pasteUserBuffer();
    void deleteCurrentLine();

    void readFromFile(MyString fileName);

    void addView(EditorView* view);

    void processCommand();
    void processCommandOpenFile();
    void processCommandSaveToFile();

    void processExit();
    void forceExit();
    
    unsigned int getTextLinesNumbers();
    int getTextToDisplayLinesNumber();
    ~EditorApp();

private:
    EditorView *_editorView;

    unsigned int _textLinesNumber, _textToDisplayLinesNumber, _maxLineLength;

    std::vector<std::vector<MyString>> _text;
    std::vector<MyString> _userBuffer;
    MyString _commandBuffer;
    FILE* _test;
    std::fstream _fileDescr;
    MyString _currFileName;

    void _readText(std::fstream& file);
    bool _isWhitespace(char c);
    int _deleteChar(unsigned int lineNumber, unsigned int subLineNumber, unsigned int charIndex, bool isContent);
    int _insertCharToText(unsigned int lineNumber, unsigned int subLineNumber, int c, unsigned int charIndex);
    int _insertCharToCommand(int c, unsigned int charIndex);
    void _insertNewLine(std::vector<MyString> line);
    void _deleteLine(int lineNumber);
    void _writeText(MyString fileName);

    bool _isFileExist(MyString& flineName);
    bool _isNumeric(MyString& line);
    bool _stringToNumber(MyString& line, int& result);



};

#endif