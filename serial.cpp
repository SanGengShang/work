/******************************************************************************

                  xcb 版权所有 (C), 2017-2020,

 ******************************************************************************
  文 件 名   :
  版 本 号   : V1.1
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
#include "serial.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>
//#include "sysconfig.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "file_list.h"
#include "xcb_avmlib.h"
//#include "xcb_videoprot5.h"
using namespace std;

constexpr uint8_t u8_0 = 0x00;
constexpr uint8_t u8_1 = 0x01;
constexpr uint8_t u8_2 = 0x02;
constexpr uint8_t u8_4 = 0x04;
constexpr uint8_t u8_8 = 0x08;
constexpr uint8_t u8_16 = 0x10;
constexpr uint8_t u8_32 = 0x20;

constexpr uint8_t u8_1_ = ~(0x01);
constexpr uint8_t u8_2_ = ~(0x02);
constexpr uint8_t u8_4_ = ~(0x04);
constexpr uint8_t u8_8_ = ~(0x08);
constexpr uint8_t u8_16_ = ~(0x10);

constexpr uint8_t u8_3 = 0x03;
constexpr uint8_t u8_5 = 0x05;
constexpr uint8_t u8_6 = 0x06;
constexpr uint8_t u8_7 = 0x07;
constexpr uint8_t u8_9 = 0x09;
constexpr uint8_t u8_a = 0x0A;
constexpr uint8_t u8_46 = 0x46;



extern bool mod_back_warning;
extern bool mod_left_warning;
extern bool mod_front_warning;
extern bool mod_right_warning;
extern int g_avm_bsd_status;

extern bool bShowMcuUpdate;
extern bool bShowMcuUpdateSucceed;

extern ViewParameter g_t3DViewParams;
/**< camera parameter of 2D single view(undistorted
 * front/back view) and front side view(both left
 * and right), and ROI on screen for showing */
extern ViewParameter g_t2DViewParams;
/**< camera parameter of front left side view,
 * and ROI on screen for showing */
extern ViewParameter g_tFLViewParams;
/**< camera parameter of front right side view,
 * and ROI on screen for showing */
extern ViewParameter g_tFRViewParams;

extern avm_car_state_type_t g_car_state_Handle;

ViewParameter g_AdjustParams;

extern int save_skin_para;

int Save_Speed_Value = -1;

int iDvrFlag =0;
int nMCUUpdateFlag = 0;
//==================================================================

///////time for rec
extern unsigned char curhourH;
extern unsigned char curhourL;
extern unsigned char curminuteH;
extern unsigned char curminuteL;
extern unsigned char cursecH;
extern unsigned char cursecL;

extern unsigned char curmonH;
extern unsigned char curmonL;
extern unsigned char curdayH;
extern unsigned char curdayL;
extern unsigned char curyearH;
extern unsigned char curyearL;

void serial::reset_car_light_state()
{
  xcb_avm_car_state_light_t &tLight = m_tMsg_t7.tState.tLight;
  tLight.bLeft = false;
  tLight.bRight = false;
  tLight.bHazard = false;
  tLight.bDipped = false;
  tLight.u8Val = u8_0;
}
void serial::reset_car_door_state()
{
  xcb_avm_car_state_door_t &tDoor = m_tMsg_t7.tState.tDoor;
  tDoor.bBonnet = false;
  tDoor.bBoot = false;
  tDoor.bFL = false;
  tDoor.bFR = false;
  tDoor.bBL = false;
  tDoor.bBR = false;
  tDoor.u8Val = u8_0;
}
void serial::set_car_light_state(uint8_t u8Val)
{
  xcb_avm_car_state_light_t &tLight = m_tMsg_t7.tState.tLight;
  if (u8Val == u8_0)
  {
    reset_car_light_state();
  }
  else
  {
    tLight.u8Val = u8Val;
    tLight.bLeft = ((u8Val & u8_1) == u8_1) ? true : false;
    tLight.bRight = ((u8Val & u8_2) == u8_2) ? true : false;
    tLight.bHazard = ((u8Val & u8_4) == u8_4) ? true : false;
    tLight.bDipped = ((u8Val & u8_8) == u8_8) ? true : false;
  }
}
void serial::set_car_door_state(uint8_t u8Val)
{
  xcb_avm_car_state_door_t &tDoor = m_tMsg_t7.tState.tDoor;
  if (u8Val == u8_0)
  {
    reset_car_door_state();
  }
  else
  {
    tDoor.u8Val = u8Val;
    tDoor.bBonnet = ((u8Val & u8_1) == u8_1) ? true : false;
    tDoor.bFL = ((u8Val & u8_2) == u8_2) ? true : false;
    tDoor.bFR = ((u8Val & u8_4) == u8_4) ? true : false;
    tDoor.bBoot = ((u8Val & u8_8) == u8_8) ? true : false;
    tDoor.bBL = ((u8Val & u8_16) == u8_16) ? true : false;
    tDoor.bBR = ((u8Val & u8_32) == u8_32) ? true : false;
  }
}
uint8_t serial::get_car_light_state() { return m_tMsg_t7.tState.tLight.u8Val; }
uint8_t serial::get_car_door_state() { return m_tMsg_t7.tState.tDoor.u8Val; }

void serial::reset_fault_state() { m_tMsg_mcu.u8Fault = u8_0; }
void serial::set_fault_state(xcb_avm_fault_type_t emFault)
{
  switch (emFault)
  {
  case XCB_AVM_FRONT_CAMERA:
    m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_FAULT_INFO;
    m_tMsg_mcu.u8Fault |= u8_1;
    m_bReadOrSend = true;
    break;
  case XCB_AVM_BACK_CAMERA:
    m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_FAULT_INFO;
    m_tMsg_mcu.u8Fault |= u8_2;
    m_bReadOrSend = true;
    break;
  case XCB_AVM_LEFT_CAMERA:
    m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_FAULT_INFO;
    m_tMsg_mcu.u8Fault |= u8_4;
    m_bReadOrSend = true;
    break;
  case XCB_AVM_RIGHT_CAMERA:
    m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_FAULT_INFO;
    m_tMsg_mcu.u8Fault |= u8_8;
    m_bReadOrSend = true;
    break;
  case XCB_AVM_HOST:
    m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_FAULT_INFO;
    m_tMsg_mcu.u8Fault |= u8_16;
    m_bReadOrSend = true;
    break;
  default:
    m_tMsg_mcu.u8Fault = u8_0;
    break;
  }
}
void serial::set_ok_state(xcb_avm_fault_type_t emFault)
{
  switch (emFault)
  {
  case XCB_AVM_FRONT_CAMERA:
    m_tMsg_mcu.u8Fault &= u8_1_;
    break;
  case XCB_AVM_BACK_CAMERA:
    m_tMsg_mcu.u8Fault &= u8_2_;
    break;
  case XCB_AVM_LEFT_CAMERA:
    m_tMsg_mcu.u8Fault &= u8_4_;
    break;
  case XCB_AVM_RIGHT_CAMERA:
    m_tMsg_mcu.u8Fault &= u8_8_;
    break;
  case XCB_AVM_HOST:
    m_tMsg_mcu.u8Fault &= u8_16_;
    break;
  default:
    m_tMsg_mcu.u8Fault = u8_0;
    break;
  }
}

void serial::send_reboot_request()
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_ENERGE_MODE;
  m_tMsg_mcu.u8Energe = 0x01;
  m_bReadOrSend = true;
}

void serial::send_beat_off()
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_BEAT_DETECT;
  m_tMsg_mcu.u8Beat = XCB_AVM_BEAT_DETECT_UPGRADE;
}

void serial::send_beat_on()
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_BEAT_DETECT;
  m_tMsg_mcu.u8Beat = XCB_AVM_BEAT_DETECT_SYNC;
}

void serial::send_calib_state(bool bCalibOK)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_CALIB_STATE;
  if (bCalibOK)
  {
    m_tMsg_mcu.u8Calib = 0x01;
  }
  else
  {
    m_tMsg_mcu.u8Calib = 0x00;
  }
  m_bReadOrSend = true;
}

//==================================================================
void serial::send_bsd_state(bool u8Adas)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_BSD;
  if (u8Adas)
  {
    m_tMsg_mcu.u8Adas = 0x01;
  }
  else
  {
    m_tMsg_mcu.u8Adas = 0x00;
  }
  m_bReadOrSend = true;
}

void serial::send_ldw_state(bool u8Adas)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_LDW;
  if (u8Adas)
  {
    m_tMsg_mcu.u8Adas = 0x01;
  }
  else
  {
    m_tMsg_mcu.u8Adas = 0x00;
  }
  m_bReadOrSend = true;
}


void serial::send_adas_state(uint8_t u8Adas)
{
	m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_ADAS;
	m_tMsg_mcu.u8Adas = u8Adas;
	m_bReadOrSend = true;
}

void serial::send_hmw_state(bool u8Adas)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_HMW;
  if (u8Adas)
  {
    m_tMsg_mcu.u8Adas = 0x01;
  }
  else
  {
    m_tMsg_mcu.u8Adas = 0x00;
  }
  m_bReadOrSend = true;
}

void serial::send_mod_state(bool u8Adas)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_MOD;
  if (u8Adas)
  {
    m_tMsg_mcu.u8Adas = 0x01;
  }
  else
  {
    m_tMsg_mcu.u8Adas = 0x00;
  }
  m_bReadOrSend = true;
}


void serial::cur_car_state_print()
{
  switch (m_tMsg_t7.emType)
  {
  case XCB_AVM_MSG_T7_CAR_STATE:
    printf("Msg type: car state\n");
    break;
  case XCB_AVM_MSG_T7_TOUCH_OPERATE:
    printf("Msg type: touch operation\n");
    break;
  case XCB_AVM_MSG_T7_VIEW_SWITCH:
    printf("Msg type: view switch\n");
    break;
  case XCB_AVM_MSG_T7_2D_3D_VIEW:
    printf("Msg type: 2d or 3d\n");
    break;
  case XCB_AVM_MSG_T7_CALIB:
    printf("Msg type: calibration\n");
    break;
  case XCB_AVM_MSG_T7_CAR_COLOR:
    printf("Msg type: car color\n");
    break;
  default:
    break;
  }
  switch (m_tMsg_t7.tState.emGear)
  {
  case XCB_AVM_CAR_STATE_GEAR_PARK:
    printf("Parking(P) gear\n");
    break;
  case XCB_AVM_CAR_STATE_GEAR_DRIVE:
    printf("Driving(D) gear\n");
    break;
  case XCB_AVM_CAR_STATE_GEAR_NEUTRAL:
    printf("Neutral(N) gear\n");
    break;
  case XCB_AVM_CAR_STATE_GEAR_REVERSE:
    printf("Reverse(R) gear\n");
    break;
  default:
    break;
  }
  xcb_avm_car_state_light_t &tLight = m_tMsg_t7.tState.tLight;
  if (!tLight.u8Val == u8_0)
  {
    if (tLight.bLeft)
      printf("Left turn light on\n");
    if (tLight.bRight)
      printf("Right turn light on\n");
    if (tLight.bHazard)
      printf("Hazard light on\n");
    if (tLight.bDipped)
      printf("Dipped light on\n");
  }
  else
  {
    printf("Light off\n");
  }
  xcb_avm_car_state_door_t &tDoor = m_tMsg_t7.tState.tDoor;
  if (tDoor.u8Val)
  {
    printf("Door closed\n");
  }
  else
  {
    if (tDoor.bBonnet)
    {
      printf("Bonnet opened\n");
    }
    if (tDoor.bBoot)
    {
      printf("Boot opened\n");
    }
    if (tDoor.bFL)
    {
      printf("Front left door opened\n");
    }
    if (tDoor.bFR)
    {
      printf("Front right door opened\n");
    }
    if (tDoor.bBL)
    {
      printf("Back left door opened\n");
    }
    if (tDoor.bBR)
    {
      printf("Back right door opened\n");
    }
  }
  printf("Wheel degree: %f\n", m_tMsg_t7.tState.fDegree);
  printf("Speed: %d\n", m_tMsg_t7.tState.u8Speed);
}

#if 0
void serial::sim_get_car_state(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.tState.emGear = XCB_AVM_CAR_STATE_GEAR_PARK;
    printf("Parking(P) gear\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.tState.emGear = XCB_AVM_CAR_STATE_GEAR_DRIVE;
    printf("Driving(D) gear\n");
    return;
  }
  if (lKey == 'e')
  {
    m_tMsg_t7.tState.emGear = XCB_AVM_CAR_STATE_GEAR_NEUTRAL;
    printf("Neutral(N) gear\n");
    return;
  }
  if (lKey == 'r')
  {
    m_tMsg_t7.tState.emGear = XCB_AVM_CAR_STATE_GEAR_REVERSE;
    printf("Reverse(R) gear\n");
    return;
  }
  xcb_avm_car_state_light_t &tLight = m_tMsg_t7.tState.tLight;
  if (lKey == 'a')
  {
    reset_car_light_state();
    printf("Light off\n");
    return;
  }
  if (lKey == 's')
  {
    if ((tLight.u8Val & u8_1) == u8_0)
    {
      tLight.bLeft = true;
      tLight.u8Val |= u8_1;
      printf("Left turn light on\n");
    }
    else
    {
      tLight.bLeft = false;
      tLight.u8Val &= (~u8_1);
      printf("Left turn light off\n");
    }
    return;
  }
  if (lKey == 'd')
  {
    if ((tLight.u8Val & u8_2) == u8_0)
    {
      tLight.bRight = true;
      tLight.u8Val |= u8_2;
      printf("right turn light on\n");
    }
    else
    {
      tLight.bRight = false;
      tLight.u8Val &= (~u8_2);
      printf("right turn light off\n");
    }
    return;
  }
  if (lKey == 'f')
  {
    if ((tLight.u8Val & u8_8) == u8_0)
    {
      tLight.bDipped = true;
      tLight.u8Val |= u8_8;
      printf("Dipped light on\n");
    }
    else
    {
      tLight.bDipped = false;
      tLight.u8Val &= (~u8_8);
      printf("Dipped light off\n");
    }
    return;
  }
  if (lKey == 'g')
  {
    if ((tLight.u8Val & u8_4) == u8_0)
    {
      tLight.bHazard = true;
      tLight.u8Val |= u8_4;
      printf("Hazard light on\n");
    }
    else
    {
      tLight.bHazard = false;
      tLight.u8Val &= (~u8_4);
      printf("Hazard light off\n");
    }
    return;
  }
  xcb_avm_car_state_door_t &tDoor = m_tMsg_t7.tState.tDoor;
  if (lKey == 'z')
  {
    reset_car_door_state();
    printf("Door closed\n");
    return;
  }
  if (lKey == 'x')
  {
    if ((tDoor.u8Val & u8_1) == u8_0)
    {
      tDoor.bBonnet = true;
      tDoor.u8Val |= u8_1;
      printf("Bonnet opened\n");
    }
    else
    {
      tDoor.bBonnet = false;
      tDoor.u8Val &= (~u8_1);
      printf("Bonnet closed\n");
    }
    return;
  }
  if (lKey == 'c')
  {
    if ((tDoor.u8Val & u8_8) == u8_0)
    {
      tDoor.bBoot = true;
      tDoor.u8Val |= u8_8;
      printf("Boot opened\n");
    }
    else
    {
      tDoor.bBoot = false;
      tDoor.u8Val &= (~u8_8);
      printf("Boot closed\n");
    }
    return;
  }
  if (lKey == 'v')
  {
    if ((tDoor.u8Val & u8_2) == u8_0)
    {
      tDoor.bFL = true;
      tDoor.u8Val |= u8_2;
      printf("Front left door opened\n");
    }
    else
    {
      tDoor.bFL = false;
      tDoor.u8Val &= (~u8_2);
      printf("Front left door closed\n");
    }
    return;
  }
  if (lKey == 'b')
  {
    if ((tDoor.u8Val & u8_4) == u8_0)
    {
      tDoor.bFR = true;
      tDoor.u8Val |= u8_4;
      printf("Front right door opened\n");
    }
    else
    {
      tDoor.bFR = false;
      tDoor.u8Val &= (~u8_4);
      printf("Front right door closed\n");
    }
    return;
  }
  if (lKey == 'n')
  {
    if ((tDoor.u8Val & u8_16) == u8_0)
    {
      tDoor.bBL = true;
      tDoor.u8Val |= u8_16;
      printf("Back left door opened\n");
    }
    else
    {
      tDoor.bBL = false;
      tDoor.u8Val &= (~u8_16);
      printf("Back left door closed\n");
    }
    return;
  }
  if (lKey == 'm')
  {
    if ((tDoor.u8Val & u8_32) == u8_0)
    {
      tDoor.bBR = true;
      tDoor.u8Val |= u8_32;
      printf("Back right door opened\n");
    }
    else
    {
      tDoor.bBR = false;
      tDoor.u8Val &= (~u8_32);
      printf("Back right door closed\n");
    }
    return;
  }

  if (lKey == 37)  //%
  { // left arrow
    if (m_tMsg_t7.tState.fDegree > -45)
    {
      m_tMsg_t7.tState.fDegree -= 1.5;
    }
    printf("Wheel degree: %f\n", m_tMsg_t7.tState.fDegree);
    return;
  }
  if (lKey == 39)  //`
  { // right arrow
    if (m_tMsg_t7.tState.fDegree < 45)
    {
      m_tMsg_t7.tState.fDegree += 1.5;
    }
    printf("Wheel degree: %f\n", m_tMsg_t7.tState.fDegree);
    return;
  }
  if (lKey == 38)  //&
  { // up arrow
    if (m_tMsg_t7.tState.u8Speed < 50)
    {
      m_tMsg_t7.tState.u8Speed += 4;
    }
    printf("Speed: %d\n", m_tMsg_t7.tState.u8Speed);
    return;
  }
  if (lKey == 40)  //(
  { // down arrow
    if (m_tMsg_t7.tState.u8Speed > 0)
    {
      m_tMsg_t7.tState.u8Speed -= 4;
    }
    printf("Speed: %d\n", m_tMsg_t7.tState.u8Speed);
    return;
  }
}
void serial::sim_get_touch_state(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_NO;
    printf("No touch operation\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_DOWN;
    printf("Touch down\n");
    return;
  }
  if (lKey == 'e')
  {
    m_tMsg_t7.tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_MOVE;
    printf("Touch move\n");
    return;
  }
  if (lKey == 'r')
  {
    m_tMsg_t7.tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_UP;
    printf("Touch up x=%d,y=%d\n",m_tMsg_t7.tTouch.u16X,m_tMsg_t7.tTouch.u16Y);
    return;
  }
  
  if (lKey == 'z') //front
  {
    m_tMsg_t7.tTouch.u16X = 132;
    m_tMsg_t7.tTouch.u16Y = 37;
    printf("set front camer x y value\n");
    return;
  }

  if (lKey == 'x')//right
  {

    m_tMsg_t7.tTouch.u16X = 229;
    m_tMsg_t7.tTouch.u16Y = 245;
    printf("set right camer x y value\n");
    return;
  }

  if (lKey == 'c') //back
  {

    m_tMsg_t7.tTouch.u16X = 135;
    m_tMsg_t7.tTouch.u16Y = 435;
    printf("set back camer x y value\n");
    return;
  } 

  if (lKey == 'v') //left
  {

    m_tMsg_t7.tTouch.u16X = 39;
    m_tMsg_t7.tTouch.u16Y = 245;
    printf("set left camer x y value\n");
    return;
  }

  if (lKey == 'b') //2d ,3d
  {
    m_tMsg_t7.tTouch.u16X = 90;
    m_tMsg_t7.tTouch.u16Y = 105;
    printf("set 2d 3d x y value\n");
    return;
  } 

  if (m_tMsg_t7.tTouch.emOpt == XCB_AVM_TOUCH_OPERATE_MOVE)
  {
    if (lKey == 37)
    { // left arrow
      if (m_tMsg_t7.tTouch.u16X > 1020)
      {
        m_tMsg_t7.tTouch.u16X -= 5;
      }
      printf("Move left,Car Center:(1470,360),Pos:(%d,%d)\n",
             m_tMsg_t7.tTouch.u16X, m_tMsg_t7.tTouch.u16Y);
      return;
    }
    if (lKey == 39)
    { // right arrow
      if (m_tMsg_t7.tTouch.u16X < 1920)
      {
        m_tMsg_t7.tTouch.u16X += 5;
      }
      printf("Move right,Car Center:(1470,360),Pos:(%d,%d)\n",
             m_tMsg_t7.tTouch.u16X, m_tMsg_t7.tTouch.u16Y);
      return;
    }
    if (lKey == 38)
    { // up arrow
      if (m_tMsg_t7.tTouch.u16Y > 0)
      {
        m_tMsg_t7.tTouch.u16Y -= 5;
      }
      printf("Move up,Car Center:(1470,360),Pos:(%d,%d)\n",
             m_tMsg_t7.tTouch.u16X, m_tMsg_t7.tTouch.u16Y);
      return;
    }
    if (lKey == 40)
    { // down arrow
      if (m_tMsg_t7.tTouch.u16Y < 720)
      {
        m_tMsg_t7.tTouch.u16Y += 5;
      }
      printf("Move down,Car Center:(1470,360),Pos:(%d,%d)\n",
             m_tMsg_t7.tTouch.u16X, m_tMsg_t7.tTouch.u16Y);
      return;
    }
  }

  #if 0
  else if (m_tMsg_t7.tTouch.emOpt == XCB_AVM_TOUCH_OPERATE_UP ||
           m_tMsg_t7.tTouch.emOpt == XCB_AVM_TOUCH_OPERATE_DOWN)
  {
    if (lKey == 'a') //after
    {
      m_tMsg_t7.tTouch.u16Y = 132;
      m_tMsg_t7.tTouch.u16X = 37;
    }
    else if (lKey == 's') //right
    {
      m_tMsg_t7.tTouch.u16Y = 229;
      m_tMsg_t7.tTouch.u16X = 245;
    }
    else if (lKey == 'd') //back
    {

      m_tMsg_t7.tTouch.u16Y = 135;
      m_tMsg_t7.tTouch.u16X = 435;
    }
    else if (lKey == 'f')
    {
      m_tMsg_t7.tTouch.u16Y = 39;
      m_tMsg_t7.tTouch.u16X = 245;
    }
  }
  #endif
}
void serial::sim_get_view_state(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_FRONT;
    printf("Bird view and front view\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_BACK;
    printf("Bird view and back view\n");
    return;
  }
  if (lKey == 'e')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_LEFT;
    printf("Bird view and left view\n");
    return;
  }
  if (lKey == 'r')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_RIGHT;
    printf("Bird view and right view\n");
    return;
  }
  if (lKey == 't')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_SIDE_FRONT;
    printf("Side view of front\n");
    return;
  }
  if (lKey == 'y')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BROAD_FRONT;
    printf("Broad view of front\n");
    return;
  }
  if (lKey == 'u')
  {
    m_tMsg_t7.emView = XCB_AVM_VIEW_BROAD_BACK;
    printf("Broad view of back\n");
    return;
  }
}
void serial::sim_get_2D3D_view(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.em2D3D = XCB_AVM_VIEW_TYPE_2D;
    printf("2D view\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.em2D3D = XCB_AVM_VIEW_TYPE_3D;
    printf("3D view\n");
    return;
  }
}
void serial::sim_get_calib_state(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
    printf("Calib closed!\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.emCalib = XCB_AVM_CALIB_OPENED;
    printf("Start to calib!\n");
    return;
  }
}
void serial::sim_get_car_color(char lKey)
{
  if (lKey == 'q')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR1;
    printf("color1\n");
    return;
  }
  if (lKey == 'w')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR2;
  	printf("color2\n");
  	return;
  }
  if (lKey == 'e')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR3;
    printf("color3\n");
    return;
  }
  if (lKey == 'r')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR4;
    printf("color4\n");
    return;
  }
  if (lKey == 't')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR5;
    printf("color5\n");
    return;
  }
  if (lKey == 'y')
  {
    m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR6;
    printf("color6\n");
    return;
  }
}

int adj_x = 0;
int adj_y = 0;

static int bsd_mod_status = 0;  //默认BSD



void serial::sim_get_adjust_para(char lKey) 
{


	  if (lKey == 'q')  //aspect_ratio +
	  {
	     adj_x += 10;
		 if(adj_x > 1000)
		 {
		    adj_x = 0;
		 }
	     g_AdjustParams.aspect_ratio = (float)(R_W+adj_x) / R_H;
	    // printf("aspect_ratio = %f adj_x = %d\n",g_AdjustParams.aspect_ratio,adj_x);
	  }
	  else if(lKey == 'a')  //aspect_ratio -
	  {
		  adj_x -= 10;
		  if(adj_x <-800)
		  {
			 adj_x = 0;
		  }
		  g_AdjustParams.aspect_ratio = (float)(R_W+adj_x) / R_H;
		  //printf("aspect_ratio = %f adj_x = %d\n",g_AdjustParams.aspect_ratio,adj_x);
	  }

	 
	  if (lKey == 'w') //ex +
	  {
		 g_AdjustParams.ex+=10;
		 if(g_AdjustParams.ex > 1000)
		 {
		    g_AdjustParams.ex = 0;
		 }
		 //printf("ex = %f\n",g_AdjustParams.ex);
	  }
	  else if(lKey == 's') //ex -
	  {
		 g_AdjustParams.ex-=10;
		 if(g_AdjustParams.ex < -1000)
		 {
		    g_AdjustParams.ex = 0;
		 }
		 //printf("ex = %f\n",g_AdjustParams.ex);	  
	  }


	  if (lKey == 'e') //ey +
	  {
		 g_AdjustParams.ey+=10;
		 if(g_AdjustParams.ey > 1000)
		 {
		    g_AdjustParams.ey = 0;
		 }
		 //printf("ey = %f\n",g_AdjustParams.ey);
	  }
	  else if(lKey == 'd') //ey -
	  {
		 g_AdjustParams.ey-=10;
		 if(g_AdjustParams.ey < -1000)
		 {
		    g_AdjustParams.ey = 0;
		 }
		 //printf("ey = %f\n",g_AdjustParams.ey);	  
	  }

	  if (lKey == 'r') //ez +
	  {
		 g_AdjustParams.ez+=10;
		 if(g_AdjustParams.ez > 7000)
		 {
		    g_AdjustParams.ez = 400;
		 }
		// printf("ez = %f\n",g_AdjustParams.ez);
	  }
	  else if(lKey == 'f') //ez -
	  {
		 g_AdjustParams.ez-=10;
		 if(g_AdjustParams.ez < 400)
		 {
		    g_AdjustParams.ez = 400;
		 }
		// printf("ez = %f\n",g_AdjustParams.ez);	  
	  }


	  if (lKey == 't') //tx +
	  {
		 g_AdjustParams.tx+=10;
		 if(g_AdjustParams.tx > 1000)
		 {
		    g_AdjustParams.tx = 0;
		 }
		 //printf("tx = %f\n",g_AdjustParams.tx);
	  }
	  else if(lKey == 'g') //tx -
	  {
		 g_AdjustParams.tx-=10;
		 if(g_AdjustParams.tx < -1000)
		 {
		    g_AdjustParams.tx = 0;
		 }
		// printf("tx = %f\n",g_AdjustParams.tx);	  
	  }

	  if (lKey == 'y') //ty +
	  {
		 g_AdjustParams.ty+=10;
		 if(g_AdjustParams.ty > 1000)
		 {
		    g_AdjustParams.ty = 0;
		 }
		 //printf("ty = %f\n",g_AdjustParams.ty);
	  }
	  else if(lKey == 'h') //ty -
	  {
		 g_AdjustParams.ty-=10;
		 if(g_AdjustParams.ty < -1000)
		 {
		    g_AdjustParams.ty = 0;
		 }
		 //printf("ty = %f\n",g_AdjustParams.ty);	  
	  }

	  if (lKey == 'u') //tz +
	  {
		 g_AdjustParams.tz+=10;
		 if(g_AdjustParams.tz > 1000)
		 {
		    g_AdjustParams.tz = 0;
		 }
		 //printf("tz = %f\n",g_AdjustParams.tz);
	  }
	  else if(lKey == 'j') //tz -
	  {
		 g_AdjustParams.tz-=10;
		 if(g_AdjustParams.tz < -1000)
		 {
		    g_AdjustParams.tz = 0;
		 }
		 //printf("tz = %f\n",g_AdjustParams.tz);	  
	  }
      printf("adj_x=%d,ex=%f,ey=%f,ez=%f,tx=%f,ty=%f,tz=%f\n",adj_x,g_AdjustParams.ex,g_AdjustParams.ey,g_AdjustParams.ez,
      g_AdjustParams.tx,g_AdjustParams.ty,g_AdjustParams.tz);
     
	  if(m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_2D_PARA)
	  {
		   g_t2DViewParams.aspect_ratio = g_AdjustParams.aspect_ratio;
		   g_t2DViewParams.ex = g_AdjustParams.ex ;
		   g_t2DViewParams.ey =  g_AdjustParams.ey ;
		   g_t2DViewParams.ez =  g_AdjustParams.ez;
		   g_t2DViewParams.tx = g_AdjustParams.tx;
		   g_t2DViewParams.ty =  g_AdjustParams.ty ;
		   g_t2DViewParams.tz = g_AdjustParams.tz;

		   xcb_avmlib_2d_Set_ViewParameter(g_t2DViewParams);
	  
	  }
	  else if(m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_3D_PARA)
	  {
		  g_t3DViewParams.aspect_ratio = g_AdjustParams.aspect_ratio;
		  g_t3DViewParams.ex = g_AdjustParams.ex ;
		  g_t3DViewParams.ey =	g_AdjustParams.ey ;
		  g_t3DViewParams.ez =	g_AdjustParams.ez;
		  g_t3DViewParams.tx = g_AdjustParams.tx;
		  g_t3DViewParams.ty =	g_AdjustParams.ty;
		  g_t3DViewParams.tz = g_AdjustParams.tz;
		  xcb_avmlib_3d_Set_ViewParameter(g_t3DViewParams, &g_car_state_Handle);

	  
	  }
	  else if(m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_FL_PARA)
	  {
		  g_tFLViewParams.aspect_ratio = g_AdjustParams.aspect_ratio;
		  g_tFLViewParams.ex = g_AdjustParams.ex ;
		  g_tFLViewParams.ey =	g_AdjustParams.ey ;
		  g_tFLViewParams.ez =	g_AdjustParams.ez;
		  g_tFLViewParams.tx = g_AdjustParams.tx;
		  g_tFLViewParams.ty =	g_AdjustParams.ty ;
		  g_tFLViewParams.tz = g_AdjustParams.tz;
		   xcb_avmlib_2d_Set_ViewParameter(g_tFLViewParams);

	  
	  }
	  else if(m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_FR_PARA)
	  {
		  g_tFRViewParams.aspect_ratio = g_AdjustParams.aspect_ratio;
		  g_tFRViewParams.ex = g_AdjustParams.ex ;
		  g_tFRViewParams.ey =	g_AdjustParams.ey ;
		  g_tFRViewParams.ez =	g_AdjustParams.ez;
		  g_tFRViewParams.tx = g_AdjustParams.tx;
		  g_tFRViewParams.ty =	g_AdjustParams.ty ;
		  g_tFRViewParams.tz = g_AdjustParams.tz;
		  xcb_avmlib_2d_Set_ViewParameter(g_tFRViewParams);
	  }
	  
	  return;
  
}




void serial::keyboard_action(char lKey)
{
  if (lKey == '1')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CAR_STATE;
    printf("Msg type: car state\n");
    return;
  }
  if (lKey == '2')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_TOUCH_OPERATE;
    printf("Msg type: touch operation\n");
    return;
  }
  if (lKey == '3')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_VIEW_SWITCH;
    printf("Msg type: view switch\n");
    return;
  }
  if (lKey == '4')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_2D_3D_VIEW;
    printf("Msg type: 2d or 3d\n");
    return;
  }
  if (lKey == '5')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CALIB;
    printf("Msg type: calibration\n");
    return;
  }
  if (lKey == '6')
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CAR_COLOR;
    printf("Msg type: car color\n");
    return;
  }
  if (lKey == '7') 
  {

    printf("adjust view 2d\n");
	m_tMsg_t7.emType = XCB_AVM_ADJ_CAMER_2D_PARA;

	g_AdjustParams.win_x = g_t2DViewParams.win_x;
	g_AdjustParams.win_y = g_t2DViewParams.win_y;
	g_AdjustParams.win_width = g_t2DViewParams.win_width;
	g_AdjustParams.win_height = g_t2DViewParams.win_height;
	g_AdjustParams.fov = g_t2DViewParams.fov;
	
	g_AdjustParams.aspect_ratio = g_t2DViewParams.aspect_ratio;
	g_AdjustParams.ex = g_t2DViewParams.ex;
	g_AdjustParams.ey = g_t2DViewParams.ey;
	g_AdjustParams.ez = g_t2DViewParams.ez;
	g_AdjustParams.tx = g_t2DViewParams.tx;
	g_AdjustParams.ty = g_t2DViewParams.ty;
	g_AdjustParams.tz = g_t2DViewParams.tz;
	adj_x = 0;
	adj_y = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_t2DViewParams);
    return;
  } 
  if (lKey == '8') 
  {

    printf("adjust view 3d\n");
	m_tMsg_t7.emType = XCB_AVM_ADJ_CAMER_3D_PARA;

	g_AdjustParams.win_x = g_t3DViewParams.win_x;
	g_AdjustParams.win_y = g_t3DViewParams.win_y;
	g_AdjustParams.win_width = g_t3DViewParams.win_width;
	g_AdjustParams.win_height = g_t3DViewParams.win_height;
	g_AdjustParams.fov = g_t3DViewParams.fov;
	
	g_AdjustParams.aspect_ratio = g_t3DViewParams.aspect_ratio;
	g_AdjustParams.ex = g_t3DViewParams.ex;
	g_AdjustParams.ey = g_t3DViewParams.ey;
	g_AdjustParams.ez = g_t3DViewParams.ez;
	g_AdjustParams.tx = g_t3DViewParams.tx;
	g_AdjustParams.ty = g_t3DViewParams.ty;
	g_AdjustParams.tz = g_t3DViewParams.tz;
	adj_x = 0;
	adj_y = 0;
    xcb_avmlib_3d_Set_ViewParameter(g_t3DViewParams, &g_car_state_Handle);


	printf("adj_x=%d,ex=%f,ey=%f,ez=%f,tx=%f,ty=%f,tz=%f\n",adj_x,g_AdjustParams.ex,g_AdjustParams.ey,g_AdjustParams.ez,
	g_AdjustParams.tx,g_AdjustParams.ty,g_AdjustParams.tz);

	
    return;
  } 
  if (lKey == '9') 
  {

    printf("adjust single view front left\n");
	m_tMsg_t7.emType = XCB_AVM_ADJ_CAMER_FL_PARA;

	g_AdjustParams.win_x = g_tFLViewParams.win_x;
	g_AdjustParams.win_y = g_tFLViewParams.win_y;
	g_AdjustParams.win_width = g_tFLViewParams.win_width;
	g_AdjustParams.win_height = g_tFLViewParams.win_height;
	g_AdjustParams.fov = g_tFLViewParams.fov;
	
	g_AdjustParams.aspect_ratio = g_tFLViewParams.aspect_ratio;
	g_AdjustParams.ex = g_tFLViewParams.ex;
	g_AdjustParams.ey = g_tFLViewParams.ey;
	g_AdjustParams.ez = g_tFLViewParams.ez;
	g_AdjustParams.tx = g_tFLViewParams.tx;
	g_AdjustParams.ty = g_tFLViewParams.ty;
	g_AdjustParams.tz = g_tFLViewParams.tz;
	adj_x = 0;
	adj_y = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tFLViewParams);
    return;
  } 

  if (lKey == 'm') 
  {

    printf("adjust single view front right\n");
	m_tMsg_t7.emType = XCB_AVM_ADJ_CAMER_FR_PARA;

	g_AdjustParams.win_x = g_tFRViewParams.win_x;
	g_AdjustParams.win_y = g_tFRViewParams.win_y;
	g_AdjustParams.win_width = g_tFRViewParams.win_width;
	g_AdjustParams.win_height = g_tFRViewParams.win_height;
	g_AdjustParams.fov = g_tFRViewParams.fov;
	
	g_AdjustParams.aspect_ratio = g_tFRViewParams.aspect_ratio;
	g_AdjustParams.ex = g_tFRViewParams.ex;
	g_AdjustParams.ey = g_tFRViewParams.ey;
	g_AdjustParams.ez = g_tFRViewParams.ez;
	g_AdjustParams.tx = g_tFRViewParams.tx;
	g_AdjustParams.ty = g_tFRViewParams.ty;
	g_AdjustParams.tz = g_tFRViewParams.tz;
	adj_x = 0;
	adj_y = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tFRViewParams);
    return;
  } 




#if 0
  if(lKey == '7')  //BSD状态 MOD状态切换
  {
      if(bsd_mod_status == 0)
      {
         bsd_mod_status = 1;
		 printf("Simulation MOD FUN\n");
      }
	  else
	  {
		 bsd_mod_status = 0;
	     printf("Simulation BSD FUN\n");

	  }

    return;
  }
  if(lKey == '8')
  {
      if(bsd_mod_status == 1)
      {
		  mod_back_warning = !mod_back_warning;
		  printf("back warn = %d\n",mod_back_warning);
      }
	  else
	  {
	      g_avm_bsd_status = 0;
		  printf("bsd no warn message\n");
	  } 
	  return;
  }
  if(lKey == '9')
  {
      if(bsd_mod_status == 1)
      {
		  mod_left_warning = !mod_left_warning;
		  printf("left warn = %d\n",mod_left_warning);
      }
	  else
	  {
		  g_avm_bsd_status = 1;
	      printf("bsd left warn\n");

	  }
	  return;
  }
  if (lKey == '*')
  {
	  if(bsd_mod_status == 1)
	  {
	     mod_front_warning = !mod_front_warning;
	     printf("font warn = %d\n",mod_front_warning);
	  }
	  else
	  {
		  g_avm_bsd_status = 2;
		  printf("bsd right warn\n");
	  
}

    return;
  }
  if (lKey == '#')
  {
	  if(bsd_mod_status == 1)
	  {
     	mod_right_warning = !mod_right_warning;
     	printf("right warn = %d\n",mod_right_warning);
	  }
	  else
	  {
		  g_avm_bsd_status = 3;
		   printf("bsd left right warn\n");
	  
   }

    return;
  }
#endif

  
  if (lKey == '0')
  {
    cur_car_state_print();
    return;
  }

  if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_CAR_STATE)
  {
    sim_get_car_state(lKey);
  }
  else if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_TOUCH_OPERATE)
  {
    sim_get_touch_state(lKey);
  }
  else if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_VIEW_SWITCH)
  {
    sim_get_view_state(lKey);
  }
  else if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_2D_3D_VIEW)
  {
    sim_get_2D3D_view(lKey);
  }
  else if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_CALIB)
  {
    sim_get_calib_state(lKey);
  }
  else if (m_tMsg_t7.emType == XCB_AVM_MSG_T7_CAR_COLOR)
  {
    sim_get_car_color(lKey);
  }
  else if(m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_2D_PARA

||m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_3D_PARA
  	    ||m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_FL_PARA||m_tMsg_t7.emType == XCB_AVM_ADJ_CAMER_FR_PARA

)
  {
    sim_get_adjust_para(lKey);
  }
}

void serial::keyboard_input_process()
{

  char lKey = GetChar();

  if (lKey)
  {
    keyboard_action(lKey);

	//sim_get_calib_state(lKey);
  }
  lKey = 0;
}
#endif


void serial::msg_init()
{
  m_tMsg_t7.emType = XCB_AVM_MSG_T7_CAR_STATE;
  m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
  m_tMsg_t7.emColor = save_skin_para;  //XCB_AVM_CAR_COLOR5;
  m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_BACK; //XCB_AVM_VIEW_BIRD_FRONT; //XCB_AVM_VIEW_BIRD_FRONT;
  m_tMsg_t7.em2D3D = XCB_AVM_VIEW_TYPE_2D; //XCB_AVM_VIEW_TYPE_2D;
  m_tMsg_t7.tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_NO;
  m_tMsg_t7.adas_switch = XCB_AVM_ADAS_OPEN;
  m_tMsg_t7.tTouch.u16X = 1470;
  m_tMsg_t7.tTouch.u16Y = 360;
  reset_car_door_state();
  reset_car_light_state();
  m_tMsg_t7.tState.emGear = XCB_AVM_CAR_STATE_GEAR_PARK;
  m_tMsg_t7.tState.u8Speed = 0;
  m_tMsg_t7.tState.fDegree = 0;

  /////////for xingtu head angle
  m_tMsg_t7.tHeadangle.nHeadAngle = 0;
  m_tMsg_t7.tHeadangle.nTimeDay   = 0;
  m_tMsg_t7.tHeadangle.nTimeHour  = 0;
  m_tMsg_t7.tHeadangle.nTimemilliSecond = 0;
  m_tMsg_t7.tHeadangle.nTimeMinute      = 0;
  m_tMsg_t7.tHeadangle.nTimeSecond      = 0;
  m_tMsg_t7.ganglemcu[0] = 0;
  m_tMsg_t7.ganglemcu[1] = 0;
  m_tMsg_t7.ganglemcu[2] = 0;
  m_tMsg_t7.ganglemcu[3] = 0;
  m_tMsg_t7.ganglemcu[4] = 0;
  m_tMsg_t7.ganglemcu[5] = 0;

  m_tMsg_t7.emView = GENERAL_MODE_XINGTU_VIEW_FRONT;
  // m_tMsg_t7.energe
  // m_tMsg_t7.ver_frac;
  // m_tMsg_t7.ver_int;

  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_BEAT_DETECT;
  m_tMsg_mcu.u8Beat = XCB_AVM_BEAT_DETECT_SYNC;
  reset_fault_state();
  // m_tMsg_mcu.emCalib;
  // m_tMsg_mcu.energe
  // m_tMsg_mcu.ver_frac;
  // m_tMsg_mcu.ver_int;
  m_bReadOrSend = false;

  uchar send_cmd2[7] = {head_val1, head_val2, 0x03, 0x00,
						0xF0, 0x01, 0xF4};

  send_data(fd, send_cmd2, sizeof(send_cmd2));


  
}

#if 0
void serial::sim_data_process()
{
  msg_init();
  while (1)
  {
    keyboard_input_process();
	//usleep(5000);
  }
}
#endif


extern pthread_rwlock_t rwlock; //定义读写锁


//===========================================================================
//循环读取函数,并发送beat
void serial::data_process()
{
  int timeNum = 0;
  int nread = 0;
  fd_set rd;
  struct timeval timeout;
  msg_init();

  while (1)
  {
    if (!m_bReadOrSend)
    {

      timeout.tv_sec = 0;
      timeout.tv_usec = 0;
      FD_ZERO(&rd);
      FD_SET(fd, &rd);

      while (FD_ISSET(fd, &rd))
      {
        int flag = select(fd + 1, &rd, NULL, NULL, &timeout);
        if (flag < 0)
        {
          perror("select error\n");
        }
        else if (flag == 0)
        {
          usleep(10000);
          timeNum++;
        }
        else
        {
          nread = read(fd, m_pReadBuf, BUFLEN);
          uchar readData[BUFLEN];

#if UARTDEBUG
          printf("nread = %d\n",nread);
          for (int i = 0; i < nread; i++)
          {
		readData[i] = m_pReadBuf[i];
		printf("%X  ", readData[i]);
          }
          printf("\n\n");

#else
          //printf("nread = %d\n",nread);
          for (int i = 0; i < nread; i++)
          {
		readData[i] = m_pReadBuf[i];
		//printf("%X  ", readData[i]);
          }
          //printf("\n\n");

#endif

          if (data_parse(recBuf, readData, nread) == 1)
          {
            switch (recBuf.Type) //判断帧类型
            {
            case CMD: // CMD
            {
              receive_cmd_process(recBuf);
              uchar send_cmd[7] = {recBuf.head_1,
                                   recBuf.head_2,
                                   0x03,
                                   0x02,
                                   recBuf.msgID,
                                   0x00,
                                   (uchar)(0x05 + recBuf.msgID)};
              send_data(fd, send_cmd, sizeof(send_cmd));
            }
			break;
            case MSG: // MSG
              receive_msg_process(recBuf);
              break;
            case ACK: // ACK
              //printf("ACK message received!\n");
              break;
            }
          }
        }

        // T5每隔4000ms发送一次同步BEAT //T5---》MCU
        if (m_tMsg_mcu.u8Beat == XCB_AVM_BEAT_DETECT_UPGRADE)
        {
          if (timeNum == 400)
          {

            uchar send_cmd2[7] = {head_val1, head_val2, 0x03, 0x00,
                                  0xF0, 0x00, 0xF3};
            send_data(fd, send_cmd2, sizeof(send_cmd2));
            printf("close beat\n");
            timeNum = 0;
          }
        }
        else
        {
          // T5每隔3000ms发送一次同步BEAT //T5---》MCU
          if (timeNum == 300)
          {

            uchar send_cmd2[7] = {head_val1, head_val2, 0x03, 0x00,
                                  0xF0, 0x01, 0xF4};
            send_data(fd, send_cmd2, sizeof(send_cmd2));
            //printf("send beat\n");
            timeNum = 0;
          }
        }
      }
    }
    else
    {
      if(mcu_updata_flag == 1)
      {
        send_mcu_data_process();
      }
      else
      {
      send_cmd_process();
      }
    }
	usleep(4000);
  }
  close(fd);
}

int  serial::get_cur_view_is_change(void)
{
    return ViewChangeFlag;
}
void  serial::set_cur_view_change_flag(int flag)
{
	ViewChangeFlag = flag;
}




void serial::mcu_updata_file_size(const char *path)       
{ 
	  FILE      *fp; 
	  fp = fopen(path, "r+"); 
	  if(fp) 
	  { 
		   fseek(fp, 0, SEEK_END); 
		   mcu_file_len = ftell(fp); 
		   fclose(fp); 
		   printf("mcu_file_size==%d...\n",mcu_file_len); 
	 } 
} 


void serial::receive_cmd_process(frameType &rec) 
{
  if (rec.msgID == u8_1) // version
  {
    printf("version\n");
	frist_data = 0;
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_VERSION_INFO;
    m_tMsg_t7.u8VerInt = rec.msgData[0].whole;
    m_tMsg_t7.u8VerFrac = rec.msgData[1].whole;
    nMCUUpdateFlag = 1;

  }
  else if (rec.msgID == u8_5) // view switch,2d/3d
  {
    uint8_t data1 = rec.msgData[0].whole;
    uint8_t data2 = rec.msgData[1].whole;
    frist_data = 0;
    if(data1 == u8_0)
    {
      printf("front\n");
      //set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_3D_ROTATE;
    }
    else if(data1 == u8_1)
    {
      printf("front\n");
      //set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_FULL_IMAGE;
    }
    else if (data1 == u8_2)
    {
      printf("front\n");
      //set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_FRONT;
    }
    else if (data1 == u8_3)
    {
      printf("back\n");
    // set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_BACK;
    }
    else if (data1 == u8_4)
    {
      printf("left\n");
      //set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_LEFT;
    }
    else if (data1 == u8_5)
    {
      printf("right\n");
      //set_cur_view_change_flag(1);
      m_tMsg_t7.emView = XCB_AVM_VIEW_BIRD_RIGHT;
    }
    else if (data1 == u8_6)
    {
      m_tMsg_t7.emView = XCB_AVM_VIEW_BROAD_FRONT;
    }
    else if (data1 == u8_7)
    {
      m_tMsg_t7.emView = XCB_AVM_VIEW_BROAD_BACK;
    }
    else if (data1 == u8_8)
    {
      m_tMsg_t7.emView = XCB_AVM_VIEW_SIDE_FRONT;
    }
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_2D_3D_VIEW;
    if (data2 == u8_1)
    {
      printf("2D selected\n");
      m_tMsg_t7.em2D3D = XCB_AVM_VIEW_TYPE_2D;
    }
    else if (data2 == u8_2)
    {
      printf("3D selected\n");
      m_tMsg_t7.em2D3D = XCB_AVM_VIEW_TYPE_3D;
    }

  }
  else if (rec.msgID == u8_6)  //节能模式
  { // energe
    printf("energe\n");
	frist_data = 0;
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_ENERGE_MODE;
    uint8_t data1 = rec.msgData[0].whole;
    if (data1 == u8_0)
    {
      m_tMsg_t7.emEnerge = XCB_AVM_ENERGE_MODE_SHUTDOWN;
    }
    else if (data1 == u8_1)
    {
      m_tMsg_t7.emEnerge = XCB_AVM_ENERGE_MODE_REBOOT;
    }
  }
  else if(rec.msgID == u8_8)//辅助驾驶开关
  {
        frist_data = 0;
	    uint8_t data1 = rec.msgData[0].whole;
	    if (data1 == u8_0)
	    {
	      m_tMsg_t7.adas_switch = XCB_AVM_ADAS_CLOSE;
	    }
	    else if (data1 == u8_1)
	    {
	      m_tMsg_t7.adas_switch = XCB_AVM_ADAS_OPEN;
	    }     
  }
  else if (rec.msgID == u8_9)  //标定
  { // calib

    frist_data = 0;
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CALIB;
      uint8_t data1 = rec.msgData[0].whole;	
    if (data1 == u8_1)
    {
      printf("calib close\n");
      m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
    }
    else if (data1 == u8_2)
    {
      printf("calib start\n");
      m_tMsg_t7.emCalib = XCB_AVM_CALIB_OPENED;
    }
  }
  else if (rec.msgID == u8_a)  //车模颜色
  { // calib

    frist_data = 0;
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CAR_COLOR;
      uint8_t data1 = rec.msgData[0].whole;	
    if (data1 == u8_1)
    {
      printf("car white \n");
      m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR1;
    }
    else if (data1 == u8_2)
    {
      printf("car silver\n");
      m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR2;
    }
    else if (data1 == u8_3)
    {
      printf("car gray\n");
      m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR3;
    }
    else if (data1 == u8_4)
    {
      printf("car brown\n");
      m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR4;
    }
    else if (data1 == u8_5)
    {
      printf("car black\n");
      m_tMsg_t7.emColor = XCB_AVM_CAR_COLOR5;
    }	
  }
  else if(rec.msgID == 0x81) //收到发送MCU升级数据指令
  {
	  uchar data1 = rec.msgData[0].whole;
	  if(data1 == 0x01)
	  {
		  upg_status = 1;
		  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_UPDATA_DATA;
		  m_bReadOrSend = true;
      bShowMcuUpdate = true;
      bShowMcuUpdateSucceed = false;
      //iDvrFlag = 0;
      //StopRecord();      
      printf("serial upg_status %d\r\n", upg_status);
	  }
  }
  else if(rec.msgID == 0x83)
  {
	  mcu_updata_flag = 0;
	  upg_status = 2;
    printf("serial upg_status %d\r\n", upg_status);
  }
  else if (rec.msgID == 0x31)//角度信息
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_ANGLEVERSION;
    m_tMsg_t7.ganglemcu[0] = rec.msgData[4].whole/10;
    m_tMsg_t7.ganglemcu[1] = rec.msgData[4].whole%10;
    m_tMsg_t7.ganglemcu[2] = rec.msgData[5].whole/10;
    m_tMsg_t7.ganglemcu[3] = rec.msgData[5].whole%10;
    m_tMsg_t7.ganglemcu[4] = rec.msgData[6].whole/10;
    m_tMsg_t7.ganglemcu[5] = rec.msgData[6].whole%10;    
    printf("recieved angle version:%d %d %d %d %d %d\r\n", m_tMsg_t7.ganglemcu[0], m_tMsg_t7.ganglemcu[1], m_tMsg_t7.ganglemcu[2], 
                                                  m_tMsg_t7.ganglemcu[3], m_tMsg_t7.ganglemcu[4], m_tMsg_t7.ganglemcu[5]);

    // xcb_avm_headangle_t &tHeadangle = m_tMsg_t7.tHeadangle;    
    // tHeadangle.nHeadAngle = rec.msgData[0].whole;
    // //tHeadangle.nTimeDay   = rec.msgData[2].whole;
    // tHeadangle.nTimeHour   = rec.msgData[2].whole;
    // tHeadangle.nTimeMinute   = rec.msgData[3].whole;
    // tHeadangle.nTimeSecond   = rec.msgData[4].whole;
    // tHeadangle.nTimemilliSecond   = rec.msgData[5].whole + rec.msgData[6].whole*100;
    // for(int i = 0; i < 6; i++)
    //   m_tMsg_t7.ganglemcu[i]   = rec.msgData[i].whole;
    //printf("head angle %d \r\n", tHeadangle.nHeadAngle);
  } 
  
}

static int  is_file_exist(char* pth)
{
  if (!pth)
  {
    return -1;
  }
  if (access(pth, F_OK) == 0)
  {
    return 0;
  }
  return -1;
}
void serial::set_mcu_upg_flag(int flag)
{
	mcu_updata_flag = flag;
	step_flag = 0;
	upg_status = 0;
	cur_num = 0;
	send_sucess_flag = 0;
	if(flag == 1)
	{
    if(!is_file_exist(MCU_UPDATE_NAME))
		  Get_file_name_is_specific_directory(MCU_UPDATE_NAME, "bin", m_McuUpdName);
    else
      Get_file_name_is_specific_directory(MCU_UPDATE_NAME1, "bin", m_McuUpdName);
		printf("xcb mcu upd file name =%s\n", m_McuUpdName);
	}
	//mcu_updata_file_size(MCU_UPDATE_NAME);
}


void serial::start_mcu_upg_cmd()
{
	m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_UPDATA_START;
	mcu_updata_flag = 1;
	m_bReadOrSend = true;
	send_sucess_flag = 0;
}

int serial::get_mcu_success_status()
{
	return send_sucess_flag;
}

int serial::get_mcu_upg_status()
{
	return mcu_updata_flag;
}


int serial::get_mcu_is_rev_first_cmd()
{
	return upg_status;
}


void serial::reset_mcu_cmd()
{

	m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_UPDATA_RESET;
	mcu_updata_flag = 1;
	m_bReadOrSend = true;

}

void serial::send_file_finsh_cmd()
{
	m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_SUCCESS;
	m_bReadOrSend = true;
}


int serial::get_cur_touch_type(void)
{
	return touch_type;
}
void serial::set_cur_touch_type(int type)
{
	touch_type = type;
}

int serial::get_cur_touch_x_value(void)
{
    return touch_x;
}
int serial::get_cur_touch_y_value(void)
{
	return touch_y;
}

void serial::set_cur_touch_x_value(int x)
{
	touch_x = x;
}
void serial::set_cur_touch_y_value(int y)
{
	touch_y = y;
}

void serial::Xingtu_Vehicle_Status_Information_Processing(void)
{
    int cur_speed = 0;
    xcb_avm_car_state_t &tState = m_tMsg_t7.tState;
    if(tState.emGear != XCB_AVM_CAR_STATE_GEAR_REVERSE)  //非倒车状态处理
    {
      cur_speed = tState.u8Speed;
      if((cur_speed <= 55 || (cur_speed>55 && cur_speed < 65 && cur_speed > Save_Speed_Value))
          &&(tState.tLight.bLeft))
      {
        m_tMsg_t7.emView = STEERING_MODE_XINGTU_LEFTDOWN;
      }
      else if((cur_speed <= 55 || (cur_speed>55 && cur_speed < 65 && cur_speed > Save_Speed_Value))
          &&(tState.tLight.bRight))
      {
        m_tMsg_t7.emView = STEERING_MODE_XINGTU_RIGHTDOWN;
      }      
      else
      {
        m_tMsg_t7.emView = GENERAL_MODE_XINGTU_VIEW_FRONT;
      }
      Save_Speed_Value = cur_speed;
    }
    else
    {
      m_tMsg_t7.emView = REVERSE_MODE_XINGTU_VIEW_BACK;
    }
}

int my_itoa(int val, char* buf)
{
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;            //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;

    b = p;

    do
    {
        a = u % radix;
        u /= radix;

        *p++ = a + '0';

    } while (u > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}

void serial::receive_msg_process(frameType &rec)
{
  //printf("message info: %d\r\n",rec.msgID);
  if (rec.msgID == u8_2)//汽车的状态
  { // car state
    // printf("car state\n");
    frist_data = 0;
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_CAR_STATE;
    xcb_avm_car_state_t &tState = m_tMsg_t7.tState;
    // gear
    uint8_t data1 = rec.msgData[0].whole;
    if (data1 == u8_0)
    {
      tState.emGear = XCB_AVM_CAR_STATE_GEAR_PARK;
      //printf("P gear\n");
    }
    else if (data1 == u8_1)
    {
      tState.emGear = XCB_AVM_CAR_STATE_GEAR_REVERSE;
      //printf("R gear\n");
    }
    else if (data1 == u8_2)
    {
      tState.emGear = XCB_AVM_CAR_STATE_GEAR_NEUTRAL;
      //printf("N gear\n");
    }
    else if (data1 == u8_3)
    {
      tState.emGear = XCB_AVM_CAR_STATE_GEAR_DRIVE;
      //printf("D gear\n");
    }

    // light
    utype data2 = rec.msgData[1];
    tState.tLight.u8Val   = data2.whole;
    tState.tLight.bLeft   = data2.bits.bit0 == u8_1 ? true : false;
    if(tState.tLight.bLeft)
    {
      //printf("turn left light on\n");
    }
    else
    {
      //printf("turn left light off\n");
    }

    tState.tLight.bRight  = data2.bits.bit1 == u8_1 ? true : false;
    if(tState.tLight.bRight)
    {
      //printf("turn right light on\n");
    }
    else
    {
      //printf("turn right light off\n");
    }

    tState.tLight.bHazard = data2.bits.bit2 == u8_1 ? true : false;
    tState.tLight.bDipped = data2.bits.bit3 == u8_1 ? true : false;

    // door
    utype data3 = rec.msgData[2];
    tState.tDoor.u8Val = data3.whole;
    tState.tDoor.bBonnet = data3.bits.bit0 == u8_1 ? true : false;
    tState.tDoor.bFL = data3.bits.bit1 == u8_1 ? true : false;
    tState.tDoor.bFR = data3.bits.bit2 == u8_1 ? true : false;
    tState.tDoor.bBoot = data3.bits.bit3 == u8_1 ? true : false;
    tState.tDoor.bBL = data3.bits.bit4 == u8_1 ? true : false;
    tState.tDoor.bBR = data3.bits.bit5 == u8_1 ? true : false;

    // speed
    tState.u8Speed = rec.msgData[3].whole;

    // TODO wheel
    uint8_t data4 = rec.msgData[4].whole;
    uint16_t u16Data1 = rec.msgData[5].whole;
    uint8_t flag = data4 & 0x80;

    uint16_t u16Data = (data4 & 0x7F);
    u16Data = (u16Data << 8) | u16Data1;

    float fwheel_steering = (float)u16Data;
    if (flag == (uint8_t)(0x80))
    { // right
      tState.fDegree = fwheel_steering / 16.87;
    }
    else
    { // left
      tState.fDegree = -fwheel_steering / 16.83;
    }
    //printf("speed %d\r\n", tState.u8Speed);
////////////////////依据车身信息获取显示模式信息
    Xingtu_Vehicle_Status_Information_Processing();
  }
  else if (rec.msgID == u8_3)//触摸操作
  { // touch operation
    pthread_rwlock_wrlock(&rwlock);
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_TOUCH_OPERATE;
    xcb_avm_touch_state_t &tTouch = m_tMsg_t7.tTouch;
    uint8_t data1 = rec.msgData[0].whole;

	if(frist_data == 0)
	{
	    frist_data = 1;
		tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_DOWN;
	}
	else
	{
	    if (data1 == u8_0)
	    {
	      tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_NO;
		  frist_data = 0;
	      //set_cur_touch_type(XCB_AVM_TOUCH_OPERATE_NO);
	    }
	    else if(data1 == u8_1)
	    {
	      tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_DOWN;
	      //set_cur_touch_type(XCB_AVM_TOUCH_OPERATE_DOWN);
	    }
	    else if(data1 == u8_2)
	    {
	      tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_UP;
		  frist_data = 0;
	      //set_cur_touch_type(XCB_AVM_TOUCH_OPERATE_UP);
	    }
	    else if(data1 == u8_3)
	    {
	      tTouch.emOpt = XCB_AVM_TOUCH_OPERATE_MOVE;
	       //set_cur_touch_type(XCB_AVM_TOUCH_OPERATE_MOVE);
	    }
	}
    uint16_t u16Data1 = rec.msgData[1].whole;
    uint16_t u16Data2 = rec.msgData[2].whole;
    tTouch.u16X = (u16Data1 << 8) | u16Data2;
	//set_cur_touch_x_value(tTouch.u16X);
    u16Data1 = rec.msgData[3].whole;
    u16Data2 = rec.msgData[4].whole;
    tTouch.u16Y = (u16Data1 << 8) | u16Data2;
	  pthread_rwlock_unlock(&rwlock);
	  //printf("xcb tTouch.emOpt=%d,tTouch.u16X=%d,tTouch.u16Y=%d\n",tTouch.emOpt,tTouch.u16X,tTouch.u16Y);
  }
  /////////for xingtu use
  else if (rec.msgID == 0x30)//转向角度
  {
#if 0
    printf("[%s][%d] ****time1*** rec.msgID == 0x30=%d \n", __func__,__LINE__,rec.msgID);
#endif
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_HEADANGLE;
    xcb_avm_headangle_t &tHeadangle = m_tMsg_t7.tHeadangle;
    tHeadangle.nHeadAngle = rec.msgData[0].whole;
    //("angle: %d\r\n", tHeadangle.nHeadAngle);
    tHeadangle.nAlarmState = rec.msgData[3].whole;
    tHeadangle.nTimeYear = rec.msgData[7].whole;
    tHeadangle.nTimeMonth = rec.msgData[8].whole;
    tHeadangle.nTimeDay = rec.msgData[9].whole;
    tHeadangle.nTimeHour = rec.msgData[10].whole;
    tHeadangle.nTimeMinute = rec.msgData[11].whole;
    tHeadangle.nTimeSecond = rec.msgData[12].whole;
    tHeadangle.nTimemilliSecond = rec.msgData[13].whole + rec.msgData[14].whole * 100;

    unsigned char ft;
    ft = (tHeadangle.nTimeYear % 100);
    curyearH = ft / 10;
    curyearL = ft % 10;

    ft = (tHeadangle.nTimeMonth % 100);
    curmonH = ft / 10;
    curmonL = ft % 10;

    ft = (tHeadangle.nTimeDay % 100);
    curdayH = ft / 10;
    curdayL = ft % 10;

    ft = (tHeadangle.nTimeHour % 100);
    curhourH = ft / 10;
    curhourL = ft % 10;

    ft = (tHeadangle.nTimeMinute % 100);
    curminuteH = ft / 10;
    curminuteL = ft % 10;

    ft = (tHeadangle.nTimeSecond % 100);
    cursecH = ft / 10;
    cursecL = ft % 10;

    // printf("time: %d%d-%d%d-20%d%d   %d%d:%d%d:%d%d\r\n", curmonH, curmonL,
    //        curdayH, curdayL, curyearH, curyearL, curhourH, curhourL,
    //        curminuteH, curminuteL, cursecH, cursecL);
    if (iDvrFlag == 0 && tHeadangle.nTimeYear > 0)
    {
#if 1
    printf("[%s][%d] ******* iDvrFlag ==%d \n", __func__,__LINE__,iDvrFlag);
#endif
		iDvrFlag  =1;
		char  datestring[100];
		char* datestr ="date -s ";//date -s "2019-6-21 15:19:45"
		char yearstr[3];
		char monthstr[3];
		char daystr[3];
		char hourstr[3];
		char minutestr[3];
		char secondstr[3];
		char* str1 ="\"";	//date -s "2019-6-21 15:19:45"
		char* str2 ="\"";	//date -s "2019-6-21 15:19:45"

		printf("nTimeYear: %d \n", tHeadangle.nTimeYear); 
        printf("nTimeMonth: %d \n", tHeadangle.nTimeMonth);
        printf("nTimeDay: %d \n", tHeadangle.nTimeDay);

        printf("nTimeHour: %d \n", tHeadangle.nTimeHour);
        printf("nTimeMinute: %d \n", tHeadangle.nTimeMinute);
        printf("nTimeSecond: %d \n", tHeadangle.nTimeSecond);
    int ntotalyear;
		//tHeadangle.nTimeYear
    ntotalyear =2000+tHeadangle.nTimeYear;

		sprintf(yearstr, "%d", ntotalyear);
		sprintf(monthstr, "%d", tHeadangle.nTimeMonth);
		sprintf(daystr, "%d", tHeadangle.nTimeDay);
		sprintf(hourstr, "%d", tHeadangle.nTimeHour);
		sprintf(minutestr, "%d", tHeadangle.nTimeMinute);
		sprintf(secondstr, "%d", tHeadangle.nTimeSecond);

		printf("yearstr: %s \n", yearstr); 
		printf("monthstr: %s \n", monthstr); 
		printf("daystr: %s \n", daystr); 

		printf("hourstr: %s \n", hourstr); 
		printf("minutestr: %s \n", minutestr); 
		printf("secondstr: %s \n", secondstr); 
		sprintf(yearstr,"%s-%s-%s %s:%s:%s",yearstr,monthstr,daystr,hourstr,minutestr,secondstr);

		char  datestr1[100];
		sprintf(datestr1,"%s%s",str1,yearstr);
		//printf("datestr1: %s \n", datestr1); 

		sprintf(datestr1,"%s%s",datestr1,str2);
		printf("datestr1: %s \n", datestr1); 

		sprintf(datestring,"%s%s",datestr,datestr1);
		printf("settime::datestring : %s \n", datestring); 

		system(datestring);
		system("sync");

		//printf("head angle %d \r\n", tHeadangle.nHeadAngle);
	}
  }
  else if (rec.msgID == 0x31)//角度信息
  {
    m_tMsg_t7.emType = XCB_AVM_MSG_T7_ANGLEVERSION;
    m_tMsg_t7.ganglemcu[0] = rec.msgData[4].whole/10;
    m_tMsg_t7.ganglemcu[1] = rec.msgData[4].whole%10;
    m_tMsg_t7.ganglemcu[2] = rec.msgData[5].whole/10;
    m_tMsg_t7.ganglemcu[3] = rec.msgData[5].whole%10;
    m_tMsg_t7.ganglemcu[4] = rec.msgData[6].whole/10;
    m_tMsg_t7.ganglemcu[5] = rec.msgData[6].whole%10;    
    printf("recieved angle version:%d %d %d %d %d %d\r\n", m_tMsg_t7.ganglemcu[0], m_tMsg_t7.ganglemcu[1], m_tMsg_t7.ganglemcu[2], 
                                                  m_tMsg_t7.ganglemcu[3], m_tMsg_t7.ganglemcu[4], m_tMsg_t7.ganglemcu[5]);

    // xcb_avm_headangle_t &tHeadangle = m_tMsg_t7.tHeadangle;    
    // tHeadangle.nHeadAngle = rec.msgData[0].whole;
    // //tHeadangle.nTimeDay   = rec.msgData[2].whole;
    // tHeadangle.nTimeHour   = rec.msgData[2].whole;
    // tHeadangle.nTimeMinute   = rec.msgData[3].whole;
    // tHeadangle.nTimeSecond   = rec.msgData[4].whole;
    // tHeadangle.nTimemilliSecond   = rec.msgData[5].whole + rec.msgData[6].whole*100;
    // for(int i = 0; i < 6; i++)
    //   m_tMsg_t7.ganglemcu[i]   = rec.msgData[i].whole;
    //printf("head angle %d \r\n", tHeadangle.nHeadAngle);
  }  
  else
  {
	  frist_data = 0;
  }
}
#define MCUCODELEN 64
void serial::send_version_info(uchar uVerHigh, uchar uVerLow)
{
  m_tMsg_mcu.emType = XCB_AVM_MSG_MCU_VERSION_INFO;
  m_tMsg_mcu.u8VerInt = uVerHigh;
  m_tMsg_mcu.u8VerFrac = uVerLow;
  m_bReadOrSend = true;
}
static long totallen = 0;
void serial::send_mcu_data_process()
{
	memset(m_pMcuBuf, 0, BUFLEN_MCU);
	m_pMcuBuf[0] = head_val1;
	m_pMcuBuf[1] = head_val2;
	m_pMcuBuf[3] = 0x01;
	switch(m_tMsg_mcu.emType)
	{
		case XCB_AVM_MSG_MCU_UPDATA_START:
		{
			m_pMcuBuf[2] = 0x03;
			m_pMcuBuf[4] = 0x80;
			m_pMcuBuf[5] = 0x01;
			m_pMcuBuf[6] = 0x85;
			send_data(fd, m_pMcuBuf, 7);
      printf("send MCU start info\n");
      //usleep(10000);
      //usleep(6000);
		}
		break;
		case XCB_AVM_MSG_MCU_UPDATA_DATA:
		{
			FILE   *fp;
			unsigned int csnum = 0;
			unsigned int len=0; 
			unsigned int offset;
			uchar file_buff[130]={0};
			fp = fopen(m_McuUpdName, "r+");
			if(fp) 
			{ 
				  offset = cur_num*MCUCODELEN;
				  fseek(fp, offset, SEEK_SET); 
				  len = fread(file_buff ,sizeof(uchar),MCUCODELEN, fp); 
				  fclose(fp);
          totallen += len;
          printf("send times: %d, len: %d, totallen: %d\r\n", cur_num, len, totallen); 
          
			} 
			if(len <= 0)
			{
				printf("file send finsh\n");
				cur_num = 0;
        totallen = 0;
				m_pMcuBuf[2] = 0x03;
				m_pMcuBuf[4] = 0x80;
				m_pMcuBuf[5] = 0x02;
				m_pMcuBuf[6] = 0x86;
				send_data(fd, m_pMcuBuf, 7);
				send_sucess_flag = 1;
        bShowMcuUpdate = false;
        bShowMcuUpdateSucceed = true;
				break;
			}
			m_pMcuBuf[2] = len+2;
			m_pMcuBuf[4] = 0x82;
			csnum = m_pMcuBuf[2]+m_pMcuBuf[3]+m_pMcuBuf[4];
			for(int i=0;i<len;i++)
			{
			   m_pMcuBuf[5+i] = file_buff[i];
			   csnum+=file_buff[i];
			}
			m_pMcuBuf[len+5] = csnum;
			send_data(fd, m_pMcuBuf, len+6);
			cur_num++;
			usleep(6000);
		}
		break;
		case XCB_AVM_MSG_MCU_SUCCESS:
		{
			m_pMcuBuf[2] = 0x03;
			m_pMcuBuf[4] = 0x80;
			m_pMcuBuf[5] = 0x02;
			m_pMcuBuf[6] = 0x86;
			send_data(fd, m_pMcuBuf, 7);
		}
		break;
		case XCB_AVM_MSG_MCU_UPDATA_RESET:
		{
			m_pMcuBuf[2] = 0x03;
			m_pMcuBuf[4] = 0x88;
			m_pMcuBuf[5] = 0x00;
			m_pMcuBuf[6] = 0x8C;
			send_data(fd, m_pMcuBuf, 7);
			mcu_updata_flag = 0;
		}
		break;
	}
	m_bReadOrSend = false;



}
void serial::send_cmd_process()
{
  memset(m_pSendBuf, 0, BUFLEN);
  m_pSendBuf[0] = head_val1;
  m_pSendBuf[1] = head_val2;
  m_pSendBuf[3] = 0x01;
  switch (m_tMsg_mcu.emType)
  {
  case XCB_AVM_MSG_MCU_VERSION_INFO:
    m_pSendBuf[2] = 0x04;
    m_pSendBuf[4] = 0x41;
    m_pSendBuf[5] = m_tMsg_mcu.u8VerInt;
    m_pSendBuf[6] = m_tMsg_mcu.u8VerFrac;
    m_pSendBuf[7] = m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] +
                    m_pSendBuf[5] + m_pSendBuf[6];
    m_nSendLen = 8;
    send_cmd_data();
    break;
  case XCB_AVM_MSG_MCU_CALIB_STATE:
    m_pSendBuf[2] = 0x03;
    m_pSendBuf[4] = 0x43;
    m_pSendBuf[5] = m_tMsg_mcu.u8Calib;
    if (m_pSendBuf[5] == u8_0)
    {
      m_pSendBuf[6] = 0x47;
    }
    else
    {
      m_pSendBuf[6] = 0x48;
    }
    m_nSendLen = 7;
    send_cmd_data();
    break;
  case XCB_AVM_MSG_MCU_ENERGE_MODE:
    m_pSendBuf[2] = 0x03;
    m_pSendBuf[4] = 0x46;
    m_pSendBuf[5] = m_tMsg_mcu.u8Energe;
    if (m_pSendBuf[5] == u8_0)
    {
      m_pSendBuf[6] = 0x4a;
    }
    else
    {
      m_pSendBuf[6] = 0x4b;
    }
    m_nSendLen = 7;
    send_cmd_data();
    break;
  case XCB_AVM_MSG_MCU_FAULT_INFO:
    m_pSendBuf[2] = 0x03;
    m_pSendBuf[4] = 0x42;
    m_pSendBuf[5] = m_tMsg_mcu.u8Fault;
    m_pSendBuf[6] =
        m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5];
    m_nSendLen = 7;
    send_cmd_data();
    break;

  case XCB_AVM_MSG_MCU_ADAS:
  {
   uint8_t Adas = m_tMsg_mcu.u8Adas;
   m_pSendBuf[2] = 0x07;
   m_pSendBuf[3] = 0x00;
   m_pSendBuf[4] = 0x50;
   m_pSendBuf[5] = (Adas&0x01)>>0;
   m_pSendBuf[6] = (Adas&0x02)>>1;
   m_pSendBuf[7] = (Adas&0x04)>>2;
   m_pSendBuf[8] = (Adas&0x08)>>3;
   m_pSendBuf[9] = (Adas&0x10)>>4;
   m_pSendBuf[10] = m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5]+ m_pSendBuf[6]+ m_pSendBuf[7]+ m_pSendBuf[8]+ m_pSendBuf[9];   
   m_nSendLen = 11;
   send_data(fd, m_pSendBuf, m_nSendLen);
   m_bReadOrSend = false;  
  }
  break;

    //====================================================
  case XCB_AVM_MSG_MCU_BSD:
    m_pSendBuf[2] = 0x03;
	m_pSendBuf[3] = 0x00;
    m_pSendBuf[4] = 0x50;
    m_pSendBuf[5] = m_tMsg_mcu.u8Adas;
    m_pSendBuf[6] =
        m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5];
    m_nSendLen = 7;
    //send_cmd_data();
    send_data(fd, m_pSendBuf, m_nSendLen);
	m_bReadOrSend = false;
    break;

  case XCB_AVM_MSG_MCU_LDW:
    m_pSendBuf[2] = 0x03;
	m_pSendBuf[3] = 0x00;
    m_pSendBuf[4] = 0x51;
    m_pSendBuf[5] = m_tMsg_mcu.u8Adas;
    m_pSendBuf[6] =
        m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5];
    m_nSendLen = 7;
    //send_cmd_data();
    send_data(fd, m_pSendBuf, m_nSendLen);
	m_bReadOrSend = false;
    break;

  case XCB_AVM_MSG_MCU_HMW:
    m_pSendBuf[2] = 0x03;
	m_pSendBuf[3] = 0x00;
    m_pSendBuf[4] = 0x54;
    m_pSendBuf[5] = m_tMsg_mcu.u8Adas;
    m_pSendBuf[6] =
        m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5];
    m_nSendLen = 7;
    //send_cmd_data();
    send_data(fd, m_pSendBuf, m_nSendLen);
	m_bReadOrSend = false;
    break;

  case XCB_AVM_MSG_MCU_MOD:
    m_pSendBuf[2] = 0x03;
	m_pSendBuf[3] = 0x00;
    m_pSendBuf[4] = 0x55;
    m_pSendBuf[5] = m_tMsg_mcu.u8Adas;
    m_pSendBuf[6] =
        m_pSendBuf[2] + m_pSendBuf[3] + m_pSendBuf[4] + m_pSendBuf[5];
    m_nSendLen = 7;
    //send_cmd_data();
    send_data(fd, m_pSendBuf, m_nSendLen);
	m_bReadOrSend = false;
    break;

  default:
    break;
  }
}
// ACK事件处理函数
void serial::send_cmd_data()
{
  struct timeval timeout1;

  for (int i = 0; i < 10; i++)
  {
    send_data(fd, m_pSendBuf, m_nSendLen);
    fd_set rd1;
    FD_ZERO(&rd1);
    FD_SET(fd, &rd1);
    timeout1.tv_sec = 0;
    timeout1.tv_usec = 100000;
    int flag1 = select(fd + 1, &rd1, NULL, NULL, &timeout1);

    if (flag1 == 0)
      continue;
    else if (flag1 == 1)
    {
      read(fd, m_pReadBuf, BUFLEN);
      //收到ACK帧马上跳出，否则延时100ms后重新发送
      if (m_pReadBuf[3] == ACK && m_pReadBuf[4] == m_pSendBuf[4] &&
          m_pReadBuf[5] == 0x00)
      {
        m_bReadOrSend = false;
        break;
      }
      else
        usleep(100000);
    }
  }
  m_bReadOrSend = false;
}

int serial::send_data(int &fd, uchar *send_buf, int length)
{
  int ret;
  ret = write(fd, send_buf, length);
  if (ret == -1)
  {
    printf("write device error\n");
    return -1;
  }
  return ret;
}

int serial::data_parse(frameType &rec, uchar *rcv_buf, int datalen)
{
  uchar csSum = 0x0;
  rec.head_1 = rcv_buf[0];
  rec.head_2 = rcv_buf[1];
  rec.Length = rcv_buf[2];
  rec.Type = rcv_buf[3];
  rec.msgID = rcv_buf[4];
  rec.msgData.clear();
  int nMsgData = (int)(rec.Length) - 2;

  for (int i = 0; i < nMsgData; i++)
  {
    utype tempmsg;
    tempmsg.whole = rcv_buf[5 + i];
    rec.msgData.push_back(tempmsg);
    csSum += tempmsg.whole;
    // printf("0x%x ", tempmsg.whole);
  }
  // printf("\n");

  csSum = csSum + rec.Length + rec.Type + rec.msgID;
  rec.cs = rcv_buf[datalen - 1];
  if (rec.cs == csSum)
    return 1;
  else
    return 0;
}

//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================
void serial::port_init(int &fd)
{
  fd = open(UART_DEVICE, O_RDWR);

  if (fd < 0)
  {
    perror(UART_DEVICE);
    exit(1);
  }
  else
  {
    printf("open ");
    printf("%s", ttyname(fd));
    printf(" succesfully\n");
  }
  set_speed(fd, BAUDRATE);
  if (set_Parity(fd, 8, 1, 'N') == FALSE)
  {
    printf("Set Parity Error\n");
    exit(0);
  }
}

serial::serial()
{
  port_init(fd);
  m_bReadOrSend = false;
  frist_data = 0;
  mcu_updata_flag = 0;
  step_flag = 0;
  cur_num = 0;
  upg_status =0;
}

serial::~serial() { close(fd); }

/**
 *@brief  设置串口通信速率
 *@param  fd     类型 int  打开串口的文件句柄
 *@param  speed  类型 int  串口速度
 *@return  void
 */
void serial::set_speed(int &fd, int speed)
{
  int speed_arr[] = {
      B115200,
      B38400,
      B19200,
      B9600,
      B4800,
      B2400,
      B1200,
      B300,
      B115200,
      B38400,
      B19200,
      B9600,
      B4800,
      B2400,
      B1200,
      B300,
  };
  int name_arr[] = {
      115200,
      38400,
      19200,
      9600,
      4800,
      2400,
      1200,
      300,
      115200,
      38400,
      19200,
      9600,
      4800,
      2400,
      1200,
      300,
  };
  int status;
  struct termios Opt;
  tcgetattr(fd, &Opt);
  for (size_t i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
  {
    if (speed == name_arr[i])
    {
      tcflush(fd, TCIOFLUSH);
      cfsetispeed(&Opt, speed_arr[i]);
      cfsetospeed(&Opt, speed_arr[i]);
      status = tcsetattr(fd, TCSANOW, &Opt);
      if (status != 0)
      {
        perror("tcsetattr fd1");
        return;
      }
      tcflush(fd, TCIOFLUSH);
    }
  }
}

/**
 *@brief   设置串口数据位，停止位和效验位
 *@param  fd     类型  int  打开的串口文件句柄
 *@param  databits 类型  int 数据位   取值 为 7 或者8
 *@param  stopbits 类型  int 停止位   取值为 1 或者2
 *@param  parity  类型  int  效验类型 取值为N,E,O,,S
 */
int serial::set_Parity(int &fd, int databits, int stopbits, int parity)
{
  struct termios options;

  if (tcgetattr(fd, &options) != 0)
  {
    perror("SetupSerial 1");
    return (FALSE);
  }
  options.c_cflag &= ~CSIZE;
  switch (databits) /*设置数据位数*/
  {
  case 7:
    options.c_cflag |= CS7;
    break;
  case 8:
    options.c_cflag |= CS8;
    break;
  default:
    fprintf(stderr, "Unsupported data size\n");
    return (FALSE);
  }
  switch (parity)
  {
  case 'n':
  case 'N':
    options.c_cflag &= ~PARENB; /* Clear parity enable */
    options.c_iflag &= ~INPCK;  /* Enable parity checking */
    break;
  case 'o':
  case 'O':
    options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
    options.c_iflag |= INPCK;             /* Disnable parity checking */
    break;
  case 'e':
  case 'E':
    options.c_cflag |= PARENB;  /* Enable parity */
    options.c_cflag &= ~PARODD; /* 转换为偶效验*/
    options.c_iflag |= INPCK;   /* Disnable parity checking */
    break;
  case 'S':
  case 's': /*as no parity*/
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    break;
  default:
    fprintf(stderr, "Unsupported parity\n");
    return (FALSE);
  }
  /* 设置停止位*/
  switch (stopbits)
  {
  case 1:
    options.c_cflag &= ~CSTOPB;
    break;
  case 2:
    options.c_cflag |= CSTOPB;
    break;
  default:
    fprintf(stderr, "Unsupported stop bits\n");
    return (FALSE);
  }
  /* Set input parity option */
  if (parity != 'n')
    options.c_iflag |= INPCK;
  tcflush(fd, TCIFLUSH);
  options.c_cc[VTIME] = 50; /* 设置超时5 seconds*/
  options.c_cc[VMIN] = 0;   /* Update the options and do it NOW */
  options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Input*/
  options.c_oflag &= ~OPOST;                          /*Output*/

  if (tcsetattr(fd, TCSANOW, &options) != 0)
  {
    perror("SetupSerial 3");
    return (FALSE);
  }
  return (TRUE);
}

void serial::startSerialThd()
{
	std::thread serialthread(&serial::data_process, this);
  serialthread.detach();
}



