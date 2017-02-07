/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_af_fv.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/05/10
  Description   : 
  History       :

  Date          : 2016/05/10
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_AF_FV_H__
#define __LS_AF_FV_H__

#include "hi_type.h"

int FV_Init(void);
int FV_MapISO(void);
int FV_GetVD(void);
int FV_GetValue(HI_U32 FV[2]);

#endif /* __LS_AF_FV_H__ */

