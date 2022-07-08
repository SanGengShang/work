#ifndef _FILE_LIST_H_
#define _FILE_LIST_H_
#include <stddef.h>

int GetSdCardCurCapacity(char *card_path,size_t *total,size_t *user);
long long int GetDirectorySize(char *dir); //计算某目录所占空间大小（包含本身的4096Byte）
int  TraverseDir_Num(const char* strVideoDir);//返回该文件夹的文件个数
int FileRemove(const char* fname);
long getNowTime(void);
int Get_file_name_is_specific_directory(char* path, char* suffix, char* get_filename);
int Get_Cur_Dir_Max_FileName(char* path, char* suffix, char* get_filename);
int Delet_Cur_Dir_Early_FileName(char* path, char* suffix);  //path-待删除文件所在文件夹的路径 suffix---待删除文件的后缀
char* cmd_system(const char* command);
#endif
