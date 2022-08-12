/*3D avm sdk
version:1.1
create time:2019.11.24
modify time:
author:kangli shi
function:
camera data :DMA(Direct Memory Access)
1.2d bird view  //Note:unit :millimeter
2.3d surround view //Note:unit :millimeter
3.single view
4.ui //Note:unit :pixle
*/
#ifndef XCB_AVMLIB_H_
#define XCB_AVMLIB_H_
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>
#define EGL_EGLEXT_PROTOTYPES
#include <EGL/eglext.h>

#include <fbdev_window.h>

//#include "xcb360stitchingprocess.h"
////////define struct

//////////////added by zliu 20200315 for adjusting virtual camera in real time
struct VirCamPara
{
  float fCamPosition[3];
  float fCamFrontDirect[3];
  float fCamUpDirect[3];
  float fCamZoom;
  float fCamfocal;
  int nOutputImgH;
  int nOutputImgW;
};
typedef struct _SideViewVirCamPara
{
  ////////////////////在原始1920*1280的图像上，截取的区域，越大则视野范围越大
  int nStartPixelX[2];
  int nEndPixelX[2];
  int nStartPixelY[2];
  int nEndPixelY[2];

  /////////////////////////虚拟视角相关参数，注意左右sideview是对称的，给的是左侧的虚拟视角参数
  float fCamPosition[3];
  float fCamFrontDirect[3];
  float fCamUpDirect[3];
  float fCamZoom;
  float fCamfocal;

  int nOutputImgH;
  int nOutputImgW;

  ///////////////////////警示线的物理位置
  float fLine[8];
} SideViewVirCamPara;
///////////////////////////////////////////////////////////////////
typedef struct
{
  int id;
  int vertex_number;   // 模型顶点数
  int tri_face_number; //三角片元总数
  GLuint *buffers;     // vertex buffer
                       // id（顶点坐标+4*纹理坐标+4*混合权重+三角片元索引）
  int verTexGroupNum[6];
  int tri_face_groupNum[6];
  GLuint *texture_id;
  void *pData; // added by zliu for update virtual camera 20200315
} Patch;

///////define camera state ,  默认状态下是工作状态
typedef struct
{
  bool camera_state[6] = {true, true, true, true, true, true};
} avm_camera_type_t;

typedef struct
{
  Patch *stitching;
  GLuint prog;        // shader program
  int mode = 0;       // 0:unfold,1:fold
  bool isDMA = false; // false:not support DMA;true:support DMA;
  void *modelsSix[2];
  void *pDynamicLeftModel;
  void *pDynamicRightModel;
  void *pDynamicFrontModel;
} avm_stitching_type_t;
typedef struct
{
  int type;              //设置2d或3d的类型：type=0，表示2d，type=1，表示3d
  int id;                //设置更新纹理的纹理索引值，注意：type=1时，id=0，车身纹理；id=1，引擎内部纹理，id=2，内饰纹理，id=3，前车门纹理，id=4，车后门纹理，id=5，轮胎纹理
  int width;             //设置纹理图片的宽
  int height;            //设置纹理图片的高
  int channels;          //设置纹理图片的通道数
  unsigned char *pixels; //指针指向纹理图片的像素
} avm_car_skin_type_t;

typedef struct
{
  int iCarWidth;        // 2d car's width
  int iCarLength;       // 2d car's length
  int iBlindAreaWidth;  //设置盲区区域的宽
  int iBlindAreaLength; //设置盲区区域的长
  Patch *car_2d;
  Patch *car_3d;
  Patch *car_bottom;
  int texture_width;    // bottom texture's width
  int texture_height;   // bottom texture's height
  int texture_channels; //底部纹理的通道数
  unsigned char *pixel; //指针指向存储底部纹理的像素地址
  GLuint prog;          // shader program
} avm_car_type_t;
/*define car state struct
@param wheel_axis input:4(0~3):left front wheel,right front wheel,left rear
wheel,right rear wheel,3(0~2):coordinate x,y,z
@param roll input:the default wheel is not rolling
@param swing input:the default wheel is not swinging
@param roll_angle input:rolling angle
@param swing_angle input:swing angle
@param isOpen input:(0~5):left front door,right front door,left rear door,right
rear door,front hood,trunk(Note:default state is closed,which opened equals
true,closed equals false)
@param open_angle intput:(0~5):left front door,right front door,left rear
door,right rear door,front hood,trunk(Note:default angle is 0)
@param mvp_wheel_matrix output:wheel matrix
@param mvp_open_matrix output:door matrix
*/
typedef struct
{
  float wheel_axis[4][3];
  bool roll = false;
  bool swing = false;
  float roll_angle = 0;
  float swing_angle = 0;
  float open_axis[4][3];
  bool isOpen[4] = {false, false, false, false};
  float open_angle[4] = {0};
  float mvp_wheel_matrix[4][16];
  float mvp_open_matrix[4][16];
} avm_car_state_type_t;
typedef struct
{
  int x;
  int y;
  int w;
  int h;
} ROIParameter;

typedef struct
{
  Patch *singleview;
  Patch *sideView;
  Patch *lineView;
  Patch *wideAngleView;
  GLuint buffer;
  GLuint prog;                // shader program
  GLuint prog_line;           // line shader program
  GLuint prog_line_textsmall; // shader program
  ROIParameter ROIPara;
  bool isDMA = false; // false:not support DMA;true:support DMA;
  /*
  view=1~10:
  1:original front image
  2:original back image
  3:left front tyre image
  4:right front tyre image
  5:undistorted back image
  6:undistorted left image
  7:undistorted front image
  8:undistorted right image
  9:original left image
  10:original right image
  */
  int view;

} avm_single_type_t;

typedef struct
{
  int win_x;            // view port: starting position x coordinate ,Note:on the screen
                        // left bottom,unit :pixel
  int win_y;            // view port: starting position y coordinate ,Note:on the screen
                        // left bottom,unit :pixel
  int win_width;        // view port: width,Note:unit pixel
  int win_height;       // view port: height,NOte:unit pixel
  float ex, ey, ez;     // the camera's  position (x,y,z)
  float tx, ty, tz;     // the camera's direction target position(x,y,z)
  float fov;            // filed of view
  float aspect_ratio;   // perspective aspect_ratio=width/height
  float mvp_Matrix[16]; // mvp_Matrix= model matrix * view matrix * perspective
                        // matrix
} ViewParameter;
/*init shader interface
const char *v_shader:vertex shader
const char *f_shader:fragmment shader
*/
GLuint xcb_avmlib_Shader_Init(const char *v_shader, const char *f_shader);
// delete shader program
void xcb_avmlib_Shader_Delete(GLuint program);
/*unuse DMA texture
 */
void xcb_avmlib_Gen_TextureBuffer(int cnt, GLuint *bufferId);
/*use DMA texture
int cnt:the number of texture buffer id;
GLuint *bufferId//store texture buffer id
*/
void xcb_avmlib_Gen_TextureBuffer_DMA(int cnt, GLuint *bufferId);
void xcb_avmlib_Use(GLuint program);                                // use shader program
void xcb_avmlib_Unuse(GLuint program);                              // unuse shader program
void xcb_avmlib_Clean_Screen();                                     // clean screen
void xcb_avmlib_Clean_FboScreen();                                  // clean screen
void xcb_avmlib_2d_Set_ViewParameter(ViewParameter &viewParameter); //
void xcb_avmlib_2d_Set_ViewParameter(ViewParameter &viewParameter, float angle);
void xcb_avmlib_2d_Set_ViewParameter(ViewParameter &viewParameter, float upx, float upy, float upz);

void xcb_avmlib_3d_Set_ViewParameter(
    ViewParameter &viewParameter, avm_car_state_type_t *avm_car_state_type); //
//////////////
/*xcb_avmlib_Window_Init interface
int Width:screen's width
int Height:screen's height
EGLDisplay &egl_display:display id or handle on system
EGLSurface &egl_surface:system window or frame buffer handle
return value: -1:fail,0:succeed
*/
int xcb_avmlib_Window_Init(int Width, int Height, EGLDisplay &egl_display,
                           EGLSurface &egl_surface);
/*display picture interface
EGLDisplay &egl_display:display id or handle on system
EGLSurface &egl_surface:system window or frame buffer handle
*/
void xcb_avmlib_swapbuffer(EGLDisplay &egl_display, EGLSurface &egl_surface);
/////////////////stitching////////////////
/*initialize stitching variable
return value: -1:fail,0:succeed
*/
int xcb_avmlib_stitching_Init(const char *auth_file,
                              avm_stitching_type_t *avm_stitching_type);
int xcb_avmlib_stitching_Exit(avm_stitching_type_t *avm_stitching_type);
/*
const char *filename,:the file's name of stitching
const char *default_filename:the default file's name of stitching(
    if "filename" cannot access,"default_filename" will be used )
avm_stitching_type_t *avm_stitching:stitching variable
int type:type=0,stitching 2d; type=1,stitching 3d;

return value: -1:fail,0:succeed
*/
int xcb_avmlib_stitching_Gen_VBO(const char *filename,
                                 const char *default_filename,
                                 avm_stitching_type_t *avm_stitching_type,
                                 int type);
int xcb_avmlib_stitching_Gen_VBO_SixCarmera(
    const char *filename, const char *default_filename,
    avm_stitching_type_t *avm_stitching_type, int type);
/////////////////////获取盲区的尺寸，给出尺寸为物理尺寸且为盲区半宽半高。3D盲区和2D俯视盲区大小可能为不同值//////////////////////////
int xcb_avmlib_stitching_ObtainBlindArea(
    float &fHalfW, float &fHalfH, avm_stitching_type_t *avm_stitching_type,
    int type);

/*
avm_stitching_type_t *avm_stitching: stitching variable
ViewParameters viewParameters:view port and camera parameters variable
GLuint *texture_id:texture buffer id.Note:single view and stitching all use the
texture buffer id int type:type=0,stitching 2d; type=1,stitching 3d; Note:
type=0,viewParameters set 2d
type=1,viewParameters set 3d
*/
void xcb_avmlib_stitching_TopView_Render(
    avm_stitching_type_t *avm_stitching_type, ViewParameter &viewParameter,
    avm_camera_type_t *camera, GLuint *g_texture_id, int index,
    float *fGreyRatio1);

void xcb_avmlib_stitching_TopView_Render_SixCamera(
    avm_stitching_type_t *avm_stitching_type, ViewParameter &viewParameter,
    avm_camera_type_t *camera, GLuint *g_texture_id, int index,
    float *fGreyRatio1);

///////////////for dynamic stitching model render/////////////////////
int xcb_avmlib_stitching_Dynamicmodel_Init(const char *filename,
                                           avm_stitching_type_t *avm_stitching_type);

int xcb_avmlib_stitching_Dynamicmodel_Exit(avm_stitching_type_t *avm_stitching_type);

////////index: index of camera; vertexnum: 某路面片顶点数量
float *xcb_avmlib_stitching_ObtainVertexCorSixRoad(
    avm_stitching_type_t *avm_stitching_type,
    int index, int &vertexnum, int type);

void xcb_avmlib_stitching_UpdateTexcoord_SixRoad(
    avm_stitching_type_t *avm_stitching_type, int index,
    float *fposition, int nvertexnum, int type);

void xcb_avmlib_stitching_ObtainUpdatedWeightandCordinates_SixRoad(avm_stitching_type_t *avm_stitching_type, int index,
                                                                   float *fOrigPos, float *fposition, int nvertexnum, float *fwei, float *fCor, int type);

void xcb_avmlib_stitching_UpdateCoord_Weight_SixRoad(avm_stitching_type_t *avm_stitching_type, int index,
                                                     float *fwei, float *fCor, int nvertexnum, int type);

////////////angle: -90 --------- 90
int xcb_avmlib_stithing_Dynamicmodel_update(avm_stitching_type_t *avm_stitching_type, int angle);

///////////car//////////////
/*initialize car variable
return value: -1:fail,0:succeed
*/
int xcb_avmlib_car_Init(avm_car_type_t *avm_car_type);
int xcb_avmlib_car_Exit(avm_car_type_t *avm_car_type);
/*

avm_car_type_t *avm_car_type:car variable
int type:type=0,car 2d; type=1,car 3d;type=2,car bottom texture;

return value: -1:fail,0:succeed
*/
int xcb_avmlib_car_Gen_VBO(const char *filename, avm_car_type_t *avm_car_type,
                           int type);

int xcb_avmlib_car_UpdateModel(const char *filename,
                               avm_car_type_t *avm_car_type, int type);

/*Render interface
 */
void xcb_avmlib_car_Render_2d(avm_car_type_t *avm_car_type,
                              ViewParameter &viewParameter);
void xcb_avmlib_car_Render_3d(avm_car_type_t *avm_car_type,
                              ViewParameter &viewParameter,
                              avm_car_state_type_t *avm_car_state_type);
void xcb_avmlib_car_Render_bottom(avm_car_type_t *avm_car_type,
                                  ViewParameter &viewParameter);
//////////////single view ///////////////////
int xcb_avmlib_single_Init(avm_single_type_t *avm_single_type);
int xcb_avmlib_single_Exit(avm_single_type_t *avm_single_type);
/*

avm_single_view_type_t *avm_single_view:singleview variable
return value: -1:fail,0:succeed
*/
int xcb_avmlib_singleView_Gen_VBO(const char *filename,
                                  const char *default_filename,
                                  avm_single_type_t *avm_single_type);
/*
int type:type=0:sideview front,type=1:sideview rear ,type=2: sideview up
*/
int xcb_avmlib_sideView_Gen_VBO(const char *filename,
                                const char *default_filename,
                                avm_single_type_t *avm_single_type, int type);
int xcb_avmlib_wideAngleView_Gen_VBO(const char *filename,
                                     const char *default_filename,
                                     avm_single_type_t *avm_single_type);

////////////////////added by zliu 20200315 for adjusting virtual camera in real
/// time
int xcb_avmlib_singleView_UpdateVirtualCamera(
    struct VirCamPara *pVCInfor, int nCameraIndex,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_singleView_SavingVirtualPara(const char *filename,
                                            avm_single_type_t *avm_single_type);
int xcb_avmlib_singleView_ObtainVirtualCamera(
    struct VirCamPara *pVCInfor, int nCameraIndex,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_singleView_ResetVirtualCamera(
    int nCameraIndex, avm_single_type_t *avm_single_type);

int xcb_avmlib_sideView_UpdateVirtualCamera(
    struct _SideViewVirCamPara *pVCInfor, int nType, float *fInputLine,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_sideView_SavingVirtualPara(const char *filename,
                                          avm_single_type_t *avm_single_type,
                                          int type);
int xcb_avmlib_sideView_ObtainVirtualCamera(
    struct _SideViewVirCamPara *pVCInfor, int nType,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_sideView_ResetVirtualCamera(int nType,
                                           avm_single_type_t *avm_single_type);

int xcb_avmlib_wideAngleView_UpdateVirtualCamera(
    struct VirCamPara *pVCInfor, int nCameraIndex,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_wideAngleView_SavingVirtualPara(
    const char *filename, avm_single_type_t *avm_single_type);
int xcb_avmlib_wideAngleView_ObtainVirtualCamera(
    struct VirCamPara *pVCInfor, int nCameraIndex,
    avm_single_type_t *avm_single_type);
int xcb_avmlib_wideAngleView_ResetVirtualCamera(
    int nCameraIndex, avm_single_type_t *avm_single_type);
////////////////////////////////////////////////////////////////////////////////////////

/*Render single view
Note:use the same texture buffer id with stitching texture_id
*/
int xcb_avmlib_singleView_Render(avm_single_type_t *avm_single_type,
                                 ViewParameter &viewParameter,
                                 avm_camera_type_t *camera,
                                 GLuint *g_texture_id);
int xcb_avmlib_lineDynamicView_Render(avm_single_type_t *avm_single_type,
                                      ViewParameter &viewParameter,
                                      avm_camera_type_t *camera,
                                      GLuint *g_texture_id);
/*Render side view
int type:type=0:sideview front,type=1:sideview rear ,type=2: sideview up
Note:use the same texture buffer id with stitching texture_id
bool left=true,显示轮胎左侧
bool right=true，显示轮胎右侧
*/
int xcb_avmlib_sideView_Render(avm_single_type_t *avm_single_type,
                               ViewParameter &viewParameter,
                               avm_camera_type_t *camera, GLuint *g_texture_id,
                               int type, bool left = true, bool right = true);
/*Render wideangle view
int type:type=0:wideangle view front,type=1:wideangle view rear
Note:use the same texture buffer id with stitching texture_id
*/
int xcb_avmlib_wideAngleView_Render(avm_single_type_t *avm_single_type,
                                    ViewParameter &viewParameter,
                                    avm_camera_type_t *camera,
                                    GLuint *g_texture_id, int type);

void xcb_avmlib_Update_TextureBuffer(const unsigned char *pixelRear,
                                     const unsigned char *pixelLeft,
                                     const unsigned char *pixelFront,
                                     const unsigned char *pixelRight,
                                     GLuint *g_texture_id);

/*
pixel_index：
0:rear camera,
1:left camera,
2:front camera,
3:right camera
*/
void xcb_avmlib_Update_TextureBuffer_Single(const unsigned char *pixel,
                                            int pixel_index,
                                            GLuint *g_texture_id);

////////////////////////////////////////////Drawpng/////////////////////
typedef struct
{
  int win_x;               // the x coordate display on the window
  int win_y;               // the y coordate display on the window
  int width;               // the image's width
  int height;              // the image's height
  GLenum format = GL_RGBA; // default format
  unsigned char *texture;
} png_texture_t;
typedef struct
{
  int win_x;      // 窗口显示起始位置X坐标
  int win_y;      //窗口显示起始位置Y坐标
  int width;      //窗口显示的宽
  int height;     //窗口显示的高
  int layers_num; //设置图层的数量
  Patch *layers;  ////初始化开辟空间，退出释放
  GLuint prog;    // shader program
} avm_ui_type_t;
int xcb_avmlib_Load_Png(const char *filename, png_texture_t &png);
int xcb_avmlib_Draw_Png_Init(avm_ui_type_t *avm_ui_type);
int xcb_avmlib_Draw_Png_Exit(avm_ui_type_t *avm_ui_type);
/*
avm_ui_type_t *avm_ui_type//指针指向ui
int layer_index//每个avm_ui_type_t对应的图层的索引
*/
////layer_index:0:后;////
int xcb_avmlib_Draw_Png(avm_ui_type_t *avm_ui_type, int layer_index);
/*
avm_ui_type_t *avm_ui_type////指针指向ui
int layer_index//每个avm_ui_type_t对应的图层的索引
png_texture_t *png_texture//每个图层更新的图片
int png_num//每个图层更新图片的数量

*/
int xcb_avmlib_Update_Png(avm_ui_type_t *avm_ui_type, int layer_index,
                          png_texture_t *png_texture, int png_num);
void xcb_avmlib_Update_TextureBuffer_Line(png_texture_t *png_texture,
                                          GLuint *g_texture_id, int iCnt);

////////////////////////draw bottom image,
////////////////////////the difference betweent it with the ui image
////////////////////////is that the coordinates of the bottom image is world coordinates, not pixel of screen
typedef struct
{
  float fPointPosition[5][3]; //图片显示位置顶点物理坐标
  float fCor_x;
  float fCor_y;
  float fWidth;
  float fHeight;
  Patch *layers; ////初始化开辟空间，退出释放
  GLuint prog;   // shader program
} avm_bottomImg_type_t;

int xcb_avmlib_bottomImg_Init(avm_bottomImg_type_t *avm_bottomImg_type);
int xcb_avmlib_bottomImg_Exit(avm_bottomImg_type_t *avm_bottomImg_type);
int xcb_avmlib_Update_bottomImgPosition(avm_bottomImg_type_t *avm_bottomImg_type, float *fPosition);
int xcb_avmlib_Update_bottomImg(avm_bottomImg_type_t *avm_bottomImg_type, png_texture_t *png_texture);
int xcb_avmlib_Draw_bottomImg(avm_bottomImg_type_t *avm_bottomImg_type, ViewParameter &viewParameter);

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////full screen//////////////////
typedef struct
{
  int win_x;
  int win_y;
  int width;
  int height;
  Patch *screen;
  GLuint prog;
  bool isDMA = false;
} avm_screen_type_t;
int xcb_avmlib_screen_Init(const char *auth_file, avm_screen_type_t *avm_screen_type);
/*
type=1,
*/
int xcb_avmlib_screen_Gen_VBO(avm_screen_type_t *avm_screen_type);
/*
subImgROI: 窗口对应画面的子区域。subImgROI.x, subImgROI.y 对应的画面左上角起始点坐标，w,h对应原图的宽高
nImgWidth, nImgHeight：图像宽高
GLuint *g_textureid: 视频数据句柄列表
index: 对应显示的图像标记。0, back; 1, left front; 2, front; 3, right front; 4, left back; 5, right back
nFlip: 是否进行左右翻转。0, 画面不做左右翻转；1, 画面进行左右翻转
*/
int xcb_avmlib_full_screen_Render(avm_screen_type_t *avm_screen_type,
                                  ROIParameter &subImgROI,
                                  avm_camera_type_t *camera, int nImgWidth,
                                  int nImgHeight, GLuint *g_textureid,
                                  int index, int nFlip = 0);
int xcb_avmlib_four_screen_Render(avm_screen_type_t *avm_screen_type,
                                  GLuint *g_textureid);
// int xcb_avmlib_six_screen_Render(
//     avm_screen_type_t *avm_screen_type,
//     ROIParameter subImgROI[6], ////////B, L, F, R, F1, R1
//     int nImgWidth, int nImgHeight, avm_camera_type_t *camera,
//     GLuint *g_textureid);

int xcb_avmlib_six_screen_Render(avm_screen_type_t *avm_screen_type,
                                 ROIParameter subImgROI[6],
                                 avm_camera_type_t *camera, int nImgWidth,
                                 int nImgHeight, GLuint *g_texture_id);
/////////////////DMA
/*
GLeglImageOES p_Img: image data
avm_stitching_type_t *avm_stitching:stitching variable
int index:the camera's index
Note:
index=0,back camera
index=1,left camera
index=2,front camera
index=3,right camera
return value: -1:fail,0:succeed
*/
int xcb_avmlib_Update_TextureBuffer_DMA(GLeglImageOES p_Img,
                                        GLuint *g_texture_id, int index);

///////////////////lines

/////////注意，单位毫米/////////
typedef struct
{
  int width;    //线图片的宽
  int heigth;   //线图片的高
  int channels; //线图片的通道数
  unsigned char
      *pixel; //指针指向使用的纹理图片的像素值，默认四通道的纹理图片，即RGBA
} Lines_Texture;
typedef struct
{
  int thick;            //设置静态轨迹线的粗细
  int scale_length;     //设置刻度尺的长度
  int stepDistance[3];  //分别表示刻度尺距车头或车尾线的距离
  GLuint *texture_id;   //存储静态线使用纹理的id，初始化开辟资源，
  Lines_Texture tex[3]; //指向使用图片的纹理,刻度尺的和纹理一一对应
  ViewParameter *view;  //指针指向鸟瞰图图的虚拟相机
  Patch *lines;         //初始化开辟资源，退出释放资源
  bool flag;            //标识是否显示静态轨迹线
} Lines_Static;

// Note:unit:milimeter
typedef struct
{
  int iCarLength;             //设置车长
  int iLineDistance;          //设施两侧线之间的距离
  int iFrontWheelDis;         //车前轮轴心距车头的距离
  int iRearWheelDis;          //车后轮轴心距车尾的距离
  int iWheelBase;             //前后车轮轴心之间的间距；
  float fMaxAngle;            //轮胎摆动的最大角度，注意此值为正值
  float fAngle;               //轮胎摆动的角度，大于0向右，小于0向左，等于0，未摆动
  int type;                   // type=0:前进轨迹线,type=1:后退轨迹线
  GLuint prog;                // shader program
  Lines_Static *lines_static; //初始化开辟空间退出释放
  bool isOriginal = true;     //设置显示再原始图像显示的线还是矫正图像的线
} avm_lines_type_t;
int xcb_avm_lines_Init(avm_lines_type_t *avm_lines_type);
int xcb_avm_line_Gen_VBO(avm_lines_type_t *avm_lines_type,
                         avm_single_type_t *avm_single_type, float deltax = 0.0, float deltay = 0.0);
int xcb_avm_line_Exit(avm_lines_type_t *avm_lines_type);
int xcb_avm_lines_Render(avm_lines_type_t *avm_lines_type);
/////////////// ADAS Auxiliary display//////////////
typedef struct
{
  Patch *adas;
  GLuint prog; // shader program
} adas_auxiliary_type_t;
int xcb_avm_adas_Init(adas_auxiliary_type_t *adas_auxiliary);
/*
adas_auxiliary_type_t *adas_auxiliary//初始化的句柄
int points_num,//输入点的个数
float *points,//指针指向输入的点的数组
int type//表示是矩形框还是线，type=0，表示矩形框，type=1，表示线
*/
int xcb_avm_adas_Update(adas_auxiliary_type_t *adas_auxiliary, int points_num,
                        float *points, int type);
///更新线的
/*
adas_auxiliary_type_t *adas_auxiliary,
int points_num, //点的个数
float *points,  //指针指向的数组
int type//type=0,//表示更新左侧的线，type=1表示更新右侧的线；
*/
int xcb_avm_adas_Lines_Update(adas_auxiliary_type_t *adas_auxiliary,
                              int points_num, float *points, int thick,
                              int type);
/*
adas_auxiliary_type_t *adas_auxiliary//初始化的句柄
ViewParameter *view//指针指向使用的虚拟参数的变量
float *color//指向使用的颜色颜色数组如：（1.0，0.0，0.0，1.0）
int type//表示是矩形框还是线，type=0，表示矩形框，type=1，表示线
*/
void xcb_avm_adas_Render(adas_auxiliary_type_t *adas_auxiliary,
                         ViewParameter *view, float *color, int type);
/*
adas_auxiliary_type_t *adas_auxiliary//初始化的句柄
ViewParameter *view//指针指向使用的虚拟参数的变量
float *color//指向使用的颜色颜色数组如：（1.0，0.0，0.0，1.0）
int type//表示是矩形框还是线，type=0，表示左侧的线，type=1，表示右侧的线
*/
void xcb_avm_adas_Lines_Render(adas_auxiliary_type_t *adas_auxiliary,
                               ViewParameter *view, float *color, int type);

int xcb_avmlib_car_Update_SkinTexture(avm_car_type_t *avm_car_type,
                                      avm_car_skin_type_t &avm_car_skin_type);
int xcb_avmlib_Update_Png_line(avm_single_type_t *avm_single_handle,
                               int layer_index, png_texture_t *png_texture,
                               int png_num);
int xcb_avmlib_DynamicLineText_Update_VBO(avm_single_type_t *avm_single_type);

/////////////////////arm stitching related function///////////////////////////

////////////////////////set fbo full screen mode////////////////////////
typedef struct
{
  int win_x;
  int win_y;
  int width;
  int height;
  int vertex_number;   //
  int tri_face_number; //
  void **fboObject;
  GLuint *ptexture_id;
  GLuint nCurId;
  int nfbobuffernum = 1;
  int nIndex = 0;
  GLuint prog;
  GLuint buffers[3]; //
} avm_fboscreen_type_t;
int xcb_avmlib_set_fboscreen_Init(avm_fboscreen_type_t *avm_fboscreen_type);
int xcb_avmlib_set_fboscreen_Inuse(avm_fboscreen_type_t *avm_fboscreen_type);
int xcb_avmlib_set_fboscreen_Outofuse(avm_fboscreen_type_t *avm_fboscreen_type);
GLuint xcb_avmlib_set_fboscreen_ObtainTextureID(avm_fboscreen_type_t *avm_fboscreen_type);
int xcb_avmlib_set_fboscreen_Exit(avm_fboscreen_type_t *avm_fboscreen_type);
int xcb_avmlib_set_fboscreen_Tex2screen(GLuint SrcId, avm_fboscreen_type_t *avm_fboscreen_type);

////////////////////////RGBA texture to YUV texture and return buffer/////////////////////
typedef struct
{
  int win_x;
  int win_y;
  int width;
  int height;
  int vertex_number;   //
  int tri_face_number; //
  void **fboObject;
  GLuint *ptexture_id;
  int nCurIndex;
  int nfbobuffernum = 1;
  int nIndex = 0;
  GLuint prog;
  GLuint buffers[3];          //
  unsigned char **pImgBuffer; //////////////////for images in arm
} avm_fboColorTransfer_type_t;
int xcb_avmlib_set_fboRGBA2YUVA_Init(avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
int xcb_avmlib_set_fboRGBA2YUVA_Inuse(avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
int xcb_avmlib_set_fboRGBA2YUVA_Outofuse(avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
unsigned char *xcb_avmlib_set_fboRGBA2YUVA_ObtainTexturePoint(GLuint SrcId, avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
int xcb_avmlib_set_fboRGBA2YUVA_Exit(avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
int xcb_avmlib_set_fboRGBA2YUVA_Tex2screen(GLuint SrcId, avm_fboColorTransfer_type_t *avm_fboColorTransfer_type);
void xcb_avmlib_YUVA2NV21(unsigned char *src, unsigned char *dst, int w, int h);

#endif