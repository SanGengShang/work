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
#ifndef _xcb_ui_H_
#define _xcb_ui_H_

#include "xcb_videoprot5.h"

/***************************UI********************************/


/**************************************************************************************/
int old_x_pos = 0;
int old_y_pos = 0;
int touch_down_flag = 0;
int touch_move_flag = 0;

static void cur_focus_icon_disp(__s32 dir);

typedef struct bmp_buf_
{
  int x;
  int y;
  char *file_name;
} bmp_buf_t;

typedef struct touch_pos_
{
	int x;
    int y;
	int w;
	int h;
}touch_pos_t;

enum  //标定
{
	CARLIB_ING=0,
	CARLIB_OK,
	CARLIB_FAIL_BACK,
	CARLIB_FAIL_LEFT,
	CARLIB_FAIL_FRONT,
	CARLIB_FAIL_RIGHT,
	CARLIB_FAIL_LEFTBACK,
	CARLIB_FAIL_RIGHTBACK,
	CARLIB_MAX_PNG,
};


static bmp_buf_t init_calib_1920bmp_res[CARLIB_MAX_PNG] =
{
        {897, 224, "calib_ing_1920.png"},
        {897, 224, "calib_ok_1920.png"},
        {897, 224, "calib_fail_back_1920.png"},
        {897, 224, "calib_fail_left_1920.png"},
        {897, 224, "calib_fail_front_1920.png"},
        {897, 224, "calib_fail_right_1920.png"},
		{897, 224, "calib_fail_left_back_1920.png"},
		{897, 224, "calib_fail_right_back_1920.png"},
};

static bmp_buf_t init_calib_1024bmp_res[CARLIB_MAX_PNG] =
{
        {514, 191, "calib_ing_1024.png"},
        {514, 191, "calib_ok_1024.png"},
        {514, 191, "calib_fail_back_1024.png"},
        {514, 191, "calib_fail_left_1024.png"},
        {514, 191, "calib_fail_front_1024.png"},
        {514, 191, "calib_fail_right_1024.png"},
		{514, 191, "calib_fail_left_back_1024.png"},
		{514, 191, "calib_fail_right_back_1024.png"},
};

static bmp_buf_t init_calib_1280bmp_res[CARLIB_MAX_PNG] =
{
        {593, 241, "calib_ing_1280.png"},
        {593, 241, "calib_ok_1280.png"},
        {593, 241, "calib_fail_back_1280.png"},
        {593, 241, "calib_fail_left_1280.png"},
        {593, 241, "calib_fail_front_1280.png"},
        {593, 241, "calib_fail_right_1280.png"},
		{593, 241, "calib_fail_left_back_1280.png"},
		{593, 241, "calib_fail_right_back_1280.png"},
};


enum  //提示错误
{
	ERR_FRONT=0,
	ERR_BACK,
	ERR_LEFT,
	ERR_RIGHT,
	ERR_MAX_PNG,
};


static bmp_buf_t init_err_1920bmp_res[ERR_MAX_PNG] =
{
        {897, 224, "cam_err_front_1920.png"},
        {897, 224, "cam_err_back_1920.png"},
        {897, 224, "cam_err_left_1920.png"},
        {897, 224, "cam_err_right_1920.png"},
};

static bmp_buf_t init_err_1024bmp_res[ERR_MAX_PNG] =
{
        {514, 191, "cam_err_front_1024.png"},
        {514, 191, "cam_err_back_1024.png"},
        {514, 191, "cam_err_left_1024.png"},
        {514, 191, "cam_err_right_1024.png"},
};

static bmp_buf_t init_err_1280bmp_res[ERR_MAX_PNG] =
{
        {593, 241, "cam_err_front_1280.png"},
        {593, 241, "cam_err_back_1280.png"},
        {593, 241, "cam_err_left_1280.png"},
        {593, 241, "cam_err_right_1280.png"},
};


enum  //升级UI
{
	UPGRADE_DET_OK=0,
	UPGRADE_DET_FAIL,
	UPGRADE_INVALID_FILE,
	UPGRADE_INVALID_VERSION,
	UPGRADE_ING1,
	UPGRADE_ING2,
	UPGRADE_ING3,
	UPGRADE_SUCCEED,
	UPGRADE_FAIL,
	UPGRADE_SYSTEM,
	UPGRADE_MAX_PNG,
};


static bmp_buf_t init_upgrade_1920bmp_res[UPGRADE_MAX_PNG] =
{
        {897, 224, "upgrade_detect_ok_1920.png"},
        {897, 224, "upgrade_detect_fail_1920.png"},
        {897, 224, "upgrade_invalid_file_1920.png"},
        {897, 224, "upgrade_invalid_version_1920.png"},
        {897, 224, "upgrad_ing1_1920.png"},
        {897, 224, "upgrad_ing2_1920.png"},
        {897, 224, "upgrad_ing3_1920.png"},
        {897, 224, "upgrade_succeed_1920.png"},
        {897, 224, "upgrade_fail_1920.png"},
        {897, 224, "upgrade_system_1920.png"},
};

static bmp_buf_t init_upgrade_1024bmp_res[UPGRADE_MAX_PNG] =
{
        {514, 191, "upgrade_detect_ok_1024.png"},
        {514, 191, "upgrade_detect_fail_1024.png"},
        {514, 191, "upgrade_invalid_file_1024.png"},
        {514, 191, "upgrade_invalid_version_1024.png"},
        {514, 191, "upgrad_ing1_1024.png"},
        {514, 191, "upgrad_ing2_1024.png"},
        {514, 191, "upgrad_ing3_1024.png"},
        {514, 191, "upgrade_succeed_1024.png"},
        {514, 191, "upgrade_fail_1024.png"},
        {514, 191, "upgrade_system_1024.png"},
};

static bmp_buf_t init_upgrade_1280bmp_res[UPGRADE_MAX_PNG] =
{
        {593, 241, "upgrade_detect_ok_1280.png"},
        {593, 241, "upgrade_detect_fail_1280.png"},
        {593, 241, "upgrade_invalid_file_1280.png"},
        {593, 241, "upgrade_invalid_version_1280.png"},
        {593, 241, "upgrad_ing1_1280.png"},
        {593, 241, "upgrad_ing2_1280.png"},
        {593, 241, "upgrad_ing3_1280.png"},
        {593, 241, "upgrade_succeed_1280.png"},
        {593, 241, "upgrade_fail_1280.png"},
        {593, 241, "upgrade_system_1280.png"},
};


enum
{
  //CAMER_FT_UF_ICON = 0,
  //CAMER_FT_F_ICON,
  //CAMER_RT_UF_ICON,
  //CAMER_RT_F_ICON,
  //CAMER_AF_UF_ICON,
  //CAMER_AF_F_ICON,
  //CAMER_LT_UF_ICON,
  //CAMER_LT_F_ICON,
  CAMER_CAR_BACK=0,
  CAMER_CAR_FRONT,
  CAMER_CAR_LEFT,
  CAMER_CAR_RIGHT,
  CAMER_TIPS_TXT,
  CAMER_RETURN,
  MAX_CMAER_PNG,
};

static bmp_buf_t init_1920bmp_res[MAX_CMAER_PNG] =
{
        //{131, 35, "cam_F0.png"},
        //{131, 35, "cam_F1.png"},
        //{228, 240, "cam_R0.png"},
        //{228, 240, "cam_R1.png"},
        //{131, 431, "cam_B0.png"},
        //{131, 431, "cam_B1.png"},
        //{36, 240, "cam_L0.png"},
        //{36, 240, "cam_L1.png"},
        {606, 548, "little_car_back_1920.png"},
        {606, 548, "little_car_front_1920.png"},
        {606, 548, "little_car_left_1920.png"},
        {606, 548, "little_car_right_1920.png"},
        {1025, 599, "chk_surrounding_1920.png"},
        {481, 0, "slider_1920.png"},
};

static bmp_buf_t init_1024bmp_res[MAX_CMAER_PNG] =
{
        //{131, 35, "cam_F0.png"},
        //{131, 35, "cam_F1.png"},
        //{228, 240, "cam_R0.png"},
        //{228, 240, "cam_R1.png"},
        //{131, 431, "cam_B0.png"},
        //{131, 431, "cam_B1.png"},
        //{36, 240, "cam_L0.png"},
        //{36, 240, "cam_L1.png"},
        {435, 487, "little_car_back_1024.png"},
        {435, 487, "little_car_front_1024.png"},
        {435, 487, "little_car_left_1024.png"},
        {435, 487, "little_car_right_1024.png"},
        {570, 518, "chk_surrounding_1024.png"},
        {341, 0, "slider_1024.png"},
};

static bmp_buf_t init_simple_1024bmp_res[MAX_CMAER_PNG] =
{
        //{131, 35, "cam_F0.png"},
        //{131, 35, "cam_F1.png"},
        //{228, 240, "cam_R0.png"},
        //{228, 240, "cam_R1.png"},
        //{131, 431, "cam_B0.png"},
        //{131, 431, "cam_B1.png"},
        //{36, 240, "cam_L0.png"},
        //{36, 240, "cam_L1.png"},
        {294, 0, "simple_back_1024.png"},
        {294, 0, "simple_front_1024.png"},
        {294, 0, "simple_left_1024.png"},
        {294, 0, "simple_right_back.png"},
        {570, 518, "chk_surrounding_1024.png"},
        {290, 0, "slider_simple_1024.png"},
};

static bmp_buf_t init_1280bmp_res[MAX_CMAER_PNG] =
{
        {538, 577, "little_car_back_1280.png"},
        {538, 577, "little_car_front_1280.png"},
        {538, 577, "little_car_left_1280.png"},
        {538, 577, "little_car_right_1280.png"},
        {696, 616, "chk_surrounding_1280.png"},
        {426, 0, "slider_1280.png"},
};



enum
{
	//adas MODB
	CAMER_MOD_B=0,
	CAMER_MOD_BL,
	CAMER_MOD_BR,
	MAX_MODB_PNG,
};


static bmp_buf_t init_modb_bmp_res[MAX_MODB_PNG] =
{
	//adas MODB
	{112-30, 399, "mod_b.png"},
	{35-30, 381, "mod_bl.png"},
	{195-30, 381, "mod_br.png"},
};



enum
{
	//adas MODF
	CAMER_MOD_F,
	CAMER_MOD_FL,
	CAMER_MOD_FR,
	MAX_MODF_PNG,
};


static bmp_buf_t init_modf_bmp_res[MAX_MODF_PNG] =
{
	//adas MODF
	{112-30, 24, "mod_f.png"},
	{35-30, 24, "mod_fl.png"},
	{195-30, 24, "mod_fr.png"},

};

enum
{
	
	CAMER_MOD_L = 0,
	CAMER_MOD_R,
	CAMER_BSD_WARNING1,
	CAMER_BSD_WARNING2,
	MAX_BSD_PNG,
};

static bmp_buf_t init_bsd_bmp_res[MAX_BSD_PNG] =
{
	//adas MODL-R
	{35-30, 108, "mod_l.png"},//MOD_L
	{212-30, 108, "mod_r.png"},//MOD_R	
	//adas BSD
	{43-30, 384, "bsd_warning.png"},//L
	{219-30,384,"bsd_warning.png"}//R
};



enum
{
  CAMER_FT_UF_ICON = 0,
  CAMER_FT_F_ICON,
  CAMER_RT_UF_ICON,
  CAMER_RT_F_ICON,
  CAMER_AF_UF_ICON,
  CAMER_AF_F_ICON,
  CAMER_LT_UF_ICON,
  CAMER_LT_F_ICON,
  MAX_CMAER_SHOW_PNG,
};

static bmp_buf_t init_bmp_show_res[MAX_CMAER_SHOW_PNG] =
{
        {131, 35, "cam_F0.png"},
        {131, 35, "cam_F1.png"},
        {228, 240, "cam_R0.png"},
        {228, 240, "cam_R1.png"},
        {131, 400, "cam_B0.png"},  // 431
        {131, 400, "cam_B1.png"},
        {36, 240, "cam_L0.png"},
        {36, 240, "cam_L1.png"},
};



enum
{
	CAR_2D_WHITE = 0,
	CAR_2D_SILVER,
	CAR_2D_GRAY,
	CAR_2D_BROWN,
	CAR_2D_BLACK,
	CAR_SKIN_WHITE,
	CAR_SKIN_SILVER,
	CAR_SKIN_GRAY,
	CAR_SKIN_BROWN,
	CAR_SKIN_BLACK,
	MAX_CAR_SKIN_PNG,
};

static bmp_buf_t init_skin_bmp_res[MAX_CAR_SKIN_PNG] =
{

	{0, 0, "car_white.png"},
	{0, 0, "car_silver.png"},
	{0, 0, "car_gray.png"},
	{0,0,  "car_brown.png"},
	{0, 0, "car_black.png"},
	
	{0, 0, "skin_white.png"},
	{0, 0, "skin_silver.png"},
	{0,0,"skin_gray.png"},
	{0, 0, "skin_brown.png"},
	{0,0,"skin_black.png"},		
};


enum
{
	//streeing
        STREE_OPEN=0,
	STREE_CLOSE,
	MAX_STREE_PNG,
};


static bmp_buf_t init_stree_bmp_res[MAX_STREE_PNG] =
{
	{0, 0, "turn_on.png"},
	{0, 0, "turn_off.png"},
};


enum
{
	VERSION_BG = 0,
	MAX_VERSION_PNG,
};

static bmp_buf_t init_version_bmp_res[MAX_VERSION_PNG] =
{
	{0, 0, "version_bg.png"}, //
};


enum
{
	CPU_HI_NUM0 = 0,
	CPU_HI_NUM1,
	CPU_HI_NUM2,
	CPU_HI_NUM3,
	CPU_HI_NUM4,
	CPU_HI_NUM5,
	CPU_HI_NUM6,
	CPU_HI_NUM7,
	CPU_HI_NUM8,
	CPU_HI_NUM9,
	MAX_CPU_HI_PNG,
};

static bmp_buf_t init_cpu_num_hi_bmp_res[MAX_CPU_HI_PNG] =
{
	{45, 40, "0.png"},
	{45, 40, "1.png"},
	{45, 40, "2.png"},
	{45, 40, "3.png"},
	{45, 40, "4.png"},
	{45, 40, "5.png"},
	{45, 40, "6.png"},
	{45, 40, "7.png"},
	{45, 40, "8.png"},
	{45, 40, "9.png"},
};

enum
{
	CPU_LO_NUM0 = 0,
	CPU_LO_NUM1,
	CPU_LO_NUM2,
	CPU_LO_NUM3,
	CPU_LO_NUM4,
	CPU_LO_NUM5,
	CPU_LO_NUM6,
	CPU_LO_NUM7,
	CPU_LO_NUM8,
	CPU_LO_NUM9,
	MAX_CPU_LO_PNG,
};

static bmp_buf_t init_cpu_num_lo_bmp_res[MAX_CPU_LO_PNG] =
{
	{78, 40, "0.png"},
	{78, 40, "1.png"},
	{78, 40, "2.png"},
	{78, 40, "3.png"},
	{78, 40, "4.png"},
	{78, 40, "5.png"},
	{78, 40, "6.png"},
	{78, 40, "7.png"},
	{78, 40, "8.png"},
	{78, 40, "9.png"},
};

static bmp_buf_t init_cpu_num_lo_bmp_res2[MAX_CPU_LO_PNG] =
{
	{103, 40, "0.png"},
	{103, 40, "1.png"},
	{103, 40, "2.png"},
	{103, 40, "3.png"},
	{103, 40, "4.png"},
	{103, 40, "5.png"},
	{103, 40, "6.png"},
	{103, 40, "7.png"},
	{103, 40, "8.png"},
	{103, 40, "9.png"},
};


enum
{
	MCU_HI_NUM0 = 0,
	MCU_HI_NUM1,
	MCU_HI_NUM2,
	MCU_HI_NUM3,
	MCU_HI_NUM4,
	MCU_HI_NUM5,
	MCU_HI_NUM6,
	MCU_HI_NUM7,
	MCU_HI_NUM8,
	MCU_HI_NUM9,
	MAX_MCU_HI_PNG,
};

static bmp_buf_t init_mcu_num_hi_bmp_res[MAX_MCU_HI_PNG] =
{
	{45, 40, "0.png"},
	{45, 40, "1.png"},
	{45, 40, "2.png"},
	{45, 40, "3.png"},
	{45, 40, "4.png"},
	{45, 40, "5.png"},
	{45, 40, "6.png"},
	{45, 40, "7.png"},
	{45, 40, "8.png"},
	{45, 40, "9.png"},
};

enum
{
	MCU_LO_NUM0 = 0,
	MCU_LO_NUM1,
	MCU_LO_NUM2,
	MCU_LO_NUM3,
	MCU_LO_NUM4,
	MCU_LO_NUM5,
	MCU_LO_NUM6,
	MCU_LO_NUM7,
	MCU_LO_NUM8,
	MCU_LO_NUM9,
	MAX_MCU_LO_PNG,
};

static bmp_buf_t init_mcu_num_lo_bmp_res[MAX_MCU_LO_PNG] =
{
	{78, 40, "0.png"},
	{78, 40, "1.png"},
	{78, 40, "2.png"},
	{78, 40, "3.png"},
	{78, 40, "4.png"},
	{78, 40, "5.png"},
	{78, 40, "6.png"},
	{78, 40, "7.png"},
	{78, 40, "8.png"},
	{78, 40, "9.png"},
};

static bmp_buf_t init_mcu_num_lo_bmp_res2[MAX_MCU_LO_PNG] =
{
	{103, 40, "0.png"},
	{103, 40, "1.png"},
	{103, 40, "2.png"},
	{103, 40, "3.png"},
	{103, 40, "4.png"},
	{103, 40, "5.png"},
	{103, 40, "6.png"},
	{103, 40, "7.png"},
	{103, 40, "8.png"},
	{103, 40, "9.png"},
};


enum
{
	ANGLEMCU_HI_NUM0 = 0,
	ANGLEMCU_HI_NUM1,
	ANGLEMCU_HI_NUM2,
	ANGLEMCU_HI_NUM3,
	ANGLEMCU_HI_NUM4,
	ANGLEMCU_HI_NUM5,
	ANGLEMCU_HI_NUM6,
	ANGLEMCU_HI_NUM7,
	ANGLEMCU_HI_NUM8,
	ANGLEMCU_HI_NUM9,
	MAX_ANGLEMCU_HI_PNG,
};

static bmp_buf_t init_anglemcu_num_hi_bmp_res[MAX_ANGLEMCU_HI_PNG] =
{
	{45, 40, "0.png"},
	{45, 40, "1.png"},
	{45, 40, "2.png"},
	{45, 40, "3.png"},
	{45, 40, "4.png"},
	{45, 40, "5.png"},
	{45, 40, "6.png"},
	{45, 40, "7.png"},
	{45, 40, "8.png"},
	{45, 40, "9.png"},
};


enum
{
	ANGLEMCU_LO_NUM0 = 0,
	ANGLEMCU_LO_NUM1,
	ANGLEMCU_LO_NUM2,
	ANGLEMCU_LO_NUM3,
	ANGLEMCU_LO_NUM4,
	ANGLEMCU_LO_NUM5,
	ANGLEMCU_LO_NUM6,
	ANGLEMCU_LO_NUM7,
	ANGLEMCU_LO_NUM8,
	ANGLEMCU_LO_NUM9,
	MAX_ANGLEMCU_LO_PNG,
};

static bmp_buf_t init_anglemcu_num_lo_bmp_res[MAX_ANGLEMCU_LO_PNG] =
{
	{78, 40, "0.png"},
	{78, 40, "1.png"},
	{78, 40, "2.png"},
	{78, 40, "3.png"},
	{78, 40, "4.png"},
	{78, 40, "5.png"},
	{78, 40, "6.png"},
	{78, 40, "7.png"},
	{78, 40, "8.png"},
	{78, 40, "9.png"},
};

static bmp_buf_t init_anglemcu_num_lo_bmp_res2[MAX_ANGLEMCU_LO_PNG] =
{
	{103, 40, "0.png"},
	{103, 40, "1.png"},
	{103, 40, "2.png"},
	{103, 40, "3.png"},
	{103, 40, "4.png"},
	{103, 40, "5.png"},
	{103, 40, "6.png"},
	{103, 40, "7.png"},
	{103, 40, "8.png"},
	{103, 40, "9.png"},
};
#endif /*_xcb_ui_H_*/





