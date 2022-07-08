#ifndef __AUTOCALIB_H__
#define __AUTOCALIB_H__

/** Errors that calibration module return*/
typedef enum xcb_after_calib_error_s {
  XCB_AFTER_CALIB_NO_ERROR = 0, /** No error*/
  XCB_AFTER_CALIB_LEFT_FAIL =
      1, /** Fail to get needed corners from left-front side*/
  XCB_AFTER_CALIB_RIGHT_FAIL =
      2, /** Fail to get needed corners from right-front side*/
  XCB_AFTER_CALIB_BACK_FAIL = 3, /** Fail to get needed corners from back side*/
  XCB_AFTER_CALIB_FRONT_FAIL =
      4, /** Fail to get needed corners from front side*/
  XCB_AFTER_CALIB_LEFT_BACK_FAIL =
      5, /** Fail to get needed corners from left-back side*/
  XCB_AFTER_CALIB_RIGHT_BACK_FAIL =
      6, /** Fail to get needed corners from right-back side*/
  XCB_AFTER_CALIB_GET_FRAME_FAIL = 7,     /** Fail to get frame of camera*/
  XCB_AFTER_CALIB_LOAD_CONFIG_FAIL = 8,   /** Fail to load configuration file*/
  XCB_AFTER_CALIB_MATCH_CPUID_FAIL = 9,   /** Fail to match cpu ID*/
  XCB_AFTER_CALIB_CONFIG_PATH_EMPTY = 10, /** Empty path of configuration file*/
  XCB_AFTER_CALIB_IMAGE_FORMAT_UNSUPPORT = 11 /** Unsupported image format*/
} xcb_after_calib_error_t;

/** Image format*/
typedef enum xcb_after_calib_image_format_s {
  XCB_CALIB_IMAGE_FORMAT_NV61 = 0, /** NV61 - YUV422_YYYY_VUVU*/
  XCB_CALIB_IMAGE_FORMAT_NV21 = 1  /** NV21 - YUV420_YYYY_VUVU*/
} xcb_after_calib_image_format_t;

/** Process calibration (TODO: If "debug mode" is set, folder "output_calib"
 *  will be created to save debug files)
 *
 *  @param [in] pConfigPath: Path of configration file, 2d bowl model and 3d
 *							 bowl model.
 *  @param [out] pAVMModelPath: Path of model files which will be used for AVM.
 * 	If "pAVMModelPath" is "NULL", "gpu_data" would be created to save model
 *  files.
 *  @param [in] pImg_back: Camera frame data of back side
 *  @param [in] pImg_left: Camera frame data of left-front side
 *  @param [in] pImg_front: Camera frame data of front side
 *  @param [in] pImg_right: Camera frame data of right-front side
 *  @param [in] pImg_leftBack: Camera frame data of left-back side
 *  @param [in] pImg_rightBack: Camera frame data of right-back side
 *  @param [in] img_format: Format of above four camera frames
 *	@param [in] width: Width of input camera frame
 *  @param [in] height: Height of input camera frame
 *
 *  @return Errors
 */
xcb_after_calib_error_t xcb_after_autocalib(
    const char *pAuthFilePath, const char *pConfigPath,
    const char *pAVMModelPath, const unsigned char *pImg_back,
    const unsigned char *pImg_left, const unsigned char *pImg_front,
    const unsigned char *pImg_right, const unsigned char *pImg_leftBack,
    const unsigned char *pImg_rightBack,
    xcb_after_calib_image_format_t img_format, int width, int height);

int xcb_check_validation(const char *pAuthFilePath);

#endif // __AUTOCALIB_H__
