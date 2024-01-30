#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include "../View/EditorView.h"

#include <fstream>
#include <vector>
#include <limits>

class EditorView;

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    void readToBuffer(unsigned int maxLineLen);
    int vectorOfIntsToInt(const std::vector<int>& vector);
    std::vector<std::vector<MyString>>* returnText();
    std::vector<MyString>* returnLine(unsigned int lineIndex);

    int findStartOfWordL(MyString& line, int startIndex);
    int findStartOfWordR(MyString& line, int startIndex);
    /* 
    TODO: 
    проверяет длину подстрок и если необходимо удаляет или создает
    новые подстроки в зависимости от того стало ли символов больше
    или меньше если же не нужно то просто перемещает
    символы из одной подстроки в другую

    потребует дополнительные методы для добавления/удаления строк

    данный метод должен вызываться после изменения пользователем строки, т.е.
    нажатие кнопки --> проверка корректности --> изменение строки --> данный метод
    в связи с этим производит лишь одну ребалансировку, т.е. одно действие
    */
    int rebalanceLine(int lineNumber);
    
    void deleteCharAfterCursor();
    void deleteWordAfterCursor();

    void copyCurrentLineToBuffer();
    void copyCurrentWordToBuffer();
    void pasteUserBuffer();
    void deleteCurrentLine();

    void writeText(std::fstream& file);

    void addView(EditorView* view);

    unsigned int getTextLinesNumbers();
    int getTextToDisplayLinesNumber();
    ~EditorApp();

private:
    EditorView *_editorView;
    unsigned int _textLinesNumber;
    int _textToDisplayLinesNumber;
    unsigned int _maxLineLength;
    std::vector<std::vector<MyString>> _text;
    std::fstream _fileDescr;
    std::vector<MyString> _userBuffer;
    
    void _readText(std::fstream& file);
    bool _isWhitespace(char c);
    int _deleteChar(unsigned int lineNumber, unsigned int subLineNumber, unsigned int charIndex);
    void _insertNewLine(std::vector<MyString> line);
    void _deleteLine(int lineNumber);

};

#endif