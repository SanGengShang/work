#ifndef __FILE_LIST_H__
#define __FILE_LIST_H__


/**********************************************************************************************/
int GetSdCardCurCapacity(char *card_path,size_t *total,size_t *user);
long long int GetDirectorySize(char *dir); //计算某目录所占空间大小（包含本身的4096Byte）
int  TraverseDir_Num(const char* strVideoDir);//返回该文件夹的文件个数
int FileRemove(const char* fname);
void getNowTime(void);
int Get_file_name_is_specific_directory(char* path, char* suffix, char* get_filename);
int Get_Cur_Dir_Max_FileName(char* path, char* suffix, char* get_filename);
int Delet_Cur_Dir_Early_FileName(char* path, char* suffix);  //path-删除文件夹路径 suffix---删除文件的后缀

/**********************************************************************************************/

#endif

