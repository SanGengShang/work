/*
* Copyright (c) 2008-2018 Allwinner Technology Co. Ltd.
* All rights reserved.
*
* File : isp_version.h
* Description :
* History :
* Author  : zhaowei <zhaowei@allwinnertech.com>
* Date    : 2018/02/08
*
*/

#ifndef ISP_VERSION_H
#define ISP_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

#define ISP_VERSION "V1.00"
#define REPO_TAG "isp-500-520-v2.00"
#define REPO_BRANCH "t7_merge"
#define REPO_COMMIT "62604e6ccfc2c12f6a1c7eeb5f22d94461a4ba1d"
#define REPO_DATE "Tue Feb 12 17:03:31 2019 +0800"
#define RELEASE_AUTHOR "zhaowei"

static inline void isp_version_info(void)
{
	printf("\n>>>>>>>>>>>>>>>>>>>> ISP VERSION INFO <<<<<<<<<<<<<<<<<<<\n"
		"version:%s\n"
		"tag   : %s\n"
		"branch: %s\n"
		"commit: %s\n"
		"date  : %s\n"
		"author: %s\n"
		"--------------------------------------------------------\n\n",
		ISP_VERSION, REPO_TAG, REPO_BRANCH, REPO_COMMIT, REPO_DATE, RELEASE_AUTHOR);
}

#ifdef __cplusplus
}
#endif

#endif

