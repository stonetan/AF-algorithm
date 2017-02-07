/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_41908.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/8
  Description   : 
  History       :

  Date          : 2016/08/8
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_41908_H__
#define __LS_LENS_41908_H__


void AN41908_Init(void);
void AN41908_Exit(void);
void AN41908_Test(void);

void ZoomForward(void);
void ZoomReverse(void);
void ZoomMove(unsigned int steps);
void SetZoomSpeed(unsigned int pps);

void ZoomPIEnable(void);
void ZoomPIDisable(void);
unsigned char GetZoomPIFlag(void);

void FocusForward(void);
void FocusReverse(void);
void FocusMove(unsigned int steps);
void SetFocusSpeed(unsigned int pps);
unsigned short GetFocusReg(void);

void FocusPIEnable(void);
void FocusPIDisable(void);
unsigned char GetFocusPIFlag(void);

void SyncMove(unsigned int z_pos, unsigned int f_pos);

#endif /* __LS_LENS_41908_H__ */

