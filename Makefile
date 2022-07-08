################################################################################
#
# Makefile project only supported on Mac OS X and Linux Platforms)
# Tailor to simple project , adjust it by xcb
################################################################################

include ../makefile_cfg
	
################################################################################

LICHEE_DIR =/home/xcb/t5-src/lichee
LICHEE_BR_OUT =$(LICHEE_DIR)/out/t507/demo2.0/longan/buildroot
SDK_PATH =$(LICHEE_DIR)/platform/framework/auto/sdk_lib

CROSS_COMPILE := $(LICHEE_DIR)/out/t507/demo2.0/longan/buildroot/host/bin/aarch64-linux-gnu-
CC := $(CROSS_COMPILE)gcc
CPP := $(CROSS_COMPILE)g++
################################################################################

DEFINES += -DUSE_LOGCAT
LOGCAT = logcat

CEDAR_LINK_NEW = -L$(SDK_PATH)/cedarx/lib/ -Wl,--no-undefined -ladecoder -laencoder -lcdx_base -lcdx_common \
	-lcdx_muxer -lcdx_parser -lcdx_playback -lcdx_stream -lMemAdapter -lcdc_base -lsubdecoder -lvdecoder \
	-lvencoder -lVE -lvideoengine -lxmetadataretriever -lxplayer -lvenc_base -lvenc_codec -lcdx_ion	
	
CEDAR_MACORS = -D_GNU_SOURCE -DCONFIG_CHIP=7 -DCONFIG_TARGET_PRODUCT= -DCONFIG_PRODUCT=2 \
				-DCONFIG_VE_IPC=1 -DROTATE_PIC_HW=0 -DVE_PHY_OFFSET=0x40000000 -DCONFIG_LOG_LEVEL=0 

#DMS_LIB = -L$(PWD)/../../dms/allwinner/lib -ldms_sdk_protobuf_parser -lhobot_dms -lhobotdms_hal -lupload_config -lstorage_manager -lhobotdms_sdk -ljpeg \
#	 	-lopencv_world -lturbojpeg -lv4l2 -lx264 -lxvidcore
#LIBS = $(DMS_LIB)
 
LIBS = -L$(SDK_LIBS) -lsdk_compose -lsdk_disp -lsdk_g2d -lsdk_dvr -lsdk_player -lsdk_log -lsdk_camera \
	-lsdk_memory -lsdk_sound -ltinyalsa -lsdk_storage -lsdk_audenc -lsdk_cfg -lsdk_ctrl -lsdk_egl -lsdk_misc 

LIBS += -Lxcb  -lnavi -lxcb_videoprot5 -llight_avg -lcalib -lauth
LIBS += -L$(LICHEE_DIR)/out/t507/demo2.0/longan/buildroot/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib \
 	-L$(LICHEE_BR_OUT)/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib -ljpeg \
	-lm -lGLESv2 -lEGL -lpng -lrt -lpthread -lz -lcrypto -lssl -fPIC -fpermissive -std=c++11 -ffast-math
LIBS +=  $(LICHEE_BR_OUT)/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib/libmali.so


ANDROID_UTILS_DEF = -DHAVE_PTHREADS -DHAVE_SYS_UIO_H -DANDROID_SMP=1 -D__ARM_HAVE_DMB -D__ARM_HAVE_LDREX_STREX -DHAVE_POSIX_CLOCKS -DHAVE_PRCTL
SDKLIB_DEF = -DWATERMARK -DCDX_V27 -DSUPPORT_NEW_DRIVER
DEFINES += $(ANDROID_UTILS_DEF) $(SDKLIB_DEF)

################################################################################

INC =  	-I./ \
	-I./include -I./include/V4l2Camera -I./include/device \
	-I./xcb  \
	-I$(SDK_PATH)/include \
	-I$(SDK_PATH)/include/opengl/include \
	-I$(SDK_PATH)/include/opengl/include/khronos/original/ \
	-I$(SDK_PATH)/include/disp2 \
	-I$(SDK_PATH)/include/utils \
	-I$(SDK_PATH)/include/cutils/ \
	-I$(SDK_PATH)/include/media \
	-I$(SDK_PATH)/include/camera \
	-I$(SDK_PATH)/sdk_camera/moudle \
	-I$(SDK_PATH)/sdk_camera \
	-I$(SDK_PATH)/include/storage/ \
	-I$(SDK_PATH)/include/sound/ \
	-I$(SDK_PATH)/include/memory/ \
	-I$(SDK_PATH)/sdk_misc/ \
	-I$(SDK_PATH)/include/audioenc/ \
	-I$(LICHEE_DIR)/out/t507/demo2.0/longan/buildroot/host/aarch64-buildroot-linux-gnu/sysroot/usr/include

################################################################################

SUBDIRS = $(shell find . -maxdepth 3 -type d)

CSRCS =  $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c)) 
CPPSRCS = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))
COBJS := $(CSRCS:.c=.o)
CPPOBJS := $(CPPSRCS:.cpp=.o)
################################################################################


BIN = sfavm360-xingtu
################################################################################

all: $(BIN)
$(COBJS) : %.o: %.c
	$(CC) -c $< -o $@ $(INC) $(CFLAGS)

$(CPPOBJS) : %.o: %.cpp
	$(CPP) -c -lpthread $< -o $@ $(DEFINES) $(INC) $(CPPFLAGS) $(CEDAR_MACORS) $(DEFINES) -DSDKLIB_TEST  -fpermissive

$(BIN) : $(COBJS) $(CPPOBJS) 
	$(CPP) -o $(BIN) -lpthread $(CPPOBJS) $(INC) $(LDFLAGS) $(LIBS) $(CEDAR_LINK_NEW) -fpermissive

clean:
	 find . -name "*.o" -exec rm -rf {} \;
	-$(RM) *.o
	-$(RM) $(BIN)

