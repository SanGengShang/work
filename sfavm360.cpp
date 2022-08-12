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
#include "sfavm360.h"

int write_save_file_data(void);
void load_3d_mode_init(void);

//#define ANGLESIMULATED

int nLastHeadAngle = -1300;
bool bShowMcuUpdate = false;
bool bShowMcuUpdateSucceed = false;

extern int iDvrFlag;
extern int nMCUUpdateFlag;
/**************************************************************************************/
int save_img_flag[6] = {0};
static int cliCnt = 0;
bool autoCalibFlag = false;
static void cbGetIrKeyVal(int *key)
{
    int i = *key;
    printf("---------------irkey-------------%d cliCnt == %d\n", i, cliCnt);
    if (i == MENU)
    {
        cliCnt = cliCnt + 1;
        if (cliCnt == 2)
        {
            cliCnt = 0;
            runRender = 0;
            autoCalibFlag = false;
        }
    }
    else if (i == SPEED_UP)
    {
        cliCnt = cliCnt + 1;
        if (cliCnt == 2)
        {
            cliCnt = 0;
            runRender = 0;
            autoCalibFlag = true;
        }
    }
    usleep(5000);
}

void ReadCarBasicInfo()
{
    if (ini.openFile("/extp/app/calib.ini", "r") == INI_SUCCESS)
    {
        float ft;
        ft = ini.getFloat("Vehicle", "BodyLength");
        if (ft > 0)
        {
            fInCarBodyLength = ft;
        }

        ft = ini.getFloat("Vehicle", "HeadLength");
        if (ft > 0)
        {
            fInCarHeadLength = ft;
        }

        ft = ini.getFloat("Vehicle", "Width");
        if (ft > 0)
        {
            fInCarHeadWidth = fInCarWidth = ft;
        }

        ft = ini.getFloat("Vehicle", "Length");
        if (ft > 0)
        {
            fInCarLength = ft;
        }

        ft = ini.getFloat("Vehicle", "axis2carcenter");
        if (ft >= 0)
        {
            fInAxis2carcenter = ft;
            printf("axis2carcenter read succeed %f \r\n", fInAxis2carcenter);
        }
        else
        {
            printf("axis2carcenter read failed\r\n");
        }
        ini.closeFile();
    }
}

/**************************************************************************************/
//#if 1
//==============================================================================
//==============================================================================
void overlapBlue2NV21(unsigned char *pSrc1, int nSrc1Width, int nSrc1Height,
                      int nStartx, int nStarty, int nSrc2Width, int nSrc2Height)
{
    unsigned char *pSrc1Y = pSrc1 + nStartx + nStarty * nSrc1Width;
    unsigned char *pSrc1Yt;
    unsigned char *pSrc1UV = pSrc1 + nSrc1Width * nSrc1Height + ((nStartx >> 1) << 1) + (nStarty >> 1) * nSrc1Width;
    unsigned char *pSrc1UVt;

    for (int i = 0; i < nSrc2Height / 2; i++)
    {
        pSrc1Yt = pSrc1Y;
        pSrc1UVt = pSrc1UV;
        for (int j = 0; j < nSrc2Width; j += 2)
        {
            // if (pSrc2t[3] > 0)
            // {
            //   pSrc1Yt[0] = pSrc2t[0];
            //   pSrc1UVt[j] = 128;
            //   pSrc1UVt[j + 1] = 128;
            // }
            // else
            //{
            pSrc1Yt[0] = (pSrc1Yt[0] + 30) >> 1;
            pSrc1UVt[j] = (pSrc1UVt[j] + 107) >> 1;
            pSrc1UVt[j + 1] = (pSrc1UVt[j + 1] + 255) >> 1;
            //}
            // pSrc2t += 4;
            pSrc1Yt++;

            // if (pSrc2t[3] > 0)
            //{
            pSrc1Yt[0] = (pSrc1Yt[0] + 30) >> 1;
            // pSrc1Yt[0] = pSrc2t[0];
            // pSrc1UVt[((j >> 1) << 1)] = 128;
            // pSrc1UVt[((j >> 1) << 1) + 1] = 128;
            // }
            // pSrc2t += 4;
            pSrc1Yt++;
        }
        pSrc1Y += nSrc1Width;
        pSrc1UV += nSrc1Width;

        pSrc1Yt = pSrc1Y;
        for (int j = 0; j < nSrc2Width; j++)
        {
            // if (pSrc2t[3] > 0)
            //{
            pSrc1Yt[0] = (pSrc1Yt[0] + 30) >> 1;
            //}
            // pSrc2t += 4;
            pSrc1Yt++;
        }
        pSrc1Y += nSrc1Width;
    }
}
void overlapYUV2NV21(unsigned char *pSrc1, int nSrc1Width, int nSrc1Height,
                     unsigned char *pSrc2, int nSrc2Width, int nSrc2Height, int nStartx, int nStarty)
{
    unsigned char *pSrc1Y = pSrc1 + nStartx + nStarty * nSrc1Width;
    unsigned char *pSrc1Yt;
    unsigned char *pSrc1UV = pSrc1 + nSrc1Width * nSrc1Height + ((nStartx >> 1) << 1) + (nStarty >> 1) * nSrc1Width;
    unsigned char *pSrc1UVt;
    unsigned char *pSrc2t = pSrc2;
    for (int i = 0; i < nSrc2Height / 2; i++)
    {
        pSrc1Yt = pSrc1Y;
        pSrc1UVt = pSrc1UV;
        for (int j = 0; j < nSrc2Width; j += 2)
        {
            if (pSrc2t[3] > 0)
            {
                pSrc1Yt[0] = pSrc2t[0];
                pSrc1UVt[j] = 128;
                pSrc1UVt[j + 1] = 128;
            }
            // else
            // {
            //   pSrc1Yt[0] = (pSrc1Yt[0] + 30) >> 1;
            //   pSrc1UVt[j] = (pSrc1UVt[j] + 255) >> 1;
            //   pSrc1UVt[j + 1] = (pSrc1UVt[j + 1] + 107) >> 1;
            // }
            pSrc2t += 4;
            pSrc1Yt++;

            if (pSrc2t[3] > 0)
            {
                pSrc1Yt[0] = pSrc2t[0];
                // pSrc1UVt[((j >> 1) << 1)] = 128;
                // pSrc1UVt[((j >> 1) << 1) + 1] = 128;
            }
            pSrc2t += 4;
            pSrc1Yt++;
        }
        pSrc1Y += nSrc1Width;
        pSrc1UV += nSrc1Width;

        pSrc1Yt = pSrc1Y;
        for (int j = 0; j < nSrc2Width; j++)
        {
            if (pSrc2t[3] > 0)
            {
                pSrc1Yt[0] = pSrc2t[0];
            }
            pSrc2t += 4;
            pSrc1Yt++;
        }
        pSrc1Y += nSrc1Width;
    }
}

void *CameraDisplay0(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 0;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video4\n");
            return NULL;
        }
    }

    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;
    printf("[%s][%d] ===========out_fmt =%d \n", __func__, __LINE__, out_fmt);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);

    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) < 0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {
        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            // printf("[%s][%d] video_wait_buffer failed%d
            // \n",__func__,__LINE__,privCap[index].Chn );
            g_bCamF = false;
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamF = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_dequeue_buffer failed(front)!\n", __func__, __LINE__);
            goto disablech;
        }

        buf0 = &video->pool->buffers[privCap[index].buffer.index];
        // memcpy(pStitchingIm[0], buf0->planes[0].mem, 1280 * 720 * 1.5);

        vb0.width = WIDTH;
        vb0.height = HEIGHT;
        vb0.channel = privCap[index].Chn;
        vb0.phy_addr = buf0->planes[0].mem_phy;
        vb0.dma_fd = buf0->planes[0].dma_fd;

        g_elgImage[0] = bind_dmafd(vb0, g_egl_display);

        // #if DVRONOFF
        //     XCB_ScaleResize(pImgForSaveT + 1280 * 360, buf0->planes[0].mem, pImgForSaveT + 1280 * 360 / 2 + 1280 * 720, buf0->planes[0].mem + 1280 * 720, 1280, 420, 360);
        // #endif

        finish_flag0 = 1;

        g_bCamF = true;

        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamF = false;
            goto disablech;
        }
        sem_post(&sema[0]);
        pthread_mutex_unlock(&mutex);

        // sem_wait(&sema1[0]);
    }
    pthread_exit(NULL);

    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

/**************************************************************************************/
/**************************************************************************************/
void *CameraDisplay1(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 1;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video4\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }

    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;
    // printf("[%s][%d] ===========out_fmt =%d \n",__func__,__LINE__,out_fmt);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);
    // pthread_mutex_lock(&mutex);
    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) <
        0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {
        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            sem_post(&sema[1]);
            pthread_mutex_lock(&mutex);
            g_bCamB = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] Error: Back camera have fault!\n", __func__, __LINE__);
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamB = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_dequeue_buffer failed(back)!\n", __func__, __LINE__);
            goto disablech;
        }

        buf1 = &video->pool->buffers[privCap[index].buffer.index];
        // memcpy(pStitchingIm[1], buf1->planes[0].mem, 1280 * 720 * 1.5);

        vb1.width = WIDTH;
        vb1.height = HEIGHT;
        vb1.channel = privCap[index].Chn;
        vb1.phy_addr = buf1->planes[0].mem_phy;
        vb1.dma_fd = buf1->planes[0].dma_fd;

        g_elgImage[1] = bind_dmafd(vb1, g_egl_display);

        // #if DVRONOFF
        //     XCB_ScaleResize(pImgForSaveT + 860 + 1280 * 360, buf1->planes[0].mem, pImgForSaveT + 860 + 1280 * 720 + 1280 * 360 / 2, buf1->planes[0].mem + 1280 * 720, 1280, 420, 360);

        // #endif

        finish_flag1 = 1;

        // printf("1 2Dplane ends\n");
        g_bCamB = true;

        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamB = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_queue_buffer failed(back)!\n", __func__, __LINE__);
            goto disablech;
        }
        sem_post(&sema[1]);
        pthread_mutex_unlock(&mutex);
        // sem_wait(&sema1[1]);
    }
    pthread_exit(NULL);

    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

/**************************************************************************************/
/**************************************************************************************/
void *CameraDisplay2(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 2;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video4\n");
            return NULL;
        }
    }
    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;
    // printf("[%s][%d] ===========out_fmt =%d \n",__func__,__LINE__,out_fmt);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);
    // pthread_mutex_lock(&mutex);
    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) <
        0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {

        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            sem_post(&sema[2]);
            pthread_mutex_lock(&mutex);
            g_bCamL = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] Error: Left camera have fault!\n", __func__, __LINE__);
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamL = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_dequeue_buffer failed(left)!\n", __func__, __LINE__);
            goto disablech;
        }

        buf2 = &video->pool->buffers[privCap[index].buffer.index];

        // memcpy(pStitchingIm[2], buf2->planes[0].mem, 1280 * 720 * 1.5);

        vb2.width = WIDTH;
        vb2.height = HEIGHT;
        vb2.channel = privCap[index].Chn;
        vb2.phy_addr = buf2->planes[0].mem_phy;
        vb2.dma_fd = buf2->planes[0].dma_fd;

        g_elgImage[2] = bind_dmafd(vb2, g_egl_display);

        // #if DVRONOFF
        //     XCB_ScaleResize(pImgForSaveT + 428 + 1280 * 360, buf2->planes[0].mem, pImgForSaveT + 1280 * 360 / 2 + 1280 * 720 + 428, buf2->planes[0].mem + 1280 * 720, 1280, 420, 360);
        //     //   XCB_ScaleResize(pImgForSaveT+428+1280*360, buf2->planes[0].mem, pImgForSaveT+1280*360/2+1280*720, buf2->planes[0].mem+1280*720, 1280, 420, 360);

        // #endif
        finish_flag2 = 1;
        // printf("2 2Dplane ends\n");
        g_bCamL = true;

        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamL = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_queue_buffer failed(left)\n", __func__, __LINE__);
            goto disablech;
        }
        sem_post(&sema[2]);
        pthread_mutex_unlock(&mutex);
        // sem_wait(&sema1[2]);
    }
    pthread_exit(NULL);

    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

/**************************************************************************************/
/**************************************************************************************/
void *CameraDisplay3(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 3;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video4\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }

    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;

    // printf("[%s][%d] ===========out_fmt =%d \n",__func__,__LINE__,out_fmt);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);
    // pthread_mutex_lock(&mutex);
    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) < 0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {

        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            // printf("[%s][%d] video_wait_buffer failed%d
            // \n",__func__,__LINE__,privCap[index].Chn );
            g_bCamR = false;
            continue;
        }

        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamR = false;
            goto disablech;
        }

        buf3 = &video->pool->buffers[privCap[index].buffer.index];

        // memcpy(pStitchingIm[3], buf3->planes[0].mem, 1280 * 720 * 1.5);

        vb3.width = WIDTH;
        vb3.height = HEIGHT;
        vb3.channel = privCap[index].Chn;
        vb3.phy_addr = buf3->planes[0].mem_phy;
        vb3.dma_fd = buf3->planes[0].dma_fd;

        g_elgImage[3] = bind_dmafd(vb3, g_egl_display);

#if 0
        static int nTimesInterval = 0;
        nTimesInterval++;
        if (nTimesInterval % 15 == 0)
        {
            nTimesInterval = 0;
            timespec time;
            clock_gettime(CLOCK_REALTIME, &time); //获取相对于1970到现在的秒数
            struct tm nowTime = {0};
            localtime_r(&time.tv_sec, &nowTime);
            lasthourH = nowTime.tm_hour / 10;
            lasthourL = nowTime.tm_hour % 10;
            lastminuteH = nowTime.tm_min / 10;
            lastminuteL = nowTime.tm_min % 10;
            lastsecH = nowTime.tm_sec / 10;
            lastsecL = nowTime.tm_sec % 10;
        }
        //memcpy(pImgForSave, buf3->planes[0].mem, 1382400);

        pthread_mutex_lock(&mutex_light);
        XCB_ScaleResize(pImgForSaveT + 428, buf3->planes[0].mem, pImgForSaveT + 428 + 1280 * 720, buf3->planes[0].mem + 1280 * 720, 1280, 420, 360);

        ///////////blue background
        overlapBlue2NV21(pImgForSaveT, 1280, 720, 420, 28, 160, g_png_RecTimeNum[lasthourH].height + 4);
        /////////////hour

        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lasthourH],
                        g_png_RecTimeNum[lasthourH].width, g_png_RecTimeNum[lasthourH].height,
                        REC_TIME_HOUR_HX, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lasthourL],
                        g_png_RecTimeNum[lasthourL].width, g_png_RecTimeNum[lasthourL].height,
                        REC_TIME_HOUR_LX, REC_TIME_Y);

        /////////////minute
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastminuteH],
                        g_png_RecTimeNum[lastminuteH].width, g_png_RecTimeNum[lastminuteH].height,
                        REC_TIME_MINUTE_HX, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastminuteL],
                        g_png_RecTimeNum[lastminuteL].width, g_png_RecTimeNum[lastminuteL].height,
                        REC_TIME_MINUTE_LX, REC_TIME_Y);

        /////////////sec
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastsecH],
                        g_png_RecTimeNum[lastsecH].width, g_png_RecTimeNum[lastsecH].height,
                        REC_TIME_SEC_HX, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastsecL],
                        g_png_RecTimeNum[lastsecL].width, g_png_RecTimeNum[lastsecL].height,
                        REC_TIME_SEC_LX, REC_TIME_Y);

        /////////////colon
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecColon,
                        g_png_RecTimeColon.width, g_png_RecTimeColon.height,
                        REC_TIME_COLON_X0, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecColon,
                        g_png_RecTimeColon.width, g_png_RecTimeColon.height,
                        REC_TIME_COLON_X1, REC_TIME_Y);
        pthread_mutex_unlock(&mutex_light);

#endif
        finish_flag3 = 1;

        // printf("3 2Dplane ends\n");
        g_bCamR = true;
        // pthread_mutex_unlock(&mutex);
        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamR = false;
            pthread_mutex_unlock(&mutex);
            printf("[%s][%d] video_queue_buffer failed(right)\n", __func__, __LINE__);
            goto disablech;
        }
        sem_post(&sema[3]);
        pthread_mutex_unlock(&mutex);
#if DVRONOFF
        // sem_wait(&sema1[3]);
    }
    pthread_exit(NULL);

    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

/**************************************************************************************/
/**************************************************************************************/
void *CameraDisplay4(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 4;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video4\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }

    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;

    printf("[%s][%d] ==========%d %d \n", __func__, __LINE__, MAXVIDEO_NUM,
           HW_VIDEO_DEVICE_NUM);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);
    // pthread_mutex_lock(&mutex);
    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) <
        0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {

        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            // printf("[%s][%d] video_wait_buffer failed%d
            // \n",__func__,__LINE__,privCap[index].Chn );
            g_bCamL1 = false;
            continue;
        }

        pthread_mutex_lock(&mutex);
        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamL1 = false;
            pthread_mutex_unlock(&mutex);
            goto disablech;
        }

        buf4 = &video->pool->buffers[privCap[index].buffer.index];
        // memcpy(pStitchingIm[4], buf4->planes[0].mem, 1280 * 720 * 1.5);

        vb4.width = WIDTH;
        vb4.height = HEIGHT;
        vb4.channel = privCap[index].Chn;
        vb4.phy_addr = buf4->planes[0].mem_phy;
        vb4.dma_fd = buf4->planes[0].dma_fd;

        g_elgImage[4] = bind_dmafd(vb4, g_egl_display);

        // #if DVRONOFF
        //     XCB_ScaleResize(pImgForSaveT + 860, buf4->planes[0].mem, pImgForSaveT + 860 + 1280 * 720, buf4->planes[0].mem + 1280 * 720, 1280, 420, 360);

        // #endif

        finish_flag4 = 1;

        // printf("4 2Dplane ends\n");
        g_bCamL1 = true;
        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamL1 = false;
            pthread_mutex_unlock(&mutex);
            goto disablech;
        }
        sem_post(&sema[4]);
        pthread_mutex_unlock(&mutex);
        // sem_wait(&sema1[4]);
    }
    pthread_exit(NULL);

    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

/**************************************************************************************/
/**************************************************************************************/
void *CameraDisplay5(void *arg)
{
    pthread_t thid[MAXVIDEO_NUM];
    int ret, j;
    struct vi_info privCap[MAXVIDEO_NUM];
    struct isp_video_device *video_dev[HW_VIDEO_DEVICE_NUM];
    struct isp_video_device *video = NULL;
    struct buffers_pool *pool = NULL;
    struct hw_isp_media_dev *media0 = NULL;
    int index = 5;

    int width = WIDTH;
    int height = HEIGHT;
    int out_fmt = OUT_FMT;
    pthread_mutex_lock(&mutex);
    if (media0 == NULL)
    {
        media0 = isp_mdopen(dev_name, index);
        if (media0 == NULL)
        {
            printf("unable to open media0 device /dev/video5\n");
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
    }

    memset(&privCap[index], 0, sizeof(struct vi_info));

    privCap[index].Chn = index;
    privCap[index].s32MilliSec = TIMEOUT;
    privCap[index].vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    privCap[index].vfmt.memtype = V4L2_MEMORY_USERPTR;

    printf("[%s][%d] ==========%d %d \n", __func__, __LINE__, MAXVIDEO_NUM,
           HW_VIDEO_DEVICE_NUM);

    switch (out_fmt)
    {
        case 0:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_SBGGR8;
            break;
        case 1:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
        case 2:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420;
            break;
        case 3:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV12M;
            break;
        case 4:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV21;
            break;
        case 5:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_NV61;
            break;
        default:
            privCap[index].vfmt.format.pixelformat = V4L2_PIX_FMT_YUV420M;
            break;
    }

    privCap[index].vfmt.format.field = V4L2_FIELD_NONE;
    privCap[index].vfmt.format.width = width;
    privCap[index].vfmt.format.height = height;
    privCap[index].vfmt.nbufs = CAMERA_UBUFS;
    privCap[index].vfmt.nplanes = 2;
    privCap[index].vfmt.fps = 30;
    privCap[index].vfmt.capturemode = V4L2_MODE_VIDEO;
    privCap[index].vfmt.use_current_win = 0;

    // printf("[%s][%d] ===========privCap->Chn=%d privCap[index].Chn =%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[index].Chn);
    // pthread_mutex_lock(&mutex);
    if (isp_videoopen(media0, index) < 0)
    {
        printf("isp_videoopen vi%d failed\n", index);
        return NULL;
    }

    if (video_set_fmt(media0->video_dev[index], &privCap[index].vfmt, index) < 0)
    {
        printf("video_set_fmt failed \n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(media0->video_dev[index], &privCap[index].vfmt, index);
    video = media0->video_dev[privCap[index].Chn];
    pool = buffers_pool_new(video, index);
    if (NULL == pool)
    {
        printf("[%s][%d] buffers_pool_new failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        return NULL;
    }

    if (video_req_buffers(video, pool, index) < 0)
    {
        printf("[%s][%d] video_req_buffers failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    video_get_fmt(video, &privCap[privCap[index].Chn].vfmt, index);
    // printf("[%s][%d] privCap[%d].vfmt.nbufs=%d
    // \n",__func__,__LINE__,privCap->Chn,privCap[privCap->Chn].vfmt.nbufs );

    for (j = 0; j < privCap[privCap[index].Chn].vfmt.nbufs; j++)
    {
        video_queue_buffer(video, j, index);
    }

    if (video_stream_on(video, index) < 0)
    {
        printf("[%s][%d] video_stream_on failed%d \n", __func__, __LINE__,
               privCap[index].Chn);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_unlock(&mutex);

    while (1)
    {

        if (video_wait_buffer(video, privCap[index].s32MilliSec, index) < 0)
        {
            // printf("[%s][%d] video_wait_buffer failed%d
            // \n",__func__,__LINE__,privCap[index].Chn );
            g_bCamR1 = false;
            continue;
        }
        pthread_mutex_lock(&mutex);
        if (video_dequeue_buffer(video, &privCap[index].buffer, index) < 0)
        {
            printf("[%s][%d] video_dequeue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamR1 = false;
            pthread_mutex_unlock(&mutex);
            goto disablech;
        }

        buf5 = &video->pool->buffers[privCap[index].buffer.index];
        // memcpy(pStitchingIm[5], buf5->planes[0].mem, 1280 * 720 * 1.5);

        vb5.width = WIDTH;
        vb5.height = HEIGHT;
        vb5.channel = privCap[index].Chn;
        vb5.phy_addr = buf5->planes[0].mem_phy;
        vb5.dma_fd = buf5->planes[0].dma_fd;

        g_elgImage[5] = bind_dmafd(vb5, g_egl_display);

#if 0

        //memcpy(pImgForSaveT1, buf5->planes[0].mem, 1382400);
        pthread_mutex_lock(&mutex_light);
        XCB_ScaleResize(pImgForSaveT, buf5->planes[0].mem, pImgForSaveT + 1280 * 720, buf5->planes[0].mem + 1280 * 720, 1280, 420, 360);
        ///////////blue background
        overlapBlue2NV21(pImgForSaveT, 1280, 720, 208, 28, 212, g_png_RecTimeNum[lastmonH].height + 4);

        lastmonH = curmonH;
        lastmonL = curmonL;
        lastdayH = curdayH;
        lastdayL = curdayL;
        lastyearH = curyearH;
        lastyearL = curyearL;

        //////////////month

        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastmonH],
                        g_png_RecTimeNum[lastmonH].width, g_png_RecTimeNum[lastmonH].height,
                        REC_TIME_MONTH_HX, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastmonL],
                        g_png_RecTimeNum[lastmonL].width, g_png_RecTimeNum[lastmonL].height,
                        REC_TIME_MONTH_LX, REC_TIME_Y);

        /////////////day
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastdayH],
                        g_png_RecTimeNum[lastdayH].width, g_png_RecTimeNum[lastdayH].height,
                        REC_TIME_DAY_HX, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastdayL],
                        g_png_RecTimeNum[lastdayL].width, g_png_RecTimeNum[lastdayL].height,
                        REC_TIME_DAY_LX, REC_TIME_Y);

        /////////////year
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[2],
                        g_png_RecTimeNum[2].width, g_png_RecTimeNum[2].height,
                        REC_TIME_YEAR_HX1, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[0],
                        g_png_RecTimeNum[0].width, g_png_RecTimeNum[0].height,
                        REC_TIME_YEAR_HX0, REC_TIME_Y);

        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastyearH],
                        g_png_RecTimeNum[lastyearH].width, g_png_RecTimeNum[lastyearH].height,
                        REC_TIME_YEAR_LX1, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecNumber[lastyearL],
                        g_png_RecTimeNum[lastyearL].width, g_png_RecTimeNum[lastyearL].height,
                        REC_TIME_YEAR_LX0, REC_TIME_Y);
        /////////////dash
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecDash,
                        g_png_RecTimeDash.width, g_png_RecTimeDash.height,
                        REC_TIME_DASH_X0, REC_TIME_Y);
        overlapYUV2NV21(pImgForSaveT, 1280, 720, pRecDash,
                        g_png_RecTimeDash.width, g_png_RecTimeDash.height,
                        REC_TIME_DASH_X1, REC_TIME_Y);
        pthread_mutex_unlock(&mutex_light);
        //printf("time0: %d%d")

#endif

        finish_flag5 = 1;

        // printf("5 2Dplane ends\n");
        g_bCamR1 = true;
        if (video_queue_buffer(video, privCap[index].buffer.index, index) < 0)
        {
            printf("[%s][%d] video_queue_buffer failed%d \n", __func__, __LINE__,
                   privCap[index].Chn);
            g_bCamR1 = false;
            pthread_mutex_unlock(&mutex);
            goto disablech;
        }
        sem_post(&sema[5]);
        pthread_mutex_unlock(&mutex);
        // sem_wait(&sema1[5]);
    }
    pthread_exit(NULL);
    return 0;
disablech:
    if (video_stream_off(video, index) < 0)
        return NULL;

    if (video_free_buffers(video, index) < 0)
        return NULL;
    buffers_pool_delete(video, index);
    return NULL;
}

void *DvrData0(void *arg)
{

    sleep(5);
    printf("[%d][XT6lane_DVR] ==================== \n", __LINE__);

    while (1)
    {
#ifdef SYS_FOUR_LANE
        if (finish_flag0 && finish_flag1 && finish_flag2 && finish_flag3) // && finish_flag4 && finish_flag5
#else
        if (finish_flag0 && finish_flag1 && finish_flag2 && finish_flag3 && finish_flag4 && finish_flag5) // && finish_flag4 && finish_flag5
#endif

        {

            if (record_start_flag)
            {
                ////////////////////////////////贴时间贴图
                // sem_wait(&sema[0]);
                // sem_wait(&sema[1]);
                // sem_wait(&sema[2]);
                // sem_wait(&sema[3]);
                // sem_wait(&sema[4]);
                // sem_wait(&sema[5]);
                pthread_mutex_lock(&mutex_light);
                memcpy(pImgForSave, pImgForSaveT, 1382400);
                pthread_mutex_unlock(&mutex_light);

                //////////////

                //////////////////////////////////
                record_process(pImgForSave, 1280, 720);//录像
                usleep(23 * 1000);
            }
            else
            {
                usleep(10 * 1000);
            }
        }
        else
        {
            usleep(10 * 1000);
        }
    }
    pthread_exit(NULL);
}

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
int setup_dmafd2eglimg(EGLDisplay egl_display, int dma_fd, int dmafd_w,
                       int dmafd_h, EGLImageKHR *eglImage, int index)
{
    int atti = 0;
    EGLint attribs[30];
    int ret;
    int i = 0;
    EGLImageKHR img;

    if (dma_fd < 0)
    {
        printf("dmafd parameter err.\n");
        return NULL;
    }

    atti = 0;
    attribs[atti++] = EGL_WIDTH;
    attribs[atti++] = CAMERA_WIDTH;
    attribs[atti++] = EGL_HEIGHT;
    attribs[atti++] = CAMERA_HEIGHT;
    attribs[atti++] = EGL_LINUX_DRM_FOURCC_EXT;

#if (CAMWER_CUR_FORMATE == N21)
    attribs[atti++] = DRM_FORMAT_NV21; // DRM_FORMAT_NV21;

#else
    attribs[atti++] = DRM_FORMAT_NV61;                                                                  // DRM_FORMAT_NV21;

#endif

    attribs[atti++] = EGL_DMA_BUF_PLANE0_FD_EXT;
    attribs[atti++] = dma_fd;
    attribs[atti++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
    attribs[atti++] = 0;
    attribs[atti++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
    attribs[atti++] = CAMERA_WIDTH;

    attribs[atti++] = EGL_DMA_BUF_PLANE1_FD_EXT;
    attribs[atti++] = dma_fd;
    attribs[atti++] = EGL_DMA_BUF_PLANE1_OFFSET_EXT;
    attribs[atti++] = CAMERA_WIDTH * CAMERA_HEIGHT;
    attribs[atti++] = EGL_DMA_BUF_PLANE1_PITCH_EXT;
    attribs[atti++] = CAMERA_WIDTH;

    attribs[atti++] = EGL_YUV_COLOR_SPACE_HINT_EXT;
    attribs[atti++] = EGL_ITU_REC709_EXT;
    attribs[atti++] = EGL_SAMPLE_RANGE_HINT_EXT;
    attribs[atti++] = EGL_YUV_FULL_RANGE_EXT;
    attribs[atti++] = EGL_NONE;
    // printf("[%s][%d] ==========NV61 \n",__func__,__LINE__);

    img = eglCreateImageKHR(egl_display, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, 0,
                            attribs);
#if 0
    printf("===========dma_fd=%d , dmafd_w=%d ,  dmafd_h=%d \n",dma_fd, dmafd_w,  dmafd_h);
    if (img == EGL_NO_IMAGE_KHR)
    {
        printf("Error %s(): failed: 0x%08X, dmafd=0x%x \n", __func__, glGetError(), dma_fd);
        return NULL;
    }
#endif
    // usleep(200000);

    *eglImage = img;

    return 0;
}

static int search_dmafd(int dmafd, EGLImageKHR *p_img, int *p_index)
{
    int ret = -1;
    EGLImageKHR img;
    int i;

    for (i = 0; i < EGLIMG_DMAFD_MAX; i++)
    {
        if (g_eglimg_dmafd_array[i].dmafd == dmafd)
        {
            img = g_eglimg_dmafd_array[i].eglimg;
            ret = 0;
            break;
        }
        if (g_eglimg_dmafd_array[i].dmafd == -1)
        {
            break;
        }
    }

    if (p_img == NULL)
    {
        printf("p img NULL exit -1\n");
    }
    else
    {
        *p_img = img;
    }

    if (p_index == NULL)
    {
        printf("p index NULL exit -1\n");
    }
    else
    {
        *p_index = i;
    }
    return ret;
}

GLeglImageOES bind_dmafd(video_buf_t video_buf, EGLDisplay dpy)
{
    int ret;
    EGLImageKHR img;
    int index;
    int dmaindex = video_buf.channel;
    int dmafd = video_buf.dma_fd;
    int dmafd_w = video_buf.width;
    int dmafd_h = video_buf.height;
    if (search_dmafd(dmafd, &img, &index) != 0)
    {
        ret = setup_dmafd2eglimg(dpy, dmafd, dmafd_w, dmafd_h, &img, dmaindex);
        if (ret != 0)
        {
            printf("setup_dmafd2eglimg err\n");
        }
        else
        {
            g_eglimg_dmafd_array[index].dmafd = dmafd;
            g_eglimg_dmafd_array[index].eglimg = img;
        }
    }
    return img;
}

int init_eglimg_dmafd(void)
{
    unsigned long i;
    for (i = 0; i < EGLIMG_DMAFD_MAX; i++)
    {
        g_eglimg_dmafd_array[i].dmafd = -1;
        g_eglimg_dmafd_array[i].eglimg = (void *)-1;
    }
    return 0;
}
#else

#endif

//========================================================================
//========================================================================
//========================================================================
//========================================================================
//========================================================================
//========================================================================
void nv21_2_nv61(unsigned char *dst_61, unsigned char *src_21)//图像格式的转换
{
    int width = 1280;
    int height = 720;
    int y_length = width * height;
    int nv21_vu_length = width * height / 2;

    memcpy(dst_61, src_21, y_length);

    unsigned char *vu_nv21 = (unsigned char *)malloc(sizeof(unsigned char) * nv21_vu_length);
    unsigned char *vu_nv61 = (unsigned char *)malloc(sizeof(unsigned char) * y_length);

    memcpy(vu_nv21, src_21 + y_length, nv21_vu_length);

    for (int i = 0; i < height / 2; i++)
    {
        memcpy(vu_nv61 + (i * 2) * width, vu_nv21 + i * width, width);
        memcpy(vu_nv61 + (i * 2 + 1) * width, vu_nv21 + i * width, width);
    }

    memcpy(dst_61 + y_length, vu_nv61, y_length);

    free(vu_nv21);
    free(vu_nv61);
}
//========================================================================
void render_init(void)
{
    FILE *fp = NULL;
    // model created by calibration
    char pathModel2D[500];
    char pathSingleModel[500];
    // default model which cannot be deleted or replaced!
    char pathModel2D_default[500];
    char pathSingleModel_default[500];
    // car model
    char path2DParams[500];
    char pathBottomPng[500];
    char pngPath[500];
    char pathModel3D[500];
    char pathModel3D_default[500];
    char path3DParams[500];

    sprintf(pathModel2D, "%s/BowlModel_2D", g_pCreateModelDir);
    sprintf(pathModel2D_default, "%s/BowlModel_2D", g_pGivenModelDir);
    sprintf(pathSingleModel, "%s/SingleModel", g_pCreateModelDir);

    sprintf(pathSingleModel_default, "%s/SingleModel", g_pGivenModelDir);
    sprintf(pathModel3D_default, "%s/BowlModel", g_pGivenModelDir);

    sprintf(pathModel3D, "%s/BowlModel", g_pCreateModelDir);
    sprintf(path2DParams, "%s/2D_Params", g_pGivenModelDir);

    sprintf(path3DParams, "%s/3D_Params", g_pGivenModelDir);
    sprintf(pathBottomPng, "%s/shadow.png", g_pPngDir);

    xcb_avmlib_Window_Init(SCREEN_W, SCREEN_H, g_egl_display, g_egl_surface);//生成一个显示图像的窗口


#if ISDMA == 1
    xcb_avmlib_Gen_TextureBuffer_DMA(NUM_TEXTURE, g_texture_id);//DMA 方式从摄像头获取数据到显示设备

    g_stitching_Handle.isDMA = true;

    g_avm_screen.win_x = 0;
    g_avm_screen.win_y = 0;
    g_avm_screen.width = 1280;
    g_avm_screen.height = 720;
    g_avm_screen.isDMA = true;
//    gettimeofday(&test2, NULL);
    xcb_avmlib_screen_Init(NULL, &g_avm_screen);//初始化显示
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************初始化全屏 = %lf(ms)\n", Time1 / 1000);
    xcb_avmlib_screen_Gen_VBO(&g_avm_screen);//生成 vbo

#else

    g_stitching_Handle.isDMA = false;
    xcb_avmlib_Gen_TextureBuffer(NUM_TEXTURE, g_texture_id);//非DMA 方式从摄像头获取数据到显示设备

    g_avm_screen.win_x = 560;
    g_avm_screen.win_y = 0;
    g_avm_screen.width = 1360;
    g_avm_screen.height = 720;
    g_avm_screen.isDMA = false;
    xcb_avmlib_screen_Init(NULL, &g_avm_screen);
    xcb_avmlib_screen_Gen_VBO(&g_avm_screen);

    /*
    FILE *fImg = fopen("./gpu_data_default/NV61_B", "rb");
    fread(pNV61_B, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV61_L", "rb");
    fread(pNV61_L, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV61_F", "rb");
    fread(pNV61_F, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV61_R", "rb");
    fread(pNV61_R, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV61_L1", "rb");
    fread(pNV61_L1, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV61_R1", "rb");
    fread(pNV61_R1, sizeof(unsigned char), size_nv61, fImg);
    fclose(fImg);
    */

    FILE *fImg = fopen("./gpu_data_default/NV21_B", "rb");
    fread(pNV21_B, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV21_L", "rb");
    fread(pNV21_L, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV21_F", "rb");
    fread(pNV21_F, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV21_R", "rb");
    fread(pNV21_R, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV21_L1", "rb");
    fread(pNV21_L1, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    fImg = fopen("./gpu_data_default/NV21_R1", "rb");
    fread(pNV21_R1, sizeof(unsigned char), size_nv21, fImg);
    fclose(fImg);
    printf("read image end\n");

    nv21_2_nv61(pNV61_B, pNV21_B);//图像格式
    nv21_2_nv61(pNV61_L, pNV21_L);
    nv21_2_nv61(pNV61_F, pNV21_F);
    nv21_2_nv61(pNV61_R, pNV21_R);
    nv21_2_nv61(pNV61_L1, pNV21_L1);
    nv21_2_nv61(pNV61_R1, pNV21_R1);

#endif
//    gettimeofday(&test2, NULL);
    pthread_mutex_lock(&mutexin);
    xcb_avmlib_stitching_Init(SAVE_LICENSE_PATH, &g_stitching_Handle); //拼接//, SAVE_LICENSE_PATH
    pthread_mutex_unlock(&mutexin);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************初始化拼接 = %lf(ms)\n", Time1 / 1000);

    //  xcb_avmlib_stitching_Gen_VBO_SixCarmera("/extp/app/gpu_data/model2DBowl04road.txt",
    //                                          "/extp/app/gpu_data/model2DBowl04road.txt",
    //                                          &g_stitching_Handle, 0);
    //  xcb_avmlib_stitching_Gen_VBO_SixCarmera("/extp/app/gpu_data/model3DBowl04road.txt",
    //                                          "/extp/app/gpu_data/model3DBowl04road.txt",
    //                                          &g_stitching_Handle, 1);

#ifdef SYS_FOUR_LANE
    //读取标定生成拼接模型
    xcb_avmlib_stitching_Gen_VBO("/extp/app/gpu_data/BowlModel_2D_four",
                                 "/extp/app/gpu_data_default/BowlModel_2D_four",
                                 &g_stitching_Handle, 0);//2D
    xcb_avmlib_stitching_Gen_VBO("/extp/app/gpu_data/BowlModel_3D_four",
                                 "/extp/app/gpu_data_default/BowlModel_3D_four",
                                 &g_stitching_Handle, 1);//3D
    printf("xcb_avmlib_stitching_Gen_VBO_SixCarmera success\n");
#else
//    gettimeofday(&test2, NULL);

    xcb_avmlib_stitching_Gen_VBO_SixCarmera("/extp/app/gpu_data/BowlModel_2D",
                                            "/extp/app/gpu_data_default/BowlModel_2D",
                                            &g_stitching_Handle, 0);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************生成2D拼接VBO = %lf(ms)\n", Time1 / 1000);
//    gettimeofday(&test2, NULL);

    xcb_avmlib_stitching_Gen_VBO_SixCarmera("/extp/app/gpu_data/BowlModel_3D",
                                            "/extp/app/gpu_data_default/BowlModel_3D",
                                            &g_stitching_Handle, 1);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************生成3D拼接VBO = %lf(ms)\n", Time1 / 1000);
    printf("xcb_avmlib_stitching_Gen_VBO_SixCarmera success\n");
#endif
    // xcb_avmlib_stitching_Gen_VBO(pathModel2D, pathModel2D_default,
    //                              &g_stitching_Handle, 0);
    // xcb_avmlib_stitching_Gen_VBO(pathModel3D, pathModel3D_default,
    //                              &g_stitching_Handle, 1);

    //====================================================================
    // xcb_avmlib_Gen_TextureBuffer(1, texture_id_multi);

    // sprintf(pngPath, "%s/track_line.png", g_pPngDir);
    // xcb_avmlib_Load_Png(pngPath, track_layer);
    // xcb_avmlib_Update_TextureBuffer_Line(&track_layer, texture_id_multi, 1);

    // g_car_Handle.iCarWidth = 1260;         //130 * 2;
    // g_car_Handle.iCarLength = 10600;       //300 * 2;
    // g_car_Handle.iBlindAreaWidth = 1300;   // 2200;
    // g_car_Handle.iBlindAreaLength = 10800; // 5050;
    // png_texture_t bottom_texture;
    // xcb_avmlib_Load_Png(pathBottomPng, bottom_texture);
    // g_car_Handle.texture_width = bottom_texture.width;
    // g_car_Handle.texture_height = bottom_texture.height;
    // g_car_Handle.texture_channels = 4;
    // g_car_Handle.pixel = bottom_texture.texture;
#ifdef SYS_FOUR_LANE
    png_texture_t car_head_bottom;
    xcb_avmlib_Load_Png("/extp/app/png_yq/blue.png", car_head_bottom);//加载图片
    png_texture_t car_head_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carhead.png", car_head_texture);
    avm2DCarHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2; //-1250;//-1450;
    avm2DCarHandle.fCor_y = fInCarLength / 2 + fHalfCarDeltaLenth;
    avm2DCarHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1450;
    avm2DCarHandle.fHeight = fInCarLength + fHalfCarDeltaLenth * 2;
    avm2DCarHeadBottom = avm2DCarHandle;
    xcb_avmlib_bottomImg_Init(&avm2DCarHandle);//底部图像
    xcb_avmlib_Update_bottomImg(&avm2DCarHandle, &car_head_texture);//更新底部图像

    avm2DCarHeadBottom.fCor_y += 80;
    avm2DCarHeadBottom.fCor_x -= 30;
    avm2DCarHeadBottom.fWidth += 60;
    avm2DCarHeadBottom.fHeight += 160;
    xcb_avmlib_bottomImg_Init(&avm2DCarHeadBottom);
    xcb_avmlib_Update_bottomImg(&avm2DCarHeadBottom, &car_head_bottom);
#else
#ifdef DYNAMIC_CAR_MODEL
    png_texture_t car_head_bottom;
    xcb_avmlib_Load_Png("/extp/app/png_yq/blue.png", car_head_bottom);
    png_texture_t car_body_bottom;
    xcb_avmlib_Load_Png("/extp/app/png_yq/blue.png", car_body_bottom);

    png_texture_t car_head_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carhead.png", car_head_texture);
    png_texture_t car_body_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carbody.png", car_body_texture);

    avm2DCarHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2; //-1250;//-1450;
    avm2DCarHandle.fCor_y = fInCarLength / 2 - fInAxis2carcenter + fHalfCarDeltaLenth;
    avm2DCarHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1450;
    avm2DCarHandle.fHeight = fInCarHeadLength + fHalfCarDeltaLenth * 2;
    avm2DCarHeadBottom = avm2DCarHandle;
    xcb_avmlib_bottomImg_Init(&avm2DCarHandle);
    xcb_avmlib_Update_bottomImg(&avm2DCarHandle, &car_head_texture);

    avm2DCarHeadBottom.fCor_y += 80;
    avm2DCarHeadBottom.fCor_x -= 30;
    avm2DCarHeadBottom.fWidth += 60;
    avm2DCarHeadBottom.fHeight += 160;
    xcb_avmlib_bottomImg_Init(&avm2DCarHeadBottom);
    xcb_avmlib_Update_bottomImg(&avm2DCarHeadBottom, &car_head_bottom);

    avmBottomHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2;                                                             //-1250;//-1550;
    avmBottomHandle.fCor_y = fInCarBodyLength + fHalfCarDeltaLenth * 2 - (fInCarLength / 2 + fInAxis2carcenter + fHalfCarDeltaLenth); // fInCarBodyLength - fInCarLength / 2 - fInAxis2carcenter + fHalfCarDeltaLenth;
    // 0 + 1600 + 100;
    avmBottomHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1550;
    avmBottomHandle.fHeight = fInCarBodyLength + fHalfCarDeltaLenth * 2;
    // 11300 + 1600 + 200 + 100;
    avm2DCarBodyBottom = avmBottomHandle;
    avm2DCarBodyBottom.fCor_x -= 30;
    avm2DCarBodyBottom.fWidth += 60;
    xcb_avmlib_bottomImg_Init(&avmBottomHandle);
    xcb_avmlib_Update_bottomImg(&avmBottomHandle, &car_body_texture);
    xcb_avmlib_bottomImg_Init(&avm2DCarBodyBottom);
    xcb_avmlib_Update_bottomImg(&avm2DCarBodyBottom, &car_body_bottom);
#else
    png_texture_t car_head_bottom;
//    gettimeofday(&test2, NULL);
    xcb_avmlib_Load_Png("/extp/app/png_yq/blue.png", car_head_bottom);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test2.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************加载一张图片 = %lf(ms)\n", Time1 / 1000);
    png_texture_t car_head_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carhead.png", car_head_texture);
    avm2DCarHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2; //-1250;//-1450;
    avm2DCarHandle.fCor_y = fInCarLength / 2 + fHalfCarDeltaLenth;
    avm2DCarHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1450;
    avm2DCarHandle.fHeight = fInCarLength + fHalfCarDeltaLenth * 2;
    avm2DCarHeadBottom = avm2DCarHandle;
    xcb_avmlib_bottomImg_Init(&avm2DCarHandle);
    xcb_avmlib_Update_bottomImg(&avm2DCarHandle, &car_head_texture);

    avm2DCarHeadBottom.fCor_y += 80;
    avm2DCarHeadBottom.fCor_x -= 30;
    avm2DCarHeadBottom.fWidth += 60;
    avm2DCarHeadBottom.fHeight += 160;
    xcb_avmlib_bottomImg_Init(&avm2DCarHeadBottom);
    xcb_avmlib_Update_bottomImg(&avm2DCarHeadBottom, &car_head_bottom);
#endif
#endif
    /*
    xcb_avmlib_car_Init(&g_car_Handle);

    xcb_avmlib_car_Gen_VBO(path2DParams, &g_car_Handle, 0);
    xcb_avmlib_car_Gen_VBO(path3DParams, &g_car_Handle, 1);
    xcb_avmlib_car_Gen_VBO(NULL, &g_car_Handle,
                           2); // use NULL replace pathUnderPrint
    */
    //====================================================================
    /*
    int j = 0;
    g_png_skin = new png_texture_t[MAX_CAR_SKIN_PNG];

    if (save_skin_para == XCB_AVM_CAR_COLOR5)
    {
      j = CAR_2D_BLACK;
      // k = CAR_SKIN_BLACK;
    }
    else if (save_skin_para == XCB_AVM_CAR_COLOR4)
    {
      j = CAR_2D_BROWN;
      // k = CAR_SKIN_BROWN;
    }
    else if (save_skin_para == XCB_AVM_CAR_COLOR3)
    {
      j = CAR_2D_GRAY;
      // k = CAR_SKIN_GRAY;
    }
    else if (save_skin_para == XCB_AVM_CAR_COLOR2)
    {
      j = CAR_2D_SILVER;
      // k = CAR_SKIN_SILVER;
    }
    else if (save_skin_para == XCB_AVM_CAR_COLOR1)
    {
      j = CAR_2D_WHITE;
      // k = CAR_SKIN_WHITE;
    }

    g_png_skin[j].win_x = init_skin_bmp_res[j].x;
    g_png_skin[j].win_y = init_skin_bmp_res[j].y;
    sprintf(pngPath, "%s/%s", g_pPngDir, init_skin_bmp_res[j].file_name);
    xcb_avmlib_Load_Png(pngPath, g_png_skin[j]);
    */
    //====================================================================
    ////////////single
#if ISDMA == 1
    g_single_Handle.isDMA = true;
#else
    g_single_Handle.isDMA = false;
#endif
//    gettimeofday(&test2, NULL);
    xcb_avmlib_single_Init(&g_single_Handle);//单视图
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************单视图初始化 = %lf(ms)\n", Time1 / 1000);
    xcb_avmlib_singleView_Gen_VBO(pathSingleModel, pathSingleModel_default,
                                  &g_single_Handle);
    g_single_Handle.view = 2;

    // g_line_handle
//    gettimeofday(&test2, NULL);
    xcb_avm_lines_Init(&g_line_handle);//线条
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************线条初始化 = %lf(ms)\n", Time1 / 1000);
    g_line_handle.iCarLength = 15.6 * 1000;
    g_line_handle.iFrontWheelDis = 10 * 1000;
    g_line_handle.iLineDistance = 3000;
    g_line_handle.iRearWheelDis = 4 * 1000;
    g_line_handle.isOriginal = true;
    g_line_handle.lines_static->flag = true;
    g_line_handle.lines_static->scale_length = 300;
    g_line_handle.lines_static->stepDistance[0] = 1000;
    g_line_handle.lines_static->stepDistance[1] = 2000;
    g_line_handle.lines_static->stepDistance[2] = 3000;
    g_line_handle.lines_static->thick = 150;
    g_line_handle.lines_static->view = &g_t2DViewParams;
    png_texture_t line[3];
    xcb_avmlib_Load_Png("/extp/app/png_yq/red.png", line[0]);
    xcb_avmlib_Load_Png("/extp/app/png_yq/blue.png", line[1]);
    xcb_avmlib_Load_Png("/extp/app/png_yq/green.png", line[2]);
    if (line[0].format == GL_RGBA)
        g_line_handle.lines_static->tex[0].channels = 4;
    else
        g_line_handle.lines_static->tex[0].channels = 3;

    g_line_handle.lines_static->tex[0].heigth = line[0].height;
    g_line_handle.lines_static->tex[0].width = line[0].width;
    g_line_handle.lines_static->tex[0].pixel = line[0].texture;

    if (line[1].format == GL_RGBA)
        g_line_handle.lines_static->tex[1].channels = 4;
    else
        g_line_handle.lines_static->tex[1].channels = 3;

    g_line_handle.lines_static->tex[1].heigth = line[1].height;
    g_line_handle.lines_static->tex[1].width = line[1].width;
    g_line_handle.lines_static->tex[1].pixel = line[1].texture;

    if (line[2].format == GL_RGBA)
        g_line_handle.lines_static->tex[2].channels = 4;
    else
        g_line_handle.lines_static->tex[2].channels = 3;

    g_line_handle.lines_static->tex[2].heigth = line[2].height;
    g_line_handle.lines_static->tex[2].width = line[2].width;
    g_line_handle.lines_static->tex[2].pixel = line[2].texture;

    g_line_handle.type = 1;//后退轨迹线
    gettimeofday(&test2, NULL);

    xcb_avm_line_Gen_VBO(&g_line_handle,
                         &g_single_Handle, 0, -3000);

    // View3D==============================3D视图=================================================
    g_t3DViewParams.win_x = 240;
    g_t3DViewParams.win_y = 480;
    g_t3DViewParams.win_width = 480;
    g_t3DViewParams.win_height = 480;
    g_t3DViewParams.fov = 35.0;
    g_t3DViewParams.aspect_ratio = (float)480 / 480;//宽高比
    g_t3DViewParams.ex = 0;
    g_t3DViewParams.ey = 0;
    g_t3DViewParams.ez = 5000; // 4500;
    g_t3DViewParams.tx = 0;
    g_t3DViewParams.ty = 0;
    g_t3DViewParams.tz = -180;

    g_car_state_Handle.wheel_axis[0][0] = -852; //轮胎参数
    g_car_state_Handle.wheel_axis[0][1] = 1704;
    g_car_state_Handle.wheel_axis[0][2] = 354;
    g_car_state_Handle.wheel_axis[1][0] = 817;
    g_car_state_Handle.wheel_axis[1][1] = 1704;
    g_car_state_Handle.wheel_axis[1][2] = 355;
    g_car_state_Handle.wheel_axis[2][0] = -853;
    g_car_state_Handle.wheel_axis[2][1] = -911;
    g_car_state_Handle.wheel_axis[2][2] = 355;
    g_car_state_Handle.wheel_axis[3][0] = 816;
    g_car_state_Handle.wheel_axis[3][1] = -911;
    g_car_state_Handle.wheel_axis[3][2] = 356;

    g_car_state_Handle.swing = false;
    g_car_state_Handle.swing_angle = 0;
    g_car_state_Handle.open_axis[0][0] = -1000;
    g_car_state_Handle.open_axis[0][1] = 1031;
    g_car_state_Handle.open_axis[0][2] = 402;
    g_car_state_Handle.open_axis[1][0] = 900;
    g_car_state_Handle.open_axis[1][1] = 1031;
    g_car_state_Handle.open_axis[1][2] = 402;
    g_car_state_Handle.open_axis[2][0] = -834;
    g_car_state_Handle.open_axis[2][1] = -6;
    g_car_state_Handle.open_axis[2][2] = 344;
    g_car_state_Handle.open_axis[3][0] = 810;
    g_car_state_Handle.open_axis[3][1] = -6;
    g_car_state_Handle.open_axis[3][2] = 343;
    g_car_state_Handle.isOpen[0] = false;
    g_car_state_Handle.isOpen[1] = false;
    g_car_state_Handle.isOpen[2] = false;
    g_car_state_Handle.isOpen[3] = false;
    xcb_avmlib_3d_Set_ViewParameter(g_t3DViewParams, &g_car_state_Handle);//设置3D视图的参数


    // ViewFront 单视图前后
    //单视图前

    g_t2DViewParams.win_x = 0;
    g_t2DViewParams.win_y = 0;
    g_t2DViewParams.win_width = (698 + 100 + 100); //-60;
    g_t2DViewParams.win_height = 720;
    g_t2DViewParams.fov = 80.0;
    g_t2DViewParams.aspect_ratio = (float)g_t2DViewParams.win_width / 720;
    g_t2DViewParams.ex = 0;
    g_t2DViewParams.ey = 0;
    g_t2DViewParams.ez = 800; // 800;
    g_t2DViewParams.tx = 0;
    g_t2DViewParams.ty = 0;
    g_t2DViewParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_t2DViewParams);


    // FLView========单视图前左
    //单视图前左
    g_tFLViewParams.win_x = R_X;
    g_tFLViewParams.win_y = R_Y;
    g_tFLViewParams.win_width = R_W;
    g_tFLViewParams.win_height = R_H;
    g_tFLViewParams.fov = 80.0;
    g_tFLViewParams.aspect_ratio = (float)(R_W - 50) / R_H;
    g_tFLViewParams.ex = -100;
    g_tFLViewParams.ey = 0;
    g_tFLViewParams.ez = 850;
    g_tFLViewParams.tx = -100;
    g_tFLViewParams.ty = 0;
    g_tFLViewParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tFLViewParams);

    // FRView========单视图前右
    //单视图前右
    g_tFRViewParams.win_x = R_X;
    g_tFRViewParams.win_y = R_Y;
    g_tFRViewParams.win_width = R_W;
    g_tFRViewParams.win_height = R_H;
    g_tFRViewParams.fov = 80.0;
    g_tFRViewParams.aspect_ratio = (float)(R_W - 100) / R_H;
    g_tFRViewParams.ex = 100;
    g_tFRViewParams.ey = 0;
    g_tFRViewParams.ez = 850;
    g_tFRViewParams.tx = 100;
    g_tFRViewParams.ty = 0; // 50;
    g_tFRViewParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tFRViewParams);

    mode_load_3d = 1;//3D模式

    printf("Render init!\n");

    // ViewFront 单视图前后
    g_tSingleViewParams.win_x = 240;
    g_tSingleViewParams.win_y = 0;
    g_tSingleViewParams.win_width = 480;
    g_tSingleViewParams.win_height = 480;
    g_tSingleViewParams.fov = 80.0;
    g_tSingleViewParams.aspect_ratio = 1.0; //(float)R_W / R_H;
    g_tSingleViewParams.ex = 0;
    g_tSingleViewParams.ey = 0;
    g_tSingleViewParams.ez = 800; // 800;
    g_tSingleViewParams.tx = 0;
    g_tSingleViewParams.ty = 0;
    g_tSingleViewParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tSingleViewParams);

    // ViewFront 单视图前后
    g_tSubSingleView1Params.win_x = 240;
    g_tSubSingleView1Params.win_y = 340;
    g_tSubSingleView1Params.win_width = 140;
    g_tSubSingleView1Params.win_height = 140;
    g_tSubSingleView1Params.fov = 80.0;
    g_tSubSingleView1Params.aspect_ratio = 1.0; //(float)R_W / R_H;
    g_tSubSingleView1Params.ex = 0;
    g_tSubSingleView1Params.ey = 0;
    g_tSubSingleView1Params.ez = 800; // 800;
    g_tSubSingleView1Params.tx = 0;
    g_tSubSingleView1Params.ty = 0;
    g_tSubSingleView1Params.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tSubSingleView1Params);

    // ViewFront 单视图前后
    g_tSubSingleView2Params.win_x = 580;
    g_tSubSingleView2Params.win_y = 340;
    g_tSubSingleView2Params.win_width = 140;
    g_tSubSingleView2Params.win_height = 140;
    g_tSubSingleView2Params.fov = 80.0;
    g_tSubSingleView2Params.aspect_ratio = 1.0; //(float)R_W / R_H;
    g_tSubSingleView2Params.ex = 0;
    g_tSubSingleView2Params.ey = 0;
    g_tSubSingleView2Params.ez = 800; // 800;
    g_tSubSingleView2Params.tx = 0;
    g_tSubSingleView2Params.ty = 0;
    g_tSubSingleView2Params.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tSubSingleView2Params);

    // ViewFront 单视图前后
    g_tSubSideViewLeftParams.win_x = 0;
    g_tSubSideViewLeftParams.win_y = 0;
    g_tSubSideViewLeftParams.win_width = 200;
    g_tSubSideViewLeftParams.win_height = 480;
    g_tSubSideViewLeftParams.fov = 80.0;
    g_tSubSideViewLeftParams.aspect_ratio = 1.0; //(float)R_W / R_H;
    g_tSubSideViewLeftParams.ex = 0;
    g_tSubSideViewLeftParams.ey = 0;
    g_tSubSideViewLeftParams.ez = 800; // 800;
    g_tSubSideViewLeftParams.tx = 0;
    g_tSubSideViewLeftParams.ty = 0;
    g_tSubSideViewLeftParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tSubSideViewLeftParams);

    // ViewFront 单视图前后
    g_tSubSideViewRightParams.win_x = 520;
    g_tSubSideViewRightParams.win_y = 0;
    g_tSubSideViewRightParams.win_width = 200;
    g_tSubSideViewRightParams.win_height = 480;
    g_tSubSideViewRightParams.fov = 80.0;
    g_tSubSideViewRightParams.aspect_ratio = 1.0; //(float)R_W / R_H;
    g_tSubSideViewRightParams.ex = 0;
    g_tSubSideViewRightParams.ey = 0;
    g_tSubSideViewRightParams.ez = 800; // 800;
    g_tSubSideViewRightParams.tx = 0;
    g_tSubSideViewRightParams.ty = 0;
    g_tSubSideViewRightParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tSubSideViewRightParams);

    // g_avm_screen.win_x = 240;
    // g_avm_screen.win_y = 0;
    // g_avm_screen.width = 480;
    // g_avm_screen.height = 480;

    ///////////////////////////FOR 2D birdview
    //左视窗口，右视窗口可分别去掉，不需同时打开
    // BirdView======================================左视窗口:
    // 显示2D俯视图============================ Bird 2d car png 左视窗口:
    // 显示2D俯视图中的车的图片

    g_tBirdViewParams.win_x = LANE6_STITCHING_WIN_X + 800 + 40 + 60; // 300
    g_tBirdViewParams.win_y = LANE6_STITCHING_WIN_Y;
    g_tBirdViewParams.win_width = LANE6_STITCHING_WIN_W - 100;
    g_tBirdViewParams.win_height = LANE6_STITCHING_WIN_H;
    g_tBirdViewParams.fov = 80;
    g_tBirdViewParams.aspect_ratio = (float)(LANE6_STITCHING_WIN_W) / LANE6_STITCHING_WIN_H / 1.3;
    g_tBirdViewParams.ex = 0;
    g_tBirdViewParams.ey = -fInAxis2carcenter; //-4850;
    g_tBirdViewParams.ez = 10000;              // 16000
    g_tBirdViewParams.tx = 0;
    g_tBirdViewParams.ty = -fInAxis2carcenter; //-4850;
    g_tBirdViewParams.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tBirdViewParams);

    printf("birdview fInAxis2carcenter %f\r\n", fInAxis2carcenter);

    //俯视图全屏
    g_tBirdViewParamsfullscreen.win_x = 0; // 300
    g_tBirdViewParamsfullscreen.win_y = 0;
    g_tBirdViewParamsfullscreen.win_width = 1280;
    g_tBirdViewParamsfullscreen.win_height = 720;
    g_tBirdViewParamsfullscreen.fov = 80;
    g_tBirdViewParamsfullscreen.aspect_ratio = ((float)(LANE6_STITCHING_WIN_W)) / LANE6_STITCHING_WIN_H * 1.6;
    g_tBirdViewParamsfullscreen.ex = 0;
    g_tBirdViewParamsfullscreen.ey = 0;    //-4850;
    g_tBirdViewParamsfullscreen.ez = 9000; // 16000
    g_tBirdViewParamsfullscreen.tx = 0;
    g_tBirdViewParamsfullscreen.ty = 0; //-4850;
    g_tBirdViewParamsfullscreen.tz = 0;
    xcb_avmlib_2d_Set_ViewParameter(g_tBirdViewParamsfullscreen, -1, 0, 0);
    ///////////////////////fbo 数据拷贝获取
    avm_fboscreen_type.win_x = 0;
    avm_fboscreen_type.win_y = 0;
    avm_fboscreen_type.width = SCREEN_W;
    avm_fboscreen_type.height = SCREEN_H;

    avm_fboColorTransfer_type.win_x = 0;
    avm_fboColorTransfer_type.win_y = 0;
    avm_fboColorTransfer_type.width = SCREEN_W;
    avm_fboColorTransfer_type.height = SCREEN_H;

    printf("---debug xcb_avmlib_set_fboscreen_Init\n");
//    gettimeofday(&test2, NULL);
    xcb_avmlib_set_fboscreen_Init(&avm_fboscreen_type);//创建 fbo 的buffer
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************创建 fbo 的buffer = %lf(ms)\n", Time1 / 1000);
    printf("---debug xcb_avmlib_set_fboRGBA2YUVA_Init\n");
//    gettimeofday(&test2, NULL);
    xcb_avmlib_set_fboRGBA2YUVA_Init(&avm_fboColorTransfer_type);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************创建YUVA 的buffer = %lf(ms)\n", Time1 / 1000);
}

int lastra = -101;
void avm_calib_ui_process()
{
    if (g_avm_status != XCB_AVM_CALIB_IDLE)
    {
        if (g_avm_status == XCB_AVM_CALIB_STARTING)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_ING], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
            printf("calib start png\r\n");
        }
        else if (g_avm_status == XCB_AVM_CALIB_FINISH)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_OK], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
            if (g_bCalibFinish)
            {
                calib_update();
                g_bCalibFinish = false;
            }
        }
        else if (g_avm_status == XCB_AVM_CALIB_BACK_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_BACK], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else if (g_avm_status == XCB_AVM_CALIB_LEFT_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_LEFT], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else if (g_avm_status == XCB_AVM_CALIB_LEFT_BACK_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_LEFTBACK], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else if (g_avm_status == XCB_AVM_CALIB_FRONT_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_FRONT], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else if (g_avm_status == XCB_AVM_CALIB_RIGHT_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_RIGHT], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else if (g_avm_status == XCB_AVM_CALIB_RIGHT_BACK_FAIL)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 1, &g_png_calib[CARLIB_FAIL_RIGHTBACK], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 1);
        }
        else
        {
            printf("calib start else\r\n");
            ;
        }
    }
    else
    {
        ;
    }
}

void avm_upgrade_ui_process(void)
{

    if (g_upgrade_status != XCB_AVM_UPGRADE_IDLE)
    {
        if (g_upgrade_status == XCB_AVM_UPGRADE_DETECTED_SUCCESS)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_DET_OK], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_DETECTED_FAILED)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_DET_FAIL], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_INVALID_FILE)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_INVALID_FILE], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_INVALID_VERSION)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_INVALID_VERSION], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_APPING1)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_ING1], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_APPING2)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_ING2], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }
        if (g_upgrade_status == XCB_AVM_UPGRADE_APPING3)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_ING3], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
        }

        if (g_upgrade_status == XCB_AVM_UPGRADE_FAILED)
        {
            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_FAIL], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);

            struct timeval tCurTime;
            gettimeofday(&tCurTime, NULL);

            if (!g_bUpgradeFail)
            {
                g_bUpgradeFail = true;
                gettimeofday(&g_tUpgradeFailTime, NULL);
            }
            else if ((tCurTime.tv_sec - g_tUpgradeFailTime.tv_sec) > 4)
            {
                g_upgrade_status = XCB_AVM_UPGRADE_IDLE;
                g_bUpgradeFail = false;
            }
        }

        if (g_upgrade_status == XCB_AVM_UPGRADE_SUCCESS)
        {
            printf("app UPGRADE_SUCCESS ###################1\n");
            printf("app UPGRADE_SUCCESS ###################2\n");
            printf("app UPGRADE_SUCCESS : display image !!! \n");

            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_SUCCEED], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
            system("sync");
            system("ls -l");
            // sleep(5);
            usleep(50000);

            // printf("app UPGRADE_SUCCESS :send_reboot_request !!! \n");
            // printf("app UPGRADE_SUCCESS :send_reboot_request !!! \n");

            // g_port.send_reboot_request();
            // g_upgrade_status = XCB_AVM_UPGRADE_IDLE;
        }

        if (g_upgrade_status == XCB_AVM_UPGRADE_SYSTEM)
        {
            printf("system UPGRADE_SYSTEM_SUCCESS !!!!!!!!!!!!!!!!!!!\n");
            printf("system UPGRADE_SYSTEM_SUCCESS !!!!!!!!!!!!!!!!!!!\n");

            xcb_avmlib_Update_Png(&avmUIHandle, 3, &g_png_upgrade[UPGRADE_SYSTEM], 1);
            xcb_avmlib_Draw_Png(&avmUIHandle, 3);
            // glFinish();
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
            system("sync");
            system("ls -l");

            // sleep(6);

            printf("UPGRADE_SYSTEM_SUCCESS :send_beat_off !!! \n");
            printf("UPGRADE_SYSTEM_SUCCESS :send_beat_off !!! \n");

            g_port.send_beat_off();
            usleep(50000);

            printf("UPGRADE_SYSTEM_SUCCESS :please power off and restart power !!! \n");
            printf("UPGRADE_SYSTEM_SUCCESS :please power off and restart power !!! \n");

            // printf("UPGRADE_SYSTEM_SUCCESS :send_reboot_request !!! \n");
            // printf("UPGRADE_SYSTEM_SUCCESS :send_reboot_request !!! \n");
            // g_port.send_reboot_request();
            // g_upgrade_status = XCB_AVM_UPGRADE_IDLE;
        }
    }
    else
    {
        // do nothing
    }
}

///////////////////show number for version
void show_version_num(int num, int wx, int wy, int ww, int wh)
{
    int nTx = avmUIVersionNum[num].win_x;
    int nTy = avmUIVersionNum[num].win_y;
    int nTw = avmUIVersionNum[num].width;
    int nTh = avmUIVersionNum[num].height;
    avmUIVersionNum[num].win_x = wx;
    avmUIVersionNum[num].win_y = wy;
    avmUIVersionNum[num].width = ww;
    avmUIVersionNum[num].height = wh;
    xcb_avmlib_Draw_Png(&(avmUIVersionNum[num]), 0);
    avmUIVersionNum[num].win_x = nTx;
    avmUIVersionNum[num].win_y = nTy;
    avmUIVersionNum[num].width = nTw;
    avmUIVersionNum[num].height = nTh;
}

//=================================================================================================
//     save_yuv_img(buf1->planes[0].mem, "RB"); //B ,F ,L, LB, R ,RB
/**
 * @brief save yuv image
 * @param imgbuffer
 * @param mode the location of camera: B ,F ,L, LB, R ,RB.
 */
static void save_yuv_img(const void *imgbuffer, const char *mode)
{
    // int len = 1280*720*3/2;
    char fname[36] = {0};
    sprintf(fname, "/extp/app/6laneyuv/nv21_1280x720_%s", mode);
    FILE *jpeg_fd, *cvfd;
    jpeg_fd = fopen(fname, "wb");
    if (!jpeg_fd)
    {
        perror("open file failed!\n");
    }
    fwrite(imgbuffer, NV21SIZE, 1, jpeg_fd);
    fclose(jpeg_fd);
    system("sync");
}
//=================================================================================================
// int nPic_Num = 0;
int nDisplayMode = 0;
int nRotateMode = 0;

/////////////radar alarm/////////////
int nAlarmMode = 0;
int nDeltaNum = 10;
int nBlinkDemo = 1;
int nAlarmAccum = 0;
void render_normal_loop(void)
{
    int ret = -1;

    xcb_avm_msg_t7_t &msg = g_port.m_tMsg_t7;
    xcb_avm_view_switch_t &view = msg.emView;
    xcb_avm_car_state_t &tState = msg.tState;

    xcb_avm_car_state_gear_t &emGear = tState.emGear;
    xcb_avm_car_state_light_t &tLight = tState.tLight;
    // unsigned char ganglemcu[6] = {0, 0, 0, 0, 0, 0};
    // unsigned char gmcuh = 0, gmcul = 0;
    gmcuh = msg.u8VerInt;
    gmcul = msg.u8VerFrac;


    memcpy(ganglemcu, msg.ganglemcu, 6);

    // printf("Light state: %d\r\n", tState.tLight);
    // printf("Gear state: %d\r\n", tState.emGear);

    static int nFrameNum = 0;

    int nAlarmState = 0;
    nAlarmState = msg.tHeadangle.nAlarmState;
#ifndef ANGLESIMULATED
    int fDegreeT = msg.tHeadangle.nHeadAngle - 90;
    int fDegree;
    // if(fabs(fDegreeT - nLastHeadAngle) < 45 || nLastHeadAngle < -1000)
    // {
    //   fDegree = fDegreeT;
    //   nLastHeadAngle = fDegree;
    // }
    // else
    // {
    //   //nLastHeadAngle = fDegree;
    // fDegree = nLastHeadAngle;
    // }
    fDegree = fDegreeT;
    // int fDegree = msg.tHeadangle.nHeadAngle-90;

    if (fDegree > 90)
        fDegree = 90;

    if (fDegree < -90)
        fDegree = -90;

    nDisplayMode = 1;

    /////////////////前进模式下不同的显示方式
    double lTime; //默认微秒数
    double startTIme = app_start.tv_sec;
    double endtime = app_end.tv_sec;
    bool bShowVersion = false;
    gettimeofday(&app_end, NULL);
    lTime = (app_end.tv_sec - app_start.tv_sec); //默认微秒为单位

    if (lTime >= 5 && lTime <= 15)
    {
        bShowVersion = true;
    }

    if (app_start_flag == 0)
    {
        gettimeofday(&app_start, NULL);
        app_start_flag = 1;
    }
    else if (app_display_flag == 0)
    {

        // printf("xcb app start time = %lf(s);  %lf;  %lf\n", lTime, startTIme, endtime);
        // printf("[xingtu][%s][%d]  ****app_display_flag=%d \n", __func__,__LINE__,app_display_flag);
        if (lTime >= 29.99)
        {
            app_display_flag = 1;
            printf("[xingtu][%s][%d]  ****app_display_flag=%d \n", __func__, __LINE__, app_display_flag);
        }
    }

    if (view == REVERSE_MODE_XINGTU_VIEW_BACK)//倒车
    {
        // printf("REVERSE_MODE_XINGTU_VIEW_BACK \r\n");
        nDisplayMode = 0;
    }
    if (view == STEERING_MODE_XINGTU_LEFTDOWN)//左转
    {
        // printf("STEERING_MODE_XINGTU_LEFTDOWN \r\n");
        nDisplayMode = 2;
    }
    if (view == STEERING_MODE_XINGTU_RIGHTDOWN)//右转
    {
        // printf("STEERING_MODE_XINGTU_LEFTDOWN \r\n");
        nDisplayMode = 3;
    }

    if (view == GENERAL_MODE_XINGTU_VIEW_FRONT)//前进
    {
        // printf("GENERAL_MODE_XINGTU_VIEW_FRONT \r\n");
        if (app_display_flag == 0)
            nDisplayMode = 1;
        else
            nDisplayMode = 4;
    }
    if (nAlarmState == 1 || nAlarmState == 2 || nAlarmState == 4 ||
        nAlarmState == 8 || nAlarmState == 12)//警报状态
    {
        static int nDeltashow = 0;
        nDisplayMode = 0;
        if (nAlarmState == 1 || nAlarmState == 4)
        {
            nAlarmMode = 1;
        }
        else if (nAlarmState == 2 || nAlarmState == 8)
        {
            nAlarmMode = 2;
        }
        else if (nAlarmState == 12)
        {
            nAlarmMode = 3;
        }
        nAlarmAccum++;

        nAlarmAccum = nAlarmAccum % nDeltaNum;

        if (nAlarmAccum == 0)
        {
            nBlinkDemo = 1 - nBlinkDemo;
        }
        nDeltashow++;
        nDeltashow = nDeltashow % 20;
        if (nDeltashow == 0)
            printf("displaymode: %d; degree: %d; mode: %d; blinkdemo: %d\r\n", nDisplayMode, fDegree, nAlarmMode, nBlinkDemo);
    }
    else
    {
        nAlarmMode = 0;
        nBlinkDemo = 1;
        nAlarmAccum = 0;
    }
#endif

#ifdef ANGLESIMULATED
    nFrameNum++;
    static int rotateNum = 0;
    if (nFrameNum > 5)
    {
        nFrameNum = 0;
        nRotateMode = 1;
        rotateNum++;
        if (rotateNum >= 20)
        {
            nDisplayMode = (nDisplayMode + 1) % 4;
            rotateNum = 0;
        }
    }
#endif

#ifdef ANGLESIMULATED
    static float rotateangle = 0;
    static int addorminus = 1;
    if (nRotateMode == 1)
    {
        rotateangle += addorminus;

        if (rotateangle > 90)
        {
            addorminus = -1;
        }
        else if (rotateangle < -90)
        {
            addorminus = 1;
        }
    }

    float theta = rotateangle / 180.0 * 3.1415926;

    if (nRotateMode == 1)
    {
        /////////////////////////拼接位置发生变更
        int ra = (int)rotateangle;
        // printf("update degree %d\r\n", ra);
        xcb_avmlib_stithing_Dynamicmodel_update(&g_stitching_Handle, ra);
    }
#else
    /////////////////////////拼接位置发生变更
    float theta = (fDegree) / 180.0 * 3.14159265358979323846;
    // theta = 0;
    int ra = (int)fDegree;
    // ra = 0;
    // if(lastra != ra)
    {
        // printf("degree update value: %d %f\r\n", ra, theta);
        lastra = ra;
    }

#ifdef DYNAMIC_CAR_MODEL
    xcb_avmlib_stithing_Dynamicmodel_update(&g_stitching_Handle, ra);
#endif
    // printf("process 3\r\n");
    //  printf("update");
#endif

#ifdef ANGLESIMULATED
    theta = -theta;
    static int nMoveTimesbottom = 0;
    if (nRotateMode == 1)
    {
        float fpPostion[15];
        float tx, ty, tx1, ty1, tz1;
        for (int i = 0; i < 5; i++)
        {
            tx = avm2DCarHandle.fPointPosition[i][0] * cos(theta) - avm2DCarHandle.fPointPosition[i][1] * sin(theta); // x = (x1 - x2)cos�� - (y1 - y2)sin�� + x2
            ty = avm2DCarHandle.fPointPosition[i][1] * cos(theta) + avm2DCarHandle.fPointPosition[i][0] * sin(theta); // y = (y1 - y2)cos�� + (x1 - x2)sin�� + y2

            fpPostion[i * 3] = tx;
            fpPostion[i * 3 + 1] = ty;
            fpPostion[i * 3 + 2] = 0.00;
        }

        xcb_avmlib_Update_bottomImgPosition(&avm2DCarHandle, fpPostion);
        nMoveTimesbottom++;
        nMoveTimesbottom = nMoveTimesbottom % 4;
    }
#else
    theta = -theta;
    static int nMoveTimesbottom = 0;
    // if(nRotateMode == 1)
#ifdef DYNAMIC_CAR_MODEL
    {
        float fpPostion[15];
        float tx, ty, tx1, ty1, tz1;
        for (int i = 0; i < 5; i++)
        {
            tx = avm2DCarHandle.fPointPosition[i][0] * cos(theta) - avm2DCarHandle.fPointPosition[i][1] * sin(theta); // x = (x1 - x2)cos�� - (y1 - y2)sin�� + x2
            ty = avm2DCarHandle.fPointPosition[i][1] * cos(theta) + avm2DCarHandle.fPointPosition[i][0] * sin(theta); // y = (y1 - y2)cos�� + (x1 - x2)sin�� + y2

            fpPostion[i * 3] = tx;
            fpPostion[i * 3 + 1] = ty;
            fpPostion[i * 3 + 2] = 0.00;
        }

        xcb_avmlib_Update_bottomImgPosition(&avm2DCarHandle, fpPostion);
        for (int i = 0; i < 5; i++)
        {
            tx = avm2DCarHeadBottom.fPointPosition[i][0] * cos(theta) - avm2DCarHeadBottom.fPointPosition[i][1] * sin(theta); // x = (x1 - x2)cos�� - (y1 - y2)sin�� + x2
            ty = avm2DCarHeadBottom.fPointPosition[i][1] * cos(theta) + avm2DCarHeadBottom.fPointPosition[i][0] * sin(theta); // y = (y1 - y2)cos�� + (x1 - x2)sin�� + y2

            fpPostion[i * 3] = tx;
            fpPostion[i * 3 + 1] = ty;
            fpPostion[i * 3 + 2] = 0.00;
        }
        xcb_avmlib_Update_bottomImgPosition(&avm2DCarHeadBottom, fpPostion);
        nMoveTimesbottom++;
        nMoveTimesbottom = nMoveTimesbottom % 4;
    }
#endif

#endif

    // gettimeofday(&app_start, NULL);
    //  printf("loop wait begin\n");
    //  sem_wait(&sema[0]);
    //  sem_wait(&sema[1]);
    //  sem_wait(&sema[2]);
    //  sem_wait(&sema[3]);
    //  sem_wait(&sema[4]);
    //  sem_wait(&sema[5]);
    //  printf("loop wait end\n");

    xcb_avmlib_set_fboscreen_Inuse(&avm_fboscreen_type);//告诉GPU之后的所有渲染都是渲染到fbo的buffer中

#if ISDMA == 1

#ifdef SYS_FOUR_LANE
    //DMA方式从摄像头获取数据到显示设备
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[1], g_texture_id, 0); // B
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[2], g_texture_id, 1); // L
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[0], g_texture_id, 2); // F
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[3], g_texture_id, 3); // R
#else
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[2], g_texture_id, 0);                                // B
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[5], g_texture_id, 1);                                // L
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[3], g_texture_id, 2);                                // F
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[4], g_texture_id, 3);                                // R
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[0], g_texture_id, 4);                                // F
    xcb_avmlib_Update_TextureBuffer_DMA(g_elgImage[1], g_texture_id, 5);                                // R
#endif

#else
#if SYS_FOUR_LANE
    xcb_avmlib_Update_TextureBuffer(pNV61_B, pNV61_L, pNV61_F, pNV61_R, g_texture_id);
#else
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_B, 0, g_texture_id);
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_L, 1, g_texture_id);
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_F, 2, g_texture_id);
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_R, 3, g_texture_id);
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_L1, 4, g_texture_id);
    xcb_avmlib_Update_TextureBuffer_Single(pNV61_R1, 5, g_texture_id);
#endif
#endif
    // printf("error 1\n");
    // memcpy(pImgForSave, pImgForSaveT, (int)(1280 * 720 * 1.5));

    g_camera_type.camera_state[0] = g_bCamB;
    g_camera_type.camera_state[1] = g_bCamL;
    g_camera_type.camera_state[2] = g_bCamF;
    g_camera_type.camera_state[3] = g_bCamR;
    g_camera_type.camera_state[4] = g_bCamL1;
    g_camera_type.camera_state[5] = g_bCamR1;

    // if (nDisplayMode != 0) {
    //   sem_post(&sema1[0]);
    //   sem_post(&sema1[1]);
    //   sem_post(&sema1[2]);
    //   sem_post(&sema1[3]);
    //   sem_post(&sema1[4]);
    //   sem_post(&sema1[5]);
    // }

    // printf("error 3\n");
    // xcb_avmlib_Clean_Screen();
    xcb_avmlib_Clean_FboScreen();//清屏
    // printf("lTIme: %f; displaymode: %d\r\n", lTime, nDisplayMode);

    // nDisplayMode = 0;
    if (nDisplayMode == 0)//后退
    {
        ///////birdview + backview
        float fratio[6];
        fratio[0] = 1.0;
        fratio[1] = 1.0;
        fratio[2] = 1.0;
        fratio[3] = 1.0;
        fratio[4] = 1.0;
        fratio[5] = 1.0;
        // printf("error 4\n");
#ifdef SYS_FOUR_LANE
        //渲染 2D/3D 拼接俯视图
        xcb_avmlib_stitching_TopView_Render(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);//2D拼接
#else
        xcb_avmlib_stitching_TopView_Render_SixCamera(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#endif
        /*
        xcb_avmlib_car_Render_bottom(&g_car_Handle, g_tBirdViewParams);
        xcb_avmlib_car_Render_2d(&g_car_Handle, g_tBirdViewParams);*/

#ifdef SYS_FOUR_LANE
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#else
#ifdef DYNAMIC_CAR_MODEL
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarBodyBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avmBottomHandle, g_tBirdViewParams);
#else
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#endif
#endif

        ////////////////single view back////////////////////
        // g_avm_screen.win_x = 0;
        // g_avm_screen.win_y = 0;
        // g_avm_screen.width = (698+100+100)-60;
        // g_avm_screen.height = 720;
        // xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
        //                               &g_camera_type, 1280, 720, g_texture_id, 0, 1);

        //单视图渲染
        xcb_avmlib_singleView_Render(&g_single_Handle, g_t2DViewParams,
                                     &g_camera_type, g_texture_id);
        //线条渲染
        xcb_avm_lines_Render(&g_line_handle);

        avmOneCharLabelUIHandle.win_x = 448 - 10; // 30;
        avmOneCharLabelUIHandle.win_y = 720 - 100;

        xcb_avmlib_Draw_Png(&avmOneCharLabelUIHandle, 0);

        if (nBlinkDemo > 0 && nAlarmMode > 0)
        {
            avmRadarAlarmUIHandle.win_x = 20;
            avmRadarAlarmUIHandle.win_y = 20;
            avmRadarAlarmUIHandle.width = 898 - 20;
            avmRadarAlarmUIHandle.height = 720 - 40;
            xcb_avmlib_Draw_Png(&avmRadarAlarmUIHandle, nAlarmMode - 1);
        }


        syncx = eglCreateSyncKHR(g_egl_display, EGL_SYNC_FENCE_KHR, NULL);//首先创建一个EGLSyncKHR 同步对象
        //阻止并等待同步对象发出信号，
        eglClientWaitSyncKHR(g_egl_display, syncx, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
        eglDestroySyncKHR(g_egl_display, syncx);//销毁同步对象
    }
    else if (nDisplayMode == 1)//前进
    {
        ///////only six splitted screen
        float fratio[6];
        fratio[0] = 1.0;
        fratio[1] = 1.0;
        fratio[2] = 1.0;
        fratio[3] = 1.0;
        fratio[4] = 1.0;
        fratio[5] = 1.0;
        // printf("error 4\n");
#ifdef SYS_FOUR_LANE
        //渲染 2D/3D 拼接画面
        xcb_avmlib_stitching_TopView_Render(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#else
        xcb_avmlib_stitching_TopView_Render_SixCamera(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#endif
        /*
        xcb_avmlib_car_Render_bottom(&g_car_Handle, g_tBirdViewParams);
        xcb_avmlib_car_Render_2d(&g_car_Handle, g_tBirdViewParams);*/

#ifdef SYS_FOUR_LANE
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#else
#ifdef DYNAMIC_CAR_MODEL
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarBodyBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avmBottomHandle, g_tBirdViewParams);
#else
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#endif
#endif

        ////////////////single view front////////////////////
        g_avm_screen.win_x = 0;
        g_avm_screen.win_y = 0;
        g_avm_screen.width = (698 + 100 + 100); //-60
        g_avm_screen.height = 720;
        //全屏渲染
        xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
                                      &g_camera_type, 1280, 720, g_texture_id, 2 );

        avmOneCharLabelUIHandle.win_x = 448 - 10; // 30;
        avmOneCharLabelUIHandle.win_y = 720 - 100;

        xcb_avmlib_Draw_Png(&avmOneCharLabelUIHandle, 1);
        //GPU处理图像
        syncx = eglCreateSyncKHR(g_egl_display, EGL_SYNC_FENCE_KHR, NULL);
        eglClientWaitSyncKHR(g_egl_display, syncx, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
        eglDestroySyncKHR(g_egl_display, syncx);
    }
    else if (nDisplayMode == 2)//左转
    {
        ///////single view and left up sub single view
        float fratio[6];
        fratio[0] = 1.0;
        fratio[1] = 1.0;
        fratio[2] = 1.0;
        fratio[3] = 1.0;
        fratio[4] = 1.0;
        fratio[5] = 1.0;
        // printf("error 4\n");

#ifdef SYS_FOUR_LANE
        xcb_avmlib_stitching_TopView_Render(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#else
        xcb_avmlib_stitching_TopView_Render_SixCamera(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#endif
        /*
        xcb_avmlib_car_Render_bottom(&g_car_Handle, g_tBirdViewParams);
        xcb_avmlib_car_Render_2d(&g_car_Handle, g_tBirdViewParams);*/

#ifdef SYS_FOUR_LANE
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#else
#ifdef DYNAMIC_CAR_MODEL
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarBodyBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avmBottomHandle, g_tBirdViewParams);
#else
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#endif
#endif

        ////////////////single view left front////////////////////
        g_avm_screen.win_x = 0;
        g_avm_screen.win_y = 0;
        g_avm_screen.width = (698 + 100 + 100); //-60
        g_avm_screen.height = 720;
        xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
                                      &g_camera_type, 1280, 720, g_texture_id, 1);

        int nTw = avmTwoCharLabelUIHandle.width;
        int nTh = avmTwoCharLabelUIHandle.height;
        avmTwoCharLabelUIHandle.width = avmTwoCharLabelUIHandle.width * 2 / 3;
        avmTwoCharLabelUIHandle.height = avmTwoCharLabelUIHandle.height * 2 / 3;

        avmTwoCharLabelUIHandle.win_x = 448 - 10; // 30;
        avmTwoCharLabelUIHandle.win_y = 720 - 100;
        xcb_avmlib_Draw_Png(&avmTwoCharLabelUIHandle, 0);

        avmTwoCharLabelUIHandle.width = nTw;
        avmTwoCharLabelUIHandle.height = nTh;

        syncx = eglCreateSyncKHR(g_egl_display, EGL_SYNC_FENCE_KHR, NULL);
        eglClientWaitSyncKHR(g_egl_display, syncx, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
        eglDestroySyncKHR(g_egl_display, syncx);
    }
    else if (nDisplayMode == 3)//右转
    {
        float fratio[6];
        fratio[0] = 1.0;
        fratio[1] = 1.0;
        fratio[2] = 1.0;
        fratio[3] = 1.0;
        fratio[4] = 1.0;
        fratio[5] = 1.0;
        // printf("error 4\n");

#ifdef SYS_FOUR_LANE
        xcb_avmlib_stitching_TopView_Render(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#else
        xcb_avmlib_stitching_TopView_Render_SixCamera(
            &g_stitching_Handle, g_tBirdViewParams, &g_camera_type, g_texture_id, 0,
            fratio);
#endif
        /*
        xcb_avmlib_car_Render_bottom(&g_car_Handle, g_tBirdViewParams);
        xcb_avmlib_car_Render_2d(&g_car_Handle, g_tBirdViewParams);*/

#ifdef SYS_FOUR_LANE
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#else
#ifdef DYNAMIC_CAR_MODEL
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avm2DCarBodyBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
        xcb_avmlib_Draw_bottomImg(&avmBottomHandle, g_tBirdViewParams);
#else
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParams);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParams);
#endif
#endif

        ////////////////single view front////////////////////
        g_avm_screen.win_x = 0;
        g_avm_screen.win_y = 0;
        g_avm_screen.width = (698 + 100 + 100); //-60
        g_avm_screen.height = 720;
        xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
                                      &g_camera_type, 1280, 720, g_texture_id, 3);

        int nTw = avmTwoCharLabelUIHandle.width;
        int nTh = avmTwoCharLabelUIHandle.height;
        avmTwoCharLabelUIHandle.width = avmTwoCharLabelUIHandle.width * 2 / 3;
        avmTwoCharLabelUIHandle.height = avmTwoCharLabelUIHandle.height * 2 / 3;

        avmTwoCharLabelUIHandle.win_x = 448 - 10; // 30;
        avmTwoCharLabelUIHandle.win_y = 720 - 100;
        xcb_avmlib_Draw_Png(&avmTwoCharLabelUIHandle, 2);

        avmTwoCharLabelUIHandle.width = nTw;
        avmTwoCharLabelUIHandle.height = nTh;

        syncx = eglCreateSyncKHR(g_egl_display, EGL_SYNC_FENCE_KHR, NULL);
        eglClientWaitSyncKHR(g_egl_display, syncx, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
        eglDestroySyncKHR(g_egl_display, syncx);
    }
    else if (nDisplayMode == 4)//前进
    {
        /////////////stitching windows wide = 380
        float fratio[6];
        fratio[0] = 1.0;
        fratio[1] = 1.0;
        fratio[2] = 1.0;
        fratio[3] = 1.0;
        fratio[4] = 1.0;
        fratio[5] = 1.0;
        // int nxt = g_tBirdViewParams.win_x;
        // g_tBirdViewParams.win_x = 420 + 30;
        //  printf("error 4\n");
#ifdef SYS_FOUR_LANE
        xcb_avmlib_stitching_TopView_Render(
            &g_stitching_Handle, g_tBirdViewParamsfullscreen, &g_camera_type, g_texture_id, 0,
            fratio);
#else
        xcb_avmlib_stitching_TopView_Render_SixCamera(
            &g_stitching_Handle, g_tBirdViewParamsfullscreen, &g_camera_type, g_texture_id, 0,
            fratio);
#endif

#ifdef SYS_FOUR_LANE
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParamsfullscreen);
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParamsfullscreen);
#else
#ifdef DYNAMIC_CAR_MODEL
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParamsfullscreen);
        xcb_avmlib_Draw_bottomImg(&avm2DCarBodyBottom, g_tBirdViewParamsfullscreen);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParamsfullscreen);
        xcb_avmlib_Draw_bottomImg(&avmBottomHandle, g_tBirdViewParamsfullscreen);
#else
#ifdef SHOW_CAR_BOTTOM
        xcb_avmlib_Draw_bottomImg(&avm2DCarHeadBottom, g_tBirdViewParamsfullscreen);
#endif
        xcb_avmlib_Draw_bottomImg(&avm2DCarHandle, g_tBirdViewParamsfullscreen);
#endif
#endif

        // g_tBirdViewParams.win_x = nxt;

        /*
            g_avm_screen.win_x = 0;
            g_avm_screen.win_y = 0;
            g_avm_screen.width = 416 + 30;
            g_avm_screen.height = 720;
            xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
                                          &g_camera_type, 1280, 720, g_texture_id, 1, 3);

            int nTw = avmTwoCharLabelUIHandle.width;
            int nTh = avmTwoCharLabelUIHandle.height;
            avmTwoCharLabelUIHandle.width = avmTwoCharLabelUIHandle.width * 2 / 3;
            avmTwoCharLabelUIHandle.height = avmTwoCharLabelUIHandle.height * 2 / 3;

            avmTwoCharLabelUIHandle.win_x = 208 - 10; // 30;
            avmTwoCharLabelUIHandle.win_y = 720 - 100;
            xcb_avmlib_Draw_Png(&avmTwoCharLabelUIHandle, 0);

            g_avm_screen.win_x = 804 + 30;
            g_avm_screen.win_y = 0;
            g_avm_screen.width = 416 + 30;
            g_avm_screen.height = 720;
            xcb_avmlib_full_screen_Render(&g_avm_screen, SingleViewFrontROI,
                                          &g_camera_type, 1280, 720, g_texture_id, 3);

            avmTwoCharLabelUIHandle.width = nTw;
            avmTwoCharLabelUIHandle.height = nTh;

            avmTwoCharLabelUIHandle.width = avmTwoCharLabelUIHandle.width * 2 / 3;
            avmTwoCharLabelUIHandle.height = avmTwoCharLabelUIHandle.height * 2 / 3;

            avmTwoCharLabelUIHandle.win_x = 1012 - 10; // 30;
            avmTwoCharLabelUIHandle.win_y = 720 - 100;
            xcb_avmlib_Draw_Png(&avmTwoCharLabelUIHandle, 2);

            avmTwoCharLabelUIHandle.width = nTw;
            avmTwoCharLabelUIHandle.height = nTh;
        */

        syncx = eglCreateSyncKHR(g_egl_display, EGL_SYNC_FENCE_KHR, NULL);
        eglClientWaitSyncKHR(g_egl_display, syncx, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
        eglDestroySyncKHR(g_egl_display, syncx);
    }

    avm_calib_ui_process();//标定的UI显示
    ////////////////////upgrade ui/////////////////
    if (g_upgrade_status == XCB_AVM_UPGRADE_IDLE)
    {

        // avm_steering_ui_processs();
        if (bShowVersion) //////////20,26, 1110, 572
        {
            int nTx, nTy, nTw, nTh;
            int nStepw = 18;
            int nSteph = 46;
            int nXstart = 1110 + 6;
            int nYstart = 720 - 572 - 27;
            int nSubimgW = 16;
            int nSubimgH = 24;
            ////////////显示背景
            xcb_avmlib_Draw_Png(&avmUIVersionbg, 0);
            ////////////soc app version
            if (gCpuVer1 >= 0 && gCpuVer1 <= 9 && gCpuVer2 >= 0 && gCpuVer2 <= 9)
            {
                show_version_num(gCpuVer1, nXstart, nYstart, nSubimgW, nSubimgH);
                show_version_num(gCpuVer2, nXstart + nStepw, nYstart, nSubimgW, nSubimgH);
            }

            ////////////mcu app version
            if (gmcuh >= 0 && gmcuh <= 9 && gmcul >= 0 && gmcul <= 9)
            {
                show_version_num(gmcuh, nXstart, nYstart - nSteph, nSubimgW, nSubimgH);
                show_version_num(gmcul, nXstart + nStepw, nYstart - nSteph, nSubimgW, nSubimgH);
            }
            // printf("version num: %d %d %d %d \r\n", gCpuVer1, gCpuVer2, gmcuh, gmcul);

            ////////////angle mcu app version
            for (int i = 0; i < 6; i++)
            {
                if (ganglemcu[i] >= 0 && ganglemcu[i] <= 9)
                    show_version_num(ganglemcu[i], nXstart + i * nStepw, nYstart - 2 * nSteph, nSubimgW, nSubimgH);
                // printf("angleversion: %d ", ganglemcu[i]);
            }
        }
        if (bShowMcuUpdate)
        {
            // printf("MCU update show\r\n");
            xcb_avmlib_Draw_Png(&avmUIMCUHandleHandle, 0);
        }

        // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
    }
    else
    {
        if (!bShowMcuUpdate)
            avm_upgrade_ui_process();//升级的UI显示
        else
        {
            xcb_avmlib_Draw_Png(&avmUIMCUHandleHandle, 0);
            // xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);
            //  printf("MCU update show\r\n");
        }
    }

    xcb_avmlib_set_fboscreen_Outofuse(&avm_fboscreen_type);//告诉GPU之后的所有渲染都是渲染到显存中
    // printf("---debug xcb_avmlib_set_fboscreen_ObtainTextureID\n");

    GLuint nScreenTexId = xcb_avmlib_set_fboscreen_ObtainTextureID(&avm_fboscreen_type);//获取buffer拼接数据的 ID 号

    // printf("---debug xcb_avmlib_set_fboscreen_ObtainTextureID finish------------------------%d\n", nScreenTexId);

    xcb_avmlib_set_fboRGBA2YUVA_Inuse(&avm_fboColorTransfer_type);//渲染到 YUVA的 buffer
    xcb_avmlib_Clean_FboScreen();//清屏
    // printf("---debug xcb_avmlib_set_fboRGBA2YUVA_ObtainTexturePoint finish\n");
    //把 YUVA 的数据直接搬运到了 ARM 端
    unsigned char *pImg = xcb_avmlib_set_fboRGBA2YUVA_ObtainTexturePoint(nScreenTexId, &avm_fboColorTransfer_type);

    glFlush();

    // glFinish();
    xcb_avmlib_YUVA2NV21(pImg, pImgForSaveT1, SCREEN_W, SCREEN_H);//将 YUVA格式的数据转换成NV21格式的数据

    xcb_avmlib_set_fboRGBA2YUVA_Outofuse(&avm_fboColorTransfer_type);//渲染到显存

    /////////////屏幕上显示
    xcb_avmlib_Clean_Screen();
    xcb_avmlib_set_fboscreen_Tex2screen(nScreenTexId, &avm_fboscreen_type);//渲染到屏幕
    xcb_avmlib_swapbuffer(g_egl_display, g_egl_surface);//将数据显示到屏幕

    pthread_mutex_lock(&mutex_light);
    memcpy(pImgForSaveT, pImgForSaveT1, 1382400);
    pthread_mutex_unlock(&mutex_light);
    // char imgName[250];
    // sprintf(imgName, "yuv%d", nImgIndex);
    // nImgIndex++;

    // FILE *pF = fopen(imgName, "wb+");
    // fwrite(pImgForSaveT1, 1, 1280 * 720 * 1.5, pF);
    // fclose(pF);
    // system("sync");

    nRotateMode = 0;

    // nDisplayMode = 0;
#if 0
    gettimeofday(&app_end, NULL);
    double lTime; //默认微秒数
    lTime = 1000000 * (app_end.tv_sec - app_start.tv_sec) + app_end.tv_usec - app_start.tv_usec; //默认微秒为单位
    printf("xcb run time = %lf(s)\n", lTime / 1000000);

#endif
}

void avm_img_roi_init()
{
    ///////////////////roi region init/////////////////////
    AllScrenSixROI[0].x = SIX_LANE_BACK_X;
    AllScrenSixROI[0].y = SIX_LANE_BACK_Y;
    AllScrenSixROI[0].w = SIX_LANE_BACK_W;
    AllScrenSixROI[0].h = SIX_LANE_BACK_H;

    AllScrenSixROI[1].x = SIX_LANE_LEFT_X;
    AllScrenSixROI[1].y = SIX_LANE_LEFT_Y;
    AllScrenSixROI[1].w = SIX_LANE_LEFT_W;
    AllScrenSixROI[1].h = SIX_LANE_LEFT_H;

    AllScrenSixROI[2].x = SIX_LANE_FRONT_X;
    AllScrenSixROI[2].y = SIX_LANE_FRONT_Y;
    AllScrenSixROI[2].w = SIX_LANE_FRONT_W;
    AllScrenSixROI[2].h = SIX_LANE_FRONT_H;

    AllScrenSixROI[3].x = SIX_LANE_RIGHT_X;
    AllScrenSixROI[3].y = SIX_LANE_RIGHT_Y;
    AllScrenSixROI[3].w = SIX_LANE_RIGHT_W;
    AllScrenSixROI[3].h = SIX_LANE_RIGHT_H;

    AllScrenSixROI[4].x = SIX_LANE_LEFT1_X;
    AllScrenSixROI[4].y = SIX_LANE_LEFT1_Y;
    AllScrenSixROI[4].w = SIX_LANE_LEFT1_W;
    AllScrenSixROI[4].h = SIX_LANE_LEFT1_H;

    AllScrenSixROI[5].x = SIX_LANE_RIGHT1_X;
    AllScrenSixROI[5].y = SIX_LANE_RIGHT1_Y;
    AllScrenSixROI[5].w = SIX_LANE_RIGHT1_W;
    AllScrenSixROI[5].h = SIX_LANE_RIGHT1_H;

    SubScreenSixROI[0].x = SUB6LANE_BACK_X;
    SubScreenSixROI[0].y = SUB6LANE_BACK_Y;
    SubScreenSixROI[0].w = SUB6LANE_BACK_W;
    SubScreenSixROI[0].h = SUB6LANE_BACK_H;

    SubScreenSixROI[1].x = SUB6LANE_LEFT_X;
    SubScreenSixROI[1].y = SUB6LANE_LEFT_Y;
    SubScreenSixROI[1].w = SUB6LANE_LEFT_W;
    SubScreenSixROI[1].h = SUB6LANE_LEFT_H;

    SubScreenSixROI[2].x = SUB6LANE_FRONT_X;
    SubScreenSixROI[2].y = SUB6LANE_FRONT_Y;
    SubScreenSixROI[2].w = SUB6LANE_FRONT_W;
    SubScreenSixROI[2].h = SUB6LANE_FRONT_H;

    SubScreenSixROI[3].x = SUB6LANE_RIGHT_X;
    SubScreenSixROI[3].y = SUB6LANE_RIGHT_Y;
    SubScreenSixROI[3].w = SUB6LANE_RIGHT_W;
    SubScreenSixROI[3].h = SUB6LANE_RIGHT_H;

    SubScreenSixROI[4].x = SUB6LANE_LEFT1_X;
    SubScreenSixROI[4].y = SUB6LANE_LEFT1_Y;
    SubScreenSixROI[4].w = SUB6LANE_LEFT1_W;
    SubScreenSixROI[4].h = SUB6LANE_LEFT1_H;

    SubScreenSixROI[5].x = SUB6LANE_RIGHT1_X;
    SubScreenSixROI[5].y = SUB6LANE_RIGHT1_Y;
    SubScreenSixROI[5].w = SUB6LANE_RIGHT1_W;
    SubScreenSixROI[5].h = SUB6LANE_RIGHT1_H;

    SingleViewRightROI.x = SINGLEVIEW_RIGHT_X;
    SingleViewRightROI.y = SINGLEVIEW_RIGHT_Y;
    SingleViewRightROI.w = SINGLEVIEW_RIGHT_W;
    SingleViewRightROI.h = SINGLEVIEW_RIGHT_H;

    SingleViewLeftROI.x = SINGLEVIEW_LEFT_X;
    SingleViewLeftROI.y = SINGLEVIEW_LEFT_Y;
    SingleViewLeftROI.w = SINGLEVIEW_LEFT_W;
    SingleViewLeftROI.h = SINGLEVIEW_LEFT_H;

    SingleViewFrontROI.x = SINGLEVIEW_FRONT_X;
    SingleViewFrontROI.y = SINGLEVIEW_FRONT_Y;
    SingleViewFrontROI.w = SINGLEVIEW_FRONT_W;
    SingleViewFrontROI.h = SINGLEVIEW_FRONT_H;

    SubSingleViewRightROI.x = SUB_SINGLEVIEW_RIGHT_X;
    SubSingleViewRightROI.y = SUB_SINGLEVIEW_RIGHT_Y;
    SubSingleViewRightROI.w = SUB_SINGLEVIEW_RIGHT_W;
    SubSingleViewRightROI.h = SUB_SINGLEVIEW_RIGHT_H;

    SubSingleViewLeftROI.x = SUB_SINGLEVIEW_LEFT_X;
    SubSingleViewLeftROI.y = SUB_SINGLEVIEW_LEFT_Y;
    SubSingleViewLeftROI.w = SUB_SINGLEVIEW_LEFT_W;
    SubSingleViewLeftROI.h = SUB_SINGLEVIEW_LEFT_H;

    RHalfSingleViewRightROI.x = SINGLEVIEW_R_HALFSCREEN_RIGHT_X;
    RHalfSingleViewRightROI.y = SINGLEVIEW_R_HALFSCREEN_RIGHT_Y;
    RHalfSingleViewRightROI.w = SINGLEVIEW_R_HALFSCREEN_RIGHT_W;
    RHalfSingleViewRightROI.h = SINGLEVIEW_R_HALFSCREEN_RIGHT_H;

    LHalfSingleViewLeftROI.x = SINGLEVIEW_L_HALFSCREEN_LEFT_X;
    LHalfSingleViewLeftROI.y = SINGLEVIEW_L_HALFSCREEN_LEFT_Y;
    LHalfSingleViewLeftROI.w = SINGLEVIEW_L_HALFSCREEN_LEFT_W;
    LHalfSingleViewLeftROI.h = SINGLEVIEW_L_HALFSCREEN_LEFT_H;
}
void avm_rectime_init(void)
{
    char pngPath[500];
    for (int i = 0; i < 10; i++)
    {
        g_png_RecTimeNum[i].win_x = 0; // bg layer 0
        g_png_RecTimeNum[i].win_y = 0;
        sprintf(pngPath, "%s/RecNum%d.png", g_pPngDir, i);
        xcb_avmlib_Load_Png(pngPath, g_png_RecTimeNum[i]);
        int ntotalnum = g_png_RecTimeNum[i].width * g_png_RecTimeNum[i].height;
        unsigned char *pin = g_png_RecTimeNum[i].texture;
        pRecNumber[i] = new unsigned char[ntotalnum * 4];
        unsigned char *pout = pRecNumber[i];
        for (int j = 0; j < ntotalnum; j++)
        {
            pout[0] = 0.299 * pin[0] + 0.587 * pin[1] + 0.114 * pin[2];
            pout[1] = (pin[0] - pout[0]) * 0.713 + 128;
            pout[2] = (pin[2] - pout[0]) * 0.564 + 128;
            pout[3] = pin[3];
            pin += 4;
            pout += 4;
        }
    }

    g_png_RecTimeColon.win_x = 0;
    g_png_RecTimeColon.win_y = 0;
    sprintf(pngPath, "%s/RecColon.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_RecTimeColon);
    int ntotalnum = g_png_RecTimeColon.width * g_png_RecTimeColon.height;
    unsigned char *pin = g_png_RecTimeColon.texture;
    pRecColon = new unsigned char[ntotalnum * 4];
    unsigned char *pout = pRecColon;
    for (int j = 0; j < ntotalnum; j++)
    {
        pout[0] = 0.299 * pin[0] + 0.587 * pin[1] + 0.114 * pin[2];
        pout[1] = (pin[0] - pout[0]) * 0.713 + 128;
        pout[2] = (pin[2] - pout[0]) * 0.564 + 128;
        pout[3] = pin[3];
        pin += 4;
        pout += 4;
    }

    g_png_RecTimeDash.win_x = 0;
    g_png_RecTimeDash.win_y = 0;
    sprintf(pngPath, "%s/RecDash.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_RecTimeDash);
    ntotalnum = g_png_RecTimeDash.width * g_png_RecTimeDash.height;
    pin = g_png_RecTimeDash.texture;
    pRecDash = new unsigned char[ntotalnum * 4];
    pout = pRecDash;
    for (int j = 0; j < ntotalnum; j++)
    {
        pout[0] = 0.299 * pin[0] + 0.587 * pin[1] + 0.114 * pin[2];
        pout[1] = (pin[0] - pout[0]) * 0.713 + 128;
        pout[2] = (pin[2] - pout[0]) * 0.564 + 128;
        pout[3] = pin[3];
        pin += 4;
        pout += 4;
    }
}
void avm_ui_init(void)
{

    char pngPath[500];
    //////////////////加载radar alarm图片
    avmRadarAlarmUIHandle.win_x = 0;
    avmRadarAlarmUIHandle.win_y = 0;
    avmRadarAlarmUIHandle.width = 898;
    avmRadarAlarmUIHandle.height = 720;
    avmRadarAlarmUIHandle.layers_num = 3;
    xcb_avmlib_Draw_Png_Init(&avmRadarAlarmUIHandle);
    g_png_RadarAlarm[0].win_x = 0;
    g_png_RadarAlarm[0].win_y = 0;
    sprintf(pngPath, "%s/alarm_yellow.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_RadarAlarm[0]);
    xcb_avmlib_Update_Png(&avmRadarAlarmUIHandle, 0, &(g_png_RadarAlarm[0]), 1);

    g_png_RadarAlarm[1].win_x = 0;
    g_png_RadarAlarm[1].win_y = 0;
    sprintf(pngPath, "%s/alarm_orange.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_RadarAlarm[1]);
    xcb_avmlib_Update_Png(&avmRadarAlarmUIHandle, 1, &(g_png_RadarAlarm[1]), 1);

    g_png_RadarAlarm[2].win_x = 0;
    g_png_RadarAlarm[2].win_y = 0;
    sprintf(pngPath, "%s/alarm_red.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_RadarAlarm[2]);
    xcb_avmlib_Update_Png(&avmRadarAlarmUIHandle, 2, &(g_png_RadarAlarm[2]), 1);


    ///////////////////加载长隔条
    avmLongHLineUIHandle.win_x = 0;
    avmLongHLineUIHandle.win_y = 359;
    avmLongHLineUIHandle.width = LONG_HORIZONTAL_LINE_W;
    avmLongHLineUIHandle.height = LONG_HORIZONTAL_LINE_H;
    avmLongHLineUIHandle.layers_num = 1; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmLongHLineUIHandle);


    g_png_LongHLine.win_x = 0; // bg layer 0
    g_png_LongHLine.win_y = 0;
    sprintf(pngPath, "%s/LHLine.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_LongHLine);

    xcb_avmlib_Update_Png(&avmLongHLineUIHandle, 0, &g_png_LongHLine, 1);
    printf("error ui 1\n");

    /////////////////加载短隔条
    avmShortHLineUIHandle.win_x = 478;
    avmShortHLineUIHandle.win_y = 359;
    avmShortHLineUIHandle.width = SHORT_HORIZONTAL_LINE_W;
    avmShortHLineUIHandle.height = SHORT_HORIZONTAL_LINE_H;
    avmShortHLineUIHandle.layers_num = 1; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmShortHLineUIHandle);

    g_png_ShortHLine.win_x = 0; // bg layer 0
    g_png_ShortHLine.win_y = 0;
    sprintf(pngPath, "%s/SHLine.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_ShortHLine);

    xcb_avmlib_Update_Png(&avmShortHLineUIHandle, 0, &g_png_ShortHLine, 1);
    printf("error ui 2\n");


    ////////////加载竖条
    avmVLineUIHandle.win_x = 0;
    avmVLineUIHandle.win_y = 0;
    avmVLineUIHandle.width = VERTICAL_LINE_W;
    avmVLineUIHandle.height = VERTICAL_LINE_H;
    avmVLineUIHandle.layers_num = 1; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmVLineUIHandle);

    g_png_VLine.win_x = 0; // bg layer 0
    g_png_VLine.win_y = 0;
    sprintf(pngPath, "%s/VLine.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_VLine);

    xcb_avmlib_Update_Png(&avmVLineUIHandle, 0, &g_png_VLine, 1);
    printf("error ui 3\n");

    /////////////加载左右上部子窗口
    avmUpSubWinUIHandle.win_x = 0;
    avmUpSubWinUIHandle.win_y = 0;
    avmUpSubWinUIHandle.width = UPSIDE_WIN_SIZE_W;
    avmUpSubWinUIHandle.height = UPSIDE_WIN_SIZE_H;
    avmUpSubWinUIHandle.layers_num = 2; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmUpSubWinUIHandle);

    g_png_UpSubWinImg[0].win_x = 0; // bg layer 0
    g_png_UpSubWinImg[0].win_y = 0;
    sprintf(pngPath, "%s/LeftUpSubWin.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_UpSubWinImg[0]);

    g_png_UpSubWinImg[1].win_x = 0; // bg layer 0
    g_png_UpSubWinImg[1].win_y = 0;
    sprintf(pngPath, "%s/RightUpSubWin.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_UpSubWinImg[1]);

    xcb_avmlib_Update_Png(&avmUpSubWinUIHandle, 0, &g_png_UpSubWinImg[0], 1);
    xcb_avmlib_Update_Png(&avmUpSubWinUIHandle, 1, &g_png_UpSubWinImg[1], 1);
    printf("error ui 4\n");

    ////////////////加载单一字符图片
    avmOneCharLabelUIHandle.win_x = 0;
    avmOneCharLabelUIHandle.win_y = 0;
    avmOneCharLabelUIHandle.width = ONECHAR_CAMERALABEL_IMG_W;
    avmOneCharLabelUIHandle.height = ONECHAR_CAMERALABEL_IMG_H;
    avmOneCharLabelUIHandle.layers_num = 2; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmOneCharLabelUIHandle);

    g_png_OneCharLabelImg[0].win_x = 0; // bg layer 0
    g_png_OneCharLabelImg[0].win_y = 0;
    sprintf(pngPath, "%s/backlabel.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_OneCharLabelImg[0]);

    g_png_OneCharLabelImg[1].win_x = 0; // bg layer 0
    g_png_OneCharLabelImg[1].win_y = 0;
    sprintf(pngPath, "%s/frontlabel.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_OneCharLabelImg[1]);
    // printf("frontal img size: %d %d\n", g_png_OneCharLabelImg[1].width,
    //        g_png_OneCharLabelImg[1].height);

    xcb_avmlib_Update_Png(&avmOneCharLabelUIHandle, 0, &g_png_OneCharLabelImg[0],
                          1);
    xcb_avmlib_Update_Png(&avmOneCharLabelUIHandle, 1, &g_png_OneCharLabelImg[1],
                          1);
    printf("error ui 5\n");

    //////////////////加载双字符图片
    avmTwoCharLabelUIHandle.win_x = 0;
    avmTwoCharLabelUIHandle.win_y = 0;
    avmTwoCharLabelUIHandle.width = TWOCHAR_CAMERALABEL_IMG_W;
    avmTwoCharLabelUIHandle.height = TWOCHAR_CAMERALABEL_IMG_H;
    avmTwoCharLabelUIHandle.layers_num = 4; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmTwoCharLabelUIHandle);

    g_png_TwoCharLabelImg[0].win_x = 0; // bg layer 0
    g_png_TwoCharLabelImg[0].win_y = 0;
    sprintf(pngPath, "%s/leftdown.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_TwoCharLabelImg[0]);

    g_png_TwoCharLabelImg[1].win_x = 0; // bg layer 0
    g_png_TwoCharLabelImg[1].win_y = 0;
    sprintf(pngPath, "%s/leftback.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_TwoCharLabelImg[1]);

    g_png_TwoCharLabelImg[2].win_x = 0; // bg layer 0
    g_png_TwoCharLabelImg[2].win_y = 0;
    sprintf(pngPath, "%s/rightdown.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_TwoCharLabelImg[2]);

    g_png_TwoCharLabelImg[3].win_x = 0; // bg layer 0
    g_png_TwoCharLabelImg[3].win_y = 0;
    sprintf(pngPath, "%s/rightback.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_TwoCharLabelImg[3]);

    xcb_avmlib_Update_Png(&avmTwoCharLabelUIHandle, 0, &g_png_TwoCharLabelImg[0],
                          1);
    xcb_avmlib_Update_Png(&avmTwoCharLabelUIHandle, 1, &g_png_TwoCharLabelImg[1],
                          1);

    xcb_avmlib_Update_Png(&avmTwoCharLabelUIHandle, 2, &g_png_TwoCharLabelImg[2],
                          1);
    xcb_avmlib_Update_Png(&avmTwoCharLabelUIHandle, 3, &g_png_TwoCharLabelImg[3],
                          1);
    printf("error ui 6\n");

    ///////////////摄像头检测
    avmCameraCheckUIHandle.win_x = 0;
    avmCameraCheckUIHandle.win_y = 0;
    avmCameraCheckUIHandle.width = CAMERA_CHECKIMG_W;
    avmCameraCheckUIHandle.height = CAMERA_CHECKIMG_H;
    avmCameraCheckUIHandle.layers_num = 1; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmCameraCheckUIHandle);

    g_png_CameraCheckImg.win_x = 0; // bg layer 0
    g_png_CameraCheckImg.win_y = 0;
    sprintf(pngPath, "%s/cameracheck.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_CameraCheckImg);

    xcb_avmlib_Update_Png(&avmCameraCheckUIHandle, 0, &g_png_CameraCheckImg, 1);
    printf("error ui 7\n");

    ////////////////注意周边安全
    avmAttentionAroundUIHandle.win_x = 0;
    avmAttentionAroundUIHandle.win_y = 0;
    avmAttentionAroundUIHandle.width = ATTENTION_AROUNDIMG_W;
    avmAttentionAroundUIHandle.height = ATTENTION_AROUNDIMG_H;
    avmAttentionAroundUIHandle.layers_num = 1; // 1个图层
    xcb_avmlib_Draw_Png_Init(&avmAttentionAroundUIHandle);

    g_png_AttentionAroundImg.win_x = 0; // bg layer 0
    g_png_AttentionAroundImg.win_y = 0;
    sprintf(pngPath, "%s/attentionaround.png", g_pPngDir);
    xcb_avmlib_Load_Png(pngPath, g_png_AttentionAroundImg);

    xcb_avmlib_Update_Png(&avmAttentionAroundUIHandle, 0,
                          &g_png_AttentionAroundImg, 1);
    printf("error ui 8\n");


    /////////////////////ui version init//////////////////////
    g_png_version = new png_texture_t[MAX_VERSION_PNG];
    for (int i = 0; i < MAX_VERSION_PNG; i++)
    {
        g_png_version[i].win_x = init_version_bmp_res[i].x;
        g_png_version[i].win_y = init_version_bmp_res[i].y;
        sprintf(pngPath, "%s/%s", g_pPngDir, init_version_bmp_res[i].file_name);
        xcb_avmlib_Load_Png(pngPath, g_png_version[i]);
    }
//    gettimeofday(&test2, NULL);
    avmUIVersionbg.win_x = 0;
    avmUIVersionbg.win_y = 0;
    avmUIVersionbg.width = 1280;
    avmUIVersionbg.height = 720;
    avmUIVersionbg.layers_num = 1; // 1个图层
    xcb_avmlib_Draw_Png_Init(&avmUIVersionbg);
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test2.tv_sec) + test1.tv_usec - test2.tv_usec; //默认微秒为单位
//    printf("******************************绘制版本信息UI后 = %lf(ms)\n", Time1 / 1000);
    xcb_avmlib_Update_Png(&avmUIVersionbg, 0, &g_png_version[0], 1);


    // CPU NUM 0-9图层------7

    avmUIVersionNum = new avm_ui_type_t[MAX_CPU_HI_PNG];

    g_png_cpuNumH = new png_texture_t[MAX_CPU_HI_PNG];
    for (int i = 0; i < MAX_CPU_HI_PNG; i++)
    {
        g_png_cpuNumH[i].win_x = 0; // init_cpu_num_hi_bmp_res[i].x;
        g_png_cpuNumH[i].win_y = 0; // init_cpu_num_hi_bmp_res[i].y;
        sprintf(pngPath, "%s/%s", g_pPngDir, init_cpu_num_hi_bmp_res[i].file_name);
        xcb_avmlib_Load_Png(pngPath, g_png_cpuNumH[i]);

        avmUIVersionNum[i].win_x = g_png_cpuNumH[i].win_x;
        avmUIVersionNum[i].win_y = g_png_cpuNumH[i].win_y;
        avmUIVersionNum[i].width = (g_png_cpuNumH[i].width);
        avmUIVersionNum[i].height = g_png_cpuNumH[i].height;
        avmUIVersionNum[i].layers_num = 1; // 1个图层

        xcb_avmlib_Draw_Png_Init(&(avmUIVersionNum[i]));
        xcb_avmlib_Update_Png(&(avmUIVersionNum[i]), 0, &(g_png_cpuNumH[i]), 1);
    }

    g_png_mcuNumH = new png_texture_t[MAX_MCU_HI_PNG];
    for (int i = 0; i < MAX_MCU_HI_PNG; i++)
    {
        g_png_mcuNumH[i].win_x = init_mcu_num_hi_bmp_res[i].x;
        g_png_mcuNumH[i].win_y = init_mcu_num_hi_bmp_res[i].y;
        sprintf(pngPath, "%s/%s", g_pPngDir, init_mcu_num_hi_bmp_res[i].file_name);
        xcb_avmlib_Load_Png(pngPath, g_png_mcuNumH[i]);
    }

    g_png_AnglemcuNumH = new png_texture_t[MAX_ANGLEMCU_HI_PNG];
    for (int i = 0; i < MAX_ANGLEMCU_HI_PNG; i++)
    {
        g_png_AnglemcuNumH[i].win_x = init_anglemcu_num_hi_bmp_res[i].x;
        g_png_AnglemcuNumH[i].win_y = init_anglemcu_num_hi_bmp_res[i].y;
        sprintf(pngPath, "%s/%s", g_pPngDir, init_anglemcu_num_hi_bmp_res[i].file_name);
        xcb_avmlib_Load_Png(pngPath, g_png_AnglemcuNumH[i]);
    }
}

void *avm_calib_proc(void *args)
{
    constexpr int width = CAMERA_WIDTH;
    constexpr int height = CAMERA_HEIGHT;

#if READIMG_CALIB
    xcb_calib_image_format_t img_format = XCB_CALIB_IMAGE_FORMAT_NV61;
#else
    xcb_after_calib_image_format_t img_format = XCB_CALIB_IMAGE_FORMAT_NV21;
#endif

    sleep(3);

    while (1)
    {
        xcb_avm_calib_or_not_t &emCalib = g_port.m_tMsg_t7.emCalib;
        if (emCalib == XCB_AVM_CALIB_OPENED)
        {
            if (!g_bCalibStart)
            {
                printf("autocalib open...,, \n");
                g_avm_status = XCB_AVM_CALIB_STARTING;
                emCalib = XCB_AVM_CALIB_CLOSED;
                g_bCalibStart = true;
            }
        }

        if (g_avm_status == XCB_AVM_CALIB_STARTING)
        {
#if READIMG_CALIB

            if (pNV21_B && pNV21_L && pNV21_F && pNV21_R)
            {
                printf("Start to autocalib...\n");

                g_calib_err_code = xcb_autocalib(SAVE_LICENSE_PATH, g_pCalibConfigDir, g_pCreateModelDir,
                                                 (unsigned char *)(NV610),
                                                 (unsigned char *)(NV611),
                                                 (unsigned char *)(NV612),
                                                 (unsigned char *)(NV613), img_format,
                                                 width, height);
#else

            if (buf0->planes[0].mem && buf1->planes[0].mem && buf2->planes[0].mem && buf3->planes[0].mem)
            {
                printf("Start to video autocalib...\n");

                g_calib_err_code = xcb_after_autocalib(SAVE_LICENSE_PATH, g_pCalibConfigDir, g_pCreateModelDir,
                                                       (unsigned char *)(buf1->planes[0].mem),
                                                       (unsigned char *)(buf2->planes[0].mem),
                                                       (unsigned char *)(buf0->planes[0].mem),
                                                       (unsigned char *)(buf3->planes[0].mem),
                                                       (unsigned char *)(buf4->planes[0].mem),
                                                       (unsigned char *)(buf5->planes[0].mem), img_format,
                                                       width, height);
#endif

                emCalib = XCB_AVM_CALIB_CLOSED;

                if (g_calib_err_code == XCB_AFTER_CALIB_NO_ERROR)
                {
                    printf("AutoCalib XCB_CALIB_NO_ERROR,finsh===========\n");
                    g_port.send_calib_state(true);
                    g_avm_status = XCB_AVM_CALIB_FINISH;
                    usleep(200000); // 200ms sleep for show UI
                    g_bCalibFinish = true;
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    // g_bCalibState = true;
                    sleep(5); // 5s sleep for calib model reload
                    emCalib = XCB_AVM_CALIB_CLOSED;

                    g_avm_status = XCB_AVM_CALIB_IDLE;
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_LEFT_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_LEFT_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_LEFT_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_LEFT_BACK_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_LEFT_BACK_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_LEFT_BACK_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_RIGHT_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_RIGHT_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_RIGHT_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_RIGHT_BACK_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_RIGHT_BACK_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_RIGHT_BACK_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_BACK_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_BACK_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_BACK_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_FRONT_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_FRONT_FAIL===========\n");
                    g_avm_status = XCB_AVM_CALIB_FRONT_FAIL;
                    // g_port.send_calib_state(false);
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                    sleep(1);
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_GET_FRAME_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_GET_FRAME_FAIL===========\n");
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_LOAD_CONFIG_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_LOAD_CONFIG_FAI===========\n");
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_MATCH_CPUID_FAIL)
                {
                    printf("AutoCalib XCB_CALIB_MATCH_CPUID_FAIL===========\n");
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_CONFIG_PATH_EMPTY)
                {
                    printf("AutoCalib XCB_CALIB_CONFIG_PATH_EMPTY===========\n");
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
                else if (g_calib_err_code == XCB_AFTER_CALIB_IMAGE_FORMAT_UNSUPPORT)
                {
                    printf("AutoCalib XCB_CALIB_IMAGE_FORMAT_UNSUPPORT===========\n");
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
                else
                {
                    g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_CLOSED;
                }
            }
            g_bCalibStart = false;
        }
    }

    pthread_exit(0);
}

void upgrade_proc(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return;
    }
    else if (pid == 0)
    {
        execl("/extp/app/upgrade", NULL);
        exit(0);
    }
    else
    {
        return;
    }
}

void DynamicModelInit()
{
    //动态模型拼接
    if (-1 == xcb_avmlib_stitching_Dynamicmodel_Init("/extp/app/gpu_data/AngleModel_2D", &g_stitching_Handle))
    {
        printf("init dynamic model /extp/app/gpu_data/AngleModel_2D error\r\n");
        int res = xcb_avmlib_stitching_Dynamicmodel_Init("/extp/app/gpu_data_default/AngleModel_2D", &g_stitching_Handle);
        if (res == -1)
        {
            printf("init dynamic model /extp/app/gpu_data_default/AngleModel_2D error\r\n");
            return;
        }
    }
    printf("init dynamic model success\r\n");
}

void avm_ui_init_upgrad_calib(void)
{

    int i = 0, j = 0, k = 0;
    char pngPath[500];

    g_hide_flag = false; // 4个摄像头图标显示隐藏处理
    g_hide_count = 0;

    avmUIHandle.win_x = 0;
    avmUIHandle.win_y = 0;
    avmUIHandle.width = 1280;
    avmUIHandle.height = 700;
    avmUIHandle.layers_num = 13; // 12个图层
    xcb_avmlib_Draw_Png_Init(&avmUIHandle);

    {
        // UI背景图层----0
        g_png_background.win_x = 430; // bg layer 0
        g_png_background.win_y = 570;
        sprintf(pngPath, "%s/background_1280.png", g_pPngDir);
        xcb_avmlib_Load_Png(pngPath, g_png_background);

        //标定UI图层----1
        g_png_calib = new png_texture_t[CARLIB_MAX_PNG];
        for (i = 0; i < CARLIB_MAX_PNG; i++)
        {

            g_png_calib[i].win_x = init_calib_1280bmp_res[i].x;
            g_png_calib[i].win_y = init_calib_1280bmp_res[i].y;
            sprintf(pngPath, "%s/%s", g_pPngDir, init_calib_1280bmp_res[i].file_name);
            xcb_avmlib_Load_Png(pngPath, g_png_calib[i]);
        }

        //升级UI图层------3
        g_png_upgrade = new png_texture_t[UPGRADE_MAX_PNG];
        for (i = 0; i < UPGRADE_MAX_PNG; i++)
        {

            g_png_upgrade[i].win_x = init_upgrade_1280bmp_res[i].x;
            g_png_upgrade[i].win_y = init_upgrade_1280bmp_res[i].y;
            sprintf(pngPath, "%s/%s", g_pPngDir, init_upgrade_1280bmp_res[i].file_name);
            xcb_avmlib_Load_Png(pngPath, g_png_upgrade[i]);
        }

        g_png_mcuupgrade = new png_texture_t[1];
        g_png_mcuupgrade[0].win_x = 0;
        g_png_mcuupgrade[0].win_y = 0;
        sprintf(pngPath, "%s/mcuupdate.png", g_pPngDir);
        xcb_avmlib_Load_Png(pngPath, g_png_mcuupgrade[0]);

        avmUIMCUHandleHandle.win_x = 592;
        avmUIMCUHandleHandle.win_y = 241;
        avmUIMCUHandleHandle.width = g_png_mcuupgrade[0].width;
        avmUIMCUHandleHandle.height = g_png_mcuupgrade[0].height;
        avmUIMCUHandleHandle.layers_num = 1; // 12个图层
        xcb_avmlib_Draw_Png_Init(&avmUIMCUHandleHandle);
        xcb_avmlib_Update_Png(&avmUIMCUHandleHandle, 0, g_png_mcuupgrade, 1);
        printf("mcu png info: %d %d %d %d\r\n", avmUIMCUHandleHandle.win_x,
               avmUIMCUHandleHandle.win_y,
               avmUIMCUHandleHandle.width,
               avmUIMCUHandleHandle.height);
        // avmUIMCUHandleHandle = 593, 241
    }

    // skin_init_flag = 1;

    printf("UI init!\n");
}

static void calib_update(void)
{
    printf("calib_update begin\r\n");
    // model created by calibration
    char pathModel3D[500], pathModel2D[500];
    char pathSingleModel[500];
    // default model which cannot be deleted or replaced!
    char pathModel3D_default[500], pathModel2D_default[500];
    char pathSingleModel_default[500];

    sprintf(pathModel2D, "%s/BowlModel_2D", g_pCreateModelDir);
    sprintf(pathModel3D, "%s/BowlModel_3D", g_pCreateModelDir);
    sprintf(pathSingleModel, "%s/SingleModel", g_pCreateModelDir);

    sprintf(pathModel2D_default, "%s/BowlModel_2D", g_pGivenModelDir);
    sprintf(pathModel3D_default, "%s/BowlModel_3D", g_pGivenModelDir);
    sprintf(pathSingleModel_default, "%s/SingleModel", g_pGivenModelDir);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
#if ISDMA == 1
    g_stitching_Handle.isDMA = true;
#else
    g_stitching_Handle.isDMA = false;
#endif
    ///////////////先释放模型相关资源

    xcb_avmlib_stitching_Dynamicmodel_Exit(&g_stitching_Handle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_stitching_Exit(&g_stitching_Handle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_stitching_Init(SAVE_LICENSE_PATH, &g_stitching_Handle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_stitching_Gen_VBO(pathModel2D, pathModel2D_default,
                                 &g_stitching_Handle, 0);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_stitching_Gen_VBO(pathModel3D, pathModel3D_default,
                                 &g_stitching_Handle, 1);
    printf("Stitching model reload!\n");

#if ISDMA == 1
    g_single_Handle.isDMA = true;
#else
    g_single_Handle.isDMA = false;
#endif

    xcb_avmlib_single_Init(&g_single_Handle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_singleView_Gen_VBO(pathSingleModel, pathSingleModel_default,
                                  &g_single_Handle);
#ifdef DYNAMIC_CAR_MODEL
    printf("Single model reload!\n");
    DynamicModelInit();
    printf("Dynamic model reload!\n");
#endif
    ///////////////////重新初始化车头、车身
    ///////////////先释放车头车身相关资源

#ifndef SYS_FOUR_LANE
#ifdef DYNAMIC_CAR_MODEL
    xcb_avmlib_bottomImg_Exit(&car_head_texture);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_bottomImg_Exit(&car_body_texture);
    png_texture_t car_head_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carhead.png", car_head_texture);
    png_texture_t car_body_texture;
    xcb_avmlib_Load_Png("/extp/app/png_yq/carbody.png", car_body_texture);

    ////////////////以下的尺寸重新读文件设置
    avm2DCarHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2; //-1250;//-1450;
    avm2DCarHandle.fCor_y = fInCarLength / 2 - fInAxis2carcenter + fHalfCarDeltaLenth;
    avm2DCarHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1450;
    avm2DCarHandle.fHeight = fInCarHeadLength + fHalfCarDeltaLenth * 2;
    xcb_avmlib_bottomImg_Init(&avm2DCarHandle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_Update_bottomImg(&avm2DCarHandle, &car_head_texture);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    avmBottomHandle.fCor_x = -(fInCarWidth + fHalfCarDeltaWidth * 2) / 2; //-1250;//-1550;
    avmBottomHandle.fCor_y = fInCarBodyLength - fInCarLength / 2 - fInAxis2carcenter + fHalfCarDeltaLenth;
    // 0 + 1600 + 100;
    avmBottomHandle.fWidth = (fInCarWidth + fHalfCarDeltaWidth * 2); // 2*1250;//2*1550;
    avmBottomHandle.fHeight = fInCarBodyLength + fHalfCarDeltaLenth * 2;
    xcb_avmlib_bottomImg_Init(&avmBottomHandle);
    printf("%s %s\r\n", __FUNCTION__, __LINE__);
    xcb_avmlib_Update_bottomImg(&avmBottomHandle, &car_body_texture);
    printf("calib_update end\r\n");
#endif
#endif
}

void avm_proc(void)
{
    system("fbinit");
#if 1
    render_init();//渲染
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test_start.tv_sec) + test1.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************渲染初始化后 = %lf(ms)\n", Time1 / 1000);
    avm_img_roi_init();//ROI以及各视图的位置大小参数
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test_start.tv_sec) + test1.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************ROI初始化后 = %lf(ms)\n", Time1 / 1000);
    printf("avm_img_roi_init end\n");
    avm_ui_init();//UI
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test_start.tv_sec) + test1.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************UI初始化后 = %lf(ms)\n", Time1 / 1000);


#if DVRONOFF
    ///////////////存储线程的开辟需要在时间素材读入之后
    pthread_create(&tidDvr0[0], NULL, DvrData0, NULL);//存储线程
#endif

    avm_ui_init_upgrad_calib();//升级、标定UI初始化
//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test_start.tv_sec) + test1.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************升级、标定UI初始化后 = %lf(ms)\n", Time1 / 1000);
    printf("ui init end\n");

#endif
    printf("dynamic model construction begin\r\n");
#ifdef DYNAMIC_CAR_MODEL
    sleep(1);
    DynamicModelInit();//动态模型
    sleep(1);
#endif
    printf("dynamic model construction end\r\n");

    while (1)
    {
        if (runRender)
        {
            //          printf("goto render_normal_loop \n");
            render_normal_loop();//循环渲染
            display_flag = 1;
            app_start_flag = 1;
            record_start_flag = 1;
        }

        if (!runRender)
        {

            if (g_port.m_tMsg_t7.emCalib == XCB_AVM_CALIB_CLOSED && autoCalibFlag)//自动标定
            {
                runRender = 1;
                record_start_flag = 0;
                display_flag = 0;
                app_start_flag = 0;
                g_port.m_tMsg_t7.emCalib = XCB_AVM_CALIB_OPENED;
                printf("recieved auto calib command\r\n");
            }
            else//手动标定
            {
                if (!qtRun)
                {
#ifndef SYS_FOUR_LANE
                    save_yuv_img(buf0->planes[0].mem, "LB");
                    save_yuv_img(buf1->planes[0].mem, "RB");
                    save_yuv_img(buf2->planes[0].mem, "B");
                    save_yuv_img(buf3->planes[0].mem, "F");
                    save_yuv_img(buf4->planes[0].mem, "R");
                    save_yuv_img(buf5->planes[0].mem, "L");
#else
                    save_yuv_img(buf0->planes[0].mem, "LB");
                    save_yuv_img(buf1->planes[0].mem, "RB");
                    save_yuv_img(buf2->planes[0].mem, "B");
                    save_yuv_img(buf3->planes[0].mem, "F");
#endif
                    printf("goto run QT---- \n");
                    irkey->stopgetIrKeyVal();//遥控器
                    qtRun = 1;
                    system("/extp/app/runqt.sh");
                }
                else
                {
                    ini.openFile("/extp/app/calib.ini", "r");
                    int gotoRender = ini.getInt("Flags", "gotoRender");
                    ini.closeFile();
                    if (gotoRender)
                    {
                        printf("getInt nKey == %d\n", gotoRender);
                        system("dd if=/dev/zero of=/dev/fb0 bs=1280 count=2880");
                        irkey->start2getIrKeyVal(cbGetIrKeyVal);
                        runRender = 1;
                        qtRun = 0;
                        //          calib_update();
                        g_port.send_beat_off();
                        system("reboot");
                    }
                    usleep(2);
                }
            }
        }
    }
}

/**************************************************************************************/
// char *get_exe_path(char *buf, int count) {
//   int i;
//   int rslt = readlink("/proc/self/exe", buf, count - 1);
//   if (rslt < 0 || (rslt >= count - 1)) {
//     return NULL;
//   }
//   buf[rslt] = '\0';
//   for (i = rslt; i >= 0; i--) {
//     if (buf[i] == '/') {
//       buf[i + 1] = '\0';
//       break;
//     }
//   }
//   return buf;
// }

void load_adas_fun_disp(void)
{
    upgrade_proc();
    upgrade_com_init();
    pthread_create(&g_thdCalib, NULL, avm_calib_proc, NULL);//自动标定线程
}

/********************************************************************************/
/********************************************************************************/
uchar mcu_updata_flag = 0;
uchar send_start_cmd = 0;
uchar upg_status_flag = 0;
int mcu_upg_count = 0;
int mcu_finsh_count = 0;
void mcu_updata_process()
{
    if (mcu_updata_flag == 1) // MCUÉý¼¶´¦Àí
    {
        int mcu_status = 0;
        mcu_upg_count++;
        if (mcu_upg_count > 10) // 100MS
        {
            mcu_upg_count = 0;
            mcu_status = g_port.get_mcu_is_rev_first_cmd();
            // printf("xcb mcu status = %d\n", mcu_status);
            if (mcu_status == 0)
            {
                g_port.start_mcu_upg_cmd();//启动MCU更新
                printf("mcu upate start\r\n");
                ///////////////////////////////show mcu update image///////////////////////
            }
            else if (mcu_status == 1)
            {
                if (upg_status_flag != 1)
                {
                    upg_status_flag = 1;
                }
            }
            else
            {
                mcu_updata_flag = 0;
                if (upg_status_flag != 2)
                {
                    upg_status_flag = 2;
                    mcu_finsh_count = 0;
                }
            }
        }
    }
    else
    {
        if (upg_status_flag == 2)
        {
            mcu_finsh_count++;
            if (mcu_finsh_count > 500)
            {
                mcu_finsh_count = 0;
                upg_status_flag = 0;
                g_port.reset_mcu_cmd();//重置MCU
                printf("g_port.reset_mcu_cmd();\r\n");
            }
        }
    }
}
/**************************************************************************************/
/**************************************************************************************/
static void repeate_record_disp(void)
{
    bool is_card_exist = 0;
    char card_path[100] = {0};
    long total_capacity = 0;

    // 0: 存在   -1: 不存在  非0即真
    if (!is_dir_exist("/extp/sdcard/mmcblk1p1"))
    {
        // printf("[SD MOUNT_DIR in /extp/sdcard/mmcblk1p1======\n");
        is_card_exist = 1;
        strcpy(card_path, "/extp/sdcard/mmcblk1p1");
    }
    // 0: 存在   -1: 不存在  非0即真
    else if (!is_dir_exist("/extp/sdcard/mmcblk1"))
    {
        // printf("[SD MOUNT_DIR in /extp/sdcard/mmcblk1======\n");
        is_card_exist = 1;
        strcpy(card_path, "/extp/sdcard/mmcblk1");
    }
    else
    {
        is_card_exist = 0;
    }

    if (is_card_exist == 1)
    {
        size_t total = 0;
        size_t user = 0;
        size_t free_space = 0;
        GetSdCardCurCapacity(card_path, &total, &user);//获取卡容量
        free_space = total - user;

        if (free_space < CARCD_FILE_DISP_NUM) //当前卡空间小于100M进行处理
        {
            Delet_Cur_Dir_Early_FileName(card_path, ".mp4");
        }
    }
}

void *otherRun(void *arg)
{
    int sd_detec_count = 0;

    int finsh_load_flag = 0;
    bool flag = false;
    int lcd_count = 0;
    int warn_count = 0;
    int sd_status = REMOVE;
    char mcu_name[256] = {0};

    while (1)
    {

        usleep(10000);
        // if(nMCUUpdateFlag)
        {
            if ((!is_file_exist(MOUNT_DIR1) || !is_file_exist(MOUNT_DIR2)) &&
                (!is_file_exist(APP_PACKET_DIR) || !is_file_exist(OS_PACKET_DIR) ||
                 !is_file_exist(MOUNT_DIR2 "/APP") || !is_file_exist(MOUNT_DIR2 "/OS") ||
                 !is_file_exist(MCU_UPDATE_NAME) || !is_file_exist(MCU_UPDATE_NAME1)))
            {
                if (sd_status == REMOVE)//检测SD卡
                {
                    sd_status = PLUG_IN;
                    g_port.send_beat_off();
                    bool bIsExit = false;
                    if (!is_file_exist(MCU_UPDATE_NAME))
                    {
                        if (1 == Get_file_name_is_specific_directory(MCU_UPDATE_NAME, "bin", mcu_name))//获取文件名的具体位置
                            bIsExit = true;

                        // int trlen = strlen(mcu_name);
                        // int nLowversion = (int)mcu_name[trlen - 5] - 48;
                        // int nHighversion = (int)mcu_name[trlen - 7] - 48;
                        // if(nLowversion == gmcul && nHighversion == gmcuh)
                        //   bIsExit = false;

                        // printf("mcu version: %d %d %d %d\n", nHighversion, nLowversion, gmcuh, gmcul);
                    }

                    if (!is_file_exist(MCU_UPDATE_NAME1))
                    {
                        if (1 == Get_file_name_is_specific_directory(MCU_UPDATE_NAME1, "bin", mcu_name))
                            bIsExit = true;

                        // int trlen = strlen(mcu_name);
                        // int nLowversion = (int)mcu_name[trlen - 5] - 48;
                        // int nHighversion = (int)mcu_name[trlen - 7] - 48;
                        // if(nLowversion == gmcul && nHighversion == gmcuh)
                        //   bIsExit = false;

                        // printf("mcu version: %d %d %d %d\n", nHighversion, nLowversion, gmcuh, gmcul);
                    }
                    if (bIsExit)
                    {
                        if (mcu_updata_flag == 0)
                        {
                            mcu_updata_flag = 1;
                            mcu_upg_count = 0;
                            send_start_cmd = 0;
                            g_port.set_mcu_upg_flag(1);
                            printf("g_port.set_mcu_upg_flag(1);\r\n");
                            StopRecord();
                            iDvrFlag = 0;
                        }
                    }
                    else if (!is_file_exist(copy_file_path))
                    {
                        char copy_cmd[50] = {"sh"};
                        strcat(copy_cmd, " ");
                        strcat(copy_cmd, copy_file_path);
                        system(copy_cmd);
                    }
                    // printf("\e[1;32m 0 SD plug in\e[0m \n");
                }
            }
            else
            {
                if (sd_status == PLUG_IN)
                {
                    sd_status = REMOVE;
                    g_port.send_beat_on();
                    if (mcu_updata_flag == 1)
                    {
                        mcu_updata_flag = 0;
                        mcu_upg_count = 0;
                        g_port.set_mcu_upg_flag(0);
                        upg_status_flag = 0;
                        printf("g_port.set_mcu_upg_flag(0);\r\n");
                    }
                    // printf("\e[1;32m 3 SD remove  \e[0m \n");
                    // iDvrFlag = 1;
                }
            }
        } // end nMCUUpdateFlag
        usleep(10000);
        mcu_updata_process();//MCU更新
        if (display_flag == 1)
        {
            if (adas_fun_flag == 0)
            {
                adas_fun_flag = 1;
                load_adas_fun_disp();//加载自动采集功能显示
            }
        }

#if DVRONOFF
        //  iDvrFlag = 1;//测试
        if (iDvrFlag)
        {
            if (!is_file_exist(MOUNT_DIR1) || !is_file_exist(MOUNT_DIR2)) //检测SD存在
            {
                if (TF_status == REMOVE) // record_start_flag = 0;
                {
                    TF_status = PLUG_IN;
                    DvrstartRecord(); //开始录像
                    record_start_flag = 1;
                    printf("\e[1;32m 1 SD plug in\e[0m \n");
                }
                else
                {
                    // sd_status == PLUG_IN;record_start_flag = 1;
                    sd_detec_count++;
                    if (sd_detec_count >= CARD_COUNT) // 100s检侧一次 20MS*5=100ms
                    {
                        sd_detec_count = 0;
                        repeate_record_disp(); //删除旧文件循环录像
                    }
                }
            }
            else
            {
                //检测SD移除
                if (TF_status == PLUG_IN)
                {
                    TF_status = REMOVE;
                    StopRecord();//停止录像
                    record_start_flag = 0;
                    printf("yhc  \e[1;32m 2 SD remove  \e[0m \n");
                }
            }
        }
#endif
    }
    pthread_exit(NULL);
}

/********************************************************************************/
static void readFifoMsg()
{
    mkfifo("/extp/app/xcbfifo", 0777);
    printf("open fifo ------------------------------\n");
    fifo_fd = open("/extp/app/xcbfifo", O_RDONLY, 0777);
    if ((fifo_fd = open("/extp/app/xcbfifo", O_RDONLY, 0777)) < 0)
    {
        perror("open");
        return;
    }
    if (fifo_fd < 0)
    {
        printf("open fifo faile\n");
    }
    else
    {
        int flag;
        flag = fcntl(fifo_fd, F_GETFL, 0);
        flag |= O_NONBLOCK;
        fcntl(fifo_fd, F_SETFL, flag);
    }
}
void My_Test(void)
{
    while(1)
    {
        printf("***********************************************%d\n",g_tBirdViewParams.win_width);
        sleep(2);

    }

}
int main(int argc, char *argv[])
{
    gettimeofday(&test_start, NULL);//获取APP启动时间
    system("mount -t debugfs none /sys/kernel/debug");
    if (access("/extp/app/6laneyuv", F_OK) != 0)
    {
        mkdir("/extp/app/6laneyuv", 0777);
    }
    irkey = new IrRemoteCtrl();
    irkey->start2getIrKeyVal(cbGetIrKeyVal);//遥控器
    system("cp /extp/app/calib_bak.ini /extp/app/calib.ini -vf");
    qtRun = 0;
    adas_fun_flag = 0;
    display_flag = 0;
    iDvrFlag = 0;
    nMCUUpdateFlag = 0;
    system("cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor");
    system("echo ondemand > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor");
    system("cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor");
    system("cat /sys/devices/virtual/thermal/thermal_zone0/trip_point_*_temp");

    get_exe_path(g_pAvmPath, PATH_MAX);//获取路径
    record_start_flag = 0;

    sprintf(g_pCalibConfigDir, "%scalibData", g_pAvmPath);
    sprintf(g_pCreateModelDir, "%sgpu_data", g_pAvmPath);
    sprintf(g_pGivenModelDir, "%sgpu_data_default", g_pAvmPath);
    sprintf(g_pPngDir, "%spng_yq", g_pAvmPath);

    memset(pImgForSave, 128, 1280 * 720 * 1.5);
    memset(pImgForSaveT, 128, 1280 * 720 * 1.5);
#if 1
    printf("[XT6lane_validation][%s][%d][%s][%s] *******\n", __func__, __LINE__, __DATE__, __TIME__);
    cpu_id_check();//检查cpu_id

    int ret = xcb_check_validation(SAVE_LICENSE_PATH);//检查LICENSE文件路径
    printf("cur encry type = %d\n", ret);
    if (ret == -1)
    {
        printf("[XT6lane]xcb start serial Encry Disp %s \n", UART_ENCTY);
        start_serial_encty_disp(UART_ENCTY);//启动显示加密串口
        while (1)
        {
            if (!is_file_exist(SAVE_LICENSE_PATH))
            {
                sleep(3);
                printf("[XT6lane_time][%s][%s] serial validation success\n", __DATE__, __TIME__);
                break;
            }
            sleep(1);
        }
    }
    else
    {
        printf("[XT6lane]serial validation Encry success \n");
    }
#endif
    for (int i = 0; i < 6; i++)
    {
        if (sem_init(&sema[i], 0, 0) != 0)
        {
            printf("sema[%d] is Error!\n", i);
        }
        if (sem_init(&sema1[i], 0, 0) != 0)
        {
            printf("sema1[%d] is Error!\n", i);
        }
    }

    for (int i = 0; i < 6; i++)
    {
        pStitchingIm[i] = (unsigned char *)malloc(1280 * 720 * 1.5);
    }

    ReadCarBasicInfo();//读取汽车基本信息(从ini文件读取参数)

    memset(pImgForSave, 128, 1280 * 720 * 1.5);
    memset(pImgForSaveT, 128, 1280 * 720 * 1.5);
    printf("[XT6lane][build_time][%s][%d][%s][%s] *******\n", __func__, __LINE__, __DATE__, __TIME__);

    init_eglimg_dmafd();
#ifndef SYS_FOUR_LANE
    open_device012345(0, 6);
#else
    open_device012345(0, 4);
#endif
#if DVRONOFF
    DvrInit1Lane(DVRTIME, 1280, 720, 1280, 720, 4, 25, 1); // 3min//DVR功能
#endif

    printf("[XT6lane_startSerialThd][%s][%d][%s][%s] *******\n", __func__, __LINE__, __DATE__, __TIME__);
    g_port.startSerialThd();//解析MCU数据
    avm_rectime_init();//时间素材读入
    pthread_create(&g_thdother, NULL, otherRun, NULL);//其他运行的线程

#if ISDMA == 1
    pthread_create(&g_thdCamera0, NULL, CameraDisplay0, NULL);
    usleep(10000);
    pthread_create(&g_thdCamera1, NULL, CameraDisplay1, NULL);
    usleep(10000);
    pthread_create(&g_thdCamera2, NULL, CameraDisplay2, NULL);
    usleep(10000);
    pthread_create(&g_thdCamera3, NULL, CameraDisplay3, NULL);
    usleep(10000);
#ifndef SYS_FOUR_LANE
    pthread_create(&g_thdCamera4, NULL, CameraDisplay4, NULL);
    usleep(10000);
    pthread_create(&g_thdCamera5, NULL, CameraDisplay5, NULL);
    usleep(10000);
#endif
#endif

    unsigned char uVerHigh, uVerLow;
    char pVerPath[100];
    sprintf(pVerPath, "%sapp_version", g_pAvmPath);
    read_version_file(pVerPath, &uVerHigh, &uVerLow);
    g_port.send_version_info(uVerHigh, uVerLow);
    gCpuVer1 = uVerHigh;
    gCpuVer2 = uVerLow;

    printf("[XT6lane][build_time][%s][%d][%s][%s] *******\n", __func__, __LINE__, __DATE__, __TIME__);

//    gettimeofday(&test1, NULL);
//    Time1 = 1000000 * (test1.tv_sec - test_start.tv_sec) + test1.tv_usec - test_start.tv_usec; //默认微秒为单位
//    printf("******************************主循环前 = %lf(ms)\n", Time1 / 1000);

#if PANLI_TEST
    pthread_t test;
    pthread_create(&test, NULL, My_Test, NULL);
    usleep(10000);
#endif
    avm_proc();//主线程


    printf("avm over!\n");
    pthread_join(g_thdother, NULL);
    pthread_join(g_thdCamera0, NULL);
    pthread_join(g_thdCamera1, NULL);
    pthread_join(g_thdCamera2, NULL);
    pthread_join(g_thdCamera3, NULL);
#ifndef SYS_FOUR_LANE
    pthread_join(g_thdCamera4, NULL);
    pthread_join(g_thdCamera5, NULL);
#endif

#if DVRONOFF
    pthread_join(tidDvr0[0], NULL);
#endif
    while (1)
    {
        sleep(1);
    }

    for (int i = 0; i < 6; i++)
    {
        free(pStitchingIm[i]);
    }

    pthread_mutex_destroy(&mutexin);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sema[0]);
    sem_destroy(&sema[1]);
    sem_destroy(&sema[2]);
    sem_destroy(&sema[3]);
    sem_destroy(&sema[4]);
    sem_destroy(&sema[5]);

    sem_destroy(&sema1[0]);
    sem_destroy(&sema1[1]);
    sem_destroy(&sema1[2]);
    sem_destroy(&sema1[3]);
    sem_destroy(&sema1[4]);
    sem_destroy(&sema1[5]);
    // ini.closeFile();
    // XCB_2DPlaneDestroy(pModel2dP);
    return 0;
}
