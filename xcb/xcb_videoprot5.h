/******************************************************************************

                  版权所有 (C), 2019-2025,

 ******************************************************************************
  文 件 名   :
  版 本 号   : V1.1
  作    者   :
  生成日期   : 2019年10月10日
  最近修改   :
  功能描述   : 
  函数列表   :
  修改历史   :
  1.日    期   : 2020年5月7日
    作    者    : team
    修改内容   : 创建文件

******************************************************************************/
/******************************************************************************/
#ifndef _xcb_videoprot5_H_
#define _xcb_videoprot5_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "xcb_auth_embed.h"
//#include "xcb_autocalib.h"

#pragma  once
#ifdef __cplusplus
extern "C"
{
#endif
 
//需要按C标准编译的代码

/**********************************************************************************************/
/**********************************************************************************************/

int video_free_buffers(struct isp_video_device *video,int index);
int video_set_fmt(struct isp_video_device *video, struct video_fmt *vfmt,int index);
struct buffers_pool *buffers_pool_new(struct isp_video_device *video,int index);
int video_get_fmt(struct isp_video_device *video, struct video_fmt *vfmt,int index);
int video_req_buffers(struct isp_video_device *video, struct buffers_pool *pool,int index);
int video_free_buffers(struct isp_video_device *video,int index);
int video_queue_buffer(struct isp_video_device *video, unsigned int buf_id,int index);
int video_stream_on(struct isp_video_device *video,int index);
int video_stream_off(struct isp_video_device *video,int index);
int video_wait_buffer(struct isp_video_device *video, int timeout,int index);
int video_dequeue_buffer(struct isp_video_device *video,struct video_buffer *buffer,int index);
void buffers_pool_delete(struct isp_video_device *video,int index);
void close_device(int index);
int isp_videoopen(struct hw_isp_media_dev *isp_md, unsigned int videoid);
struct hw_isp_media_dev *isp_mdopen(const char *dev_name, int index);

void open_device(int startindx,int indexcnt);
void open_device0123(int startindx,int indexcnt);
void open_device2345(int startindx,int indexcnt);
void open_device012345(int startindx,int indexcnt);

struct ConfigInfo
{
	char key[64];
	char val[128];
};

//获得文件有效行数
int getLines_ConfigFile(FILE *file);
//加载配置文件
void loadFile_ConfigFile(const char *filePath,char ***fileData,int *lines);
//解析配置文件
void parseFile_ConfigFile(char **fileData, int lines, struct ConfigInfo **info);
//获得指定配置信息
char* getInfo_ConfigFile(const char *key, struct ConfigInfo *info,int line);
//释放配置文件信息
void destroInfo_ConfigFile(struct ConfigInfo *info);
//判断当前行是否有效
int isValid_ConfigFile(const char *buf);

/**********************************************************************************************/
/**********************************************************************************************/

#ifdef __cplusplus
}
#endif

/**********************************************************************************************/
/**********************************************************************************************/

unsigned char * buf16_to24(unsigned int * buf,int w,int h);
unsigned char * rgb32to24(unsigned  int * buf,int w,int h);
int encode_jpeg(const char * filename, unsigned char * rgbbuf,short widthPtr, short heightPtr);
/**********************************************************************************************/

void record_process(void *ptr,int RecordWith,int RecordHeith);
void record_process1(void *ptr,int RecordWith,int RecordHeith);
void record_process2(void *ptr,int RecordWith,int RecordHeith);
void record_process3(void *ptr,int RecordWith,int RecordHeith);
void DvrstartRecord(void);
void StopRecord(void);

void DvrInit1Lane(int times,int CameraWith,int CameraHeith,int RecordWith,int RecordHeith,int ml,int pfs,int format);
void DvrInit4Lane(int times,int CameraWith,int CameraHeith,int RecordWith,int RecordHeith,int ml,int pfs,int format);

/**********************************************************************************************/
void LedOnOff(bool flag);

/**********************************************************************************************/
/**********************************************************************************************/
struct KeyState{
	unsigned char  mKbKey;
	unsigned char  mKbMode;  
};

extern struct KeyState Keystate;
extern int iKey ;

void KeyboardInputProcess(struct KeyState Keystate);

/**********************************************************************************************/
/**********************************************************************************************/

int process_image0( void *addr, int length,int id);
int process_image1( void *addr, int length,int id);
int process_image2( void *addr, int length,int id);
int process_image3( void *addr, int length,int id);
int process_image4( void *addr, int length,int id);
int process_image5( void *addr, int length,int id);


/**********************************************************************************************/

#define BAUDRATE0   	38400
#define UART_ENCTY 	"/dev/ttyS5"
#define HEAD_VAL1 	0x2A
#define HEAD_VAL2 	0xA2

#define UART_SEND_BYTE   10

#define UART_BUFLEN  	200

#define uchar unsigned char
//设置保存加密ID的路径文件
#define SAVE_LICENSE_PATH   "/private/idlicense"

void set_save_serial_encty_id_path(char *path);

//tty的值:"ttyS0"~"ttySn"  当前通讯使用的串口
void start_serial_encty_disp(char tty[]);
void wait_serial_exit_disp(void);

/**********************************************************************************************/
void nv21_2_nv61(unsigned char *dst_61, unsigned char *src_21);

int read_file(char *pth); 
int is_file_exist(char *pth);
int is_dir_exist(char *pth);
int read_version_file(char *pth, unsigned char *pVerHigh, unsigned char *pVerLow);
char *get_exe_path(char *buf, int count);
void upgrade_com_init(void);

/**********************************************************************************************/

int cpu_id_check(void);
int atsha204x(int slot_num);
/**********************************************************************************************/

int setcolor(int iHigh,int iLow);
int setbright(void);

/**********************************************************************************************/

#endif

