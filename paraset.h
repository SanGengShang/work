/******************************************************************************

                  xcb 版权所有 (C), 2017-2020,

 ******************************************************************************
  文 件 名   :
  版 本 号   : V1.0
  作    者   :
  生成日期   : 2019年4月25日
  最近修改   :
  功能描述   :
  函数列表   :
  修改历史   :
  1.日    期   : 2019年4月25日
    作    者    : pengtao
    修改内容   : 创建文件

******************************************************************************/
#ifndef PARASET_H
#define PARASET_H

#include "stdint.h"
#include <iostream>
#include <vector>
using namespace std;

#define uchar unsigned char

#define head_val1 0x55
#define head_val2 0xAA

#define MSG 0x00 //帧类型
#define CMD 0x01
#define ACK 0x02

typedef union {
  uchar whole;
  struct {
    uchar bit0 : 1;
    uchar bit1 : 1;
    uchar bit2 : 1;
    uchar bit3 : 1;
    uchar bit4 : 1;
    uchar bit5 : 1;
    uchar bit6 : 1;
    uchar bit7 : 1;
  } bits;
} utype;

typedef struct {
  uchar head_1;
  uchar head_2;
  uchar Length;
  uchar Type;
  uchar msgID;
  vector<utype> msgData;
  uchar cs;
} frameType;

typedef struct {
  uchar Type;
  uchar msgID;
} revDataPrefix;

/*.......解析的数据，包括类型、ID和内容...........*/
typedef struct {
  revDataPrefix msgPrefix;
  vector<utype> msgData;
} frameCont;

/*************************************************************/
/*                  Message from MCU to T7                   */
/*************************************************************/


/* Message type from MCU to T7
 *
 */
typedef enum xcb_avm_msg_type_t7_s {
  XCB_AVM_MSG_T7_VERSION_INFO,  /**< Version information*/
  XCB_AVM_MSG_T7_CAR_STATE,     /**< Car state */
  XCB_AVM_MSG_T7_TOUCH_OPERATE, /**< Touch operation */
  XCB_AVM_MSG_T7_VIEW_SWITCH,   /**< View switch: back/front view ... */
  XCB_AVM_MSG_T7_2D_3D_VIEW,    /**< Right view port is 2D or 3D view */
  XCB_AVM_MSG_T7_CALIB,         /**< Calibration */
  XCB_AVM_MSG_T7_CAR_COLOR,     /**< Car color switch*/

  XCB_AVM_MSG_T7_ADJUST_FRONT_PARA,      /* ADJUST PARA*/
  XCB_AVM_MSG_T7_ADJUST_BACK_PARA,       /* ADJUST PARA*/
  XCB_AVM_MSG_T7_ADJUST_WIDEFRONT_PARA,  /* ADJUST PARA*/
  XCB_AVM_MSG_T7_ADJUST_WIDEBACK_PARA,   /* ADJUST PARA*/
  XCB_AVM_MSG_T7_ADJUST_SLIDERVIEW_PARA, /* ADJUST PARA*/
  XCB_AVM_MSG_T7_ADJUST_RIGHT_PARA,      /* ADJUST PARA*/
  XCB_AVM_MSG_ADJ_3D_PARA,
  XCB_AVM_READ_TXT_ADJ_PARA,
  XCB_AVM_BIRDVIEW_PARA,   //BirdView
  XCB_AVM_ADJ_CAMER_2D_PARA,
  XCB_AVM_ADJ_CAMER_3D_PARA,
  XCB_AVM_ADJ_CAMER_FL_PARA,
  XCB_AVM_ADJ_CAMER_FR_PARA,
  XCB_AVM_ADJ_VIRTUAL_CAMER_PARA,
  XCB_AVM_MSG_T7_ADJUST_LEFT_PARA,
  //XCB_AVM_READ_TXT_ADJ_PARA,
  XCB_AVM_MSG_T7_ENERGE_MODE,    /**< Energe mode, reboot/shut down*/
  /////////for xingtu use
  XCB_AVM_MSG_T7_HEADANGLE,
  XCB_AVM_MSG_T7_ANGLEVERSION

} xcb_avm_msg_type_t7_t;

typedef enum xcb_avm_car_state_gear_s {
  XCB_AVM_CAR_STATE_GEAR_PARK,    /**< 0/0x00, P gear(Parking gear) */
  XCB_AVM_CAR_STATE_GEAR_REVERSE, /**< 1/0x01, R gear(Reverse gear) */
  XCB_AVM_CAR_STATE_GEAR_NEUTRAL, /**< 2/0x02, N gear(Neutral gear) */
  XCB_AVM_CAR_STATE_GEAR_DRIVE    /**< 3/0x03, D gear(Drive gear) */
} xcb_avm_car_state_gear_t;

typedef struct xcb_avm_car_state_light_s {
  uint8_t u8Val; /**< 0/0x00-All light off, else-some light on */
  bool bLeft;    /**< 1/0x01, True-Left turn light on, False-off */
  bool bRight;   /**< 2/0x02, True-Right turn light on, False-off */
  bool bHazard;  /**< 4/0x04, True-Hazard light on, False-off */
  bool bDipped;  /**< 8/0x08, True-Dipped light on, False-off */
} xcb_avm_car_state_light_t;

typedef struct xcb_avm_car_state_door_s {
  uint8_t u8Val; /**< 0/0x00-All door closed, else-some door on */
  bool bBonnet;  /**< 1/0x01, True-Bonnet(front) opened, False-off */
  bool bFL;      /**< 2/0x02, True-Driver door opened, False-off */
  bool bFR;      /**< 4/0x04, True-Front right door opened, False-off */
  bool bBoot;    /**< 8/0x08, True-Boot(back) opened, False-off */
  bool bBL;      /**< 16/0x10, True-Back left door opened, False-off */
  bool bBR;      /**< 32/0x20, True-Back right door opened, False-off */
} xcb_avm_car_state_door_t;

/* Car state
 *
 * gear/light/door/steering direction/
 * steering degree/car speed
 * TODO: wheel degree(left)=steering wheel degree(left)/16.83,
 * wheel degree(right) = steering wheel degree(right)/16.87
 */
typedef struct xcb_avm_car_state_s {
  xcb_avm_car_state_gear_t emGear;
  xcb_avm_car_state_light_t tLight;
  xcb_avm_car_state_door_t tDoor;
  /**< degree(left)=steering wheel degree(left)/16.83
   * degree(right)=steering wheel degree(right)/16.87
   * degree~(-90,90), degree(left)<0, degree(right)>0
   */
  float fDegree;
  uint8_t u8Speed; /**< Car speed: [0,255] */
} xcb_avm_car_state_t;

/* Touch operation
 *
 * no touch/down/up/move
 */
typedef enum xcb_avm_touch_operate_s {
  XCB_AVM_TOUCH_OPERATE_NO,   /**< 0/0x00, No touch operation */
  XCB_AVM_TOUCH_OPERATE_DOWN, /**< 1/0x01, Touch down*/
  XCB_AVM_TOUCH_OPERATE_UP,   /**< 2/0x02, Touch up*/
  XCB_AVM_TOUCH_OPERATE_MOVE  /**< 3/0x03, Touch move*/
} xcb_avm_touch_operate_t;

/* Touch state
 *
 * touch operation/ touch coordinate
 * coordinate system: origin is left-up point,
 *  direction of x-axis is from left to right,
 *  direction of y-axis is from up to down
 */
typedef struct xcb_avm_touch_state_s {
  xcb_avm_touch_operate_t emOpt;
  uint16_t u16X;
  uint16_t u16Y;
} xcb_avm_touch_state_t;

/* Head angle
 *
 * the information of current head angle for dynamic stwitching
 */

typedef struct xcb_avm_headangle_s {
  short nHeadAngle;
  short nAlarmState;
  unsigned char nTimeYear;
  unsigned char nTimeMonth;
  unsigned char nTimeDay;
  unsigned char nTimeHour;
  unsigned char nTimeMinute;
  unsigned char nTimeSecond;

  unsigned short nTimemilliSecond;
} xcb_avm_headangle_t;

/* View switch
 *
 * Bird view and front/back/left/right view,
 * bird view and front left and right view,
 * front broad view, back broad view
 */
typedef enum xcb_avm_view_switch_s {
  XCB_AVM_VIEW_BIRD_FRONT,  /**< Bird view and front view */
  XCB_AVM_VIEW_BIRD_BACK,   /**< Bird view and back view */
  XCB_AVM_VIEW_BIRD_LEFT,   /**< Bird view and left view */
  XCB_AVM_VIEW_BIRD_RIGHT,  /**< Bird view and right view */
  XCB_AVM_VIEW_SIDE_FRONT,  /**< Bird view and front left and right view */
  XCB_AVM_VIEW_BROAD_FRONT, /**< Broad view of front */
  XCB_AVM_VIEW_BROAD_BACK,   /**< Broad view of back */
  XCB_AVM_VIEW_3D_ROTATE,   /**< 3D旋转 */
  XCB_AVM_VIEW_FULL_IMAGE,
  
  
   ////////////for xingtu use///////////////
   REVERSE_MODE_XINGTU_VIEW_BACK,
   STEERING_MODE_XINGTU_LEFTDOWN,
   STEERING_MODE_XINGTU_RIGHTDOWN,
   GENERAL_MODE_XINGTU_VIEW_FRONT  
} xcb_avm_view_switch_t;

/* View type
 *
 * Right view port is 2D view or 3D view
 */
typedef enum xcb_avm_view_2D3D_s {
  XCB_AVM_VIEW_TYPE_2D, /**< 2D view */
  XCB_AVM_VIEW_TYPE_3D,  /**< 3D view */
  XCB_AVM_VIEW_TYPE_4LANE,
  XCB_AVM_VIEW_TYPE_6LANE
} xcb_avm_view_2D3D_t;

/* Calibrate or not
 *
 * Calibration closed/opened
 */
typedef enum xcb_avm_calib_or_not_s {
  XCB_AVM_CALIB_CLOSED, /**< Not to calibrate */
  XCB_AVM_CALIB_OPENED  /**< Start to calibrate */
} xcb_avm_calib_or_not_t;

/* Color of car model
 *
 */
typedef enum xcb_avm_car_color_s {
  XCB_AVM_CAR_COLOR1,
  XCB_AVM_CAR_COLOR2,
  XCB_AVM_CAR_COLOR3,
  XCB_AVM_CAR_COLOR4,
  XCB_AVM_CAR_COLOR5,
  XCB_AVM_CAR_COLOR6
} xcb_avm_car_color_t;

/* Energe mode
 *
 * shut down/reboot
 */
typedef enum xcb_avm_energe_mode_s {
  XCB_AVM_ENERGE_MODE_SHUTDOWN, /**< Shut down */
  XCB_AVM_ENERGE_MODE_REBOOT    /**< Reboot */
} xcb_avm_energe_mode_t;


/* ADAS SWITCH
 *
 * ADAS open/down
 */
typedef enum xcb_avm_adas_switch_s 
{
  XCB_AVM_ADAS_OPEN,    /**< open */
  XCB_AVM_ADAS_CLOSE    /**< close */
} xcb_avm_adas_sw_t;



/* Message from mcu to t7
 *
 */
typedef struct xcb_avm_msg_t7_s {
  xcb_avm_msg_type_t7_t emType;   /**< Message type from mcu to t7 */
  xcb_avm_car_state_t tState;     /**< Car state, gear/light/door...*/
  xcb_avm_touch_state_t tTouch;   /**< Touch state */
  xcb_avm_view_switch_t emView;   /**< View switch, front/back view... */
  xcb_avm_view_2D3D_t em2D3D;     /**< View type, 2d or 3d view */
  xcb_avm_calib_or_not_t emCalib; /**< Calib or not */
  xcb_avm_car_color_t emColor;    /**< Color of car model */
  xcb_avm_energe_mode_t emEnerge; /**< Energe mode, shut down/reboot */
  xcb_avm_adas_sw_t adas_switch;
  uint8_t u8VerInt;               /**< Version's integral part, 1.02->0x01 */
  uint8_t u8VerFrac;              /**< Version's fractional part, 1.02->0x02 */
  xcb_avm_headangle_t tHeadangle;  /**head angle value and time for dynamic stitching**/
  unsigned char ganglemcu[6];
} xcb_avm_msg_t7_t;

/*************************************************************/
/****************** Message from t7 to mcu  ******************/
/*************************************************************/

/* Message type from t7 to mcu
 *
 */
typedef enum xcb_avm_msg_type_mcu_s {
	XCB_AVM_MSG_MCU_VERSION_INFO, /**< Version information */
	XCB_AVM_MSG_MCU_BEAT_DETECT,  /**< Beat detection for upgrade/sync */
	XCB_AVM_MSG_MCU_FAULT_INFO,   /**< Fault information */
	XCB_AVM_MSG_MCU_CALIB_STATE,  /**< Calibration state */
	XCB_AVM_MSG_MCU_ENERGE_MODE,   /**< Energe mode, reboot/shut down */
	XCB_AVM_MSG_MCU_BSD,
  XCB_AVM_MSG_MCU_UPDATA_START, 
  XCB_AVM_MSG_MCU_UPDATA_DATA, 
  XCB_AVM_MSG_MCU_UPDATA_RESET,
  XCB_AVM_MSG_MCU_SUCCESS,		
	XCB_AVM_MSG_MCU_LDW,
	XCB_AVM_MSG_MCU_HMW,
	XCB_AVM_MSG_MCU_MOD,
	XCB_AVM_MSG_MCU_ADAS

} xcb_avm_msg_type_mcu_t;

typedef enum xcb_avm_beat_detect_s {
  XCB_AVM_BEAT_DETECT_UPGRADE, /**< For upgrading app */
  XCB_AVM_BEAT_DETECT_SYNC     /**< For synchronization */
} xcb_avm_beat_detect_t;

typedef enum xcb_avm_fault_type_s {
  XCB_AVM_FRONT_CAMERA, /**< 1/0x01, Fault of front camera */
  XCB_AVM_BACK_CAMERA,  /**< 2/0x02, Fault of back camera */
  XCB_AVM_LEFT_CAMERA,  /**< 4/0x04, Fault of left camera */
  XCB_AVM_RIGHT_CAMERA, /**< 8/0x08, Fault of right camera */
  XCB_AVM_HOST          /**< 16/0x10, Fault of host */
} xcb_avm_fault_type_t;

/* Success to calibrate or not
 *
 * Calibration success/fail
 */
typedef enum xcb_avm_calib_state_s {
  XCB_AVM_CALIB_STATE_SUCCESS, /**< Success to calibrate */
  XCB_AVM_CALIB_STATE_FAIL     /**< Fail to calibrate */
} xcb_avm_calib_state_t;

/* Message from t7 to mcu
 *
 */
typedef struct xcb_avm_msg_mcu_s {
  xcb_avm_msg_type_mcu_t emType; /**< Message type from t7 to mcu */
  uint8_t u8Fault;               /**< Fault information */
  uint8_t u8Beat;                /**< Beat detection for sync/upgrade */
  uint8_t u8Calib;               /**< Calibration state, success/fail */
  uint8_t u8Energe;              /**< Reboot after upgrading app*/
  uint8_t u8VerInt;              /**< Version's integral part, 1.02->0x01 */
  uint8_t u8VerFrac;             /**< Version's fractional part, 1.02->0x02 */
  uint8_t u8Adas;             /**< Version's fractional part, 1.02->0x02 */
} xcb_avm_msg_mcu_t;

#endif // PARASET_H
