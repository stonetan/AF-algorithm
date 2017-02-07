/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_af_visca.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/11/01
  Description   : 
  History       :

  Date          : 2016/11/01
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_AF_VISCA_H_
#define __LS_AF_VISCA_H_

#ifdef __cpusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static const short VISCA_ZOOM_RATIO[40] = 
{
	0x0010,    // X01 tzq@20170116 change for speed cannot change at 1x
	0x077A,    // tzq@20170116 add for zoom point
	0x0EF4,    // tzq@20170116 add for zoom point
	0x166F,
	0x1B2F,    // tzq@20170116 add for zoom point
	0x1FF0,
	0x257D,
	0x2940,    // X05
	0x2C02,
	0x2E2B,
	0x2FEE,
	0x316A,
	0x32B2,    // X10
	0x33D4,
	0x34D9,
	0x35C8,
	0x36A4,
	0x3773,    // X15
	0x3836,
	0x38F0,
	0x39A0,
	0x3A49,
	0x3AE8,    // X20
	0x3B7F,
	0x3C0C,
	0x3C8E,
	0x3D06,
	0x3D73,    // X25
	0x3DD4,
	0x3E27,
	0x3E7C,
	0x3EC2,
	0x3F00,    // X30
	0x3F38,
	0x3F68,
	0x3F94,
	0x3FBD,
	0x3FDF,
	0x4000,    // X40
};

static const char VISCA_SET_ZOOM_POS[5] = {0x81, 0x09, 0x04, 0x47, 0xff}; // for hi3516 is set pos
static const char VISCA_GET_ZOOM_POS[9] = {0x81, 0x01, 0x04, 0x47, 0x00, 0x00, 0x00, 0x00, 0xff}; // for hi3516 is get pos
static const char VISCA_AF_TRIGGER[6]   = {0x81, 0x01, 0x04, 0x18, 0x01, 0xff};
static const char VISCA_AUTO_FOCUS[6]   = {0x81, 0x01, 0x04, 0x38, 0x02, 0xff};
static const char VISCA_MANUAL_FOCUS[6] = {0x81, 0x01, 0x04, 0x38, 0x03, 0xff};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

