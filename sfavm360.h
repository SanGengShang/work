/******************************************************************************

                  XCB 版权所有 (C), 2017-2020,

 ******************************************************************************
  文 件 名   :
  版 本 号   : V1.0
  作    者   :
  生成日期   : 2019年4月25日
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2020年4月1日
    作    者    : pengtao
    修改内容   : 创建文件

******************************************************************************/
#ifndef _sf_avm360_H_
#define _sf_avm360_H_

#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <error.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <linux/fb.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/types.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <semaphore.h>

#include "drm_fourcc.h"
#include "isp.h"
#include "isp_dev.h"
#include "video.h"
#include "video_hal.h"
#include "xcb_ui.h"

#include "sys_config.h"
#include "xcb_ui.h"
#include "Ini.h"
#include "Light_avg.h"
//#include "serial_encty.h"
//#include "xcb_auth_embed.h"
//#include "xcb_autocalib.h"

#include "xcb_videoprot5.h"

#include "XCB_360stitching.h"
#include "serial.h"
#include "xcb_after_autocalib.h"
#include "xcb_avmlib.h"
#include <sys/time.h>
#include "file_list.h"
#include <time.h>
#include "irremotectrl.h"
/**********************************************************************************************/
IrRemoteCtrl *irkey;
CIni ini;
int runRender = 1;
int fifo_fd = -1;
int qtRun = 0;
enum
{
  leftback = 0,
  rightback,
  back,
  front,
  leftdown,
  rightdown
};
/**********************************************************************************************/
char dev_name[32];
#define FILE_VIDEO "/dev/video"

struct vi_info
{
  int Chn;
  int isp_id;
  int s32MilliSec;
  struct video_fmt vfmt;
  struct video_buffer buffer;
};

struct video_buffer *buf0 = NULL;
struct video_buffer *buf1 = NULL;
struct video_buffer *buf2 = NULL;
struct video_buffer *buf3 = NULL;
struct video_buffer *buf4 = NULL;
struct video_buffer *buf5 = NULL;

video_buf_t vb0;
video_buf_t vb1;
video_buf_t vb2;
video_buf_t vb3;
video_buf_t vb4;
video_buf_t vb5;

struct hw_isp_media_dev *media0 = NULL;

//struct timeval app_start, app_end;

/**********************************************************************************************/
#define CUSTOM_COUNT 1

int finish_flag0 = 0;
int finish_flag1 = 0;
int finish_flag2 = 0;
int finish_flag3 = 0;
int finish_flag4 = 0;
int finish_flag5 = 0;

int record_start_flag = 0;
//////////////////////////////////////////////////////
int app_start_flag = 0;
int app_display_flag = 0;
//测试APP启动时间
struct timeval app_start, app_end;
//////////////////////////////////////////////////////

pthread_t tidDvr0[CUSTOM_COUNT];
pthread_t tidDvr1[CUSTOM_COUNT];
pthread_t tidDvr2[CUSTOM_COUNT];
pthread_t tidDvr3[CUSTOM_COUNT];

void record_process(void *ptr);
void record_process1(void *ptr);
void record_process2(void *ptr);
void record_process3(void *ptr);

void RecordCamera_init(void);

/**********************************************************************************************/
/**********************************************************************************************/
// thread for getting data of front camera
pthread_t g_thdCamera0;
// thread for getting data of back camera
pthread_t g_thdCamera1;
// thread for getting data of left camera
pthread_t g_thdCamera2;
// thread for getting data of right camera
pthread_t g_thdCamera3;

pthread_t g_thdCamera4;
pthread_t g_thdCamera5;

// thread for calibration
pthread_t g_thdCalib;
// thread for BSD
pthread_t g_thdBSD;
pthread_t g_thdFCW;

// thread for LDW MOD
pthread_t g_thdLDWMOD;
pthread_t g_thdLightBala;
pthread_t g_thdother;
pthread_t g_thTestSerial;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexin = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_light = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t get_data1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t get_data2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t get_data3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t get_data4 = PTHREAD_MUTEX_INITIALIZER;

static sem_t sema[6];
static sem_t sema1[6];

int adas_fun_flag = 0;
int display_flag = 0;

/********************************SD
 * check************************************************/

char skin_file_name[] = {"/extp/app/skin_para.txt"};
int save_skin_para = XCB_AVM_CAR_COLOR5;

typedef enum
{
  REMOVE = 0,
  PLUG_IN = !REMOVE
} SDstatus;
int sd_status = REMOVE;
int TF_status = REMOVE;

/********************************************************************************/
/***************************upgrade********************************/

struct timeval g_tUpgradeFailTime;
bool g_bUpgradeFail = false;

typedef enum upgrade_status_enum
{
  XCB_AVM_UPGRADE_IDLE = 0,
  XCB_AVM_UPGRADE_DETECTED_SUCCESS,
  XCB_AVM_UPGRADE_DETECTED_FAILED,
  XCB_AVM_UPGRADE_INVALID_FILE,
  XCB_AVM_UPGRADE_INVALID_VERSION,
  XCB_AVM_UPGRADE_APPING1,
  XCB_AVM_UPGRADE_APPING2,
  XCB_AVM_UPGRADE_APPING3,
  XCB_AVM_UPGRADE_SUCCESS,
  XCB_AVM_UPGRADE_FAILED,
  XCB_AVM_UPGRADE_SYSTEM
} xcb_avm_upgrade_status_e;

extern xcb_avm_upgrade_status_e g_upgrade_status;

/***************************calibration********************************/
typedef enum calib_status_enum
{
  XCB_AVM_CALIB_IDLE = 0,
  XCB_AVM_CALIB_STARTING,
  XCB_AVM_CALIB_FINISH,
  XCB_AVM_CALIB_LEFT_FAIL,
  XCB_AVM_CALIB_RIGHT_FAIL,
  XCB_AVM_CALIB_BACK_FAIL,
  XCB_AVM_CALIB_LEFT_BACK_FAIL,
  XCB_AVM_CALIB_RIGHT_BACK_FAIL,
  XCB_AVM_CALIB_FRONT_FAIL
} xcb_avm_calib_status_e;

xcb_avm_calib_status_e g_avm_status = XCB_AVM_CALIB_IDLE;
xcb_after_calib_error_s g_calib_err_code;

bool g_bCalibFinish = false;
bool g_bCalibStart = false;
bool g_bCalibState = false;
//===============================================================================
//================================serial=========================================
serial g_port;

/**********************************************************************/
EGLDisplay g_egl_display;
EGLSurface g_egl_surface;

avm_screen_type_t g_avm_screen;

/**< 3D view camera parameter, and ROI on screen
 * for showing */
ViewParameter g_t3DViewParams;
/**< camera parameter of 2D single view(undistorted
 * front/back view) and front side view(both left
 * and right), and ROI on screen for showing */
ViewParameter g_t2DViewParams;
/**< camera parameter of front left side view,
 * and ROI on screen for showing */
ViewParameter g_tFLViewParams;
/**< camera parameter of front right side view,
 * and ROI on screen for showing */
ViewParameter g_tFRViewParams;

/*
for single view window and subviewwindow
*/
ViewParameter g_tSingleViewParams;
ViewParameter g_tSubSingleView1Params;
ViewParameter g_tSubSingleView2Params;

ViewParameter g_tSubSideViewLeftParams;
ViewParameter g_tSubSideViewRightParams;

#if ISDMA == 1
/**< 4 textures for 4 cameras*/
#define NUM_TEXTURE 8
GLuint g_texture_id[NUM_TEXTURE];
#else
/**< 4 textures for 4 cameras*/
#define NUM_TEXTURE 12
GLuint g_texture_id[NUM_TEXTURE];

constexpr int size_nv61 = 1280 * 720 * 2;
constexpr int size_nv21 = 1280 * 720 * 1.5;
unsigned char pNV61_B[size_nv61];
unsigned char pNV61_L[size_nv61];
unsigned char pNV61_F[size_nv61];
unsigned char pNV61_R[size_nv61];
unsigned char pNV61_L1[size_nv61];
unsigned char pNV61_R1[size_nv61];

unsigned char pNV21_B[size_nv21];
unsigned char pNV21_L[size_nv21];
unsigned char pNV21_F[size_nv21];
unsigned char pNV21_R[size_nv21];
unsigned char pNV21_L1[size_nv21];
unsigned char pNV21_R1[size_nv21];
#endif

GLuint texture_id_multi[1];

int car_skin_default = XCB_AVM_CAR_COLOR5;
int car_3dskin_default = XCB_AVM_CAR_COLOR5;

int old_3d_view = -1;
int turn_around = 0;
int turn_angle = 0;
int step_count = 1;
bool g_bTouchMove = false;
int Tire_Frame_Step = 0;
int Tire_Frame_Rate = 0;
constexpr int size_ti = 1280 * 720;
unsigned char LIGHTIMG[4][size_ti];
float usrfratio[4] = {1.0, 1.0, 1.0, 1.0};
pthread_rwlock_t rwlock; //定义读写锁

/**< Index of camera's moving trajectory points
 * only used in 3D view
 */
int g_nIdxTraj = IDX_TRAJ_FRONT;
/**< Number of camera's moving trajectory points
 * (with one circle) only used in 3D view
 */
int g_nNumTraj = 0;
/**< Points coordinate of camera's moving trajectory
 * (with one circle) only used in 3D view
 */
double *g_pTrajPts = NULL;
/**< Index of camera's moving trajectory points
 * for 3D front/back/left/right view
 */

/************************stitching******************************/
avm_stitching_type_t avm_stitching_type;
avm_bottomImg_type_t car_head_texture;
avm_bottomImg_type_t car_body_texture;
/**< 2D and 3D stitching */
avm_stitching_type_t g_stitching_Handle;
/**< 2d car, 2d bottom, 3d car and 3d bottom */
avm_car_type_t g_car_Handle;
/**< single front/back/left/right view, side view and
 * wide view
 */
avm_single_type_t g_single_Handle;
/**< 3D car state */
avm_car_state_type_t g_car_state_Handle;
/**< trajectory line*/
avm_lines_type_t g_line_handle;
/**< camera state */
avm_camera_type_t g_camera_type;
/**< adas line rect handle */
adas_auxiliary_type_t adasHandle;

///////////静态轨迹线设置

png_texture_t line_red;
png_texture_t line_yellow;
png_texture_t line_green;

/**< TODO  Test*/
float matrix_F[9] = {-36.001579, -2017.977011, 4010.577319,
                     108.799132, -1945.798321, 3729.453214,
                     0.027623, -0.502215, 1.000000};
float matrix_B[9] = {365.989561, 2283.871423, 3945.201422,
                     187.185748, 2226.842337, 3657.633380,
                     0.047656, 0.576244, 1.000000};

/***************************UI*********************************/

/**< UI for calibration and upgrading */

avm_ui_type_t avmUIHandle;
avm_bottomImg_type_t avm2DCarHandle;
avm_bottomImg_type_t avmBottomHandle;
png_texture_t g_png_background;
png_texture_t *g_png_calib;
png_texture_t *g_png_cam_err;
png_texture_t *g_png_upgrade;
png_texture_t *g_png_interface;
png_texture_t *g_png_show;
png_texture_t *g_png_skin;

avm_bottomImg_type_t avm2DCarHeadBottom;
avm_bottomImg_type_t avm2DCarBodyBottom;
/////////////////////////////mcu update////////////////////////
png_texture_t *g_png_mcuupgrade;
avm_ui_type_t avmUIMCUHandleHandle;

png_texture_t *g_png_modB;
png_texture_t *g_png_modF;
png_texture_t g_png_modL;
png_texture_t g_png_modR;
png_texture_t g_png_bsdL;
png_texture_t g_png_bsdR;
png_texture_t g_png_fcw;
png_texture_t track_layer;

//===============================================================
/**< Four EGLImageTargets for front, back, left and right respectively */
GLeglImageOES g_elgImage[MAXVIDEO_NUM];

typedef struct eglimg_dmafd_
{
  EGLImageKHR eglimg;
  int dmafd;
} eglimg_dmafd_t;

static eglimg_dmafd_t g_eglimg_dmafd_array[EGLIMG_DMAFD_MAX];

//==============================================================

/***********************Fault information***********************/
/**< false - camera have fault, true - camera is ok  */
/**< Back camera have fault or not */
bool g_bCamB = true;
/**< Left camera have fault or not */
bool g_bCamL = true;
/**< Front camera have fault or not */
bool g_bCamF = true;
/**< Right camera have fault or not */
bool g_bCamR = true;
/**< Left back camera have fault or not */
bool g_bCamL1 = true;
/**< Right back camera have fault or not */
bool g_bCamR1 = true;

////////////////////////每个窗口下的viewparameter的参数//////////////////
/**< Bird view camera parameter, and ROI on screen
 * for showing */
ViewParameter g_tBirdViewParams;
ViewParameter g_tBirdViewParamsfullscreen;

ROIParameter AllScrenSixROI[6];
ROIParameter SubScreenSixROI[6];

ROIParameter SingleViewRightROI;
ROIParameter SingleViewLeftROI;
ROIParameter SingleViewFrontROI;

ROIParameter SubSingleViewRightROI;
ROIParameter SubSingleViewLeftROI;

ROIParameter RHalfSingleViewRightROI;
ROIParameter LHalfSingleViewLeftROI;

///////////////////ui images
avm_ui_type_t avmLongHLineUIHandle;
avm_ui_type_t avmShortHLineUIHandle;
avm_ui_type_t avmVLineUIHandle;
avm_ui_type_t avmUpSubWinUIHandle;
avm_ui_type_t avmOneCharLabelUIHandle;
avm_ui_type_t avmTwoCharLabelUIHandle;
avm_ui_type_t avmCameraCheckUIHandle;
avm_ui_type_t avmAttentionAroundUIHandle;

/////////////rec time images
png_texture_t g_png_RecTimeNum[10];
png_texture_t g_png_RecTimeColon;
png_texture_t g_png_RecTimeDash;
unsigned char *pRecNumber[10];
unsigned char *pRecColon;
unsigned char *pRecDash;
avm_ui_type_t avmRadarAlarmUIHandle;
png_texture_t g_png_RadarAlarm[3]; // 0,orange; 1, yellow; 2, red

png_texture_t g_png_LongHLine;
png_texture_t g_png_ShortHLine;
png_texture_t g_png_VLine;
png_texture_t g_png_UpSubWinImg[2];     //////////0: left up; 1: right up
png_texture_t g_png_OneCharLabelImg[2]; ///////////0: 前；1：后
png_texture_t g_png_TwoCharLabelImg[4]; ///////////0：左下；1：右下；2：左后；3：右后
png_texture_t g_png_CameraCheckImg;
png_texture_t g_png_AttentionAroundImg;

///////////////////version
png_texture_t *g_png_version;
png_texture_t *g_png_mcuNumH;
png_texture_t *g_png_mcuNumL;
png_texture_t *g_png_AnglemcuNumH;
png_texture_t *g_png_AnglemcuNumL;

png_texture_t *g_png_cpuNumH;
png_texture_t *g_png_cpuNumL;
png_texture_t *g_png_cpuNumL2;

avm_ui_type_t avmUIVersionbg;
avm_ui_type_t *avmUIVersionNum;

unsigned char gmcuh = 0, gmcul = 0;

unsigned char gCpuVer1 = 0, gCpuVer2 = 0;
unsigned char ganglemcu[6] = {0, 0, 0, 0, 0, 0};

EGLSyncKHR syncx;
/***************************MOD********************************/

/******************************Touch***************************/
constexpr uint16_t g_u16CarCenterX = R_X + R_W / 2;
constexpr uint16_t g_u16CarCenterY = R_Y + R_H / 2;

bool g_bTouchDown = false;
uint16_t g_u16LastX;
uint16_t g_u16LastY;

/* hide show icon disp */
bool g_hide_flag = false;
int g_hide_count = 0;

/**********************wheel rolling angle*********************/
int g_nWheelRollAngle = 0;

//==============================================================
#define PATH_MAX 500
char g_pAvmPath[500];
char g_pCreateModelDir[500];
char g_pGivenModelDir[500];
char g_pPngDir[500];
char g_pCalibConfigDir[500];
int skin_init_flag = 0;
int mode_load_3d = 0;

//=========================================================================
static int search_dmafd(int dmafd, EGLImageKHR *p_img, int *p_index);
int setup_dmafd2eglimg(EGLDisplay egl_display, int dma_fd, int dmafd_w,
                       int dmafd_h, EGLImageKHR *eglImage, int index);
GLeglImageOES bind_dmafd(video_buf_t video_buf, EGLDisplay dpy);

void *CameraDisplay0(void *args);
void *CameraDisplay1(void *args);
void *CameraDisplay2(void *args);
void *CameraDisplay3(void *args);

int init_eglimg_dmafd(void);

void avm_ui_init(void);
void render_init(void);
void avm_img_roi_init(void);

void avm_cam_ui_process(void);
void avm_calib_ui_process(void);
void avm_upgrade_ui_process(void);

void view_2d_process(void);
void view_3d_process(void);

/**< Render bird view
 *
 * @param[in] bFrontTraj: trajectory is at front or back,
 *  true - front, false - back
 */
void render_bird_view(void);
void render_3D_view(int idxTraj);
void render_traj_line(bool bFrontTraj, bool bRightShow, float fDegree);

void calib_update(void);
void upgrade_proc(void);

unsigned char *pStitchingIm[6];
unsigned char pImgForSaveT[1382400];
unsigned char pImgForSave[1382400];
unsigned char pImgForSaveT1[1382400];
unsigned char pStitchingImgT[172800];
unsigned char pStitchingImg[240 * 480 * 2];

////////////////////2D plane model
void *pModel2dP;

//#define DEBUG
#ifdef DEBUG
#define debug(format, ...)                                                  \
  printf("Info:[%s:%s(%d)]:" format "\n", __FILE__, __FUNCTION__, __LINE__, \
         ##__VA_ARGS__);
#else
#define debug(format, ...) ;
#endif

char copy_file_path[] = {"/extp/sdcard/mmcblk1p1/COPY/copy_file.sh"};

//////////time for rec
unsigned char curhourH = 0;
unsigned char curhourL = 0;
unsigned char curminuteH = 0;
unsigned char curminuteL = 0;
unsigned char cursecH = 0;
unsigned char cursecL = 0;

unsigned char curmonH = 0;
unsigned char curmonL = 0;
unsigned char curdayH = 0;
unsigned char curdayL = 0;
unsigned char curyearH = 0;
unsigned char curyearL = 0;

unsigned char lasthourH = 0;
unsigned char lasthourL = 0;
unsigned char lastminuteH = 0;
unsigned char lastminuteL = 0;
unsigned char lastsecH = 0;
unsigned char lastsecL = 0;

unsigned char lastmonH = 0;
unsigned char lastmonL = 0;
unsigned char lastdayH = 0;
unsigned char lastdayL = 0;
unsigned char lastyearH = 0;
unsigned char lastyearL = 0;

///////////coordinates of time label
#define REC_TIME_Y 30

#define REC_TIME_MONTH_HX 210 //80  //100
#define REC_TIME_MONTH_LX 230 //110 //125

#define REC_TIME_DAY_HX 260 //150 //160
#define REC_TIME_DAY_LX 280 //180 //185

#define REC_TIME_YEAR_HX1 310 //220
#define REC_TIME_YEAR_HX0 330 //250 //245

#define REC_TIME_YEAR_LX1 350 //280 //270
#define REC_TIME_YEAR_LX0 370 //310 //295
////////////////////////
#define REC_TIME_HOUR_HX 430 //360 //340
#define REC_TIME_HOUR_LX 450 //390 //365

#define REC_TIME_MINUTE_HX 480 //430 //400
#define REC_TIME_MINUTE_LX 500 //460 //425

#define REC_TIME_SEC_HX 530 //500 //460
#define REC_TIME_SEC_LX 550 //530 //485

#define REC_TIME_DASH_X0 246 //133 //140
#define REC_TIME_DASH_X1 296 //203 //200

#define REC_TIME_COLON_X0 464 //413 //380
#define REC_TIME_COLON_X1 514 //483 //440

/////////////////////for read car information/////////////////
float fInCarLength = 15600;
float fInCarHeadLength = 6950;
float fInCarBodyLength = 12900;
float fInCarHeadWidth = 1650 * 2;
float fInCarWidth = 1650 * 2;
float fInAxis2carcenter = 0;
float fHalfCarDeltaLenth = 100;
float fHalfCarDeltaWidth = 100;

////////////////////获取拼接数据
avm_fboscreen_type_t avm_fboscreen_type;
avm_fboColorTransfer_type_t avm_fboColorTransfer_type;
int nImgIndex = 0;
#endif /*_sfavm360_H_*/
