#include <stdio.h>

#include <unistd.h>

#include "hi_comm_3a.h"
#include "hi_type.h"
#include "mpi_isp.h"

#include "ls_af_adp.h"
#include "ls_af_fv.h"
#include "ls_af_ptz.h"
#include "ls_af_algorithm.h"


static HI_BOOL st_flag = HI_FALSE;
static HI_BOOL af_flag = HI_FALSE;
static HI_BOOL cal_gate= HI_FALSE;

HI_S32 AF_IRCUT(void)
{
	printf("AF_IRCUT......\n");

	if (st_flag != HI_TRUE) return -1;
	if (af_flag == HI_TRUE) return -1;

	af_flag = HI_TRUE;
	sleep(3);
	CMD_OneKeyFocus();
	af_flag = HI_FALSE;

	return 0;
}

HI_S32 AF_Init(HI_S32 s32Handle, const ISP_AF_PARAM_S *pstAfParam)
{
	printf("AF_Init......\n");

	FV_Init();
	AFAlgInit();
	PtzInit(PROTOCOL_PELCO_D, UART_BAUD_RATE_9600, 0x1);

//	if (CMD_LensInit() != 0)	//tzq@20160622 by low frame black screen
//	{
//		af_flag = HI_TRUE;
//	}

	return HI_SUCCESS;
}

HI_S32 AF_Run(HI_S32 s32Handle,
	const ISP_AF_INFO_S *pstAfInfo,
	ISP_AF_RESULT_S *pstAfResult,
	HI_S32 s32Rsv)
{
	int ret = 0;

	if (HI_FALSE == st_flag)
	{
		if (pstAfInfo->u32FrameCnt <= 500) return HI_SUCCESS;
	}

	if (HI_TRUE == af_flag) return HI_SUCCESS;

	if (HI_FALSE == st_flag)
	{
		st_flag = HI_TRUE;
		af_flag = HI_TRUE;
/*
		ret = CMD_LensInit();  // tzq@20160622 by low frame black screen
		if (ret != 0)
		{
			printf("CMD_LensInit failed!\n");
			return HI_FAILURE; // exit AF_Run
		}
		ret = ReadZoomData();
		if (ret < 0)
		{
			printf("ReadZoomData failed!\n");
			return HI_FAILURE; // exit AF_RUN
		}
		sleep(1);              // tzq@20170121 add for lens lost step problem
		CMD_SetZoomPoint(ret);
*/
		ret = CMD_LensResume();
		if (ret != 0)
		{
			printf("CMD_LensResume failed!\n");
			return HI_FAILURE; // exit AF_Run
		}

		sleep(1);
		CMD_OneKeyFocus();
		CreatePtzThread();
		af_flag = HI_FALSE;

		return HI_SUCCESS;
	}
/*
	if (PtzCtrl() == 0) return HI_SUCCESS;

	if (pstAfInfo->u32FrameCnt % 2)
	{
		if (HI_TRUE == CheckFrame())
		{
			af_flag = HI_TRUE;
			CMD_OneKeyFocus();
			af_flag = HI_FALSE;
		}
	}
	else
	{
		FV_MapISO();
	}
*/
	return HI_SUCCESS;
}

HI_S32 AF_Ctrl(HI_S32 s32Handle, HI_U32 u32Cmd, HI_VOID *pValue)
{
	printf("AF_Ctrl......\n");
	if (PTZ_CMD_LENS_CORRECTION == u32Cmd)
	{
		if (HI_FALSE == cal_gate)
		{
			printf("Cal in...\n");
			cal_gate = HI_TRUE;
			SetPtzCmd(u32Cmd, pValue);
			return HI_SUCCESS;
		}
		else
		{
			printf("Cal do...\n");
			if (HI_FALSE == GetCalResult())
			{
				return HI_FAILURE;
			}
			else
			{
				return HI_SUCCESS;
			}
		}
	}
	else
	{
		SetPtzCmd(u32Cmd, pValue);
	}

	return HI_SUCCESS;
}

HI_S32 AF_Exit(HI_S32 s32Handle)
{
	printf("AF_Exit......\n");

	return HI_SUCCESS;
}

HI_S32 HI_MPI_AF_Register(ISP_DEV IspDev, ALG_LIB_S *pstAfLib)
{
	ISP_AF_REGISTER_S stRegister;
	HI_S32 s32Ret = HI_SUCCESS;

	AF_CHECK_DEV(IspDev);
	AF_CHECK_POINTER(pstAfLib);
	AF_CHECK_HANDLE_ID(pstAfLib->s32Id);
	AF_CHECK_LIB_NAME(pstAfLib->acLibName);

	stRegister.stAfExpFunc.pfn_af_init = AF_Init;
	stRegister.stAfExpFunc.pfn_af_run  = AF_Run;
	stRegister.stAfExpFunc.pfn_af_ctrl = AF_Ctrl;
	stRegister.stAfExpFunc.pfn_af_exit = AF_Exit;
	s32Ret = HI_MPI_ISP_AFLibRegCallBack(IspDev, pstAfLib, &stRegister);
	if (HI_SUCCESS != s32Ret)
	{
		printf("Hi_af register failed!\n");
	}

	return s32Ret;
}

HI_S32 HI_MPI_AF_UnRegister(ISP_DEV IspDev, ALG_LIB_S *pstAfLib)
{
	HI_S32 s32Ret = HI_SUCCESS;

	AF_CHECK_DEV(IspDev);
	AF_CHECK_POINTER(pstAfLib);
	AF_CHECK_HANDLE_ID(pstAfLib->s32Id);
	AF_CHECK_LIB_NAME(pstAfLib->acLibName);

	s32Ret = HI_MPI_ISP_AFLibUnRegCallBack(IspDev, pstAfLib);
	if (HI_SUCCESS != s32Ret)
	{
		printf("Hi_af unregister failed!\n");
	}

	return s32Ret;
}

