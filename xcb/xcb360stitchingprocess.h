#pragma once
#include "stitchingbasicdef.h"


///////////////////stitching model construction process/////////////////////////
///////////////////Texture parameters//////////////////////////
//////int nTextureWidth;                  /////input, ��������ͼ�����
//////int nTextureHeight;                 /////input, ��������ͼ��߶�
//////bool bWriteNewObj;              /////write new obj or not
//////pInStiPara, pInModelPara, pInCamPara    /////////input parameters
//////pOutBinModel, output, it is constructed by function stichingmodelconstructing and free by the main function
//////return value, if the value is less than zero, the process failed
//int stichingmodelconstructing(int nTextureWidth, int nTextureHeight, bool bWriteNewObj, struct InputStitchingParam * pInStiPara, struct InputModelParam * pInModelPara, struct InputCamPara * pInCamPara, struct OutPutBinaryModel * pOutBinModel);


int stichingmodelconstructingwithsimulatedcamSixRoad(int nTextureWidth, int nTextureHeight, struct SimulatedImagePara * pSimImgPara, bool bWriteNewObj, struct InputStitchingParam * pInStiPara, struct InputModelParam * pInModelPara, struct InputCamPara * pInCamPara, struct OutPutBinaryModel * pOutBinModel);
int stichingmodelconstructingwithoutsimulatedcamSixRoad(int nTextureWidth, int nTextureHeight, bool bWriteNewObj, struct InputStitchingParam * pInStiPara, struct InputModelParam * pInModelPara, struct InputCamPara * pInCamPara, struct OutPutBinaryModel * pOutBinModel);

int stichingmodelconstructingwithsimulatedcam(int nTextureWidth, int nTextureHeight, struct SimulatedImagePara * pSimImgPara, bool bWriteNewObj, struct InputStitchingParam * pInStiPara, struct InputModelParam * pInModelPara, struct InputCamPara * pInCamPara, struct OutPutBinaryModel * pOutBinModel);
int stichingmodelconstructingwithoutsimulatedcam(int nTextureWidth, int nTextureHeight, bool bWriteNewObj, struct InputStitchingParam * pInStiPara, struct InputModelParam * pInModelPara, struct InputCamPara * pInCamPara, struct OutPutBinaryModel * pOutBinModel);


bool Generate2DPlaneModelSixRoad(str2DPlaneInfo & strPlaneInput, strPlaneModel2DInfo & strPlaneOutput);
bool Generate2DPlaneModel(str2DPlaneInfo & strPlaneInput, strPlaneModel2DInfo & strPlaneOutput);
bool Generate2DPlaneModel(str2DPlaneInfo & strPlaneInput, strPlaneModel2DInfo & strModel2D, struct InputCamPara strCamPara);


bool Generate2DBroadviewModel(strs2DBroadViewInfo & strBroadViewPlaneInput, strPlane2DRevisedbroadViewInfo & strBroadViewPlaneOutput);
bool Generate2DBroadviewModelwithsimulatedcam(strs2DBroadViewInfo & strBroadViewPlaneInput, strPlane2DRevisedbroadViewInfo & strBroadViewPlaneOutput, unsigned char * pOutputImage[2], unsigned char * pInputImage[2]);

////////////////mode: 0, S32V color image�� mode: 1, T7 color image(��ʱδʵ��)�� mode: 2, S32V grey image��mode: 3, T7 grey image��
bool GenerateLdwImgMap(struct LDWImgMapInputInfo & strLDWInput, struct InputCamPara strCamPara, struct strOnePointImageMap & strMappingBack, struct strOnePointImageMap & strMappingLeft, struct strOnePointImageMap & strMappingFront, struct strOnePointImageMap & strMappingRight, int mode);


////////////////Side View Point//////////////////////
bool Generate2DPlaneSideViewImage(struct _SideViewInputInfo & str2DSideViewInput, struct  PlaneModelSideViewInfo & str2DSideViewoutput, int mode = 0);
bool Generate2DPlaneSideViewImagewithsimulatedcam(struct _SideViewInputInfo & str2DSideViewInput, struct  PlaneModelSideViewInfo & str2DSideViewoutput, unsigned char * pOutputImage[2], unsigned char * pInputImage[2], int mode = 0);


bool ObtainTexcordfromGrondPoints(
    float *fPointsIn, int pointnum, float *fTexcordOut, float * fpWeight,
    struct InputCamPara *pInCamPara, struct InputModelParam * pInModelPara, 
	int nTextureWidth, int nTextureHeight, int nChannel);


