/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_af_adp.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/05/10
  Description   : 
  History       :

  Date          : 2016/05/10
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_AF_ADP_H__
#define __LS_AF_ADP_H__

#include <string.h>
#include "hi_type.h"
#include "hi_comm_3a.h"
#include "hi_af_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define MAX_AF_LIB_NUM        2

#define AF_CHECK_HANDLE_ID(s32Handle)\
	do {\
		if (((s32Handle) < 0) || ((s32Handle) >= MAX_AF_LIB_NUM))\
		{\
			printf("Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
			return HI_FAILURE;\
		}\
	}while(0)

#define AF_CHECK_LIB_NAME(acName)\
	do {\
		if (0 != strcmp((acName), HI_AF_LIB_NAME))\
		{\
			printf("Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
			return HI_FAILURE;\
		}\
	}while(0)

#define AF_CHECK_POINTER(ptr)\
	do {\
		if (HI_NULL == ptr)\
		{\
			printf("Null Pointer in %s!\n", __FUNCTION__);\
			return HI_FAILURE;\
		}\
	}while(0)


#define AF_CHECK_DEV(dev)\
	do {\
		if (0 != dev)\
		{\
			ISP_TRACE(HI_DBG_ERR, "Err AF dev %d in %s!\n", dev, __FUNCTION__);\
			return HI_ERR_ISP_ILLEGAL_PARAM;\
		}\
	}while(0) 


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __LS_AF_ADP_H__ */

