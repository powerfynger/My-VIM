#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstring>
#include <stddef.h>

char *my_strcpy(char *str1, char *str2);
std::string my_strcpy(std::string str1, std::string str2);
char *my_strcpy(char *str1, std::string str2);

char *my_strcat(char *str1, char *str2);
char *my_strcat(char *str1, std::string str2);
std::string my_strcat(std::string str1, std::string str2);

int my_strcmp(char *str1, char *str2);
int my_strcmp(std::string str1, std::string str2);
int my_strcmp(char *str1, std::string str2);

int my_strlen(char *str);
int my_strlen(std::string str);

char *my_substr(char *str, int index, int count);
char *my_substr(std::string str, int index, int count);

class MyString
{
private:
    int len_;
    int cap_;
    char *str_;

public:
    /*
    CONSTRUCTS
    */
    MyString();
    MyString(const char *data);
    MyString(const std::initializer_list<int> values);
    MyString(const std::string &values);
    MyString(const char *data, int count);
    MyString(int count, char c);
    MyString(const MyString &data);
    ~MyString();
    /*
    OPERATORS
    */
    friend std::ostream &operator<<(std::ostream &os, const MyString obj);
    friend std::istream &operator>>(std::istream &is, MyString &obj);
    char operator[](int index);

    MyString operator+(MyString &data);
    MyString operator+(const char *data);
    MyString operator+(const std::string &data);
    MyString &operator+=(const MyString &data);
    MyString &operator+=(std::string &data);
    MyString &operator+=(const char *data);
    MyString &operator=(MyString data);
    MyString &operator=(const char *data);
    MyString &operator=(std::string data);
    MyString &operator=(char data);

    bool operator>(MyString &data);
    bool operator<(MyString &data);
    bool operator>=(MyString &data);
    bool operator<=(MyString &data);
    bool operator==(MyString &data);
    bool operator==(const char *);
    bool operator!=(MyString &data);
    /*
    METHODS
    */
    const char *data();
    const char *c_str();
    const int length();
    const int size();
    const bool empty();
    const int capacity();
    
    void clear();
    void shrink_to_fit();
    void insert(int index, int count, char c);
    void insert(int index, const char *string);
    void insert(int index, const char *string, int count);
    void insert(int index, std::string string);
    void insert(int index, std::string string, int count);
    
    void erase(int index, int count);
    
    void append(int count, char c);
    void append(const char *string);
    void append(const char *string, int index, int count);
    void append(std::string string);

    void append(std::string string, int index, int count);
    void replace(int index, int count, const char *string);
    void replace(int index, int count, std::string string);
    
    const MyString substr(int index);
    const MyString substr(int index, int count);
    
    const int find(const char *string);
    const int find(const char *string, int index);
    const int find(std::string string);
    const int find(std::string string, int index);
};

#endif
