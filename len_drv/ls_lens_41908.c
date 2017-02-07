#include <linux/delay.h>

#include <asm/io.h>

#include "ls_lens_41908.h"
#include "ls_lens_spi.h"
#include "ls_lens_pwm.h"
#include "ls_lens_timer.h"


#define MUX_ADDRESS_BASE          0x200F0000
#define GPIO0_ADDRESS_BASE        0x20140000
#define GPIO1_ADDRESS_BASE        0x20150000
#define GPIO2_ADDRESS_BASE        0x20160000
#define GPIO3_ADDRESS_BASE        0x20170000
#define GPIO4_ADDRESS_BASE        0x20180000
#define GPIO5_ADDRESS_BASE        0x20190000
#define GPIO6_ADDRESS_BASE        0x201A0000
#define GPIO7_ADDRESS_BASE        0x201B0000
#define GPIO8_ADDRESS_BASE        0x201C0000
#define GPIO9_ADDRESS_BASE        0x201D0000
#define GPIO10_ADDRESS_BASE       0x201E0000
#define GPIO11_ADDRESS_BASE       0x201F0000
#define GPIO12_ADDRESS_BASE       0x20200000
#define GPIO13_ADDRESS_BASE       0x20210000
#define GPIO14_ADDRESS_BASE       0x20220000
#define GPIO15_ADDRESS_BASE       0x20260000

void __iomem* reg_mux_base_va     = 0;
void __iomem* reg_gpio0_base_va   = 0;
void __iomem* reg_gpio2_base_va   = 0;
void __iomem* reg_gpio8_base_va   = 0;
void __iomem* reg_gpio9_base_va   = 0;
void __iomem* reg_gpio14_base_va  = 0;

#define HI_IO_MUX_ADDRESS(x)      (reg_mux_base_va + ((x)-(MUX_ADDRESS_BASE)))
#define HI_IO_GPIO0_ADDRESS(x)    (reg_gpio0_base_va + ((x)-(GPIO0_ADDRESS_BASE)))
#define HI_IO_GPIO2_ADDRESS(x)    (reg_gpio2_base_va + ((x)-(GPIO2_ADDRESS_BASE)))
#define HI_IO_GPIO8_ADDRESS(x)    (reg_gpio8_base_va + ((x)-(GPIO8_ADDRESS_BASE)))
#define HI_IO_GPIO9_ADDRESS(x)    (reg_gpio9_base_va + ((x)-(GPIO9_ADDRESS_BASE)))
#define HI_IO_GPIO14_ADDRESS(x)   (reg_gpio14_base_va + ((x)-(GPIO14_ADDRESS_BASE)))

#define MUX_REG(x)                HI_IO_MUX_ADDRESS(MUX_ADDRESS_BASE + x)

//GPIO
#define GPIO0_DATA                HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x000)
#define GPIO0_DIR                 HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x400)
#define GPIO0_IS                  HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x404)
#define GPIO0_IBE                 HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x408)
#define GPIO0_IEV                 HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x40C)
#define GPIO0_IE                  HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x410)
#define GPIO0_RIS                 HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x414)
#define GPIO0_MIS                 HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x418)
#define GPIO0_IC                  HI_IO_GPIO0_ADDRESS(GPIO0_ADDRESS_BASE + 0x41C)

#define GPIO2_DATA                HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x000)
#define GPIO2_DIR                 HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x400)
#define GPIO2_IS                  HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x404)
#define GPIO2_IBE                 HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x408)
#define GPIO2_IEV                 HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x40C)
#define GPIO2_IE                  HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x410)
#define GPIO2_RIS                 HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x414)
#define GPIO2_MIS                 HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x418)
#define GPIO2_IC                  HI_IO_GPIO2_ADDRESS(GPIO2_ADDRESS_BASE + 0x41C)

#define GPIO8_DATA                HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x000)
#define GPIO8_DIR                 HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x400)
#define GPIO8_IS                  HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x404)
#define GPIO8_IBE                 HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x408)
#define GPIO8_IEV                 HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x40C)
#define GPIO8_IE                  HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x410)
#define GPIO8_RIS                 HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x414)
#define GPIO8_MIS                 HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x418)
#define GPIO8_IC                  HI_IO_GPIO8_ADDRESS(GPIO8_ADDRESS_BASE + 0x41C)

#define GPIO9_DATA                HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x000)
#define GPIO9_DIR                 HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x400)
#define GPIO9_IS                  HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x404)
#define GPIO9_IBE                 HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x408)
#define GPIO9_IEV                 HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x40C)
#define GPIO9_IE                  HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x410)
#define GPIO9_RIS                 HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x414)
#define GPIO9_MIS                 HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x418)
#define GPIO9_IC                  HI_IO_GPIO9_ADDRESS(GPIO9_ADDRESS_BASE + 0x41C)

#define GPIO14_DATA               HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x000)
#define GPIO14_DIR                HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x400)
#define GPIO14_IS                 HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x404)
#define GPIO14_IBE                HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x408)
#define GPIO14_IEV                HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x40C)
#define GPIO14_IE                 HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x410)
#define GPIO14_RIS                HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x414)
#define GPIO14_MIS                HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x418)
#define GPIO14_IC                 HI_IO_GPIO14_ADDRESS(GPIO14_ADDRESS_BASE + 0x41C)

#define GPIO_WRITE_REG(Addr, Value)  ((*(volatile unsigned int *)(Addr)) = (Value))
#define GPIO_READ_REG(Addr)          (*(volatile unsigned int *)(Addr))


#define ZOOM_REG                  (0x24)
#define ZOOM_SPD                  (0x25)
#define FOC_REG                   (0x29)
#define FOC_SPD                   (0x2a)

#define ZOOM_PHASE                (4)    //2-2 phase
#define FOC_PHASE                 (8)    //1-2 phase

#define VD_FZ                     (100)  //100Hz
#define VD_TIME                   (1000/VD_FZ)
#define PWM_DUTY                  (100) // tzq@20161012 VD_TIME no include PWM_DUTY
#define PWM_PERIOD                (PWM_CLK/VD_FZ-PWM_DUTY) 
#define PWM_NUM                   (0)

static unsigned short intct_z=0, intct_f=0;
static unsigned short zoom_reg=0, focus_reg=0;

void GPIO_Init(void)
{
	unsigned int i, index;
	unsigned int reg_value = 0;

	reg_mux_base_va    = (void __iomem*)IO_ADDRESS(MUX_ADDRESS_BASE);
	reg_gpio0_base_va  = (void __iomem*)IO_ADDRESS(GPIO0_ADDRESS_BASE);
	reg_gpio2_base_va  = (void __iomem*)IO_ADDRESS(GPIO2_ADDRESS_BASE);
	reg_gpio8_base_va  = (void __iomem*)IO_ADDRESS(GPIO8_ADDRESS_BASE);
	reg_gpio9_base_va  = (void __iomem*)IO_ADDRESS(GPIO9_ADDRESS_BASE);
	reg_gpio14_base_va = (void __iomem*)IO_ADDRESS(GPIO14_ADDRESS_BASE);

	index = 1*4;
	GPIO_WRITE_REG(MUX_REG(index), 0x00);    // motor RSTB mux gpio mode

	reg_value = GPIO_READ_REG(GPIO0_DIR);    // motor RSTB output dir
	GPIO_WRITE_REG(GPIO0_DIR, (reg_value | (0x01 << 6)));

	index = 24*4;
	for (i=0; i<4; i++)                      // motor mux SPI mode
	{
		GPIO_WRITE_REG(MUX_REG(index), 0x01);
		index += 0x04;
	}

	index = 83*4;                            // motor VD_FZ,VD_IS mux PWM mode
	for (i=0; i<2; i++)
	{
		GPIO_WRITE_REG(MUX_REG(index), 0x00);
		index += 0x04;
	}

	index = 49*4;
	GPIO_WRITE_REG(MUX_REG(index), 0x00);    // ZOOM PI mux gpio mode
	index = 50*4;
	GPIO_WRITE_REG(MUX_REG(index), 0x00);    // FOC PI mux gpio mode

	reg_value = GPIO_READ_REG(GPIO2_DIR);    // ZOOM PI, FOC PI input
	GPIO_WRITE_REG(GPIO2_DIR, (reg_value & ~(0x3 << 5)));
/*
	index = 27*4;
	GPIO_WRITE_REG(MUX_REG(index), 0x00);    // motor SPI_CS mux gpio mode

	reg_value = GPIO_READ_REG(GPIO8_DIR);    // motor SPI_CS output dir
	GPIO_WRITE_REG(GPIO8_DIR, (reg_value | (0x01 << 7)));

	GPIO_WRITE_REG(GPIO8_DATA+(0x04 << 7), (0x01 << 7));
*/
}

void GPIO_Exit(void)
{
	reg_mux_base_va    = NULL;
	reg_gpio0_base_va  = NULL;
	reg_gpio2_base_va  = NULL;
	reg_gpio8_base_va  = NULL;
	reg_gpio9_base_va  = NULL;
	reg_gpio14_base_va = NULL;
}

void AN41908_Reset(void)
{
	GPIO_WRITE_REG(GPIO0_DATA+(0x04 << 6), (0x00 << 6));
	msleep(10);
	GPIO_WRITE_REG(GPIO0_DATA+(0x04 << 6), (0x01 << 6));
	msleep(10);    // tzq@20160820 add for SPI write and read error
}

void AN41908_Test(void)
{
	unsigned short val = 0;

	//SPI_Write(0x20, 0x5c0a);
	SPI_Read(0x24, &val);
	printk("Read val = %#x\n", val);

	//SPI_Test();

	//PWM_Write(PWM_NUM, 30, 30000, 10);

	SetZoomSpeed(800);
	SetFocusSpeed(800);

	ZoomMove(10);
	FocusMove(10);
}

void MotorInit(void)
{
	SPI_Write(0x0b, 0x0080);   // PLS1 output enable
	SPI_Write(0x21, 0x0085);   // PLS2 output enable; monitor output

	SPI_Write(0x20, 0x1c03);   // write register time - 20*8192/27 = 20*303.4us

	// zoom motor param init
	SPI_Write(0x22, 0x0003);   // DT2x for motor current is enough to rotation - 3*303.4us
	SPI_Write(0x23, 0xc8c8);   // motor A\B peak pulse width
	SPI_Write(0x24, 0x1800);   // 64 devisions; PI OFF
	zoom_reg = 0x1800;

	// focus motor param init
	SPI_Write(0x27, 0x0003);   // DT2x for motor current is enough to rotation - 3*303.4us
	SPI_Write(0x28, 0xc8c8);   // motor C\D  peak pulse width
	SPI_Write(0x29, 0x1800);   // 64 devisions; PI OFF
	focus_reg = 0x1800;
}

void AN41908_Init(void)
{
	GPIO_Init();

	PWM_Init();

	SPI_Init();

	AN41908_Reset();

	MotorInit();
}

void AN41908_Exit(void)
{
	GPIO_Exit();

	PWM_Exit();
}

void ZoomForward(void)
{
	zoom_reg &= ~(0x1 << 8);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomReverse(void)
{
	zoom_reg |= (0x1 << 8);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomNormal(void)
{
	zoom_reg &= ~(0x1 << 9);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomBrake(void)
{
	zoom_reg |= (0x1 << 9);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomEnable(void)
{
	zoom_reg |= (0x1 << 10);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomDisable(void)
{
	zoom_reg &= ~(0x1 << 10);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void ZoomPIEnable(void)
{
	focus_reg |= (0x1 << 11);
	SPI_Write(FOC_REG, focus_reg);
}

void ZoomPIDisable(void)
{
	focus_reg &= ~(0x1 << 11);
	SPI_Write(FOC_REG, focus_reg);
}

void SetZoomSpeed(unsigned int pps)
{
	unsigned char  psum  = 0;

	if (pps <= 100) pps = 100;

	intct_z = (27 * 1000000 * ZOOM_PHASE) / (pps * 768);
	psum    = (27 * 1000000) / (VD_FZ * intct_z * 24);
	intct_z = (27 * 1000000) / (VD_FZ * psum * 24);

	printk("SetZoomSpeed--->pps=%d, psum=%#x intct_z=%#x\n", pps, psum, intct_z);
	SPI_Write(ZOOM_SPD, intct_z);
}

static unsigned char GetZoomPsum(void)
{
	unsigned char psum = 0;

	psum = (27 * 1000000) / (VD_FZ * intct_z * 24);

	return psum;
}

void ZoomMove(unsigned int steps)
{
	unsigned int vd_nums = 0;
	unsigned char psum, psum_left;

	if (steps == 0) return;

	psum = GetZoomPsum();
	vd_nums = steps*(32/ZOOM_PHASE) / psum;
	psum_left = steps*(32/ZOOM_PHASE) % psum;
	printk("zoom-->psum=%d, vd_nums=%d, psum_left=%d\n", psum, vd_nums, psum_left);

	ZoomEnable();
	//PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, vd_nums+1);
	//PWM_Wait(PWM_NUM);

	if (vd_nums)
	{
		zoom_reg = (zoom_reg & 0xFF00) | psum;
		SPI_Write(ZOOM_REG, zoom_reg);
		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, vd_nums);
		PWM_Wait(PWM_NUM);
		msleep(vd_nums*VD_TIME+3); // +3ms must delay for DT1 and DT2
	}

	if (psum_left)
	{
		zoom_reg = (zoom_reg & 0xFF00) | psum_left;
		SPI_Write(ZOOM_REG, zoom_reg);
		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, 1);
		PWM_Wait(PWM_NUM);
		msleep(VD_TIME+3);         // +3ms must delay for DT1 and DT2
	}

	zoom_reg = (zoom_reg & 0xFF00) | 0x0;
	SPI_Write(ZOOM_REG, zoom_reg);
	ZoomDisable();
}

void FocusForward(void)
{
	focus_reg &= ~(0x1 << 8);
	SPI_Write(FOC_REG, focus_reg);
}

void FocusReverse(void)
{
	focus_reg |= (0x1 << 8);
	SPI_Write(FOC_REG, focus_reg);
}

unsigned short GetFocusReg(void)
{
	return focus_reg;
}

void FocusNormal(void)
{
	focus_reg &= ~(0x1 << 9);
	SPI_Write(FOC_REG, focus_reg);
}

void FocusBrake(void)
{
	focus_reg |= (0x1 << 9);
	SPI_Write(FOC_REG, focus_reg);
}

void FocusEnable(void)
{
	focus_reg |= (0x1 << 10);
	SPI_Write(FOC_REG, focus_reg);
}

void FocusDisable(void)
{
	focus_reg &= ~(0x1 << 10);
	SPI_Write(FOC_REG, focus_reg);
}

void FocusPIEnable(void)
{
	zoom_reg |= (0x1 << 11);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void FocusPIDisable(void)
{
	zoom_reg &= ~(0x1 << 11);
	SPI_Write(ZOOM_REG, zoom_reg);
}

void SetFocusSpeed(unsigned int pps)
{
	unsigned char  psum  = 0;

	if (pps <= 100) pps = 100;

	intct_f = (27 * 1000000 * FOC_PHASE) / (pps * 768);
	psum    = (27 * 1000000) / (VD_FZ * intct_f * 24);
	intct_f = (27 * 1000000) / (VD_FZ * psum * 24);
	
	printk("SetFocusSpeed--->pps=%d, psum=%#x intct_f=%#x\n", pps, psum, intct_f);
	SPI_Write(FOC_SPD, intct_f);
}

static unsigned char GetFocusPsum(void)
{
	unsigned char psum = 0;

	psum = (27 * 1000000) / (VD_FZ * intct_f * 24);

	return psum;
}

void FocusMove(unsigned int steps)
{
	unsigned int vd_nums = 0;
	unsigned char psum, psum_left;

	if (steps == 0) return;

	psum = GetFocusPsum();
	vd_nums = steps*(32/FOC_PHASE) / psum;
	psum_left = steps*(32/FOC_PHASE) % psum;
	printk("focus--->psum=%d, vd_nums=%d, psum_left=%d\n", psum, vd_nums, psum_left);

	FocusEnable();
	//PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, vd_nums+1);
	//PWM_Wait(PWM_NUM);

	if (vd_nums)
	{
		focus_reg = (focus_reg & 0xFF00) | psum;
		SPI_Write(FOC_REG, focus_reg);
		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, vd_nums);
		PWM_Wait(PWM_NUM);
		msleep(vd_nums*VD_TIME+3); // +3ms must delay for DT1 and DT2
	}

	if (psum_left)
	{
		focus_reg = (focus_reg & 0xFF00) | psum_left;
		SPI_Write(FOC_REG, focus_reg);
		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, 1);
		PWM_Wait(PWM_NUM);
		msleep(VD_TIME+3);         // +3ms must delay for DT1 and DT2
	}

	focus_reg = (focus_reg & 0xFF00) | 0;
	SPI_Write(FOC_REG, focus_reg);
	FocusDisable();
}

void SyncMove(unsigned int z_pos, unsigned int f_pos)
{
	unsigned int z_vds, f_vds;
	unsigned char z_psum, z_psum_left, f_psum, f_psum_left;

	if ((z_pos == 0) && (f_pos == 0)) return;

	z_psum = GetZoomPsum();
	z_vds = z_pos*(32/ZOOM_PHASE) / z_psum;
	z_psum_left = z_pos*(32/ZOOM_PHASE) % z_psum;
	printk("zoom--->z_vds=%d, z_psum=%d, z_psum_left=%d\n", z_vds, z_psum, z_psum_left);

	f_psum = GetFocusPsum();
	f_vds = f_pos*(32/FOC_PHASE) / f_psum;
	f_psum_left = f_pos*(32/FOC_PHASE) % f_psum;
	printk("focus-->f_vds=%d, f_psum=%d, f_psum_left=%d\n", f_vds, f_psum, f_psum_left);

	ZoomEnable();
	FocusEnable();

	if ((z_vds == 0) && (f_vds == 0))
	{
		printk("has no vds!\n");
	}
	else if (z_vds > f_vds)
	{
		zoom_reg = (zoom_reg & 0xFF00) | z_psum;
		SPI_Write(ZOOM_REG, zoom_reg);

		f_psum = f_pos*(32/FOC_PHASE) / z_vds;
		f_psum_left = f_pos*(32/FOC_PHASE) % z_vds;
		focus_reg = (focus_reg & 0xFF00) | f_psum;
		SPI_Write(FOC_REG, focus_reg);

		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, z_vds);
		PWM_Wait(PWM_NUM);
		msleep(z_vds*VD_TIME+3);
/*		if (f_vds)
		{
			PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, f_vds);
			PWM_Wait(PWM_NUM);
			msleep(f_vds*VD_TIME+3);
		}
		focus_reg = (focus_reg & 0xFF00) | 0;
		SPI_Write(FOC_REG, focus_reg);

		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, (z_vds-f_vds)+1);
		PWM_Wait(PWM_NUM);
		msleep((z_vds-f_vds)*VD_TIME+3);
*/	}
	else
	{
		focus_reg = (focus_reg & 0xFF00) | f_psum;
		SPI_Write(FOC_REG, focus_reg);

		z_psum = z_pos*(32/ZOOM_PHASE) / f_vds;
		z_psum_left = z_pos*(32/ZOOM_PHASE) % f_vds;
		zoom_reg = (zoom_reg & 0xFF00) | z_psum;
		SPI_Write(ZOOM_REG, zoom_reg);

		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, f_vds);
		PWM_Wait(PWM_NUM);
		msleep(f_vds*VD_TIME+3);
/*		if (z_vds)
		{
			PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, z_vds);
			PWM_Wait(PWM_NUM);
			msleep(z_vds*VD_TIME+3);
		}
		zoom_reg = (zoom_reg & 0xFF00) | 0;
		SPI_Write(ZOOM_REG, zoom_reg);

		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, (f_vds-z_vds)+1);
		PWM_Wait(PWM_NUM);
		msleep((f_vds-z_vds)*VD_TIME+3);
*/	}
	printk("zoom--->z_vds=%d, z_psum=%d, z_psum_left=%d\n", z_vds, z_psum, z_psum_left);
	printk("focus-->f_vds=%d, f_psum=%d, f_psum_left=%d\n", f_vds, f_psum, f_psum_left);

	if (z_psum_left || f_psum_left)
	{
		zoom_reg = (zoom_reg & 0xFF00) | z_psum_left;
		SPI_Write(ZOOM_REG, zoom_reg);
		focus_reg = (focus_reg & 0xFF00) | f_psum_left;
		SPI_Write(FOC_REG, focus_reg);
		PWM_Write(PWM_NUM, PWM_DUTY, PWM_PERIOD, 1);
		PWM_Wait(PWM_NUM);
		msleep(VD_TIME+3);
	}

	zoom_reg = (zoom_reg & 0xFF00) | 0;
	SPI_Write(ZOOM_REG, zoom_reg);
	focus_reg = (focus_reg & 0xFF00) | 0;
	SPI_Write(FOC_REG, focus_reg);
	ZoomDisable();
	FocusDisable();
}

unsigned char GetZoomPIFlag(void)
{
	unsigned int reg_value = 0;

	reg_value = GPIO_READ_REG(GPIO2_DATA+0x3FC);
	if (reg_value & (0x1 << 5))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char GetFocusPIFlag(void)
{
	unsigned int reg_value = 0;

	reg_value = GPIO_READ_REG(GPIO2_DATA+0x3FC);
	if (reg_value & (0x1 << 6))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

