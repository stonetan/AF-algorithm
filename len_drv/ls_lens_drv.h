/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_drv.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/8
  Description   : 
  History       :

  Date          : 2016/08/8
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_DRV_H__
#define __LS_LENS_DRV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include <linux/ioctl.h>

#include "ls_lens_types.h"

typedef enum
{
	MOTOR_FWD    = 0x00,
	MOTOR_REV    = 0x01,

	MOTOR_FREE   = 0x80,
}MOTOR_DIR_E;

typedef enum
{
	MOTOR_SPEED_LOW    = 200,
	MOTOR_SPEED_MID    = 500,
	MOTOR_SPEED_HIGH   = 800,
}MOTOR_SPEED_S;

#define LENS_IOC_MAGIC         'x'

#define LENS_IOC_INIT          _IO(LENS_IOC_MAGIC, 0)
#define LENS_IOC_FOCPLUS       _IO(LENS_IOC_MAGIC, 1)
#define LENS_IOC_FOCMINUS      _IO(LENS_IOC_MAGIC, 2)
#define LENS_IOC_ZOOMPLUS      _IO(LENS_IOC_MAGIC, 3)
#define LENS_IOC_ZOOMMINUS     _IO(LENS_IOC_MAGIC, 4)
#define LENS_IOC_LENSOFF       _IO(LENS_IOC_MAGIC, 5)

#define LENS_IOC_GETFOCDIR     _IOR(LENS_IOC_MAGIC, 6, MOTOR_DIR_E)
#define LENS_IOC_SETFOCDIR     _IOW(LENS_IOC_MAGIC, 6, MOTOR_DIR_E)

#define LENS_IOC_GETFOCPOS     _IOR(LENS_IOC_MAGIC, 7, unsigned int)
#define LENS_IOC_SETFOCPOS     _IOW(LENS_IOC_MAGIC, 7, unsigned int)

#define LENS_IOC_GETZOOMPOINT  _IOR(LENS_IOC_MAGIC, 8, unsigned int)
#define LENS_IOC_SETZOOMPOINT  _IOW(LENS_IOC_MAGIC, 8, unsigned int)

#define LENS_IOC_GETFOCREG     _IOR(LENS_IOC_MAGIC, 9, unsigned short)

#define LENS_IOC_MAXNR         9

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LS_LENS_DRV_H__ */

