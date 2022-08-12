#include <dirent.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include<cstring>
#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */
#include <errno.h>   /* ERROR Number Definitions           */
#include <time.h>
#include <stddef.h>                                                                                                                
#include <stdlib.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "file_list.h"
#include "xcb_videoprot5.h"
using namespace std;

#define SIZE  127
char fullpath[SIZE] = { 0 };
char File_relative_path[SIZE] = {0};
char file_suffix[10] = {0};
char card_path[100] = {0};
static int datesort(const struct dirent **file1, const struct dirent **file2)
{

    struct stat info1, info2;

#if 1
    // 0: 存在   -1: 不存在  非0即真
    if( !is_dir_exist("/extp/sdcard/mmcblk1p1") )
    {
        strcpy(card_path,"/extp/sdcard/mmcblk1p1");
    }
    // 0: 存在   -1: 不存在  非0即真
    else if( !is_dir_exist("/extp/sdcard/mmcblk1") )
    {
        strcpy(card_path,"/extp/sdcard/mmcblk1");
    }
#endif
    char subdir1[256], subdir2[256];
    sprintf(subdir1, "%s/%s", card_path, (*file1)->d_name);
    sprintf(subdir2, "%s/%s", card_path, (*file2)->d_name);
    stat(subdir1, &info1);
    stat(subdir1, &info2);
    return (info1.st_mtime - info2.st_mtime);
}

int GetSdCardCurCapacity(char *card_path,size_t *total,size_t *user)
{
    struct statfs diskInfo; 
    size_t mbTotalsize;
    size_t mbFreedisk;
    unsigned long long totalBlocks;
    unsigned long long totalSize;
    unsigned long long freeDisk;
    statfs(card_path, &diskInfo);
    totalBlocks = diskInfo.f_bsize;  
    totalSize = totalBlocks * diskInfo.f_blocks;  
    mbTotalsize = totalSize>>20;  
    freeDisk = diskInfo.f_bfree*totalBlocks;  
    mbFreedisk = freeDisk>>20;    
    *total = mbTotalsize;
    *user = mbTotalsize - mbFreedisk;
    //printf("cardpath=%s,total=%dMB, free=%dMB\n",card_path,mbTotalsize, mbFreedisk); 
    return 0;
}


//计算某目录所占空间大小（包含本身的4096Byte）
long long int GetDirectorySize(char *dir)
{
     DIR *dp;
     struct dirent *entry;
     struct stat statbuf;
     long long int totalSize=0;
 
     if ((dp = opendir(dir)) == NULL)
     {
         fprintf(stderr, "Cannot open dir: %s\n", dir);
         return -1; //可能是个文件，或者目录不存在
     }
     
     //先加上自身目录的大小
     lstat(dir, &statbuf);
     totalSize+=statbuf.st_size;
 
     while ((entry = readdir(dp)) != NULL)
     {
         char subdir[256];
         sprintf(subdir, "%s/%s", dir, entry->d_name);
         lstat(subdir, &statbuf);
         
         if(S_ISDIR(statbuf.st_mode))
         {
             if(strcmp(".", entry->d_name) == 0 ||
                 strcmp("..", entry->d_name) == 0)
             {
                 continue;
             }
 
             long long int subDirSize = GetDirectorySize(subdir);
             totalSize+=subDirSize;
         }
         else
         {
             totalSize+=statbuf.st_size;
         }
     }
     closedir(dp);    
     return totalSize;
}

int FileRemove(const char* fname)
{
    return remove(fname);
}

int  TraverseDir_Num(const char* strVideoDir)//返回该文件夹的文件个数
{
	//遍历目录
	static  int num = 0;
	DIR*		dp;
	struct dirent *entry;
	struct stat statbuf;
	dp = opendir(strVideoDir);
	if(!dp)
	{
		printf("无法打开目录:%s", strVideoDir);
		return -1;
	}
	chdir(strVideoDir);
	while((entry = readdir(dp)) != nullptr)
	{
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode))
		{
			if(!strcmp(".",entry->d_name) || !strcmp("..",entry->d_name))
			{
				continue;
			}
			char		strNewDir[256];
			sprintf(strNewDir, "%s/%s", strVideoDir, entry->d_name);
			TraverseDir_Num(strNewDir);
		}
		else
		{
			num += 1;
		}
	};
	
	chdir("..");
	closedir(dp);
	return num;
}

long getNowTime(void)
{
#if 0
	struct tm nowTime;
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);  //获取相对于1970到现在的秒数
	localtime_r(&time.tv_sec, &nowTime);
	char current[1024];
	sprintf(current, "%04d%02d%02d%02d:%02d:%02d", nowTime.tm_year + 1900, nowTime.tm_mon+1, nowTime.tm_mday, 
    nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
    printf("xcb current = %s\n",current);
#else
   // 一秒等于1000毫秒
   // 一秒等于1000 000微秒
   // 一秒等于1000 000 000纳秒
    struct timeval tv;
    gettimeofday(&tv,NULL);
    long  timelen = tv.tv_sec*1000000 + tv.tv_usec;
    //printf("second:%ld\n",tv.tv_sec);  //秒
//    printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
//    printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒
    return timelen;
#endif
}

int Get_file_name_is_specific_directory(char* path, char* suffix,char *get_filename)
{
    DIR* dir;
    dir = opendir(path);
    if (dir == NULL)
    {
        return -1;
    }
    struct stat st;
    struct dirent* entry;
    memset(fullpath, 0, sizeof(fullpath));
    while ((entry = readdir(dir)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        {
            continue;
        }
        sprintf(fullpath, "%s/%s", path, entry->d_name);
        //if((strcmp(entry->d_name,filename))==0)
        if(strstr(fullpath, suffix) != NULL)
        {

            printf("文件%s存在,其绝对路径为:%s\n", suffix, fullpath);
            strcpy(get_filename, fullpath);
            return 1;
        }
        if (lstat(fullpath, &st) != 0)
        {
            continue;
        }
        if (S_ISDIR(st.st_mode))
        {
//            dir_run(fullpath, filename);
            printf("%s目录下不存在该文件\n", fullpath);
        }
    }
    closedir(dir);
    return 0;
}

int file_filter(const struct dirent *entry)
{
    if(strstr(entry->d_name, file_suffix) != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    } 
}
 
int Get_Cur_Dir_Max_FileName(char* path, char* suffix, char* get_filename)
{
    struct dirent **entry_list;
    int count;
    int i;

    strcpy(file_suffix,suffix);
    printf("xcb cur scan file path=%s,file_suffix=%s\n",path,file_suffix);

    memset(File_relative_path,0,sizeof(File_relative_path));
    count = scandir(path, &entry_list, file_filter, alphasort);

    printf("xcb scan file count = %d\n",count);
    if (count <= 0) 
    {
        printf("scandir fail\n");
        return -1;
    }
    for (i = 0; i < count; i++) 
    {
        struct dirent *entry;
        entry = entry_list[i];
        printf("%s\n", entry->d_name);

        if(i == count-1)
        {
            strcpy(File_relative_path,entry->d_name);
            strcpy(get_filename,File_relative_path);
        }
        else
        {
            char del_cmd[80]={"rm -rf"};
            strcat(del_cmd," ");
            strcat(del_cmd,path);
            strcat(del_cmd,"/");
            strcat(del_cmd,"'");
            strcat(del_cmd,entry->d_name);
            strcat(del_cmd,"'");
            system(del_cmd);
        }
        free(entry);
    }
    free(entry_list);
    return 0;
}

char* cmd_system(const char* command)
{
    char* result = "";
    FILE *fpRead;
    fpRead = popen(command, "r");
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,1024-1,fpRead)!=NULL)
    {
        result = buf;
    }
    if(fpRead!=NULL)
        pclose(fpRead);
    return result;
}
#if 1
int Delet_Cur_Dir_Early_FileName(char* path, char* suffix)
{
    struct dirent **entry_list;
    int count;
    strcpy(file_suffix,suffix);
    strcpy(card_path,path);
    char videoPath[40]={0};
    sprintf(videoPath,"%s%s",path,"/Video/");
    count = scandir(videoPath, &entry_list, file_filter, datesort);
    //printf("xcb scan file count = %d\n",count);

    if (count <= 0) 
    {
        printf("scandir fail\n");
        return -1;
    }
    free(entry_list);
    //cmd_system("ls `pwd`/*.mp4 -tr | head -1 | xargs");
    char ls_cmd[80]={"ls"};
    strcat(ls_cmd," ");
    strcat(ls_cmd,path);
    strcat(ls_cmd,"/Video/");
    strcat(ls_cmd,"*.mp4 -tr | head -1 | xargs");
    char* result = cmd_system(ls_cmd);
    printf("The file:%s will be deleted\n",result);
    char del_cmd[80]={"rm -vf"};
    strcat(del_cmd," ");
    strcat(del_cmd,result);
    system(del_cmd);
    system("sync");
    printf("deleted file---%s----------------\n",del_cmd);

    return 0;
}
#else
int Delet_Cur_Dir_Early_FileName(char* path, char* suffix)
{
    struct dirent **entry_list;
    int count;
    int i;
    struct dirent *entry;

    strcpy(file_suffix,suffix);

    printf("xcb cur scan file path=%s,file_suffix=%s\n",path,file_suffix);

    memset(File_relative_path,0,sizeof(File_relative_path));
    count = scandir(path, &entry_list, file_filter, alphasort);

    printf("xcb scan file count = %d\n",count);

    if (count <= 0)
    {
        printf("scandir fail\n");
        return -1;
    }
    entry = entry_list[0];
    printf("start to delete file-------------------\n");
    {
        char del_cmd[80]={"rm -rf"};
        strcat(del_cmd," ");
        strcat(del_cmd,path);
        strcat(del_cmd,"/");
        strcat(del_cmd,entry->d_name);
        system(del_cmd);
    }
    free(entry);

#if 0
    for (i = 0; i < count; i++)
    {
        struct dirent *entry;
        entry = entry_list[i];
        printf("%s\n", entry->d_name);


        if(i == count-1)
        {
            strcpy(File_relative_path,entry->d_name);
            strcpy(get_filename,File_relative_path);
        }
        else
        {
            char del_cmd[80]={"rm -rf"};
            strcat(del_cmd," ");
            strcat(del_cmd,path);
            strcat(del_cmd,"/");
            strcat(del_cmd,entry->d_name);
            system(del_cmd);

        }

        free(entry);
    }
 #endif
    free(entry_list);

    return 0;
}
#endif
static int EmptyDir(char *destDir)
{

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if ((dp = opendir(destDir)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", destDir);
        return -1;
    }
    chdir (destDir);
    while ((entry = readdir(dp)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISREG(statbuf.st_mode))//一般文件
        {
            remove(entry->d_name);
        }
    }

    return 0;
}




