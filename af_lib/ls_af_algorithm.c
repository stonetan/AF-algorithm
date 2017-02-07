#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "hi_type.h"

#include "ls_af_algorithm.h"
#include "ls_af_fv.h"

static const char *dev_name = "/dev/lslens";
static const char *zoom_name= "/param/lens_zoom.dat";
static const char *cal_name = "/param/lens_cal.dat";
static const char *cor_name = "/param/lens_cal.bak";

typedef struct _AF_ALG
{
	HI_BOOL af_gate;

	unsigned int fv_num;
	unsigned int best_pos;
	HI_U32 FV_PEAK[2];
}AF_ALG_S;

static AF_ALG_S sAF = {0};
static unsigned int foc_cal_tbl[ZOOM_POINT_MAX] = {0};
static unsigned int foc_cal_tbl_base[ZOOM_POINT_MAX] = {0};

void AFAlgInit(void)
{
	sAF.af_gate    = HI_TRUE;

	sAF.fv_num     = 1;
	sAF.best_pos   = 0;
	sAF.FV_PEAK[0] = 0;
	sAF.FV_PEAK[1] = 0;
}

int ReadZoomData(void)
{
	FILE *fp;
	unsigned int zp = 0;

	fp = fopen(zoom_name, "a+");
	if (fp == NULL)
	{
		printf("open %s failed!\n", zoom_name);
		return -1;
	}

	fread(&zp, 4, 1, fp);
	if ( zp > ZOOM_POINT_MAX-1)
	{
		printf("read zoom data failed!\n");
		return -1;
	}
	printf("read zp is %d\n", zp);
	fclose(fp);

	return zp;
}

int WriteZoomData(unsigned int zp)
{
	FILE *fp;

	fp = fopen(zoom_name, "w+");
	if (fp == NULL)
	{
		printf("open %s failed!\n", zoom_name);
		return -1;
	}

	fwrite(&zp, 4, 1, fp);
	fclose(fp);

	return 0;
}

static int ReadCalData(const char *f_name)
{
	FILE *fp;
	unsigned int i=0;

	fp = fopen(f_name, "r");
	if (fp == NULL)
	{
		printf("open %s failed!\n", f_name);
		return -1;
	}

	for (i=0; i<=ZOOM_POINT_MAX-1; i++)
	{
		fread(&foc_cal_tbl[i], 4, 1, fp);
		if ( (foc_cal_tbl[i] < FOC_POS_MIN) || (foc_cal_tbl[i] > FOC_POS_MAX) )
		{
			printf("cal data outoff range!\n");
			return -1;
		}
		printf("read %2d data is %d\n", i, foc_cal_tbl[i]);
	}
	fclose(fp);

	memcpy(foc_cal_tbl_base, foc_cal_tbl, sizeof(foc_cal_tbl));

	return 0;
}

static int WriteCalData(void)
{
	FILE *fp;
	unsigned int i = 0;

	fp = fopen(cal_name, "w+");
	if (fp == NULL)
	{
		printf("open %s failed!\n", cal_name);
		return -1;
	}

	for (i=0; i<=ZOOM_POINT_MAX-1; i++)
	{
		fwrite(&foc_cal_tbl[i], 4, 1, fp);
	}
	fclose(fp);

	return 0;
}

static int FV_Test(void)
{
	int i;
	HI_U32 FV_P[2];

	printf("FV_Test......\n");
	for (i=0; i<5; i++)
	{
		FV_GetValue(FV_P);
		printf("Adjust--->FV_P=%4d\n", FV_P[1]);
		FV_GetVD();
	}

	return 0;
}

static int AdjustCalData(unsigned int zp)
{
	int i, ret, fd;
	float k = 1.0;
	unsigned int pos;
	MOTOR_DIR_E dir;
	HI_U32 FV_P[2], FV_N[2];

	if ( (zp != 7)  && (zp != 9)  && (zp != 11) && (zp != 13) &&
		 (zp != 15) && (zp != 17) && (zp != 18) && (zp != 19) )
	//if ( (zp < 7) || (zp == 20))
	{
		return 0;
	}

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	pos = FOC_INT_TBL[zp]*2;
	for (i=0; i<4; i++)
	{
		FV_GetValue(FV_P);
		printf("Adjust--->FV_P=%4d\n", FV_P[1]);
		FV_GetVD();
	}
	//usleep(150*1000);
	//FV_GetValue(FV_P);
	ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
	ioctl(fd, LENS_IOC_GETFOCDIR, &dir);
	for (i=0; i<4; i++)
	{
		FV_GetValue(FV_N);
		printf("Adjust--->FV_N=%4d\n", FV_N[1]);
		FV_GetVD();
	}
	//FV_GetVD();
	//usleep(150*1000);
	//FV_GetValue(FV_N);
	if (FV_P[1] != 0)
	{
		k = (float)FV_N[1] / (float)FV_P[1];
	}
	printf("Adjust--->zp=%d k=%f FV_P=%4d FV_N=%4d\n", zp, k, FV_P[1], FV_N[1]);

	if ( ((dir == MOTOR_REV) && ((int)(FV_N[1] - FV_P[1]) > 10) ) ||
	     ((dir == MOTOR_FWD) && ((int)(FV_P[1] - FV_N[1]) > 10) ) )
	{
		for (i=6; i<=ZOOM_POINT_MAX-1; i++)
		{
			if ( (foc_cal_tbl[i] + FOC_ADJ_TBL[i]) <= (foc_cal_tbl_base[i] + FOC_INF_TBL[i]) )
			{
				if ( (k < 1.15) && (k > 0.85) )
				{
					foc_cal_tbl[i] += FOC_ADJ_TBL[i]*2;
				}
				else
				{
					foc_cal_tbl[i] += FOC_ADJ_TBL[i];
				}
			}
		}
	}
	else if ( ((dir == MOTOR_FWD) && ((int)(FV_N[1] - FV_P[1]) > 10) ) ||
			  ((dir == MOTOR_REV) && ((int)(FV_P[1] - FV_N[1]) > 10) ) )
	{
		for (i=6; i<=ZOOM_POINT_MAX-1; i++)
		{
			if ( (foc_cal_tbl[i] - FOC_ADJ_TBL[i]) >= (foc_cal_tbl_base[i] - FOC_INF_TBL[i]) )
			{
				if ( (k < 1.15) && (k > 0.85) )
				{
					foc_cal_tbl[i] -= FOC_ADJ_TBL[i]*2;
				}
				else
				{
					foc_cal_tbl[i] -= FOC_ADJ_TBL[i];
				}
			}
		}
	}

	for (i=0; i<=ZOOM_POINT_MAX-1; i++)
	{
		printf("Adjust %2d data is %d\n", i, foc_cal_tbl[i]);
	}

	ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
	if (ret != sizeof(foc_cal_tbl))
	{
		printf("write cal data failed!\n");
		return -1;
	}

	return 0;
}

static int AdjustLensData(void)
{
	int i, ret, fd;
	int diff=0;
	float k=1.0;
	unsigned int zp=0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	if (zp < 7) return 0;
	
	diff = sAF.best_pos - foc_cal_tbl_base[zp];
	printf("diff=%d, best_pos=%4d\n", diff, sAF.best_pos);
	for (i=7; i<=ZOOM_POINT_MAX-1; i++)
	{
		k = (float)FOC_CVR_TBL[i] / (float)FOC_CVR_TBL[zp];
		foc_cal_tbl[i] = foc_cal_tbl_base[i] + k * diff;
		printf("Adjust %2d %f data is %d\n", i, k, foc_cal_tbl[i]);
	}
	
	ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
	if (ret != sizeof(foc_cal_tbl))
	{
		printf("write cal data failed!\n");
		return -1;
	}

	return 0;
}
/*
static int FocusRangeConfig(int fd)
{
	int ret = -1;
	unsigned int zp  = 0;
	unsigned int pos = 0;

	ret  = ioctl(fd, LENS_IOC_GETFOCPOS, &pos);
	ret |= ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);

	//if (abs((focus_tbl[zp] + FOCUS_HOME_POS) - pos) > focus_sch_tbl[zp] )
	if (abs(foc_cal_tbl[zp] - pos) > 120 )
	{
		return -1;
	}
	if ( (pos <= FOCUS_POS_MIN) || (pos >= FOCUS_POS_MAX) )
	{
		return -1;
	}

	return 0;
}
*/
int CMD_LensInit(void)
{
	int fd  = -1;
	int ret = -1;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}
	
	ret = ReadCalData(cal_name);
	if (ret < 0 )
	{
		printf("read cal data failed!\n");
		return -1;
	}
	ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
	if (ret != sizeof(foc_cal_tbl))
	{
		printf("write cal data failed!\n");
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_INIT);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_INIT failed!\n");
	}
	close(fd);

	return ret;
}

int CMD_LensResume(void)
{
	int fd  = -1;
	int ret = -1;
	int i,zp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ReadCalData(cal_name);
	if (ret < 0 )
	{
		printf("read cal data failed!\n");
		return -1;
	}
	ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
	if (ret != sizeof(foc_cal_tbl))
	{
		printf("write cal data failed!\n");
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_INIT);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_INIT failed!\n");
		goto err;
	}
	sleep(1); // tzq@20170110 add for init no complete will lost step
	
	zp = ReadZoomData();
	if (zp < 0)
	{
		printf("ReadZoomData failed!\n");
		goto err;
	}
	for (i=1; i<=zp; i++)
	{
		ioctl(fd, LENS_IOC_SETZOOMPOINT, &i);
		usleep(10000);
	}

	close(fd);
	return 0;

err:
	close(fd);
	return -1;
}

int CMD_LensFocPlus(unsigned int pos)
{
	int fd  = -1;
	int ret = -1;
	unsigned int zp = 0;
	unsigned int fp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_GETFOCPOS, &fp);
	ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	//if (FocusRangeConfig(fd) != 0)
	if ( ((int)(fp - foc_cal_tbl[zp]) > (int)FOC_SCH_TBL[zp]) || (fp >= FOC_POS_MAX) )
	{
		printf("Lens in max focus!\n");
		return -1;
	}
	//FV_Test();

	ret = ioctl(fd, LENS_IOC_FOCPLUS, &pos);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_FOCPLUS failed!\n");
	}
	close(fd);

	return ret;
}

int CMD_LensFocMinus(unsigned int pos)
{
	int fd  = -1;
	int ret = -1;
	unsigned int zp = 0;
	unsigned int fp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_GETFOCPOS, &fp);
	ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	//if (FocusRangeConfig(fd) != 0)
	if ( ((int)(foc_cal_tbl[zp] - fp) > (int)FOC_SCH_TBL[zp]) || (fp <= FOC_POS_MIN) )
	{
		printf("Lens in minus focus!\n");
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_FOCMINUS, &pos);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_ZOOMPLUS failed!\n");
	}
	close(fd);

	return ret;
}

int CMD_LensZoomPlus(void)
{
	int fd  = -1;
	int ret = -1;
	unsigned int zp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	if (zp >= ZOOM_POINT_MAX-1)
	{
		printf("Lens in max zoom!\n");
		close(fd);
		return -1;
	}
	//AdjustCalData(zp);

	ret = ioctl(fd, LENS_IOC_ZOOMPLUS);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_ZOOMPLUS failed!\n");
	}
	close(fd);

	return ret;
}

int CMD_LensZoomMinus(void)
{
	int fd  = -1;
	int ret = -1;
	unsigned int zp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	if (zp <= 0)
	{
		printf("Lens in less zoom!\n");
		close(fd);
		return -1;
	}
	//AdjustCalData(zp);

	ret |= ioctl(fd, LENS_IOC_ZOOMMINUS);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_ZOOMMINUS failed!\n");
	}
	close(fd);

	return ret;
}

int CMD_OneKeyFocus(void)
{
	HI_BOOL bRet = HI_FALSE;

	bRet = doAF();

	return bRet;
}

int CMD_GetZoomPoint(void)
{
	int fd  = -1;
	int ret = -1;
	unsigned int zp = 0;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_GETZOOMPOINT failed!\n");
		return ret;
	}
	close(fd);

	return zp;
}
/*
int CMD_SetZoomPoint(unsigned int zp)
{
	int fd  = -1;
	int ret = -1;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_SETZOOMPOINT, &zp);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_SETZOOMPOINT failed!\n");
		return ret;
	}
	close(fd);

	return 0;
}
*/
HI_BOOL GetAFGate(void)
{
	return sAF.af_gate;
}

void SetAFGate(HI_BOOL af_gate)
{
	sAF.af_gate = af_gate;
}

HI_BOOL CheckFrmChange(void)
{
	HI_U32 FV[2];
	static HI_U32 u32ChgCnt = 0;

	FV_GetValue(FV);
	if (abs(FV[sAF.fv_num]-sAF.FV_PEAK[sAF.fv_num]) > sAF.FV_PEAK[sAF.fv_num]*FV_CHN_RATIO)
	{
		u32ChgCnt++;
		printf("u32ChgCnt=%d\n", u32ChgCnt);
		printf("FV=%4d, FV_PEAK=%4d\n", FV[sAF.fv_num], sAF.FV_PEAK[sAF.fv_num]);
	}
	else
	{
		u32ChgCnt = 0;
		return HI_FALSE;
	}

	if (u32ChgCnt >= 30)
	{
		u32ChgCnt = 0;
		return HI_TRUE;
	}

	return HI_FALSE;
}

HI_BOOL CheckFrmStable(void)
{
	HI_U32 FV_N[2];
	static HI_U32 FV_P[2];
	static HI_U32 u32StbCnt = 0;

	FV_GetValue(FV_N);
	if (abs(FV_P[sAF.fv_num]-FV_N[sAF.fv_num]) < FV_P[sAF.fv_num]*FV_STB_RATIO)
	{
		u32StbCnt++;
		//printf("u32StbCnt=%d\n", u32StbCnt);
	}
	else 
	{
		u32StbCnt = 0;
	}
	memcpy(FV_P, FV_N, sizeof(HI_U32)*2);

	if (u32StbCnt >= 15)
	{
		u32StbCnt = 0;
		return HI_TRUE;
	}

	return HI_FALSE;
} 

HI_BOOL CheckFrame(void)
{
	HI_BOOL bRet;
	static HI_BOOL bChgFlag = HI_FALSE;
	static HI_U32 unStbCnt = 0;

	if (HI_FALSE == bChgFlag)
	{
		bChgFlag = CheckFrmChange();
	}

	if (HI_TRUE == bChgFlag)
	{
		bRet = CheckFrmStable();
		if (HI_TRUE == bRet)
		{
			unStbCnt = 0;
			bChgFlag = HI_FALSE;
			return HI_TRUE;
		}
		else    //trraffic has no stable
		{
			unStbCnt++;
			if (unStbCnt >= 300)
			{
				unStbCnt = 0;
				bChgFlag = HI_FALSE;
			}
		}
	}

	return HI_FALSE;
}

int CalLensInit(void)
{
	int fd  = -1;
	int ret = -1;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}
	
	ret = ReadCalData(cor_name);
	if (ret < 0 )
	{
		printf("read cal data failed!\n");
		return -1;
	}
	ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
	if (ret != sizeof(foc_cal_tbl))
	{
		printf("write cal data failed!\n");
		return -1;
	}

	ret = ioctl(fd, LENS_IOC_INIT);
	if (ret < 0)
	{
		printf("ioctl LENS_IOC_INIT failed!\n");
	}
	close(fd);

	return ret;
}

HI_BOOL AutoFocus(void)
{
	int fd  = -1;
	int ret = -1;
	unsigned int pos = 0;
	MOTOR_DIR_E dir=0;
	HI_U32 FV_P[2], FV_N[2];
	HI_U32 i, CNT, down_cnt;

	printf("do AutoFocus......\n");
	
	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return HI_FALSE;
	}

	i = 30;
	pos = 1;
	CNT = 4;
	down_cnt = 0;
	sAF.FV_PEAK[0] = 0;
	sAF.FV_PEAK[1] = 0;
	FV_GetValue(FV_P);
	do
	{
		ret = ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
		if (ret < 0)
		{
			close(fd);
			printf("ioctl: LENS_IOC_SETFOCPOS failed.\n");
			return HI_FALSE;
		}
		FV_GetVD();
		FV_GetValue(FV_N);
		if (FV_N[sAF.fv_num] < FV_P[sAF.fv_num])
		{
			down_cnt++;
		}
		else down_cnt = 0;
		memcpy(FV_P, FV_N, sizeof(HI_U32)*2);

		if (FV_N[sAF.fv_num] > sAF.FV_PEAK[sAF.fv_num])
		{
			memcpy(sAF.FV_PEAK, FV_N, sizeof(HI_U32)*2);
			ioctl(fd, LENS_IOC_GETFOCPOS, &sAF.best_pos);
		}

		//printf("FV0=%4d FV1=%4d\n", FV_N[0], FV_N[1]);
		//printf("AF i=%3d pos=%d down_cnt=%d\n", i, pos, down_cnt);
		if (down_cnt >= CNT)
		{
			break;
		}
		if (sAF.af_gate == HI_FALSE)
		{
			return HI_FALSE;
		}
	}while(--i);

	//if (i <= 0)
	//{
	//	close(fd);
	//	return HI_FALSE;
	//}
	ioctl(fd, LENS_IOC_GETFOCDIR, &dir);
	if (MOTOR_FWD == dir)
	{
		dir = MOTOR_REV;
		ret = ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}
	else
	{
		dir = MOTOR_FWD;
		ret = ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}

	printf("AF best pos is:%d\n", sAF.best_pos);
	ioctl(fd, LENS_IOC_GETFOCPOS, &pos);
	pos = abs(pos-sAF.best_pos)+FOC_BACK_RUSH;
	ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
	dir = (dir == MOTOR_FWD ? MOTOR_REV:MOTOR_FWD);
	ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	pos = FOC_BACK_RUSH;
	ioctl(fd, LENS_IOC_SETFOCPOS, &pos);

	close(fd);

	return HI_TRUE;
}

static HI_BOOL SearchConfig(void)
{
	int fd  = -1;
	unsigned int zp = 0;
	unsigned int pos = 0;
	unsigned int pre_pos = 0;
	MOTOR_DIR_E dir;
	HI_U32 FV_P[2], FV_N[2];
	HI_U32 i, CNT, up_cnt, down_cnt, peak_cnt;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ioctl(fd, LENS_IOC_GETFOCPOS, &pre_pos);
	ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	printf("ioctl pre_pos is: %d\n", pre_pos);

	if (FOC_INT_TBL[zp] >= 4)
	{
		CNT = 3;
	}
	else
	{
		CNT = 4;
	}

	if (pre_pos < foc_cal_tbl_base[zp])
	{
		dir = MOTOR_REV;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}
	else
	{
		dir = MOTOR_FWD;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}
/*
	pos = abs(foc_cal_tbl[zp] - pos); // do for manul focus
	if ( (pos > FOC_SCH_TBL[zp]) || (pos > 12))
	{
		ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
	}
*/

	printf("\ndo SearchConfig 1......\n\n");

	//i = 30;
	i = FOC_SCH_TBL[zp] / FOC_INT_TBL[zp] + CNT + 1;
	up_cnt   = 0;
	down_cnt = 0;
	peak_cnt = 0;
	FV_GetValue(FV_P);
	do
	{
		ioctl(fd, LENS_IOC_SETFOCPOS, &FOC_INT_TBL[zp]);
		FV_GetVD();
		FV_GetValue(FV_N);
		if (FV_N[sAF.fv_num] < FV_P[sAF.fv_num])
		{
			up_cnt = 0;
			down_cnt++;
		}
		else
		{
			down_cnt = 0;
			up_cnt++;
		}
		memcpy(FV_P, FV_N, sizeof(HI_U32)*2);

		printf("FV0=%4d FV1=%4d\n", FV_N[0], FV_N[1]);
		printf("AF i=%3d pos=%d down_cnt=%d up_cnt=%d\n", i, FOC_INT_TBL[zp], down_cnt, up_cnt);
		if (down_cnt >= CNT)
		{
			down_cnt = 0;
			peak_cnt++;
			//i = 30;
			i = FOC_SCH_TBL[zp] / FOC_INT_TBL[zp] + CNT + 1;
			dir = (dir == MOTOR_FWD ? MOTOR_REV:MOTOR_FWD);
			ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
			pos = FOC_INT_TBL[zp]*CNT;
			ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
			//return HI_TRUE;
		}
		if (up_cnt >= CNT)
		{
			up_cnt = 0;
			peak_cnt = 1;
			//return HI_TRUE;
		}
		if (peak_cnt >= 2)
		{
			close(fd);
			return HI_TRUE;
		}
		if (sAF.af_gate == HI_FALSE)
		{
			return HI_FALSE;
		}
	} while(--i);

	ioctl(fd, LENS_IOC_SETZOOMPOINT, &zp); // while search failed return default pos
	usleep(10000);

	if (pre_pos < foc_cal_tbl_base[zp])
	{
		dir = MOTOR_FWD;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}
	else
	{
		dir = MOTOR_REV;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}

	printf("\ndo SearchConfig 2......\n\n");

	//i = 30;
	i = FOC_SCH_TBL[zp] / FOC_INT_TBL[zp] + CNT + 1;
	up_cnt   = 0;
	down_cnt = 0;
	peak_cnt = 0;
	FV_GetValue(FV_P);
	do
	{
		ioctl(fd, LENS_IOC_SETFOCPOS, &FOC_INT_TBL[zp]);
		FV_GetVD();
		FV_GetValue(FV_N);
		if (FV_N[sAF.fv_num] < FV_P[sAF.fv_num])
		{
			up_cnt = 0;
			down_cnt++;
		}
		else
		{
			down_cnt = 0;
			up_cnt++;
		}
		memcpy(FV_P, FV_N, sizeof(HI_U32)*2);

		printf("FV0=%4d FV1=%4d\n", FV_N[0], FV_N[1]);
		printf("AF i=%3d pos=%d down_cnt=%d up_cnt=%d\n", i, FOC_INT_TBL[zp], down_cnt, up_cnt);
		if (down_cnt >= CNT)
		{
			down_cnt = 0;
			peak_cnt++;
			//i = 30;
			i = FOC_SCH_TBL[zp] / FOC_INT_TBL[zp] + CNT + 1;
			dir = (dir == MOTOR_FWD ? MOTOR_REV:MOTOR_FWD);
			ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
			pos = FOC_INT_TBL[zp]*CNT;
			ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
			//return HI_TRUE;
		}
		if (up_cnt >= CNT)
		{
			up_cnt = 0;
			peak_cnt = 1;
			//return HI_TRUE;
		}
		if (peak_cnt >= 2)
		{
			close(fd);
			return HI_TRUE;
		}
		if (sAF.af_gate == HI_FALSE)
		{
			return HI_FALSE;
		}
	} while(--i);

	ioctl(fd, LENS_IOC_SETZOOMPOINT, &zp); // while search failed return default pos
	close(fd);

	return HI_FALSE;
}

HI_BOOL doAF(void)
{
	HI_BOOL bRet = HI_FALSE;

	bRet = SearchConfig();
	printf("SearchConfig bRet=%d\n", bRet);
	if (bRet)
	{
		bRet = AutoFocus();
		if (bRet)
		{
			AdjustLensData();
		}
	}

	return bRet;
}

static HI_BOOL CalSch(void)
{
	int fd  = -1;
	unsigned int zp = 0;
	unsigned int pos = 0;
	MOTOR_DIR_E dir;
	HI_U32 FV_P[2], FV_N[2];
	HI_U32 i, CNT, up_cnt, down_cnt, peak_cnt;

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	ioctl(fd, LENS_IOC_GETFOCPOS, &pos);
	ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
	printf("ioctl pos is: %d\n", pos);

	if (FOC_INT_TBL[zp] >= 4)
	{
		CNT = 3;
	}
	else
	{
		CNT = 4;
	}

	if (pos < foc_cal_tbl[zp])
	{
		dir = MOTOR_REV;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}
	else
	{
		dir = MOTOR_FWD;
		ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
	}

	i = 100;
	up_cnt   = 0;
	down_cnt = 0;
	peak_cnt = 0;
	FV_GetValue(FV_P);
	do
	{
		ioctl(fd, LENS_IOC_SETFOCPOS, &FOC_INT_TBL[zp]);
		FV_GetVD();
		FV_GetValue(FV_N);
		if (FV_N[sAF.fv_num] < FV_P[sAF.fv_num])
		{
			up_cnt = 0;
			down_cnt++;
		}
		else
		{
			down_cnt = 0;
			up_cnt++;
		}
		memcpy(FV_P, FV_N, sizeof(HI_U32)*2);

		printf("FV0=%4d FV1=%4d\n", FV_N[0], FV_N[1]);
		printf("AF i=%3d pos=%d down_cnt=%d up_cnt=%d\n", i, FOC_INT_TBL[zp], down_cnt, up_cnt);
		if (down_cnt >= CNT)
		{
			down_cnt = 0;
			peak_cnt++;
			i = 100;
			dir = (dir == MOTOR_FWD ? MOTOR_REV:MOTOR_FWD);
			ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
			pos = FOC_INT_TBL[zp]*CNT;
			ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
			//return HI_TRUE;
		}
		if (up_cnt >= CNT)
		{
			up_cnt = 0;
			peak_cnt = 1;
			//return HI_TRUE;
		}
		if (peak_cnt >= 2)
		{
			close(fd);
			return HI_TRUE;
		}
		if (sAF.af_gate == HI_FALSE)
		{
			return HI_FALSE;
		}
	} while(--i);

	ioctl(fd, LENS_IOC_SETZOOMPOINT, &zp); // while search failed return default pos
	close(fd);

	return HI_FALSE;
}

static HI_BOOL CalAF(void)
{
	HI_BOOL bRet = HI_FALSE;

	bRet = CalSch();
	printf("SearchConfig bRet=%d\n", bRet);
	if (bRet)
	{
		bRet = AutoFocus();
	}

	return bRet;
}

HI_BOOL doCal(void)
{
	unsigned int i = 0;
	HI_BOOL bRet = HI_FALSE;

	if (CalLensInit() != 0)
	{
		printf("CalLensInit failed!\n");
		return HI_FALSE;
	}
	sleep(1);
	
	for (i=0; i<=ZOOM_POINT_MAX-1; i++)
	{
		bRet = CalAF();
		if (HI_FALSE == bRet)
		{
			printf("CalAF failed!\n");
			return HI_FALSE;
		}

		printf("ZP=%2d, Pos=%4d FV=%4d\n", i, sAF.best_pos, sAF.FV_PEAK[sAF.fv_num]);
		foc_cal_tbl[i] = sAF.best_pos;

		CMD_LensZoomPlus();
		usleep(100000);
	}
	//CMD_LensInit();
	if (WriteCalData() != 0) return HI_FALSE;	

	return HI_TRUE;
}

