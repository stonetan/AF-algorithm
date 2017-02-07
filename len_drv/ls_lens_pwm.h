/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_pwm.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/8
  Description   : 
  History       :

  Date          : 2016/08/8
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_PWM_H__
#define __LS_LENS_PWM_H__

#define PWM_CLK    3000000

void PWM_Init(void);
void PWM_Exit(void);

int PWM_Disable(unsigned char num);
int PWM_Write(unsigned char num, unsigned int duty, unsigned int period, unsigned int pwm_num);
int PWM_Wait(unsigned char num);

#endif

