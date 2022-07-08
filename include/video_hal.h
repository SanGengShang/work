/*************************************************************************
  > File Name: video_hal.h
  > Author:hirudo 
  > Mail: zhangyong@wissenstar.com
  > Created Time: 2018-04-27
 ************************************************************************/

#ifndef VIDEO_HAL_H_
#define VIDEO_HAL_H_

/*  Four-character-code (FOURCC) */
#define fmt_fourcc(a, b, c, d) ((unsigned int)(a) | ((unsigned int)(b) << 8) | ((unsigned int)(c) << 16) | ((unsigned int)(d) << 24))
#define WIS_FMT_NV21    fmt_fourcc('N', 'V', '2', '1') /* 12  Y/CrCb 4:2:0  */
#define WIS_FMT_UYVY    fmt_fourcc('U', 'Y', 'V', 'Y') /* 16  YUV 4:2:2     */
#define CAMERA_TYPE_NTSC	0
#define CAMERA_TYPE_PAL 	1
#define CAMERA_TYPE_NVP6124	2
#define CAMERA_TYPE_NVP6324	3
#define CAMERA_TYPE_MAX9286	4


enum enc_status_
{
    ENC_START,
    ENC_STOP,
    ENC_EXIT,
};

enum dec_status_
{
    DEC_PAUSE,
    DEC_PLAY,
    DEC_STOP,
    DEC_EXIT,
    DEC_ERROR,
};

enum cam_status_
{
    CAM_START,
    CAM_PENDING,
};

enum dvr_error_  //故障时的回调
{
    NO_ERROR,
    FILE_OPEN_ERROR,
    FILE_READ_ERROR,
    FILE_WRITE_ERROR,
    MEM_ALLOC_ERROR,
    MEM_FREE_ERROR,

    DEC_INIT_ERROR,
    DEC_DECODER_ERROR,
    DEC_DEINIT_ERROR,

    ENC_INIT_ERROR,
    ENC_RECORDER_ERROR,
    ENC_DEINIT_ERROR,
};

typedef enum video_complex_type_
{
    NORMAL_ONE,
    HORIZONTAL_TWO,
    VERTICAL_TWO,
    COMBINE_FOUR,
    HORIZONTAL_FOUR,
}video_complex_type_t;

typedef struct video_buf_
{
    int     phy_addr;  //physical address
    void   *vir_addr;  //virtual  address
    int     dma_fd;

    int     size;
    int     width;
    int     height;
    int     format;
    int     frame_cnt;
    int     channel;
    struct  timeval timestamp;
    video_complex_type_t complex;
}video_buf_t;

typedef struct decode_info_
{
    int seekset;
    int speed;
    int duration;
    int fps;
}decode_info_t;

enum enc_file_status_
{
    ENC_FILE_START,
    ENC_FILE_STOP,
};
typedef struct enc_file_info_
{
    char *file;
    enum enc_file_status_ status;
}enc_file_info_t;

typedef struct encode_info_
{
    int fps;
    int duration;
    int bitrate;
    int width;
    int height;
    enc_file_info_t new_cutfile;
}encode_info_t;

typedef struct camera_callback_
{
    video_buf_t video_buf;
    enum cam_status_ cam_status;
}cam_callback_t;

typedef struct decode_callback_
{
    video_buf_t video_buf;
    enum dec_status_ dec_status;
    enum dvr_error_ err; //故障时的回调
    decode_info_t info_data;
}dec_callback_t;

typedef struct encode_callback_
{
    enum enc_status_ enc_status;
    enum  dvr_error_ err; //故障时的回调
    encode_info_t info_data;
}enc_callback_t;

enum event_
{
    DECODER_BUFFER,
    RECORDER_BUFFER,
    CAM_BUFFER,
};
typedef struct wis_event_
{
    enum event_ e;
    void *buffer;
    int size;
}wis_event_t;

typedef int (*eventCallback)(wis_event_t *args);

#endif

