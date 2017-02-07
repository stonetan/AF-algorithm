/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_timer.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/05/10
  Description   : 
  History       :

  Date          : 2016/05/10
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_TIMER_H__
#define __LS_LENS_TIMER_H__

int ModifyTimer(unsigned long expires);
int TimerInit(void* func, unsigned long dat, unsigned long expires);
int TimerExit(void);

int MotorDelay(unsigned int ms);

#endif /* __LS_LENS_TIMER_H__ */

