#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include <fstream>
#include <vector>
#include <limits>

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    void readToBuffer(unsigned int maxLineLen);
    int vectorOfIntsToInt(const std::vector<int>& vector);
    void readText(std::fstream& file, unsigned int maxLineLen);
    std::vector<std::vector<MyString>>* returnText();
    std::vector<MyString>* returnLine(unsigned int lineIndex);

    int findStartOfWordL(MyString& line, int startIndex);
    int findStartOfWordR(MyString& line, int startIndex);

    void writeText(std::fstream& file);

    unsigned int getRealLinesNumbers();
    int getTextToDisplayLinesNumber();
    ~EditorApp();

private:
    unsigned int _textLinesNumber;
    int _textToDisplayLinesNumber;
    std::vector<std::vector<MyString>> _text;
    unsigned int _maxLineLength;
    std::fstream _fileDescr;
    
    bool _isWhitespace(char c);
};

#endif