#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "ls_lens_drv.h"


static const char *dev_name = "/dev/lslens";
static const char *cal_name = "/param/lens_cal.dat";
/*
static unsigned int foc_cal_tbl[ZOOM_POINT_MAX] = {
1548, 1748, 1864, 1932, 1966, 1976,
1964, 1930, 1874, 1798, 1710, 1618,
1526, 1428, 1336, 1272, 1200, 1148,
};
*/
/*
static unsigned int foc_cal_tbl[ZOOM_POINT_MAX] = {
1539, 1740, 1852, 1918, 1950, 1964,
1948, 1914, 1858, 1788, 1702, 1610,
1520, 1422, 1340, 1276, 1208, 1142,
};
*/
static unsigned int foc_cal_tbl[ZOOM_POINT_MAX] = {
1539, 1600, 1663, 1740, 1788, 1852, 1918, 1950, 1964,
1948, 1914, 1858, 1788, 1702, 1610,
1520, 1422, 1340, 1276, 1208, 1142,
};

static void usage(void)
{
	printf(
			"\n"
			"Usage: ./test [options] [parameter1] ...\n"
			"Options: \n"
			"	-i        Lens init\n"
			"	-fp       Focus plus\n"
			"	-fm       Focus minus\n"
			"	-zp       Zoom plus\n"
			"	-zm       Zoom minus\n"
			"	-fd       Set focus dir,    e.g '-fd <dir>'\n"
			"	-gd       Get focus dir,    e.g '-gd'\n"
			"	-sf       Set focus pos,    e.g '-sf <pos>'\n"
			"	-gf       Get focus pos,    e.g '-gf'\n"
			"	-sz       Set zoom point,   e.g '-sz <zp>'\n"
			"	-gz       Get zoom point,   e.g '-gz'\n"
			"\n");
	exit(1);
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
		printf("write %2d data is %d\n", i, foc_cal_tbl[i]);
	}
	fclose(fp);

	return 0;
}

static int ReadCalData(void)
{
	FILE *fp;
	unsigned int i = 0;

	fp = fopen(cal_name, "r");
	if (fp == NULL)
	{
		printf("open %s failed!\n", cal_name);
		return -1;
	}

	for (i=0; i<=ZOOM_POINT_MAX-1; i++)
	{   
		fread(&foc_cal_tbl[i], 4, 1, fp);
		printf("read %2d data is %d\n", i, foc_cal_tbl[i]);
	}
	fclose(fp);

	return 0;
}

int main(int argc, const char *argv[])
{
	int fd  = -1;
	int ret = -1;
	unsigned int pos;
	unsigned int zp;
	unsigned short reg_value;
	MOTOR_DIR_E dir;

	if (argc < 2)
	{
		usage();
		return 0;
	}

	fd = open(dev_name, O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed!\n", dev_name);
		return -1;
	}

	if (!strcmp(argv[1], "-i"))
	{
		ret = WriteCalData();
		ret = ReadCalData();
		if (ret < 0 )
		{
			printf("read cal data failed!\n");
			return -1;
		}
		sleep(1);
		//ret = write(fd, (unsigned char *)foc_cal_tbl, sizeof(foc_cal_tbl));
		//if (ret != sizeof(foc_cal_tbl))
		//{
		//	printf("write cal data failed!\n");
		//	return -1;
		//}

		ret = ioctl(fd, LENS_IOC_INIT);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_INIT failed.\n");
			goto err;
		}
	}
	else if (!strcmp(argv[1], "-fp"))
	{
		if (argc < 3)
		{
			usage();
			goto err;
		}

		pos = atoi(argv[2]);
		ret = ioctl(fd, LENS_IOC_FOCPLUS, &pos);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_FOCPLUS failed.\n");
			goto err;
		}
	}
	else if (!strcmp(argv[1], "-fm"))
	{
		if (argc < 3)
		{
			usage();
			goto err;
		}

		pos = atoi(argv[2]);
		ret = ioctl(fd, LENS_IOC_FOCMINUS, &pos);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_FOCMINUS failed.\n");
			goto err;
		}
	}
	else if (!strcmp(argv[1], "-zp"))
	{
		ret = ioctl(fd, LENS_IOC_ZOOMPLUS);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_ZOOMPLUS failed.\n");
			goto err;
		}
	}
	else if(!strcmp(argv[1], "-zm"))
	{
		ret = ioctl(fd, LENS_IOC_ZOOMMINUS);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_ZOOMMINUS failed.\n");
			goto err;
		}
	}
	else if(!strcmp(argv[1], "-fd"))
	{
		if (argc < 3)
		{
			usage();
			goto err;
		}

		dir = atoi(argv[2]);
		ret = ioctl(fd, LENS_IOC_SETFOCDIR, &dir);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_SETFOCDIR failed.\n");
			goto err;
		}
	}
	else if(!strcmp(argv[1], "-gd"))
	{
		ret = ioctl(fd, LENS_IOC_GETFOCDIR, &dir);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_GETFOCDIR failed.\n");
			goto err;
		}
		printf("Get focus dir is: %d\n", dir);
	}
	else if(!strcmp(argv[1], "-sf"))
	{
		if (argc < 3)
		{
			usage();
			goto err;
		}

		pos = atoi(argv[2]);
		ret = ioctl(fd, LENS_IOC_SETFOCPOS, &pos);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_SETFOCPOS failed.\n");
			goto err;
		}
	}
	else if(!strcmp(argv[1], "-gf"))
	{
		ret = ioctl(fd, LENS_IOC_GETFOCPOS, &pos);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_GETFOCPOS failed.\n");
			goto err;
		}
		printf("Get focus pos is: %d\n", pos);
	}
	else if(!strcmp(argv[1], "-sz"))
	{
		if (argc < 3)
		{
			usage();
			goto err;
		}

		zp = atoi(argv[2]);
		ret = ioctl(fd, LENS_IOC_SETZOOMPOINT, &zp);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_SETZOOMPOINT failed.\n");
			goto err;
		}
	}
	else if(!strcmp(argv[1], "-gz"))
	{
		ret = ioctl(fd, LENS_IOC_GETZOOMPOINT, &zp);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_GETZOOMPOINT failed.\n");
			goto err;
		}
		printf("Get zoom point is: %d\n", zp);
	}
	else if(!strcmp(argv[1], "-fr"))
	{
		ret = ioctl(fd, LENS_IOC_GETFOCREG, &reg_value);
		if (ret < 0)
		{
			printf("ioctl: LENS_IOC_GETFOCREG failed.\n");
			goto err;
		}
		printf("Get focus reg value is:%#x\n", reg_value);
	}
	else
	{
		usage();
		goto err;
	}

err:
	close(fd);

	return 0;
}

