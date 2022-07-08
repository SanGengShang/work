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

#ifndef SERIAL_H
#define SERIAL_H

#include "paraset.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "sys_config.h"
#include <math.h>


#define BUFLEN_MCU  150

class serial {
public:
  serial();
  ~serial();

private:
  void port_init(int &fd);
  void set_speed(int &fd, int speed);
  int send_data(int &fd, uchar *send_buf, int length);
  int set_Parity(int &fd, int databits, int stopbits, int parity);
  int data_parse(frameType &rec, uchar *rcv_buf, int datalen);

  void msg_init();
  void keyboard_input_process();
  //没有设计“版本信息”类型消息和“电源模式”类型消息的键盘模拟
  void keyboard_action(char lKey);
  void mcu_updata_file_size(const char *path);
  void send_mcu_data_process();
  

private:
  int fd;
  frameType recBuf;
  uchar m_pReadBuf[BUFLEN];
  uchar m_pSendBuf[BUFLEN];
  uchar m_pMcuBuf[BUFLEN_MCU];
  char  m_McuUpdName[127];
  int m_nSendLen;
  int ViewChangeFlag;
  int frist_data;
  /**< false-read, true-send*/
  bool m_bReadOrSend;
  int touch_x;
  int touch_y;
  int touch_type;
  int mcu_updata_flag;
  int step_flag;
  int mcu_file_len;
  int cur_num;
  int upg_status;
  int send_sucess_flag;  
public:
  /**< Message from mcu to t7*/
  xcb_avm_msg_t7_t m_tMsg_t7;

  /**< Message from t7 to mcu*/
  xcb_avm_msg_mcu_t m_tMsg_mcu;

  void reset_car_light_state();
  void set_car_light_state(uint8_t u8Val);
  uint8_t get_car_light_state();

  void reset_car_door_state();
  void set_car_door_state(uint8_t u8Val);
  uint8_t get_car_door_state();

  /*********************************************/
  /*     Process message from t7 to mcu        */
  /*********************************************/

  void sim_data_process();
  void sim_get_car_state(char lKey);
  void sim_get_touch_state(char lKey);
  void sim_get_view_state(char lKey);
  void sim_get_2D3D_view(char lKey);
  void sim_get_calib_state(char lKey);
  void sim_get_car_color(char lKey);
  void sim_get_adjust_para(char lKey);
  void cur_car_state_print();

  void data_process();
  void receive_cmd_process(frameType &rec);
  void receive_msg_process(frameType &rec);
  void send_cmd_process();
  void send_cmd_data();

  void send_beat_on();
  void send_beat_off();
  void send_reboot_request();

  void reset_fault_state();
  void set_fault_state(xcb_avm_fault_type_t emFault);
  void set_ok_state(xcb_avm_fault_type_t emFault);

  void send_version_info(uchar uVerHigh, uchar uVerLow);
  void send_calib_state(bool bCalibOK);

  void send_mod_state(bool u8Adas);
  void send_hmw_state(bool u8Adas);
  void send_bsd_state(bool u8Adas);
  void send_ldw_state(bool u8Adas);
  void send_adas_state(uint8_t u8Adas);

  int get_cur_touch_type(void);
 void set_cur_touch_type(int type);
  int get_cur_touch_x_value(void);
  int get_cur_touch_y_value(void);
  void set_cur_touch_x_value(int x);
  void set_cur_touch_y_value(int y);
  int  get_cur_view_is_change(void);
  void  set_cur_view_change_flag(int flag);

  void set_mcu_upg_flag(int flag);

  void start_mcu_upg_cmd();

  int get_mcu_upg_status();

  int get_mcu_is_rev_first_cmd();

  int get_mcu_success_status();

  void send_file_finsh_cmd();

  void reset_mcu_cmd();

  void startSerialThd();
  void Xingtu_Vehicle_Status_Information_Processing(void);
};

#endif // SERIAL_H
