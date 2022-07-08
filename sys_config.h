#ifndef __SYS_CONFIG_H__
#define __SYS_CONFIG_H__

#define PI 3.14159265358979323846

/*******************************读图开关**************************************************/
#define ISDMA 1 //读图显示打开=====0  //读图显示关闭=====1

#define DVRONOFF 1
#define DVRTIME 5

#define LOGONOFF 0

//======================================================================================
#define READIMG_CALIB 0 //读图测试标定  1---读图标定打开    0---读图标定关闭

//======================================================================================
#define BAUDRATE 115200
#define UART_DEVICE "/dev/ttyS3"
#define FALSE -1
#define TRUE 0
#define BUFLEN 30

/***********************摄像头参数*********************************************************/

#define CUR_PROJECT NV21 // NV61  NV21

#define CAMERA_OUT_FMT 4
#define MAX_VIDEO_NUM 6
#define TIMEOUT 2000

#define CAMERA_WIDTH 1280
#define CAMERA_HEIGHT 720
#define CAMERA_SIZE 1382400 // 1280*720*3/2

#define WIDTH 1280
#define HEIGHT 720

#define DMAFD_ARRAY_NUM 48  // 16===>10
#define EGLIMG_DMAFD_MAX 48 // 32===>10
#define MAX_DMAFD_NUM 48

#define NV21SIZE 1382400
#define CAMERA_UBUFS 8
#define CAMERA_FPS 30

#define OUT_FMT 4
#define MAXVIDEO_NUM 6

#define CAMWER_CUR_FORMATE N21

/****************************UI界面******************************************************/
/**< width and height of screen*/
#define SCREEN_W 1280 // 1920
#define SCREEN_H 720

/**< ROI for AVM(and wide view) on screen */
#define AVM_X 0 // 640
#define AVM_Y 0
#define AVM_W 800
#define AVM_H 480

/**< ROI for left view port(bird-view) on
 *  screen */
#define L_X 0 // 640
#define L_Y 0
#define L_W 297
#define L_H 480

/**< ROI for right view port(2D view and 3D
 * view) on screen */
#define R_X 300 // 380 // 1020 // 380
#define R_Y 120
#define R_W 500 // 900
#define R_H 360 // 720

/**< Left-Top position coordinate of UI
 * ("upgrade" UI and "calib" UI) on screen
 */
#define UI_X 230 // 980
#define UI_Y 190

// adas  png  position coordinate of UI
#define CAM_BX 131
#define CAM_BY 431
#define CAM_LX 36
#define CAM_LY 240
#define CAM_FX 131
#define CAM_FY 35
#define CAM_RX 228
#define CAM_RY 240

#define MOD_BX 112
#define MOD_BY 399
#define MOD_LX 35
#define MOD_LY 108
#define MOD_FX 112
#define MOD_FY 24
#define MOD_RX 212
#define MOD_RY 108
#define MOD_BLX 35
#define MOD_BLY 381
#define MOD_BRX 195
#define MOD_BRY 381
#define MOD_FLX 35
#define MOD_FLY 24
#define MOD_FRX 195
#define MOD_FRY 24

#define BSD_LX 43
#define BSD_LY 384
#define BSD_RX 219
#define BSD_RY 384

#define MOD_LITTLE_CAR_X 300
#define MOD_LITTLE_CAR_Y 372
#define MOD_CHECK_SURD_X 384
#define MOD_CHECK_SURD_Y 408
#define BACK_X 700
#define BACK_Y 396

//旋转
#define ANGLE_STEP 3
#define IDX_TRAJ_FRONT 270 / ANGLE_STEP
#define IDX_TRAJ_BACK 90 / ANGLE_STEP
#define IDX_TRAJ_LEFT 225 / ANGLE_STEP
#define IDX_TRAJ_RIGHT 315 / ANGLE_STEP

// Touch effective distance
#define TOUCH_MOVE_ADD_DISTANCE 2
#define TOUCH_MOVE_DEC_DISTANCE -2

#define RORATE_MAX_VAL 2
#define RORATE_MIN_VAL -2

//===============================================================
//================================================================================
//================================================================================
//================================================================================
#define CARD_COUNT 5                   //卡容量检侧 现在是100S检侧一次
#define CARD_CAPACITY_DETEC_TIME 10000 //卡容量检侧 10MS为基数 现在是100S检侧一次
#define CARCD_FILE_DISP_NUM 600        // 1---1MB   100--100M 卡空间少于100M的进行处理

#define MOUNT_DIR1 "/extp/sdcard/mmcblk1p1"
#define MOUNT_DIR2 "/extp/sdcard/mmcblk1"

#define APP_PACKET_DIR MOUNT_DIR1 "/APP"
#define OS_PACKET_DIR MOUNT_DIR1 "/OS"
#define MCU_UPDATE_NAME MOUNT_DIR1 "/MCU"  // MOUNT_DIR"/mcuAPP.bin"
#define MCU_UPDATE_NAME1 MOUNT_DIR2 "/MCU" // MOUNT_DIR"/mcuAPP.bin"

//////////////////////parameters for 6 lane demostration
//////////////////////各个模态下的窗口参数
#define LANE6_STITCHING_WIN_X 0
#define LANE6_STITCHING_WIN_Y 0
#define LANE6_STITCHING_WIN_W 480
#define LANE6_STITCHING_WIN_H 720

#define SIXSCREEN_SUBWIN_X 480
#define SIXSCREEN_SUBWIN_Y 0
#define SIXSCREEN_SUBWIN_W 1440
#define SIXSCREEN_SUBWIN_H 720

#define LEFTUP_SUBWIN_X 0
#define LEFTUP_SUBWIN_Y (SCREEN_H - 300)
#define LEFTUP_SUBWIN_W 400
#define LEFTUP_SUBWIN_H 300

#define RIGHTUP_SUBWIN_X 1520
#define RIGHTUP_SUBWIN_Y (SCREEN_H - 300)
#define RIGHTUP_SUBWIN_W 400
#define RIGHTUP_SUBWIN_H 300

#define LEFT_SUBWIN_X 0
#define LEFT_SUBWIN_Y 0
#define LEFT_SUBWIN_W 960
#define LEFT_SUBWIN_H 720

#define RIGHT_SUBWIN_X 960
#define RIGHT_SUBWIN_Y 0
#define RIGHT_SUBWIN_W 960
#define RIGHT_SUBWIN_H 720

///////////////////ROI Region of different single view//////////////////////
////////////////////sub six lane display
#define SUB6LANE_BACK_X 0
#define SUB6LANE_BACK_Y 0
#define SUB6LANE_BACK_W 1280
#define SUB6LANE_BACK_H 720

#define SUB6LANE_LEFT_X 0
#define SUB6LANE_LEFT_Y 0
#define SUB6LANE_LEFT_W 1280
#define SUB6LANE_LEFT_H 720

#define SUB6LANE_FRONT_X 0
#define SUB6LANE_FRONT_Y 0
#define SUB6LANE_FRONT_W 1280
#define SUB6LANE_FRONT_H 720

#define SUB6LANE_RIGHT_X 0
#define SUB6LANE_RIGHT_Y 0
#define SUB6LANE_RIGHT_W 1280
#define SUB6LANE_RIGHT_H 720

#define SUB6LANE_LEFT1_X 0
#define SUB6LANE_LEFT1_Y 0
#define SUB6LANE_LEFT1_W 1280
#define SUB6LANE_LEFT1_H 720

#define SUB6LANE_RIGHT1_X 0
#define SUB6LANE_RIGHT1_Y 0
#define SUB6LANE_RIGHT1_W 1280
#define SUB6LANE_RIGHT1_H 720

//////////////////////six lane display for all screen
#define SIX_LANE_BACK_X 0
#define SIX_LANE_BACK_Y 0
#define SIX_LANE_BACK_W 1280
#define SIX_LANE_BACK_H 720

#define SIX_LANE_LEFT_X 0
#define SIX_LANE_LEFT_Y 0
#define SIX_LANE_LEFT_W 1280
#define SIX_LANE_LEFT_H 720

#define SIX_LANE_FRONT_X 0
#define SIX_LANE_FRONT_Y 0
#define SIX_LANE_FRONT_W 1280
#define SIX_LANE_FRONT_H 720

#define SIX_LANE_RIGHT_X 0
#define SIX_LANE_RIGHT_Y 0
#define SIX_LANE_RIGHT_W 1280
#define SIX_LANE_RIGHT_H 720

#define SIX_LANE_LEFT1_X 0
#define SIX_LANE_LEFT1_Y 0
#define SIX_LANE_LEFT1_W 1280
#define SIX_LANE_LEFT1_H 720

#define SIX_LANE_RIGHT1_X 0
#define SIX_LANE_RIGHT1_Y 0
#define SIX_LANE_RIGHT1_W 1280
#define SIX_LANE_RIGHT1_H 720

//////////////////////ROI REGION OF SINGLE VIEW
#define SINGLEVIEW_RIGHT_X 0
#define SINGLEVIEW_RIGHT_Y 0
#define SINGLEVIEW_RIGHT_W 1280
#define SINGLEVIEW_RIGHT_H 720

#define SINGLEVIEW_LEFT_X 0
#define SINGLEVIEW_LEFT_Y 0
#define SINGLEVIEW_LEFT_W 1280
#define SINGLEVIEW_LEFT_H 720

#define SINGLEVIEW_FRONT_X 232
#define SINGLEVIEW_FRONT_Y 0
#define SINGLEVIEW_FRONT_W (1280 - 464)
#define SINGLEVIEW_FRONT_H 720

#define SUB_SINGLEVIEW_RIGHT_X 0
#define SUB_SINGLEVIEW_RIGHT_Y 0
#define SUB_SINGLEVIEW_RIGHT_W 1280
#define SUB_SINGLEVIEW_RIGHT_H 720

#define SUB_SINGLEVIEW_LEFT_X 0
#define SUB_SINGLEVIEW_LEFT_Y 0
#define SUB_SINGLEVIEW_LEFT_W 1280
#define SUB_SINGLEVIEW_LEFT_H 720

#define SINGLEVIEW_R_HALFSCREEN_RIGHT_X 0
#define SINGLEVIEW_R_HALFSCREEN_RIGHT_Y 0
#define SINGLEVIEW_R_HALFSCREEN_RIGHT_W 1280
#define SINGLEVIEW_R_HALFSCREEN_RIGHT_H 720

#define SINGLEVIEW_L_HALFSCREEN_LEFT_X 0
#define SINGLEVIEW_L_HALFSCREEN_LEFT_Y 0
#define SINGLEVIEW_L_HALFSCREEN_LEFT_W 1280
#define SINGLEVIEW_L_HALFSCREEN_LEFT_H 720

///////////////////png painting size/////////////
#define CAMERA_CHECKIMG_W 204
#define CAMERA_CHECKIMG_H 27

#define ATTENTION_AROUNDIMG_W 48
#define ATTENTION_AROUNDIMG_H 274

#define ONECHAR_CAMERALABEL_IMG_W 56
#define ONECHAR_CAMERALABEL_IMG_H 53

#define TWOCHAR_CAMERALABEL_IMG_W 96
#define TWOCHAR_CAMERALABEL_IMG_H 53

#define LONG_HORIZONTAL_LINE_W 1920
#define LONG_HORIZONTAL_LINE_H 3

#define SHORT_HORIZONTAL_LINE_W 1442
#define SHORT_HORIZONTAL_LINE_H 3

#define VERTICAL_LINE_W 16
#define VERTICAL_LINE_H 720

#define UPSIDE_WIN_SIZE_W 400
#define UPSIDE_WIN_SIZE_H 301

////////////////ui img position///////////
#define ATTENTION_AROUNDUI_X 0
#define ATTENTION_AROUNDUI_Y 222
/////////////在六分屏中，上部摄像头index图片下边缘y值，坐标系原点为屏幕左下角。图片
#define SIXSCREEN_UPY 648
/////////////在六分屏中，下部摄像头index图片下边缘y值，坐标系原点为屏幕左下角。图片
#define SIXSCREEN_LOWY 288

/////////////不包含拼接方式的六宫格显示模式中，最左侧摄像头index图片左边缘x值
#define FULLSCREEN_LEFTDOWNIMG_X 22
#define FULLSCREEN_LEFTDOWNIMG_Y SIXSCREEN_UPY

#define FULLSCREEN_LEFTBACKIMG_X FULLSCREEN_LEFTDOWNIMG_X
#define FULLSCREEN_LEFTBACKIMG_Y SIXSCREEN_LOWY
// 934
/////////////不包含拼接方式的六宫格显示模式中，中间摄像头index图片左边缘x值
#define FULLSCREEN_FRONTIMG_X (SCREEN_W / 3 + 22)
#define FULLSCREEN_FRONTIMG_Y SIXSCREEN_UPY

#define FULLSCREEN_BACKIMG_X FULLSCREEN_FRONTIMG_X
#define FULLSCREEN_BACKIMG_Y SIXSCREEN_LOWY
// 1805
/////////////不包含拼接方式的六宫格显示模式中，右间摄像头index图片左边缘x值
#define FULLSCREEN_RIGHTDOWNIMG_X (SCREEN_W / 3.0 * 2 + 22)
#define FULLSCREEN_RIGHTDOWNIMG_Y SIXSCREEN_UPY

#define FULLSCREEN_RIGHTBACKIMG_X FULLSCREEN_RIGHTDOWNIMG_X
#define FULLSCREEN_RIGHTBACKIMG_Y SIXSCREEN_LOWY

/////////////包含拼接方式的六宫格显示模式中，左间摄像头index图片左边缘x值
#define SUBSCREEN_LEFTDOWNIMG_X (LANE6_STITCHING_WIN_W + 22)
#define SUBSCREEN_LEFTDOWNIMG_Y SIXSCREEN_UPY

#define SUBSCREEN_LEFTBACKIMG_X SUBSCREEN_LEFTDOWNIMG_X
#define SUBSCREEN_LEFTBACKIMG_Y SIXSCREEN_LOWY

/////////////包含拼接方式的六宫格显示模式中，中间摄像头index图片左边缘x值
#define SUBSCREEN_FRONTIMG_X \
  ((SCREEN_W - LANE6_STITCHING_WIN_W) / 3.0 + (LANE6_STITCHING_WIN_W + 22))
#define SUBSCREEN_FRONTIMG_Y SIXSCREEN_UPY

#define SUBSCREEN_BACKIMG_X SUBSCREEN_FRONTIMG_X
#define SUBSCREEN_BACKIMG_Y SIXSCREEN_LOWY

/////////////包含拼接方式的六宫格显示模式中，右间摄像头index图片左边缘x值
#define SUBSCREEN_RIGHTDOWNIMG_X \
  ((SCREEN_W - LANE6_STITCHING_WIN_W) / 3.0 * 2 + (LANE6_STITCHING_WIN_W + 22))
#define SUBSCREEN_RIGHTDOWNIMG_Y SIXSCREEN_UPY

#define SUBSCREEN_RIGHTBACKIMG_X SUBSCREEN_RIGHTDOWNIMG_X
#define SUBSCREEN_RIGHTBACKIMG_Y SIXSCREEN_LOWY

#define SINGLE_FRONTIMG_X 22
#define SINGLE_FRONTIMG_Y SIXSCREEN_UPY

#define SUBVIEW_RIGHTDOWNIMG_X 1805
#define SUBVIEW_RIGHTDOWNIMG_Y SIXSCREEN_UPY

#define SINGLEVIEW_RIGHTBACKIMG_X SUBVIEW_RIGHTDOWNIMG_X
#define SINGLEVIEW_RIGHTBACKIMG_Y 19

#define SUBVIEW_LEFTDOWNIMG_X 22
#define SUBVIEW_LEFTDOWNIMG_Y SIXSCREEN_UPY

#define SINGLEVIEW_LEFTBACKIMG_X SUBVIEW_LEFTDOWNIMG_X
#define SINGLEVIEW_LEFTBACKIMG_Y SINGLEVIEW_RIGHTBACKIMG_Y

#define LEFTSUBVIEW_LEFTDOWN_X 825
#define LEFTSUBVIEW_LEFTDOWN_Y SIXSCREEN_UPY

#define RIGHTSUBVIEW_RIGHTDOWN_X 1002
#define RIGHTSUBVIEW_RIGHTDOWN_Y SIXSCREEN_UPY

/////////////////////4lane、6lane兼容//////////////////
//#define SYS_FOUR_LANE
#ifndef SYS_FOUR_LANE
//#define DYNAMIC_CAR_MODEL
//#define SHOW_CAR_BOTTOM
#endif
#endif
