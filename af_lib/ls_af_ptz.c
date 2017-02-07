#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "ls_af_ptz.h"
#include "ls_af_fv.h"
#include "ls_af_visca.h"
#include "ls_af_algorithm.h"

#define PTZ_DEV_NAME        "/dev/ttyAMA1"
#define PTZ_LEN_NAME        "/dev/ttyAMA0"

typedef enum
{
	PELCO_CMD_LEFT      = 0x0004,
	PELCO_CMD_RIGHT     = 0x0002,
	PELCO_CMD_UP        = 0x0008,
	PELCO_CMD_DOWN      = 0x0010,

	PELCO_CMD_LEFTUP    = 0x000C,
	PELCO_CMD_LEFTDOWN  = 0x0014,
	PELCO_CMD_RIGHTUP   = 0x000A,
	PELCO_CMD_RIGHTDOWN = 0x0012,

	PELCO_CMD_ZOOMIN    = 0x0020,
	PELCO_CMD_ZOOMOUT   = 0x0040,
	PELCO_CMD_FOCUSNEAR = 0x0080,
	PELCO_CMD_FOCUSFAR  = 0x0100,
	PELCO_CMD_IRISSMALL = 0x0200,
	PELCO_CMD_IRISBIG   = 0x0400,

	PELCO_CMD_PRE_SET   = 0x0003,
	PELCO_CMD_PRE_CLEAR = 0x0005,
	PELCO_CMD_PRE_GOTO  = 0x0007,
}PELCO_CMD_E;

typedef struct
{
	HI_U32 baudrate;
	HI_U8  dev_addr;
	PROTOCOL_PELCO_E protocol;

	HI_U8 lr_spd;
	HI_U8 ud_spd;
	HI_U8 pp_idx;
	HI_U8 cruise_line;             // 巡航线序号
	HI_U8 cruise_point_nums;       // 巡航线预置点个数
	HI_U8 cruise_point_time;       // 巡航线预置点间隔
	HI_U8 cruise_point_spd;        // 巡航线速度
	HI_U8 cruise_point_index;      // 巡航线预置点序号
	HI_U8 cruise_state;            // 巡航状态
	Ptz_Config_Data_Ex ptz_conf_dat;

	HI_U16 cmd;
	HI_U16 dat;

	HI_BOOL cal_ret;
	HI_BOOL af_flag;
	HI_BOOL ptz_flag;
	HI_BOOL cmd_pairs;

	PtzControlCmd ptz_cmd;
} PTZ_S;

static HI_S32 rs485_fd = -1;
static HI_S32 rslen_fd = -1;
static PTZ_S ptz = {0};

int UartConfig(int fd, UART_BAUD_RATE_E baudrate, HI_U8 n_bits, char parity, HI_U8 n_stop)
{
	struct termios opt_new;

	if (tcgetattr(fd, &opt_new) != 0)
	{
		printf("tcgetattr failed!\n");
	}

	cfmakeraw(&opt_new);
	opt_new.c_cflag |= CLOCAL | CREAD;
	opt_new.c_cflag &= ~CSIZE;

	switch (n_bits) // set number of data bits
	{
		case 6:
			opt_new.c_cflag |= CS6;
			break;
		case 7:
			opt_new.c_cflag |= CS7;
			break;
		case 8:
			opt_new.c_cflag |= CS8;
			break;
		default:
			opt_new.c_cflag |= CS8;
			break;
	}

	switch (parity) // set parrity
	{
		case 'O':
		case 'o':
			opt_new.c_cflag |= PARENB;
			opt_new.c_cflag |= PARODD;
			opt_new.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'E':
		case 'e':
			opt_new.c_cflag |= PARENB;
			opt_new.c_cflag &= ~PARODD;
			opt_new.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'N':
		case 'n':
			opt_new.c_cflag &= ~PARENB;
			opt_new.c_iflag |= IGNPAR;
			break;
		case 'S':
		case 's':
			opt_new.c_cflag &= ~PARENB;
			opt_new.c_iflag &= ~CSTOPB;
			break;
		default:
			opt_new.c_cflag |= PARENB;
			opt_new.c_cflag |= PARODD;
			opt_new.c_iflag |= (INPCK | ISTRIP);
			break;
	}

	switch (baudrate) // set baudrate
	{
		case UART_BAUD_RATE_1200:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B1200);
			cfsetospeed(&opt_new, B1200);
			break;
		case UART_BAUD_RATE_2400:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B2400);
			cfsetospeed(&opt_new, B2400);
			break;
		case UART_BAUD_RATE_4800:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B4800);
			cfsetospeed(&opt_new, B4800);
			break;
		case UART_BAUD_RATE_9600:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B9600);
			cfsetospeed(&opt_new, B9600);
			break;
		case UART_BAUD_RATE_19200:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B19200);
			cfsetospeed(&opt_new, B19200);
			break;
		case UART_BAUD_RATE_57600:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B57600);
			cfsetospeed(&opt_new, B57600);
			break;
		default:
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&opt_new, B9600);
			cfsetospeed(&opt_new, B9600);
			break;
	}

	if (n_stop == 1)
	{
		opt_new.c_cflag &= ~CSTOPB;
	}
	else if (n_stop == 2)
	{
		opt_new.c_cflag |= CSTOPB;
	}
	else
	{
		printf("param n_stop=%d error!\n", n_stop);
	}

	opt_new.c_cflag &= ~CRTSCTS; // don't use data stream

	opt_new.c_cc[VTIME] = 0;
	opt_new.c_cc[VMIN]  = 0;

	opt_new.c_oflag &= ~OPOST; // to avoid the sys convert the 10 to (\r\n)

	tcflush(fd, TCIFLUSH);

	if (tcsetattr(fd, TCSANOW, &opt_new) != 0)
	{
		printf("UartConfig failed!\n");
	}

	return 0;
}

int UartInit(UART_BAUD_RATE_E baudrate, HI_U8 n_bits, char parity, HI_U8 n_stop)
{
	rs485_fd = open(PTZ_DEV_NAME, O_RDWR | O_NOCTTY | O_NDELAY);
	if (rs485_fd < 0)
	{
		printf("open %s failed!\n", PTZ_DEV_NAME);
		return -1;
	}

	UartConfig(rs485_fd, baudrate, n_bits, parity, n_stop);

	if (fcntl(rs485_fd, F_SETFL, FNDELAY) < 0)
	{
		printf("fcntl failed!\n");
		return -1;
	}
	if (isatty(rs485_fd) == 0)
	{
		printf("this is not a terminal!\n");
		return -1;
	}

	rslen_fd = open(PTZ_LEN_NAME, O_RDWR | O_NOCTTY | O_NDELAY);
	if (rslen_fd < 0)
	{
		printf("open %s failed!\n", PTZ_LEN_NAME);
		return -2;
	}

	UartConfig(rslen_fd, baudrate, n_bits, parity, n_stop);

	if (fcntl(rslen_fd, F_SETFL, FNDELAY) < 0)
	{
		printf("fcntl failed!\n");
		return -2;
	}
	if (isatty(rslen_fd) == 0)
	{
		printf("this is not a terminal!\n");
		return -2;
	}

	return 0;
}

void UartExit(void)
{
	close(rs485_fd);
	rs485_fd = 0;
	close(rslen_fd);
	rslen_fd = 0;
}

int PtzInit(PROTOCOL_PELCO_E protocal, UART_BAUD_RATE_E baudrate, HI_U8 dev_addr)
{
	int ret = 0;

	ptz.protocol  = protocal;
	ptz.baudrate  = baudrate;
	ptz.dev_addr  = dev_addr;

	ptz.lr_spd    = 0x4*8-1;
	ptz.ud_spd    = 0x4*8-1;
	ptz.pp_idx    = 0;
	ptz.cruise_line        = 0;
	ptz.cruise_point_nums  = 0;
	ptz.cruise_point_time  = 2;
	ptz.cruise_point_spd   = 0x4*8-1;
	ptz.cruise_point_index = 0;
	ptz.cruise_state       = 0;

	ptz.cmd       = 0x00;
	ptz.dat       = 0x00;

	ptz.cal_ret   = HI_FALSE;
	ptz.af_flag   = HI_FALSE;
	ptz.ptz_flag  = HI_FALSE;
	ptz.cmd_pairs = HI_FALSE;
	ptz.ptz_cmd   = PTZ_CMD_END;

	system("himm 0x200f007c 0x1");  // mux UART1_RXD
	system("himm 0x200f0080 0x0");  // mux gpio9_4
	system("himm 0x200f0084 0x1");  // mux UART1_TXD
	
	//system("himm 0x201d0400 0x10"); // gpio9_4 output
	//system("himm 0x201d0040 0x10"); // gpio9_4 output 1

	ret = UartInit(baudrate, 8, 'n', 1);

	SendPtzOn();

	return ret;
}

int PtzExit(void)
{
	UartExit();
	
	return 0;
}

static int SendPtzCmd(void)
{
	int i = 0;
	int ret = 0;
	unsigned char index = 0;
	unsigned char buf[128] = {0};

	if (ptz.protocol == PROTOCOL_PELCO_P)
	{
		buf[index++] = 0xa0;
	}
	else
	{
		buf[index++] = 0xff;
	}

	buf[index++] = ptz.dev_addr;

	buf[index++] = ptz.cmd >> 8;
	buf[index++] = ptz.cmd >> 0;

	buf[index++] = ptz.dat >> 8; // horizontal speed
	buf[index++] = ptz.dat >> 0; // vertical speed

	if (ptz.protocol == PROTOCOL_PELCO_P)
	{
		buf[index++] = 0xaf;
		buf[index++] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3] ^ buf[4] ^ buf[5];
	}
	else
	{
		buf[index++] = (buf[1] + buf[2] + buf[3] + buf[4] + buf[5]) % 256;
	}

	printf("PtzCmd--->");
	for (i=0; i<index; i++)
	{
		printf("0x%x ", buf[i]);
	}
	printf("\n");

	ret = write(rs485_fd, buf, index);
	if (ret < 0)
	{
		printf("PTZ write failed!\n");
	}

	return ret;
}

static int SendPtzStop(void)
{
	int ret = 0;
	unsigned char index = 0;
	unsigned char buf[128] = {0};

	if (ptz.protocol == PROTOCOL_PELCO_P)
	{
		buf[index++] = 0xa0;
	}
	else
	{
		buf[index++] = 0xff;
	}

	buf[index++] = ptz.dev_addr;
	buf[index++] = 0;
	buf[index++] = 0;
	buf[index++] = 0;
	buf[index++] = 0;

	if (ptz.protocol == PROTOCOL_PELCO_P)
	{
		buf[index++] = 0xaf;
		buf[index++] = buf[0] ^ buf[1] ^ buf[2] ^ buf[3] ^ buf[4] ^ buf[5];
	}
	else
	{
		buf[index++] = (buf[1] + buf[2] + buf[3] + buf[4] + buf[5]) % 256;
	}

	ret = write(rs485_fd, buf, index);
	if (ret < 0)
	{
		printf("PTZ write failed!\n");
	}

	return ret;
}

int SendPtzOn(void)
{
	int ret = -1;
	unsigned char index = 0;
	unsigned char buf[128] = {0};

	index = 0;
	buf[index++] = 0x90;
	buf[index++] = 0x38;
	buf[index++] = 0xff;
	ret = write(rslen_fd, buf, index);
	
	index = 0;
	buf[index++] = 0x90;
	buf[index++] = 0x38;
	buf[index++] = 0xff;
	ret = write(rslen_fd, buf, index);

	return ret;
}

static int SendPtzAck(void)
{
	int ret = -1;
	unsigned char index = 0;
	unsigned char buf[128] = {0};

	index = 0;
	buf[index++] = 0x90;
	buf[index++] = 0x41;
	buf[index++] = 0xff;
	ret = write(rslen_fd, buf, index);

	index = 0;
	buf[index++] = 0x90;
	buf[index++] = 0x51;
	buf[index++] = 0xff;
	ret = write(rslen_fd, buf, index);

	return ret;
}

static unsigned int GetZoomDis(void)
{
	unsigned int zp = 0;

	zp = CMD_GetZoomPoint();
	if (zp >= 5)
	{
		zp -= 3;
	}
	else if ((zp == 3) || (zp == 4))
	{
		zp = 1;
	}
	else
	{
		zp = 0;
	}

	return zp;
}

static int GetZoomVisca(unsigned int zp_visca)
{
	unsigned int i = 0;

	for (i=0; i<40; i++)
	{
		if (zp_visca == VISCA_ZOOM_RATIO[i])
		{
			return i;
		}
	}

	return -1;
}

static HI_BOOL CheckTourInvalid(void)
{
	int i = 0;

	if (ptz.cruise_line >= MAX_CRUISE_ROUTE)
	{
		return HI_FALSE;
	}

	for (i=0; i<MAX_CRUISE_ROUTE_PRESENT; i++)
	{
		if(ptz.ptz_conf_dat.Cruise_Config.cruise_route[0][ptz.cruise_line].cruise_point[i].ucEnable == 0)
		{
			break;
		}
	}

	ptz.cruise_point_nums = i;

	if (ptz.cruise_point_nums > 0 )
	{
		return HI_TRUE;
	}

	return HI_FALSE;
}

static void PtzTourHandler(int dat)
{
	HI_U8 time, spd, index;
	HI_U8 line  = ptz.cruise_line;
	HI_U8 point = ptz.cruise_point_index;

	time  = ptz.ptz_conf_dat.Cruise_Config.cruise_route[0][line].cruise_point[point].ucCruiseTime;
	spd   = ptz.ptz_conf_dat.Cruise_Config.cruise_route[0][line].cruise_point[point].ucCruiseSpeed;
	index = ptz.ptz_conf_dat.Cruise_Config.cruise_route[0][line].cruise_point[point].index + 1;

	ptz.cruise_point_time = time;
	ptz.cruise_point_spd  = (spd*8) / 5;
	if (ptz.cruise_point_spd > 0x3f)
	{
		spd = 0x3f;
	}
	else
	{
		spd = ptz.cruise_point_spd;
	}

	printf("line=%d, point=%d, time=%d, spd=%d, index=%d\n", line, point, time, spd, index);
	ptz.cmd = PELCO_CMD_PRE_GOTO;
	ptz.dat = (spd << 8) + index;
	SendPtzCmd();

	ptz.cruise_point_index++;
	if (ptz.cruise_point_index >= ptz.cruise_point_nums)
	{
		ptz.cruise_point_index = 0;
	}
}

static int PtzToureProc(void *dat)
{
	struct sigaction act;
	union sigval tsval;
	time_t tBegin, tEnd;

	act.sa_handler = PtzTourHandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(50, &act, NULL);

	time(&tBegin);
	ptz.cruise_point_time  = 0;
	ptz.cruise_point_index = 0;
	while (ptz.cruise_state)
	{
		time(&tEnd);
		if (abs(tBegin - tEnd )>= ptz.cruise_point_time)
		{
			sigqueue(getpid(), 50, tsval);
			tBegin = tEnd;
		}
		usleep(200);
	}
	pthread_detach(pthread_self());

	return 0;
}

static int StartTour(void)
{
	int ret = 0;
	pthread_t p_thread;

	printf("StartTour......\n");
	if (CheckTourInvalid() == HI_FALSE)
	{
		printf("Invalid Line!\n");
		return -1;
	}

	if (0 == ptz.cruise_state)
	{
		ptz.cruise_state = 1;
		ret = pthread_create(&p_thread, NULL, (void *)&PtzToureProc, NULL);
		if (ret != 0)
		{
			printf("Create thread PtzToureProc err!!!\n");
			return -1;
		}
	}

	return 0;
}

static int StopTour(void)
{
	printf("StopTour......\n");
	ptz.cruise_state = 0;
	usleep(500);
	SendPtzStop();

	return 0;
}

HI_BOOL GetCalResult(void)
{
	return ptz.cal_ret;
}

int SetPtzCmd(HI_U32 cmd, HI_VOID *pValue)
{
	SysInfo_PtzOption *pSysInfo_PtzOption = (SysInfo_PtzOption *)pValue;

	if ( (cmd >= PTZ_CMD_END) || (pValue == NULL) )
	{
		printf("Invalid cmd=%d.\n", cmd);
		return -1;
	}

	printf("SetPtzCmd=%d\n", cmd);
	if (PTZ_CMD_ZOOM_STOP == cmd)
	{
		pSysInfo_PtzOption->pSysInfo->Ptz_config.ucZoomPos = GetZoomDis()*10;
		printf("OCX zp=%d\n", pSysInfo_PtzOption->pSysInfo->Ptz_config.ucZoomPos);
	}
	else if (PTZ_CMD_INITLENS == cmd)
	{
		pSysInfo_PtzOption->pSysInfo->Ptz_config.ucZoomPos = 0;
		printf("OCX zp=%d\n", pSysInfo_PtzOption->pSysInfo->Ptz_config.ucZoomPos);
	}
	else if (PTZ_CMD_TOUR_START == cmd)
	{
		ptz.cruise_line = pSysInfo_PtzOption->pPtzOption->ucCruiseLineIndex;
		memcpy(&ptz.ptz_conf_dat, &pSysInfo_PtzOption->pSysInfo->Ptz_Config_Ex, sizeof(Ptz_Config_Data_Ex));
	}
	else
	{
		ptz.lr_spd = pSysInfo_PtzOption->pSysInfo->Ptz_config.ucStep*8 - 1;
		ptz.ud_spd = pSysInfo_PtzOption->pSysInfo->Ptz_config.ucStep*8 - 1;
		ptz.pp_idx = pSysInfo_PtzOption->pPtzOption->ucPresetPointIndex + 1;
		printf("spd=%d index=%d\n", ptz.lr_spd, ptz.pp_idx); // 00 - 3F
	}

	ptz.ptz_cmd = cmd;

	return 0;
}

static int WaitAFExit(void)
{
	unsigned int cnt = 0;

	SetAFGate(HI_FALSE);
	while (ptz.af_flag && cnt++<300 ) 
	{
		if (cnt == 256) printf("\n\n\n------WaitAFExit!!!------\n\n\n");
		usleep(10*1000);
	}
	SetAFGate(HI_TRUE);

	return 0;
}

static int SetAFFlag(HI_BOOL af_flag)
{
	if (af_flag == ptz.af_flag) return 1;

	ptz.af_flag = af_flag;

	return 0;
}

static int PtzRollCtrl(PtzControlCmd cmd)
{
	int ret = -1;

	switch (cmd)
	{
		case PTZ_CMD_LEFT:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_LEFT;
			ptz.dat = (ptz.lr_spd << 8);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_RIGHT:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_RIGHT;
			ptz.dat = (ptz.lr_spd << 8);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_UP:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_UP;
			ptz.dat = (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_DOWN:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_DOWN;
			ptz.dat = (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;

		case PTZ_CMD_LEFTUP:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_LEFTUP;
			ptz.dat = (ptz.lr_spd << 8) + (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_LEFTDOWN:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_LEFTDOWN;
			ptz.dat = (ptz.lr_spd << 8) + (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_RIGHTUP:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_RIGHTUP;
			ptz.dat = (ptz.lr_spd << 8) + (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_RIGHTDOWN:
			WaitAFExit();
			ptz.cmd = PELCO_CMD_RIGHTDOWN;
			ptz.dat = (ptz.lr_spd << 8) + (ptz.ud_spd << 0);
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;

		case PTZ_CMD_PRESET_SET:
			ptz.cmd = PELCO_CMD_PRE_SET;
			ptz.dat = ptz.pp_idx;
			SendPtzCmd();
			break;
		case PTZ_CMD_PRESET_CLEAR:
			ptz.cmd = PELCO_CMD_PRE_CLEAR;
			ptz.dat = ptz.pp_idx;
			SendPtzCmd();
			break;
		case PTZ_CMD_PRESET_GOTO:
			ptz.cmd = PELCO_CMD_PRE_GOTO;
			ptz.dat = ptz.pp_idx;
			SendPtzCmd();
			break;
		case PTZ_CMD_STOP:
			SendPtzStop();
			if (HI_FALSE == ptz.cmd_pairs)    //tzq@20160627 ocx send two more stop cmd case do OneKeyFocus again.
			{
				break;
			}
			ptz.cmd_pairs = HI_FALSE;
			break;

		case PTZ_CMD_TOUR_START:
			StartTour();
			break;
		case PTZ_CMD_TOUR_STOP:
			StopTour();
			break;
		default:
			break;
	}

	return ret;
}

static int PtzLensCtrl(PtzControlCmd cmd)
{
	int ret = -1;
	
	switch(cmd)
	{
		//case PTZ_CMD_ZOOM_WIDE:
		case PTZ_CMD_ZOOM_TELE:        //tzq@20160726 change by protocol define
			WaitAFExit();
			ret = CMD_LensZoomMinus();
			if (ret != 0) return AF_PTZ_ZOOM_LIMIT;
			ptz.cmd = PELCO_CMD_ZOOMOUT;
			ptz.dat = 0;
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		//case PTZ_CMD_ZOOM_TELE:
		case PTZ_CMD_ZOOM_WIDE:        //tzq@20160726 change by protocol define
			WaitAFExit();
			ret = CMD_LensZoomPlus();
			if (ret != 0) return AF_PTZ_ZOOM_LIMIT;
			ptz.cmd = PELCO_CMD_ZOOMIN;
			ptz.dat = 0;
			SendPtzCmd();
			ptz.cmd_pairs = HI_TRUE;
			break;
		case PTZ_CMD_FOCUS_FAR:
			WaitAFExit();
			ret = CMD_LensFocPlus(1);
			if (ret != 0) return AF_PTZ_FOCUS_LIMIT;
			//ptz.cmd = PELCO_CMD_FOCUSFAR;
			//ptz.dat = 0;
			//SendPtzCmd();
			break;
		case PTZ_CMD_FOCUS_NEAR:
			WaitAFExit();
			ret = CMD_LensFocMinus(1);
			if (ret != 0) return AF_PTZ_FOCUS_LIMIT;
			//ptz.cmd = PELCO_CMD_FOCUSNEAR;
			//ptz.dat = 0;
			//SendPtzCmd();
			break;
		case PTZ_CMD_ONEKEYFOCUS:
			WaitAFExit();
			CMD_OneKeyFocus();
			break;
		case PTZ_CMD_INITLENS:
			printf("PTZ_CMD_INITLENS...\n");
			WaitAFExit();
			WriteZoomData(0);
			CMD_LensInit();
			usleep(500*1000);
			CMD_OneKeyFocus();
			break;
		case PTZ_CMD_ZOOM_STOP:
			if (HI_FALSE == ptz.cmd_pairs)    //tzq@20160627 ocx send two more stop cmd case do OneKeyFocus again.
			{
				SendPtzStop();
				break;
			}

			SendPtzStop();
			WriteZoomData(CMD_GetZoomPoint());
			//SetAFGate(HI_TRUE);
			//ptz.af_flag = HI_TRUE;
			ptz.cmd_pairs = HI_FALSE;
			break;
		case PTZ_CMD_FOCUS_STOP:
			SendPtzStop();
			break;
		case PTZ_CMD_LENS_CORRECTION:
			ptz.cal_ret = HI_FALSE;
			ptz.cal_ret = doCal();
			break;
		default:
			break;
	}

	return ret;
}

static int PtzUartCtrl(char *dat, int lenght)
{
	int ret = -1;
	unsigned int i, zp, zp_cur;
	unsigned int z_ratio = 0;
	unsigned char index = 0;
	unsigned char buf[128] = {0};

	if ( (sizeof(VISCA_SET_ZOOM_POS) == lenght) && (memcmp(VISCA_SET_ZOOM_POS, dat, lenght) == 0) )
	{
		zp = CMD_GetZoomPoint();
		buf[index++] = 0x90;
		buf[index++] = 0x50;
		//buf[index++] = VISCA_ZOOM_RATIO[zp] >> 0  & 0x0f;
		//buf[index++] = VISCA_ZOOM_RATIO[zp] >> 4  & 0x0f;
		//buf[index++] = VISCA_ZOOM_RATIO[zp] >> 8  & 0x0f;
		//buf[index++] = VISCA_ZOOM_RATIO[zp] >> 12 & 0x0f;
		buf[index++] = VISCA_ZOOM_RATIO[zp] >> 12 & 0x0f;
		buf[index++] = VISCA_ZOOM_RATIO[zp] >> 8  & 0x0f;
		buf[index++] = VISCA_ZOOM_RATIO[zp] >> 4  & 0x0f;
		buf[index++] = VISCA_ZOOM_RATIO[zp] >> 0  & 0x0f;
		buf[index++] = 0xff;
		ret = write(rslen_fd, buf, index);
		printf("Lens reply:0x=%x 0x=%x 0x=%x 0x=%x 0x=%x 0x=%x 0x=%x\n", \
			   	buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6]);
	}
	else if ( (sizeof(VISCA_GET_ZOOM_POS) == lenght) && (memcmp(VISCA_GET_ZOOM_POS, dat, 4) == 0) )
	{
		z_ratio = (dat[4] << 12) + (dat[5] << 8) + (dat[6] << 4) + (dat[7] << 0);
		zp = GetZoomVisca(z_ratio);
		printf("preset zp=%d\n", zp);
		zp_cur = CMD_GetZoomPoint();
		if (zp > zp_cur)
		{
			i = zp - zp_cur;
			if (i >= ZOOM_POINT_MAX) return -1;
			
			while(i--)
			{
				CMD_LensZoomPlus();
			}
		}
		else if (zp < zp_cur)
		{
			i = zp_cur - zp;
			if (i >= ZOOM_POINT_MAX) return -1;

			while(i--)
			{
				CMD_LensZoomMinus();
			}
		}

		//WaitAFExit();                    // tzq@20170117 when roll af then stop not do af again
		//SetAFGate(HI_TRUE);
		//ptz.af_flag = HI_TRUE;
		SetAFFlag(HI_TRUE);
		tcflush(rslen_fd, TCIOFLUSH);    // tzq@20170116 clear uart buf
	}
	else if ( (sizeof(VISCA_AF_TRIGGER) == lenght) && (memcmp(VISCA_AF_TRIGGER, dat, lenght) == 0) )
	{
		//WaitAFExit();
		//SetAFGate(HI_TRUE);
		//ptz.af_flag = HI_TRUE;
		SetAFFlag(HI_TRUE);
	}
	else
	{
		ret = SendPtzAck();
	}

	//if (ret < 0)
	//{
	//	printf("PTZ write failed!\n");
	//}

	return ret;
}

int PtzCtrl(void)
{
	int ret = -1;
	//struct timeval tBegin, tEnd;

	//if (ptz.ptz_flag == HI_TRUE) return -1;
	if (ptz.ptz_cmd  >= PTZ_CMD_END) return -1;

	//gettimeofday(&tBegin, NULL);	
	//ptz.ptz_flag = HI_TRUE;
	
	ret = PtzRollCtrl(ptz.ptz_cmd);
	ret = PtzLensCtrl(ptz.ptz_cmd);
	if (ret != 0)
	{
		if (ret == AF_PTZ_ZOOM_LIMIT)
		{
			WriteZoomData(CMD_GetZoomPoint());
			//SetAFGate(HI_TRUE);
			//ptz.af_flag = HI_TRUE;
			SetAFFlag(HI_TRUE);
			ptz.cmd_pairs = HI_FALSE;
		}
		ptz.ptz_cmd = PTZ_CMD_END;
	}
	
	//ptz.ptz_flag = HI_FALSE;
	//gettimeofday(&tEnd, NULL);
	//long deltaTime = 1000000L*(tEnd.tv_sec-tBegin.tv_sec)+(tEnd.tv_usec-tBegin.tv_usec);
	//memcpy(&tBegin, &tEnd, sizeof(struct timeval));
	//printf("Time spent: %ldus\n", deltaTime);

	return ret;
}

void AFProc(void *arg)
{
	while (1)
	{
		if ( (HI_TRUE == GetAFGate() )  &&
			 (HI_TRUE == ptz.af_flag) )
		{
			//ptz.cmd_pairs = HI_FALSE;
			usleep(500*1000);
			CMD_OneKeyFocus();
			ptz.af_flag   = HI_FALSE;
			printf("AF Exit!!!\n");
		}

		//if (ptz.ptz_flag == HI_FALSE)
		//{
		//	FV_MapISO();
		//}

		usleep(100*1000);
	}
}

void PtzProc(void *arg)
{
	int ret = -1;
	int  i, nread = 0;
	char buff[128] = {0};

	while (1)
	{
		ret = PtzCtrl();

		if (ret == AF_PTZ_ZOOM_LIMIT)
		{
			tcflush(rslen_fd, TCIOFLUSH);
			continue;
		}
		
		if (ptz.ptz_cmd  != PTZ_CMD_END) // tzq@20170113 zoom in/out case AF Trigger error!
		{
			tcflush(rslen_fd, TCIOFLUSH);
			continue;
		}
		
		while (nread = read(rslen_fd, buff, sizeof(buff)))
		{
			if (nread <= 0) break;

			printf("\nLen %d\n",nread);
			buff[nread+1] = '\0';
			for (i=0; i<nread; i++)
			{
				printf( "0x=%x ", buff[i]);
			}
			printf("\n");

			PtzUartCtrl(buff, nread);
		}

		usleep(1000);
	}
}

int CreatePtzThread(void)
{
	int ret = 0;
	pthread_t p_thread;

	//ret = PtzInit(PROTOCOL_PELCO_D, UART_BAUD_RATE_9600, 0x1);
	//if (ret != 0)
	//{
	//	printf("PtzInit failed ret=%d\n", ret);
	//}

	ret = pthread_create(&p_thread, NULL, (void *)&AFProc, NULL);
	if (ret != 0)
	{
		printf("pthread_create AFProc err...\n");
	}

	ret = pthread_create(&p_thread, NULL, (void *)&PtzProc, NULL);
	if (ret != 0)
	{
		printf("pthread_create PtzProc err...\n");
	}

	return ret;
}

