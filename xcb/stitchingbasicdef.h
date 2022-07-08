#pragma once
///////////////stitching parameters input///////////////////
struct InputStitchingParam {
  float flhratio;
  float flhratioc;

  float flhratio1;
  float flhratioc1;
  float fSVCFX[6];
  float fSVCFY[6];
};
///////////////model parameters input/////////////////////
struct InputModelParam {
  char InputObjName[250]; ////////////����obj�ļ�Ŀ¼�Ѿ�����//////////////
  char outputObjName[250]; ////////////���obj�ļ�Ŀ¼�Ѿ�����//////////////
  float fmodela; ////////////��״ģ����ز���//////////////////////
  float fmodelb;
  float fmodelh;
  float fcuttedhalfw; ////////////�м��ڿ�����İ�����///////////////
  float fcuttedhalfh;

  float ftranshalfw;
  float ftranshalfh;

  int nMode; /////////////ƴ�ӷ�ʽ0���Ⱦ�ƴ��ģʽ��1������ƴ��ģʽ

  float fedgewidth;  ///////////ƴ�����//////////
  float fslopevalue; ///////////ƴ��б��//////////

  float fStartTheta; ///////////�����������ʼ�Ƕȣ���λ����//////////////
  float fEndTheta; ///////////��������Ľ�ֹ�Ƕȣ���λ����//////////////

  float fHalfOverLapH;
  float fStartH;
  float fUpStartH;
  ////////////盲区偏移量/////////////////
  float fCarMoveX;
  float fCarMoveY;
	////////////两侧的融合位置点
	float fSplittedHalfW;
	float fSplittedHalfH;


  float fdeltaHeadDis;
};

//////////////camera parameters input//////////////////////
struct InputCamPara
{
	double fHomMatrixBack[3][3];          /////////////////4个摄像头的homography矩阵
	double fHomMatrixLeft[3][3];
	double fHomMatrixFront[3][3];
	double fHomMatrixRight[3][3];

	double fHomMatrixLeft1[3][3];
	double fHomMatrixRight1[3][3];

	float fEllipsoidCenterX[4];              /////////////////鱼眼镜头的矫正参数，顺序为back、left、front、right
	float fEllipsoidCenterY[4];
	float fEllipsoida[4];
	float fEllipsoidb[4];
	float fEllipsoidc[4];
	float fZ0;

	/////////////////////self cali para//////////////
	float fMultiNomialIntrinsic[6][9];
	float fMultiNomialCoef[6][4];

	int nTextureImgW;
	int nTextureImgH;
};

//////////////model output/////////////////////////////////
struct OutPutBinaryModel {
  int id;
  int vertex_number;
  int tri_face_number;
  float *position;
  int *tri_face_index;

  float *texcoord_0;
  float *texcoord_1;
  float *texcoord_2;
  float *texcoord_3;
  float *texcoord_4;
  float *texcoord_5;

  float *alpha_weight_0;
  float *alpha_weight_1;
  float *alpha_weight_2;
  float *alpha_weight_3;
  float *alpha_weight_4;
  float *alpha_weight_5;

  float fHalfW;
  float fHalfH;
  float fOverLapH;

  struct InputModelParam * pInModel;
  struct InputCamPara * pCamInfo;
};
////////////////Simulated View Parameters//////////////////////////////////////
struct SimulatedImagePara {
  float fCamPosition[3];
  float fCamFrontDirect[3];
  float fCamUpDirect[3];
  float fCamZoom;
  float fCamfocal;
  int nInImgW;
  int nInImgH;
  char *pInImg[6]; //////////////////////four input color image, back, left,
                   /// front, right
  int nOutputImgW;
  int nOutputImgH;
  char *pOutImg; //////////////////////output color image
};

typedef struct _OriginImgInfo {
  bool nImgLabel[6];
  float Xcor[6];
  float Ycor[6];
  double fWeight[6];
  unsigned char greyValue;
  float BGR[3];
  int nValid;
} OriginImgInfo;

typedef struct _SideViewInputInfo {
  ////////////////////��ԭʼ1920*1280��ͼ���ϣ���ȡ������
  int nStartPixelX[2];
  int nEndPixelX[2];
  int nStartPixelY[2];
  int nEndPixelY[2];

  ////////////////////����2Dģ�͵�����
  int nBWNum;
  int nBHNum;

  /////////////////////////ƽ����Ƭ�����Ͻ����꣬ԭ��������
  float fUpLeftx;
  float fUpLefty;
  float fWidth; ////////////////��������ͼ�����

  /////////////////////////�����ӽ���ز�����ע������sideview�ǶԳƵģ����������������ӽǲ���
  float fCamPosition[3];
  float fCamFrontDirect[3];
  float fCamUpDirect[3];
  float fCamZoom;
  float fCamfocal;

  ////////////////////////Model Parameter//////////////////////////
  float fmodela; ////////////��״ģ����ز���//////////////////////
  float fmodelb;
  float fmodelh;
  int nOutputImgHSim;
  int nOutputImgWSim;
  int nTextureImgH;
  int nTextureImgW;

  ///////////////////////
  float fLine[8];
  int nOutputImgH;
  int nOutputImgW;
  int nSubImgW;
} SideViewInputInfo;

typedef struct _XCBStitchingPoint2D32f {
  float x;
  float y;
} XCBStitchingPoint2D32f;

#define CALIBRATEDHEIGHT 8000
#define CALIBRATEDWIDTH 8000
#define MAXOBJNUM 10

typedef struct TriFace {
  int x;
  int y;
  int z;
} strTriFace;

typedef struct VertexStruct {
  float x;
  float y;
  float z;
} strVertexStruct;

typedef struct VtStruct {
  float x;
  float y;
  float z;
} strVtStruct;
typedef struct ImgROI {
  int x;
  int y;
  int w;
  int h;
} strImgROI;

typedef struct s2DPlaneInfoARM {
  double fHalfW;
  double fHalfH;

  int nBWNum;
  int nBHNum;

  int nOutputW;
  int nOutputH;

  int nTextureImgW;
  int nTextureImgH;

  float InPara1[9];
  float InPara2[9];
  float InPara3[9];
  float InPara4[9];
  float InPara5[9];
  float InPara6[9];

  float Coeffi1[4];
  float Coeffi2[4];
  float Coeffi3[4];
  float Coeffi4[4];
  float Coeffi5[4];
  float Coeffi6[4];

  strImgROI strOriginImg;
  strImgROI strLeftFrontImgROI;
  strImgROI strRightFrontImgROI;
  strImgROI strRevisedROI;

  unsigned char *pInImg[6];
  unsigned char *pOutImg;
  double fRevisedRatio;
  double fRevisedRatio1;
  double fRevisedRatio2;
  double fRevisedRatio3;
  float fRatiox[6];
  float fRatioy[6];

  int nDeltaX[6]; //////////////�����㣺�ӵ�����
  int nDeltaY[6]; //////////////�����������ƶ���index 0Ϊǰ�ӣ�1Ϊ����

  int nOutputWH[8];
} str2DPlaneInfoARM;

typedef struct s2DPlaneInfo {
  double fHalfW;
  double fHalfH;

  int nBWNum;
  int nBHNum;

  int nTextureImgW;
  int nTextureImgH;

  float InPara1[9];
  float InPara2[9];
  float InPara3[9];
  float InPara4[9];
  float InPara5[9];
  float InPara6[9];

  float Coeffi1[4];
  float Coeffi2[4];
  float Coeffi3[4];
  float Coeffi4[4];
  float Coeffi5[4];
  float Coeffi6[4];

  strImgROI strOriginImg;
  strImgROI strLeftFrontImgROI;
  strImgROI strRightFrontImgROI;
  strImgROI strRevisedROI;

  double fRevisedRatio;
  int nOutputWH[6];
} str2DPlaneInfo;

typedef struct PlaneModel2DInfo {
  int vertex_number;     //////////////��������
  int tri_face_number;   //////////////������Ƭ����
  float *position;       //////////////����λ��
  short *tri_face_index; //////////////������Ƭ����

  float *
      texcoordOriginImage; //////////////�Ǻ������ͷ��ԭʼͼ����������

  float *texcoordLeftFrontTire; //////////////��ǰ�ֶ�Ӧͼ����������
  float *texcoordRightFrontTire; //////////////��ǰ�ֶ�Ӧͼ����������

  float *texcoordrevisedb; //////////////���ͼ��У������������
  float *texcoordrevisedl; //////////////���ͼ��У������������
  float *texcoordrevisedf; //////////////ǰ��ͼ��У������������
  float *texcoordrevisedr; //////////////�Ҳ�ͼ��У������������
  float *texcoordrevisedl1; //////////////左侧图像校正后纹理坐标
  float *texcoordrevisedr1; //////////////右侧图像校正后纹理坐标

  float *
      texcoordBOriginImage; //////////////�������ͷ��ԭʼ�������꣬��Ҫ����ƫת

} strPlaneModel2DInfo;

typedef struct s2DBroadViewInfo {
  double fHalfW;
  double fHalfH;

  int nBWNum;
  int nBHNum;

  int nTextureImgW;
  int nTextureImgH;

  float InPara1[9];
  float InPara2[9];
  float InPara3[9];
  float InPara4[9];

  float Coeffi1[4];
  float Coeffi2[4];
  float Coeffi3[4];
  float Coeffi4[4];

  float fRevisedEdge;
  float fTheta;

  int nSimW;
  int nSimH;

  //////////////////Y��X����ƫ��//////////////////
  int nDeltaX[2]; //////////////�����㣺�ӵ�����
  int nDeltaY[2]; //////////////�����������ƶ���index 0Ϊǰ�ӣ�1Ϊ����

} strs2DBroadViewInfo;

typedef struct s2DBroadViewInfoARM {
  int nOutputW;
  int nOutputH;
  double fHalfW;
  double fHalfH;

  int nBWNum;
  int nBHNum;

  int nTextureImgW;
  int nTextureImgH;

  float InPara1[9];
  float InPara2[9];
  float InPara3[9];
  float InPara4[9];
  float InPara5[9];
  float InPara6[9];

  float Coeffi1[4];
  float Coeffi2[4];
  float Coeffi3[4];
  float Coeffi4[4];
  float Coeffi5[4];
  float Coeffi6[4];

  float fRevisedEdge;
  float fTheta;

  int nSimW;
  int nSimH;

  //////////////////Y��X����ƫ��//////////////////
  int nDeltaX[2]; //////////////�����㣺�ӵ�����
  int nDeltaY[2]; //////////////�����������ƶ���index 0Ϊǰ�ӣ�1Ϊ����

  float fRatiox;
  float fRatioy;
} strs2DBroadViewInfoARM;

typedef struct Plane2DRevisedbroadViewInfo {
  int vertex_number;     //////////////��������
  int tri_face_number;   //////////////������Ƭ����
  float *position;       //////////////����λ��
  short *tri_face_index; //////////////������Ƭ����

  ////////////////broad view texture infor
  float *texcoordbroadviewf; //////////////ǰ��ͼ��У������������
  float *texcoordbroadviewb; //////////////�Ҳ�ͼ��У������������

} strPlane2DRevisedbroadViewInfo;

typedef struct PlaneModelSideViewInfo {
  int vertex_number[2];     //////////////��������
  int tri_face_number[2];   //////////////������Ƭ����
  float *position[2];       //////////////����λ��
  short *tri_face_index[2]; //////////////������Ƭ����

  float *texcoordImage
      [2]; //////////////�Ǻ������ͷ��ԭʼͼ����������

  /////////////////////////ƽ����Ƭ�����Ͻ����꣬ԭ��������
  float fUpLeftx;
  float fUpLefty;
  float fWidth; ////////////////��������ͼ�����

  /////////////////////////output line coordinates
  float fLine[8];      ////////////////���gpu���߽��
  float fLineDebug[8]; ////////////////������ڵ��Ի���ʹ��

} strPlaneModelSideViewInfo;

struct strxcbpjPoint {
  short x, y;
};
// struct strOnePointImageMap
//{
//	struct strxcbpjPoint rectCord[2];
//	unsigned int * absCord;
//	short * refCord;
//	unsigned short * coef;
//};

struct LDWImgMapInputInfo {
  float fHalfW;   /////////////////////////////?????????????
  float fHalfH;   /////////////////////////////?????????????
  float fCuttedW; /////////////////////////////??????????????
  float fCuttedH; /////////////////////////////??????????????
  float fSlope;   /////////////////////////////??????????
  float fpx2cm;

  int nImgOutputW;    /////////////////////////////???????????
  int nImgOutputH;    /////////////////////////////???????????
  int nTextureWidth;  /////////////////////////////????????
  int nTextureHeight; /////////////////////////////????????
  int nMode;
};
