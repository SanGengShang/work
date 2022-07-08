#ifndef __XCB_360STITCHING_H__
#define __XCB_360STITCHING_H__


void* XCB_360stitchingInit(char * pModelName);
int XCB_360stitchingDestroy( void * pModel);

void* XCB_2DPlaneInit(char * pModelName);
int XCB_2DPlaneDestroy( void * pModel);

void* XCB_SideViewInit(char * pModelName);
int XCB_SideViewDestroy( void * pModel);
///////////////////////////////////////////////////
///pImgBack, pImgLeft, pImgFront, pImgRight: corresponding original image for top-down view, YUV422 format, size: 720P
///nShowModeL: demonstration mode for the 360 stitching image, 0: small view; 1: broad view;
///nShowModel: 0: Left origin view; 1: Right origin view; 2: Back origin view; 3: Front origin view; 4: Left revised view; 
///pOutPutMem: 输出图像的内容地址，由调用方提供
unsigned char * XCB_360stitchingProcess(void * pModel, 
    unsigned char * pImg[6],
    int nWidStep,
    unsigned char * pOutPutMem,
	unsigned char * pOutPutUV);

unsigned char * XCB_2Dplane(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10

unsigned char * XCB_BroadView(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10

unsigned char * XCB_SideView(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	float * pLinePos, 
	int mode);



unsigned char * XCB_360stitchingProcessAmap(void * pModel, 
	unsigned char * pImg,
	int nWidStep,
	int nMapLabel,
	int nMode, 
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV);

unsigned char * XCB_2DplaneUP(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10

unsigned char * XCB_2DplaneLOW(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10


unsigned char * XCB_BroadViewUP(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10

unsigned char * XCB_BroadViewLOW(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int mode);///mode < 10

unsigned char * XCB_SideViewUP(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	float * pLinePos, 
	int mode);
unsigned char * XCB_SideViewLOW(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	float * pLinePos, 
	int mode);


unsigned char * XCB_ADDTWOPlane(void * pModel, 
	unsigned char * psrc,
	unsigned char * psrcUV,
	unsigned char * pdst,
	unsigned char * pdstUV,
	int nWidStep);


unsigned char * XCB_MultiThreadProcess(void * pModel, 
	unsigned char * pImg[6],
	int nWidStep,
	unsigned char * pOutPutMem,
	unsigned char * pOutPutUV,
	int submode,
	int highmode,
	float * pLinePos);

unsigned char * XCB_InitMultiThread();
unsigned char * XCB_MultiThreadDestroy();

unsigned char * XCB_ScaleResize(unsigned char * pOutputY, unsigned char * pInputY, 
                                unsigned char * pOutputUV, unsigned char * pInputUV, int nStep, int nWidth, int nHeight);
unsigned char * XCB_ScaleResize_fourRoad(unsigned char * pOutputY, unsigned char * pInputY, 
                                unsigned char * pOutputUV, unsigned char * pInputUV, int nStep, int nWidth, int nHeight);
#endif
