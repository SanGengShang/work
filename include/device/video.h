
#ifndef __VIDEO_H_
#define __VIDEO_H_

#include <stdbool.h>

#include "../V4l2Camera/sunxi_camera_v2.h"

#define HW_VIDEO_DEVICE_NUM 6

#if 1
struct video_plane {
	unsigned int size;
	int dma_fd;
	void *mem;
	unsigned int  mem_phy;
};

struct video_buffer {
	unsigned int index;
	unsigned int bytesused;
	unsigned int frame_cnt;
	unsigned int exp_time;
	struct timeval timestamp;
	bool error;
	bool allocated;
	unsigned int nplanes;
	struct video_plane *planes;
};

struct buffers_pool {
	unsigned int nbufs;
	struct video_buffer *buffers;
};
#endif

struct media_dev {
	struct isp_video_device *video_dev[4];
};

struct video_fmt {
	enum v4l2_buf_type type;
	enum v4l2_memory memtype;
	struct v4l2_pix_format_mplane format;
	unsigned int nbufs;
	unsigned int nplanes;
	unsigned int fps;
	unsigned int capturemode;
	unsigned int use_current_win;
	unsigned int wdr_mode;
};

struct osd_fmt {
	int clipcount;		/* number of clips */
	int chromakey;
	int global_alpha;
	int reverse_close[64];	/*osd reverse close, 1:close, 0:open*/
	int rgb_cover[8];
	int glb_alpha[64];
	void *bitmap[64];
	struct v4l2_rect region[64];	/* overlay or cover win */
};

struct video_event {
	unsigned int event_id;
	unsigned int event_type;
	unsigned int frame_cnt;
	struct timespec vsync_ts;
};

struct isp_video_device {
	unsigned int id;
	unsigned int isp_id;
	struct media_entity *entity;

	enum v4l2_buf_type type;
	enum v4l2_memory memtype;

	struct v4l2_pix_format_mplane format;

	unsigned int nbufs;
	unsigned int nplanes;
	struct buffers_pool *pool;
	unsigned int fps;

	void *priv;
};


#endif /* __VIDEO_H_ */
