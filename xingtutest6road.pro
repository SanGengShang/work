TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    Ini.cpp \
    file_list.cpp \
    irremotectrl.cpp \
    serial.cpp \
    sfavm360.cpp

DISTFILES += \
    Makefile \
    releasenot.txt

HEADERS += \
    Ini.h \
    file_list.h \
    include/V4l2Camera/linux/v4l2-common.h \
    include/V4l2Camera/linux/v4l2-controls.h \
    include/V4l2Camera/linux/v4l2-dv-timings.h \
    include/V4l2Camera/linux/v4l2-mediabus.h \
    include/V4l2Camera/linux/v4l2-subdev.h \
    include/V4l2Camera/linux/videodev2.h \
    include/V4l2Camera/sunxi_camera_v2.h \
    include/device/isp_dev.h \
    include/device/video.h \
    include/dictionary.h \
    include/events.h \
    include/g2d_driver.h \
    include/g2d_hal.h \
    include/hal_debug.h \
    include/iniparser.h \
    include/isp.h \
    include/isp_3a_ae.h \
    include/isp_3a_af.h \
    include/isp_3a_afs.h \
    include/isp_3a_awb.h \
    include/isp_3a_md.h \
    include/isp_base.h \
    include/isp_cmd_intf.h \
    include/isp_comm.h \
    include/isp_debug.h \
    include/isp_ini_parse.h \
    include/isp_iso_config.h \
    include/isp_manage.h \
    include/isp_module_cfg.h \
    include/isp_pltm.h \
    include/isp_rolloff.h \
    include/isp_tone_mapping.h \
    include/isp_tuning.h \
    include/isp_tuning_priv.h \
    include/isp_type.h \
    include/isp_v4l2_helper.h \
    include/isp_version.h \
    include/list.h \
    include/media.h \
    include/tools.h \
    include/type_camera.h \
    include/video_hal.h \
    irremotectrl.h \
    paraset.h \
    serial.h \
    sfavm360.h \
    sys_config.h \
    xcb/Light_avg.h \
    xcb/XCB_360stitching.h \
    xcb/file_list.h \
    xcb/stitchingbasicdef.h \
    xcb/xcb360stitchingprocess.h \
    xcb/xcb_after_autocalib.h \
    xcb/xcb_auth_embed.h \
    xcb/xcb_avmlib.h \
    xcb/xcb_videoprot5.h \
    xcb_ui.h \
    xcb_after_autocalib.h
