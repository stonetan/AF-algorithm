#include <stdio.h>
#include <string.h>

#include "ls_af_fv.h"

#include "hi_type.h"
#include "mpi_isp.h"
#include "mpi_ae.h"

#define BLEND_SHIFT        6
#define ALPHA              64
#define BELTA              54

#define AF_BLOCK_8X8

#ifdef AF_BLOCK_8X8
static int AFWeight[8][8] = {
	{1,1,1,1,1,1,1,1,},
	{1,2,2,2,2,2,2,1,},
	{1,2,2,2,2,2,2,1,},
	{1,2,2,2,2,2,2,1,},
	{1,2,2,2,2,2,2,1,},
	{1,2,2,2,2,2,2,1,},
	{1,2,2,2,2,2,2,1,},
	{1,1,1,1,1,1,1,1,},
}; 
#else
static int AFWeight[15][17] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
#endif

#ifdef AF_BLOCK_8X8
static ISP_FOCUS_STATISTICS_CFG_S stFocusCfg = {
	{1, 8, 8, 1920, 1080, 1, 0},
	{{0, 1, 1}, {188, 0, 0, 414, -165, 400, -164}, {7, 0, 3, 2}, 127},
	{{0, 1, 0}, {200, 0, 0, 200, -55, 0, 0}, {6, 0, 1, 0}, 31},
	{{-6, 12, 22, 12, -6}, 511},
	{{-16, 21, 0, -21, 16}, 10},
	{0, {0, 0}, {3, 2}}
};
#else
static ISP_FOCUS_STATISTICS_CFG_S stFocusCfg = {
	{1, 17, 15, 1920, 1080, 1, 0},
	{{0, 1, 1}, {188, 0, 0, 414, -165, 400, -164}, {7, 0, 3, 2}, 127},
	{{0, 1, 0}, {200, 0, 0, 200, -55, 0, 0}, {6, 0, 1, 0}, 31},
	{{-6, 12, 22, 12, -6}, 511},
	{{-16, 21, 0, -21, 16}, 10},
	{0, {0, 0}, {3, 2}}
}
#endif

static int MapISO(int iso)
{
	int j, i = (iso >= 200);

	i += ( (iso >= (200 << 1)) + (iso >= (400 << 1)) + (iso >= (400 << 2)) +
		   (iso >= (400 << 3)) + (iso >= (400 << 4)) );
	i += ( (iso >= (400 << 5)) + (iso >= (400 << 6)) + (iso >= (400 << 7)) +
		   (iso >= (400 << 8)) + (iso >= (400 << 9)) );
	j  = ( (iso >  (112 << i)) + (iso >  (125 << i)) + (iso >  (141 << i)) + 
		   (iso >  (158 << i)) + (iso >  (178 << i)) );

	return (i * 6 + j + (iso >= 25) + (iso >= 50) + (iso >= 100));
}

/* 
 * step1: set AF static widnow to 8x8 block
 */
int FV_Init(void)
{
	HI_S32 s32Ret = HI_SUCCESS;
	ISP_DEV IspDev = 0;
	ISP_STATISTICS_CFG_S stIspStaticsCfg;
	ISP_PUB_ATTR_S stPubattr;

	s32Ret  = HI_MPI_ISP_GetStatisticsConfig(IspDev, & stIspStaticsCfg);
	s32Ret |= HI_MPI_ISP_GetPubAttr(IspDev, &stPubattr);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_ISP_GetStatisticsConfig error!(s32Ret = 0x%x)\n", s32Ret);
		return HI_FAILURE;
	}

	stFocusCfg.stConfig.u16Vsize = stPubattr.stWndRect.u32Height;
	stFocusCfg.stConfig.u16Hsize = stPubattr.stWndRect.u32Width;

	memcpy(&stIspStaticsCfg.stFocusCfg, &stFocusCfg, sizeof(ISP_FOCUS_STATISTICS_CFG_S));
	s32Ret = HI_MPI_ISP_SetStatisticsConfig(IspDev, & stIspStaticsCfg);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_ISP_SetStatisticsConfig error!(s32Ret = 0x%x)\n", s32Ret);
		return HI_FAILURE;
	}
	//FV_MapISO();

	return HI_SUCCESS;
}

int FV_MapISO(void)
{
	HI_S32 s32Ret = HI_SUCCESS;
	ISP_DEV IspDev = 0;
	HI_U32 u32Iso;
	ISP_EXP_INFO_S stExpInfo;
	ISP_STATISTICS_CFG_S stIspStaticsCfg;

	s32Ret = HI_MPI_ISP_GetStatisticsConfig(IspDev, &stIspStaticsCfg);
	HI_MPI_ISP_QueryExposureInfo(IspDev, &stExpInfo);
	u32Iso = (HI_U64)stExpInfo.u32AGain * stExpInfo.u32DGain *
		stExpInfo.u32ISPDGain * 100 >> 30;
	stIspStaticsCfg.stFocusCfg.stHParam_IIR0.u16IIRThd = MapISO(u32Iso) << 3;
	s32Ret |= HI_MPI_ISP_SetStatisticsConfig(IspDev, &stIspStaticsCfg);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_ISP_SetStatisticsConfig error!(s32Ret = 0x%x)\n", s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

int FV_GetVD(void)
{
	HI_S32 s32Ret = HI_SUCCESS;
	ISP_DEV IspDev = 0;
	ISP_VD_INFO_S stVdInfo;

	s32Ret = HI_MPI_ISP_GetVDTimeOut(IspDev, &stVdInfo, 5000);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_ISP_GetVDTimeOut error!(s32Ret = 0x%x)\n", s32Ret);
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}

int FV_GetValue(HI_U32 FV[2])
{
	HI_S32 s32Ret = HI_SUCCESS;
	ISP_DEV IspDev = 0;
	ISP_STATISTICS_S stIspStatics;

	s32Ret = HI_MPI_ISP_GetStatistics(IspDev, &stIspStatics);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_ISP_GetStatistics error!(s32Ret = 0x%x)\n", s32Ret);
		return HI_FAILURE;
	}

	HI_U32 i, j;
	HI_U32 u32SumFv1 = 0;
	HI_U32 u32SumFv2 = 0;
	HI_U32 u32WgtSum = 0;
	HI_U32 u32Fv1_n, u32Fv2_n;
	for ( i = 0 ; i < stFocusCfg.stConfig.u16Vwnd; i++ )
	{
		for ( j = 0 ; j < stFocusCfg.stConfig.u16Hwnd; j++ )
		{
			HI_U32 u32H1 = stIspStatics.stFocusStat.stZoneMetrics[i][j].u16h1;
			HI_U32 u32H2 = stIspStatics.stFocusStat.stZoneMetrics[i][j].u16h2;
			HI_U32 u32V1 = stIspStatics.stFocusStat.stZoneMetrics[i][j].u16v1;
			HI_U32 u32V2 = stIspStatics.stFocusStat.stZoneMetrics[i][j].u16v2;
			u32Fv1_n = (u32H1 * ALPHA + u32V1 * ((1<<BLEND_SHIFT) - ALPHA)) >> BLEND_SHIFT;
			u32Fv2_n = (u32H2 * BELTA + u32V2 * ((1<<BLEND_SHIFT) - BELTA)) >> BLEND_SHIFT;
			u32SumFv1 += AFWeight[i][j] * u32Fv1_n;
			u32SumFv2 += AFWeight[i][j] * u32Fv2_n;
			u32WgtSum += AFWeight[i][j];
		}
	}
	FV[0] = u32SumFv1 / u32WgtSum;
	FV[1] = u32SumFv2 / u32WgtSum;
	//printf("%4d %4d\n", FV[0], FV[1]);
	//FV_MapISO();

	return HI_SUCCESS;
}

