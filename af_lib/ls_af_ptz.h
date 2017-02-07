/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_af_ptz.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/24
  Description   : 
  History       :

  Date          : 2016/08/24
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_AF_PTZ_H__
#define __LS_AF_PTZ_H__

#include "hi_type.h"
#include "sys_env_type.h"
#include "AVAccessEnvType.h"

typedef enum
{
	UART_BAUD_RATE_110 = 0,
	UART_BAUD_RATE_300,
	UART_BAUD_RATE_600,
	UART_BAUD_RATE_1200,
	UART_BAUD_RATE_2400,
	UART_BAUD_RATE_4800,
	UART_BAUD_RATE_9600,
	UART_BAUD_RATE_19200,
	UART_BAUD_RATE_38400,
	UART_BAUD_RATE_57600,
	UART_BAUD_RATE_115200,
	UART_BAUD_RATE_230400,
	UART_BAUD_RATE_460800,
	UART_BAUD_RATE_921600,
} UART_BAUD_RATE_E;

typedef enum
{
	PROTOCOL_PELCO_D = 0,
	PROTOCOL_PELCO_P,
} PROTOCOL_PELCO_E;

typedef enum
{
	PTZ_CMD_STOP = 0,
	PTZ_CMD_LEFT,
	PTZ_CMD_RIGHT,
	PTZ_CMD_UP,
	PTZ_CMD_DOWN,
	PTZ_CMD_LEFTUP,              /*向左上*/
	PTZ_CMD_LEFTDOWN,            /*向左下*/
	PTZ_CMD_RIGHTUP,             /*向右上*/
	PTZ_CMD_RIGHTDOWN,           /*向右下*/

	PTZ_CMD_ZOOM_WIDE = 9,
	PTZ_CMD_ZOOM_TELE,
	PTZ_CMD_FOCUS_FAR,
	PTZ_CMD_FOCUS_NEAR,
	PTZ_CMD_FOCUS_STOP,
	PTZ_CMD_IRIS_OPEN,           //光圈加
	PTZ_CMD_IRIS_CLOSE,          //光圈减
	PTZ_CMD_PRESET_SET,
	PTZ_CMD_PRESET_CLEAR,
	PTZ_CMD_PRESET_GOTO,
	PTZ_CMD_HOME_GOTO,
	PTZ_CMD_HOME_SET = 20 ,
	PTZ_CMD_ONEKEYFOCUS,         //一键聚集
	PTZ_CMD_INITLENS,            //初始化镜头
	PTZ_CMD_LENS_CORRECTION,     //镜头校正
	PTZ_CMD_ZOOM_STOP,
	PTZ_CMD_TOUR_START,          //开始巡航
	PTZ_CMD_TOUR_STOP,           //停止巡航
	PTZ_CMD_PT,                  //PT位置值
	PTZ_CMD_END
} PtzControlCmd;

typedef enum
{
	AF_OK                 = 0,
	
	AF_PTZ_ZOOM_LIMIT     = 1,
	AF_PTZ_FOCUS_LIMIT    = 2,
} AF_ERRCODE_E;

typedef struct
{
	AVAccessPtzOption *pPtzOption;
	SysInfo *pSysInfo;
}SysInfo_PtzOption;

HI_BOOL GetCalResult(void);

int PtzInit(PROTOCOL_PELCO_E protocal, UART_BAUD_RATE_E baudrate, HI_U8 dev_addr);
int PtzExit(void);

int SetPtzCmd(HI_U32 cmd, HI_VOID *pValue);
int PtzCtrl(void);
int SendPtzOn(void);
int CreatePtzThread(void);

#endif /* __LS_AF_PTZ_H__ */

