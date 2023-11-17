#include "MyString.h"

int my_strlen(char *str_)
{
    int i = 0;
    while (str_[i] != '\0')
    {
        i++;
    }
    return i;
}

char *my_strcpy(char *str1, char *str2)
{
    int len1, len2;
    len1 = my_strlen(str1);
    len2 = my_strlen(str2);
    // if (len2 > len1) return NULL;
    for (int i = 0; i < len2 + 1; i++)
    {
        str1[i] = str2[i];
    }
    return str1;
}

char *my_strcat(char *str1, char *str2)
{
    int len1, len2, new_len;
    len1 = my_strlen(str1);
    len2 = my_strlen(str2);
    new_len = len1 + len2;
    for (int i = 0; i < len2; i++)
    {
        str1[len1 + i] = str2[i];
    }
    str1[new_len] = '\0';
    return str1;
}

int my_strcmp(char *str1, char *str2)
{
    for (int i = 0;; i++)
    {
        if (str1[i] == '\0')
        {
            if (str2[i] == '\0')
                return 0;
            return -1;
        }
        if (str2[i] == '\0')
            return 1;

        if (str1[i] > str2[i])
            return 1;
        if (str1[i] < str2[i])
            return -1;
    }
}

int my_strlen(std::string str)
{
    return my_strlen(str.data());
}

std::string my_strcpy(std::string str1, std::string str2)
{
    return my_strcpy(str1.data(), str2.data());
}

std::string my_strcat(std::string str1, std::string str2)
{
    return my_strcat(str1.data(), str2.data());
}

int my_strcmp(std::string str1, std::string str2)
{
    return my_strcmp(str1.data(), str2.data());
}

char *my_strcat(char *str1, std::string str2)
{
    return my_strcat(str1, str2.data());
}

char *my_strcpy(char *str1, std::string str2)
{
    return my_strcpy(str1, str2.data());
}

int my_strcmp(char *str1, std::string str2)
{
    return my_strcmp(str1, str2.data());
}

char *my_substr(char *str, int index, int count)
{
    int tmp_len = count;
    int tmp_cap = tmp_len + 1;
    char *tmp = new char[tmp_cap];
    for (int i = 0; i < tmp_len; i++)
    {
        tmp[i] = str[i + index];
    }
    str = tmp;
    return tmp;
}
char *my_substr(std::string str, int index, int count)
{
    return my_substr(str.data(), index, count);
}
/*
CONSTRUCTS
*/
MyString::MyString()
{
    len_ = 0;
    cap_ = 1;
    str_ = new char[cap_];
    str_[0] = '\0';
    // std::cout << "Without params" << std::endl;
}
MyString::MyString(const char *data)
{
    len_ = 0;
    // str_ = data;
    while (data[len_] != '\0')
    {
        len_++; 
    }
    cap_ = len_ + 1;
    str_ = new char[cap_];
    for (int i = 0; i < len_; i++)
    {
        str_[i] = data[i];
    }
    str_[len_] = '\0';
    // std::cout << "Char string" << std::endl;
}
MyString::MyString(const std::initializer_list<int> values)
{
    int i = 0;
    len_ = 0;
    for (auto value : values)
    {
        len_++;
    }
    cap_ = len_ + 1;
    str_ = new char[cap_];
    for (auto value : values)
    {
        str_[i] = value;
        i++;
    }
    str_[len_] = '\0';
    // std::cout << "List" << std::endl;
}
MyString::MyString(const std::string &values)
{
    new (this) MyString(values.data());
}
MyString::MyString(const char *data, int count)
{
    char* tmp = my_substr((char*) data, 0, count);
    tmp[count] = 0;
    new (this) MyString(tmp);
    delete tmp;

}
MyString::MyString(int count, char c)
{
    len_ = count;
    cap_ = len_ + 1;
    str_ = new char[cap_];
    for (int i = 0; i < len_; i++)
    {
        str_[i] = c;
    }
    str_[len_] = '\0';
}
MyString::MyString(const MyString &data)
{
    new (this) MyString(data.str_);

}
MyString::~MyString()
{
    delete[] str_;
}
/*
OPERATORS
*/
std::ostream &operator<<(std::ostream &os, const MyString obj)
{
    if (obj.len_ == 0)
        return os;
    // os << obj.str_;
    for (int i = 0; obj.str_[i] != 0; i++)
    {
        os << obj.str_[i];
    }
    return os;
}
std::istream &operator>>(std::istream &is, MyString &obj)
{
    // delete[] obj.str_;
    char buf[256];
    is >> buf;
    obj = MyString(buf);
    // for (int i = 0; i < obj.len_; i++){
    // is >> obj.str_[i];
    // }
    return is;
}

char MyString::operator[](int index)
{
    if (index >= len_)
    {
        std::cout << "Index out of range" << std::endl;
        exit(EXIT_FAILURE);
    }
    return str_[index];
}
MyString MyString::operator+(const char *data)
{
    int tmp_len = len_ + my_strlen((char *)data);
    int tmp_cap = tmp_len + 1;
    char *tmp = new char[tmp_cap];
    my_strcpy(tmp, str_);
    my_strcat(tmp, (char *)data);
    MyString res(tmp);
    delete[] tmp;
    return res;
}
MyString MyString::operator+(MyString &data)
{
    const char *tmp = data.data();
    MyString res = *this + tmp;
    return res;
}

MyString MyString::operator+(const std::string &data)
{
    const char *tmp = data.c_str();
    MyString res = *this + tmp;
    return res;
}
MyString &MyString::operator+=(const char *data)
{
    *this = *this + data;
    return *this;
}
MyString &MyString::operator+=(const MyString &data)
{
    *this = *this + data.str_;
    return *this;
}
MyString &MyString::operator+=(std::string &data)
{
    const char *c = data.c_str();
    *this = *this + c;
    return *this;
}
MyString &MyString::operator=(const char *data)
{
    delete[] str_;
    len_ = my_strlen(data);
    cap_ = len_ + 1;
    str_ = new char[cap_];
    my_strcpy(str_, (char *)data);
    return *this;
}
MyString &MyString::operator=(MyString data)
{
    *this = data.data();
    return *this;
}

MyString &MyString::operator=(std::string data)
{
    const char *tmp = data.c_str();
    *this = tmp;
    return *this;
}
MyString &MyString::operator=(char data)
{
    delete[] str_;
    len_ = 1;
    cap_ = len_ + 1;
    str_ = new char[cap_];
    str_[0] = data;
    str_[1] = '\0';
    return *this;
}
bool MyString::operator>(MyString &data)
{
    if (my_strcmp(str_, data.str_) > 0)
        return 1;
    return 0;
}
bool MyString::operator<(MyString &data)
{
    if (my_strcmp(str_, data.str_) < 0)
        return 1;
    return 0;
}
bool MyString::operator>=(MyString &data)
{
    if (my_strcmp(str_, data.str_) >= 0)
        return 1;
    return 0;
}
bool MyString::operator<=(MyString &data)
{
    if (my_strcmp(str_, data.str_) <= 0)
        return 1;
    return 0;
}
bool MyString::operator==(MyString &data)
{
    if (my_strcmp(str_, data.str_) != 0)
        return 0;
    return 1;
}
bool MyString::operator==(const char *data)
{
    if (my_strcmp(str_, data) != 0)
        return 0;
    return 1;
}
bool MyString::operator!=(MyString &data)
{
    if (my_strcmp(str_, data.str_) != 0)
        return 1;
    return 0;
}
/*
1.

*/
/*
METHODS
*/
const char *MyString::c_str()
{
    return str_;
}
const char *MyString::data()
{
    return str_;
}

const int MyString::length()
{
    return size();
}

const int MyString::size()
{
    return len_;
}

const bool MyString::empty()
{
    return (len_ == 0);
}

const int MyString::capacity()
{
    return cap_;
}

void MyString::clear()
{
    len_ = 0;
    str_[0] = '\0';
}

void MyString::shrink_to_fit()
{
    if (cap_ == len_ + 1)
        return;
    char *tmp;
    cap_ = len_ + 1;
    tmp = new char[cap_];
    my_strcpy(tmp, str_);
    delete[] str_;
    str_ = tmp;
}
void MyString::insert(int index, int count, char c)
{
    MyString tmp(count, c);

    this->insert(index, tmp.c_str());
}
void MyString::insert(int index, const char *string)
{
    insert(index, string, my_strlen(string));
}
void MyString::insert(int index, const char *string, int count)
{
    char *tmp;
    int string_len = count;
    int tmp_len = len_ + string_len;
    int tmp_cap = tmp_len + 1;

    // if (string_len + len_ >= cap_)
    tmp = new char[tmp_cap];

    for (int i = 0; i < index; i++)
    {
        tmp[i] = str_[i];
    }
    for (int i = index; i < index + string_len; i++)
    {
        tmp[i] = string[i - index];
    }
    for (int i = index + string_len; i <= len_ + string_len; i++)
    {
        tmp[i] = str_[i - string_len];
    }
    cap_ = (cap_ > tmp_cap) ? cap_ : tmp_cap;
    delete[] str_;
    str_ = tmp;
    len_ = tmp_len;
    // str_[len_] = '\0';
}

void MyString::insert(int index, std::string string)
{
    insert(index, string.data());
}

void MyString::insert(int index, std::string string, int count)
{
    this->insert(index, string.data(), count);
}
void MyString::erase(int index, int count)
{
    char* tmp = new char[len_ - count];
    my_strcpy(tmp, my_substr(str_, 0, index));
    my_strcat(tmp, my_substr(str_, index + count, len_));

    tmp[len_ - count] = '\0';
    
    *this = tmp;
    delete tmp;

}
void MyString::append(int count, char c)
{
    this->insert(len_, count, c);
}
void MyString::append(const char *string)
{
    this->insert(len_, string);
}
// index -- откуда начинать вставлять; count -- сколько вставлять
void MyString::append(const char *string, int index, int count)
{
    char *tmp = my_substr(string, index, count);
    this->insert(len_, tmp, count);
    delete tmp;
}
void MyString::append(std::string string)
{
    insert(len_, string);
}
// index -- откуда начинать вставлять; count -- сколько вставлять
void MyString::append(std::string string, int index, int count)
{
    char *tmp = my_substr(string, index, count);
    this->insert(len_, tmp);
    delete tmp;
}
void MyString::replace(int index, int count, const char *string)
{
    this->erase(index, count-1);
    this->insert(index, string, count-1);
}
void MyString::replace(int index, int count, std::string string)
{
    this->erase(index, count);
    // string[count] = '\0';
    this->insert(index, string, count);
}

const MyString MyString::substr(int index)
{
    char *tmp = my_substr(str_, index, len_ - index);
    tmp[len_ - index] = '\0';
    MyString res(tmp);
    delete tmp;
    return res;
}

const MyString MyString::substr(int index, int count)
{
    char *tmp = my_substr(str_, index, count);
    tmp[count + 1] = '\0';
    MyString res(tmp);
    // printf("%s")
    delete tmp;
    return res;
}

const int MyString::find(const char *string)
{
    return this->find(string, 0);
}

const int MyString::find(const char *string, int index)
{
    int string_len = my_strlen(string);
    int k = 1;
    for (int i = index; i < len_; i++)
    {
        if (str_[i] == string[0])
        {
            for (k; k < string_len; k++)
            {
                if (str_[i + k] != string[k])
                    break;
            }
            // printf("k:%d str_:%d ", k, string_len);
            if (k == string_len)
                return i;
            else
                k = 1;
        }
    }
    return -1;
}

const int MyString::find(std::string string)
{
    return this->find(string.data());
}

const int MyString::find(std::string string, int index)
{
    return this->find(string.data(), index);
}
