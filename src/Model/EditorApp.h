#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include <fstream>
#include <vector>

class Buffer
{
public:
    Buffer() {};
    void readText(std::fstream& file);
    std::vector<MyString>* returnText();
    void writeText(std::fstream& file);

private:
    int _firstLine, _lastLine, _currentLine;
    std::vector<MyString> _text;


};

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    ~EditorApp();
    void readToBuffer();

    Buffer buf;
private:
    std::fstream _fileDescr;
};

#endif