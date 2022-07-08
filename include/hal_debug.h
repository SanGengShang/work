/*************************************************************************
    > File Name: hal_debug.h
    > Author: leniux
    > Mail: xiaxy@wissenstar.com
    > Created Time: 2017-01-11
 ************************************************************************/


#include <stdio.h>
//#include <iostream>

#ifndef HAL_DEBUG_H
#define HAL_DEBUG_H


#if 0
#define LOGV(format,...)
#define LOGW(format,...)
#define LOGD(format,...)
#else
#define LOGV printf
#define LOGW printf
#define LOGD printf
#endif

#define LOGE printf




static inline int  fps_calc(const char *print_tag)
{
    static int fps;
    static struct timeval tv_start, tv_current;
    int total_timems;

    gettimeofday(&tv_current, 0);
    total_timems = (tv_current.tv_sec - tv_start.tv_sec) * 1000L;
    total_timems += (tv_current.tv_usec - tv_start.tv_usec) / 1000;
    if(total_timems >= 1000)
    {
        gettimeofday(&tv_start, 0);
        LOGV("%s(), fps:%d\n",print_tag , fps);
        fps=0;
    }

    fps++;
    return 0;
}



#endif
