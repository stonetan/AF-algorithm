#include <linux/module.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
 #include <linux/delay.h>

#include <asm/uaccess.h>

#include "ls_lens_drv.h"
#include "ls_lens_41908.h"


/* debug */
#define LSLENS_PFX "LsLens: "
#define lslens_dbg(params...) printk(KERN_INFO LSLENS_PFX params)

typedef struct
{
	MOTOR_DIR_E   focus_dir;
	unsigned int  focus_pos;
	unsigned char focus_cnt;
	MOTOR_SPEED_S focus_spd;

	MOTOR_DIR_E   zoom_dir;
	unsigned int  zoom_pos;
	unsigned char zoom_cnt;
	MOTOR_SPEED_S zoom_spd;

	unsigned int  zoom_point;
}LENS_STATUS_S;

static LENS_STATUS_S sLens = {0};
static unsigned int foc_cal_tbl[ZOOM_POINT_MAX] = {0};

void LensParamInit(void)
{
	sLens.focus_dir = MOTOR_FWD;
	sLens.focus_pos = FOC_HOME_POS;
	sLens.focus_cnt = 0;
	sLens.focus_spd = MOTOR_SPEED_MID;

	sLens.zoom_dir  = MOTOR_FWD;
	sLens.zoom_pos  = ZOOM_HOME_POS;
	sLens.zoom_cnt  = 0;
	sLens.zoom_spd  = MOTOR_SPEED_MID;
	sLens.zoom_point= 0;

	SetZoomSpeed(sLens.zoom_spd);
	SetFocusSpeed(sLens.focus_spd);
}

int ZoomHomeInit(void)
{
	unsigned int pos_cnt = 0;
	unsigned char zoom_pi = 0;

	ZoomPIEnable();
	msleep(10);

	pos_cnt = 0;
	zoom_pi = GetZoomPIFlag();
	printk("zoom_pi_0=%d\n", zoom_pi);
	if (zoom_pi) {
		ZoomForward();
	}
	else {
		ZoomReverse();
	}
	do {
		ZoomMove(10);
		pos_cnt += 10;
		if (pos_cnt > ZOOM_POS_RANGE) return -1;
		if (GetZoomPIFlag() != zoom_pi) break;
	} while(1);

	pos_cnt = 0;
	zoom_pi = GetZoomPIFlag();
	printk("zoom_pi_1=%d\n", zoom_pi);
	if (zoom_pi) {
		ZoomForward();
	}
	else {
		ZoomReverse();
	}
	do {
		ZoomMove(1);
		pos_cnt += 1;
		if (pos_cnt > ZOOM_POS_RANGE) return -1;
		//if (GetZoomPIFlag() != zoom_pi) break;
		if (GetZoomPIFlag() != zoom_pi) //tzq@20161012 add for HOME_POS at low
		{
			if (!zoom_pi)
			{
				zoom_pi = 1;
				pos_cnt = 0;
				ZoomForward();
			}
			else break;
		}
	} while(1);

	sLens.zoom_pos = ZOOM_HOME_POS;

	ZoomPIDisable();
	msleep(10);

	return 0;
}

int FocusHomeInit(void)
{
	unsigned int pos_cnt = 0;
	unsigned char focus_pi = 0;

	FocusPIEnable();
	msleep(10);

	pos_cnt = 0;
	focus_pi = GetFocusPIFlag();
	printk("focus_pi_0=%d\n", focus_pi);
	if (focus_pi) {
		FocusForward();
	}
	else {
		FocusReverse();
	}
	do {
		FocusMove(10);
		pos_cnt += 10;
		if (pos_cnt > FOC_POS_RANGE) return -1;
		if (GetFocusPIFlag() != focus_pi) break;
	} while(1);

	pos_cnt = 0;
	focus_pi = GetFocusPIFlag();
	printk("focus_pi_1=%d\n", focus_pi);
	if (focus_pi) {
		FocusForward();
	}
	else {
		FocusReverse();
	}
	do {
		FocusMove(1);
		pos_cnt += 1;
		if (pos_cnt > FOC_POS_RANGE) return -1;
		//if (GetFocusPIFlag() != focus_pi) break;
		if (GetFocusPIFlag() != focus_pi) //tzq@20161012 add for HOME_POS at low
		{
			if (!focus_pi)
			{
				focus_pi= 1;
				pos_cnt = 0;
				FocusForward();
			}
			else break;
		}
	} while(1);

	sLens.focus_pos = FOC_HOME_POS;

	FocusPIDisable();
	msleep(10);

	return 0;
}

int SetZoomDir(MOTOR_DIR_E dir)
{
	if (dir == MOTOR_FWD)
	{
		ZoomForward();
	}
	else
	{
		ZoomReverse();
	}
	sLens.zoom_dir = dir;

	return 0;
}

MOTOR_DIR_E GetZoomDir(void)
{
	return sLens.zoom_dir;
}

int SetFocusDir(MOTOR_DIR_E dir)
{
	if (dir == MOTOR_FWD)
	{
		FocusForward();
	}
	else
	{
		FocusReverse();
	}
	sLens.focus_dir = dir;

	return 0;
}

MOTOR_DIR_E GetFocusDir(void)
{
	return sLens.focus_dir;
}

void SetZoomPos(unsigned int pos)
{
	if (pos == 0) return;

	if (sLens.zoom_dir == MOTOR_REV)
	{
		if ((sLens.zoom_pos + pos) > (ZOOM_POS_FRONT_D+ZOOM_HOME_POS))
		{
			pos = (ZOOM_POS_FRONT_D+ZOOM_HOME_POS) - sLens.zoom_pos;
		}
		sLens.zoom_pos += pos;
	}
	else
	{
		if ((sLens.zoom_pos - pos) < (ZOOM_POS_REVER_D+ZOOM_HOME_POS))
		{
			pos = sLens.zoom_pos - (ZOOM_POS_REVER_D+ZOOM_HOME_POS);
		}
		sLens.zoom_pos -= pos;
	}
	ZoomMove(pos);
}

unsigned int GetZoomPos(void)
{
	return sLens.zoom_pos;
}

void SetFocusPos(unsigned int pos)
{
	if (pos == 0) return;

	if (sLens.focus_dir == MOTOR_REV)
	{
		if ((sLens.focus_pos + pos) > (FOC_POS_FRONT_D+FOC_HOME_POS))
		{
			pos = (FOC_POS_FRONT_D+FOC_HOME_POS) - sLens.focus_pos;
		}
		sLens.focus_pos += pos;
	}
	else
	{
		if ((sLens.focus_pos - pos) < (FOC_POS_REVER_D+FOC_HOME_POS))
		{
			pos = sLens.focus_pos - (FOC_POS_REVER_D+FOC_HOME_POS);
		}
		sLens.focus_pos -= pos;
	}
	FocusMove(pos);
}

unsigned int GetFocusPos(void)
{
	return sLens.focus_pos;
}

int SetZoomPoint(unsigned int zp)
{
	int steps = 0;
	unsigned int z_pos, f_pos;
	MOTOR_DIR_E dir;

	if (zp >= ZOOM_POINT_MAX) return -1;

	sLens.zoom_point = zp;
	steps = ZOOM_TBL[zp] + ZOOM_HOME_POS - sLens.zoom_pos;
	dir = (steps >= 0 ? MOTOR_REV:MOTOR_FWD);
	SetZoomDir(dir);
	sLens.zoom_pos += steps;
	z_pos = (steps >= 0 ? steps:-steps);

	//steps = FOC_TBL[zp] + FOC_HOME_POS - sLens.focus_pos;
	steps = foc_cal_tbl[zp] - sLens.focus_pos;
	dir = (steps >= 0 ? MOTOR_REV:MOTOR_FWD);
	SetFocusDir(dir);
	sLens.focus_pos += steps;
	f_pos = (steps >= 0 ? steps:-steps);

	if ((z_pos == 0) && (f_pos == 0)) return 0;
/*
	//z_pos = 10; f_pos=100;
	if (z_pos > f_pos)
	{
		//SetZoomSpeed(MOTOR_SPEED_MID);
		SetFocusSpeed((MOTOR_SPEED_MID*f_pos*2+z_pos)/(2*z_pos));
	}
	else
	{
		//SetFocusSpeed(MOTOR_SPEED_MID);
		SetZoomSpeed((MOTOR_SPEED_MID*z_pos*2+f_pos)/(2*f_pos));
	}
*/
	SyncMove(z_pos, f_pos);

//	SetZoomSpeed(sLens.zoom_spd);
//	SetFocusSpeed(sLens.focus_spd);

	return 0;
}

unsigned int GetZoomPoint(void)
{
	return sLens.zoom_point;
}

int LensInit(void)
{
	int ret = 0;
	int steps = 0;
	unsigned int pos;
	MOTOR_DIR_E dir;

	ret |= FocusHomeInit();
	ret |= ZoomHomeInit();

	//ret |= SetZoomPoint(0);
	steps = ZOOM_TBL[0] + ZOOM_HOME_POS - sLens.zoom_pos;
	dir = (steps >= 0 ? MOTOR_REV:MOTOR_FWD);
	pos = (steps >= 0 ? steps:-steps);
	SetZoomDir(dir);
	SetZoomPos(pos);
	sLens.zoom_point = 0;

	steps = foc_cal_tbl[0] - sLens.focus_pos;
	dir = (steps >= 0 ? MOTOR_REV:MOTOR_FWD);
	pos = (steps >= 0 ? steps:-steps);
	SetFocusDir(dir);
	SetFocusPos(pos);

	return ret;
}

//------------------------------------------------------------------------------------------

/* file operation */

static int lslens_open(struct inode* inode, struct file* file)
{
	return 0;
}

static int lslens_close(struct inode* inode, struct file* file)
{
	return 0;
}

ssize_t lslens_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	if (count != sizeof(foc_cal_tbl))
	{
		return -EFAULT;
	}

	if (copy_from_user(foc_cal_tbl, buf, count))
	{
		return -EFAULT;
	}

	return count;
}

ssize_t lslens_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return count;
}

static long lslens_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
	int err=0, ret;
	unsigned int zp, pos;
	unsigned short reg_value = 0;
	MOTOR_DIR_E dir;

	if (_IOC_TYPE(cmd) != LENS_IOC_MAGIC)
	{
		return -ENOTTY;
	}
	if (_IOC_NR(cmd) > LENS_IOC_MAXNR)
	{
		return -ENOTTY;
	}
	if (_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok(VERIFY_WRITE, (void __user*)arg, _IOC_SIZE(cmd));
	}
	if (_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok(VERIFY_READ, (void __user*)arg, _IOC_SIZE(cmd));
	}
	if (err)
	{
		return -ENOTTY;
	}

	lslens_dbg("ioctl->cmd=%#x\n", cmd);
	switch (cmd)
	{
		case LENS_IOC_INIT:
			LensInit();
			break;
		case LENS_IOC_FOCPLUS:
			ret = __get_user(pos, (unsigned int __user*)arg);
			if (0 == ret)
			{
				SetFocusDir(MOTOR_REV);
				SetFocusPos(pos);
			}
			break;
		case LENS_IOC_FOCMINUS:
			ret = __get_user(pos, (unsigned int __user*)arg);
			if (0 == ret)
			{
				SetFocusDir(MOTOR_FWD);
				SetFocusPos(pos);
			}
			break;
		case LENS_IOC_ZOOMPLUS:
			zp = GetZoomPoint();
			if (zp < ZOOM_POINT_MAX-1)
			{
				SetZoomPoint(++zp);
			}
			break;
		case LENS_IOC_ZOOMMINUS:
			zp = GetZoomPoint();
			if (zp > 0)
			{
				SetZoomPoint(--zp);
			}
			break;
		case LENS_IOC_SETFOCDIR:
			ret = __get_user(dir, (MOTOR_DIR_E __user*)arg);
			if (0 == ret)
			{
				SetFocusDir(dir);
			}
			break;
		case LENS_IOC_GETFOCDIR:
			dir = GetFocusDir();
			__put_user(dir, (MOTOR_DIR_E __user*)arg);
			break;
		case LENS_IOC_SETFOCPOS:
			ret = __get_user(pos, (unsigned int __user*)arg);
			if (0 == ret)
			{
				SetFocusPos(pos);
			}
			break;
		case LENS_IOC_GETFOCPOS:
			pos = GetFocusPos();
			__put_user(pos, (unsigned int __user*)arg);
			break;
		case LENS_IOC_SETZOOMPOINT:
			ret = __get_user(zp, (unsigned int __user*)arg);
			if (0 == ret)
			{
				SetZoomPoint(zp);
			}
			break;
		case LENS_IOC_GETZOOMPOINT:
			zp = GetZoomPoint();
			__put_user(zp, (unsigned int __user*)arg);
			break;
		case LENS_IOC_GETFOCREG:
			reg_value = GetFocusReg();
			__put_user(reg_value, (unsigned short __user*)arg);
			break;
		default:
			{
				lslens_dbg("invalid ioctl command!\n");
				return -ENOIOCTLCMD;
			}
	}

	return 0;
}

 static struct file_operations lslens_fops = {
 	.owner          = THIS_MODULE,
	.read           = lslens_read,
	.write          = lslens_write,
	.unlocked_ioctl = lslens_ioctl,
	.open           = lslens_open,
	.release        = lslens_close,
 };

static struct miscdevice lslens_miscdev = {
	.minor        = MISC_DYNAMIC_MINOR,
	.name         = "lslens",
	.fops         = &lslens_fops,
};

static int __init lslens_init(void)
{
	int ret = 0;

	lslens_dbg("lslens_init.\n");
	ret = misc_register(&lslens_miscdev);
	if (ret != 0)
	{
		lslens_dbg("register lslens device failed with %#x!\n", ret);
		return -1;
	}

	AN41908_Init();
	LensParamInit();
	//LensInit();
	//AN41908_Test();

	return 0;
}

static void __exit lslens_exit(void)
{
	lslens_dbg("lslens_exit.\n");
	misc_deregister(&lslens_miscdev);

	AN41908_Exit();
}

module_init(lslens_init);
module_exit(lslens_exit);

MODULE_DESCRIPTION("Longse lens driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("TZQ");

