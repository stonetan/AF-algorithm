/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_af_algorithm.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/05/10
  Description   : 
  History       :

  Date          : 2016/05/10
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_AF_ALGORITHM_H__
#define __LS_AF_ALGORITHM_H__

#define FV_CHN_RATIO        0.12
#define FV_STB_RATIO        0.12

#include "hi_type.h"
#include "../len_drv/ls_lens_drv.h"


void AFAlgInit(void);

int CMD_LensInit(void);
int CMD_LensResume(void);
int CMD_LensFocPlus(unsigned int pos);
int CMD_LensFocMinus(unsigned int pos);
int CMD_LensZoomPlus(void);
int CMD_LensZoomMinus(void);
int CMD_OneKeyFocus(void);
int CMD_GetZoomPoint(void);
int CMD_SetZoomPoint(unsigned int zp);

int ReadZoomData(void);
int WriteZoomData(unsigned int zp);

HI_BOOL GetAFGate(void);
void SetAFGate(HI_BOOL af_gate);
HI_BOOL CheckFrame(void);
HI_BOOL doCal(void);
HI_BOOL doAF(void);

#endif /* __LS_AF_ALGORITHM_H__ */

