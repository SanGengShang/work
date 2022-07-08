#ifndef INI_H
#define INI_H
#include <map>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

enum INI_RES
{
    INI_SUCCESS,        //成功
    INI_ERROR,          //普通错误
    INI_OPENFILE_ERROR, //打开文件失败
    INI_NO_ATTR         //无对应的键值
};

class CIni
{
public:
    CIni();
    virtual ~CIni();

public: //getFloat
    int getInt(const char *mAttr, const char *cAttr);
    float getFloat(const char *mAttr, const char *cAttr);
    char *getStr(const char *mAttr, const char *cAttr);
    INI_RES openFile(const char *pathName, const char *type);
    INI_RES closeFile();

protected:
    INI_RES getKey(const char *mAttr, const char *cAttr, char *value);
};
#endif // INI_H
