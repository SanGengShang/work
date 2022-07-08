/*************************************************************************
    > File Name: g2d_hal.h
    > Author: leniux
    > Mail: xiaxy@wissenstar.com
    > Created Time: 2017-02-15
 ************************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include "type_camera.h"
#include "g2d_driver.h"
//#include "ion.h"
#include "hal_debug.h"

#ifndef __G2D_HAL_H__
#define __G2D_HAL_H__

typedef struct roi_{
    int roi_x;
    int roi_y;
    int roi_width;
    int roi_height;
}roi_t;

typedef struct g2d_roi_{
    roi_t src;
    int src_width;
    int src_height;
	int src_fmt;
    roi_t dst;
    int dst_width;
    int dst_height;
	int dst_fmt;
}g2d_roi_t;



#ifdef __cplusplus
extern "C" {
#endif

void g2d_init(void);
int g2d_blit_ext(int src_phy, int dst_phy, g2d_roi_t g2d_roi_para);

#ifdef __cplusplus
}
#endif

#endif


