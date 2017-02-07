#include<linux/kernel.h>
#include <linux/delay.h>

#include <asm/io.h>

#include "ls_lens_pwm.h"


#define PERI_ADDRESS_BASE         0x20030000
#define MISC_ADDRESS_BASE         0x20120000
#define PWM_ADDRESS_BASE          0x20130000

void __iomem* reg_peri_base_va    = 0;
void __iomem* reg_misc_base_va    = 0;
void __iomem* reg_pwm_base_va     = 0;

#define HI_IO_PERI_ADDRESS(x)     (reg_peri_base_va + ((x) - (PERI_ADDRESS_BASE)))
#define HI_IO_MISC_ADDRESS(x)     (reg_misc_base_va + ((x) - (MISC_ADDRESS_BASE)))
#define HI_IO_PWM_ADDRESS(x)      (reg_pwm_base_va + ((x) - (PWM_ADDRESS_BASE)))

#define PERI_REG(x)               HI_IO_PERI_ADDRESS(PERI_ADDRESS_BASE + x)
#define MISC_REG(x)               HI_IO_MISC_ADDRESS(MISC_ADDRESS_BASE + x)

//PWM
#define PWM0_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0000)
#define PWM0_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0004)
#define PWM0_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0008)
#define PWM0_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x000C)
#define PWM0_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0010)
#define PWM0_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0014)
#define PWM0_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0018)

#define PWM1_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0020)
#define PWM1_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0024)
#define PWM1_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0028)
#define PWM1_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x002C)
#define PWM1_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0030)
#define PWM1_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0034)
#define PWM1_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0038)

#define PWM2_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0040)
#define PWM2_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0044)
#define PWM2_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0048)
#define PWM2_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x004C)
#define PWM2_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0050)
#define PWM2_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0054)
#define PWM2_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0058)

#define PWM3_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0060)
#define PWM3_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0064)
#define PWM3_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0068)
#define PWM3_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x006C)
#define PWM3_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0070)
#define PWM3_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0074)
#define PWM3_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x0078)

#define PWM4_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1000)
#define PWM4_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1004)
#define PWM4_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1008)
#define PWM4_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x100C)
#define PWM4_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1010)
#define PWM4_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1014)
#define PWM4_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1018)

#define PWM5_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1020)
#define PWM5_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1024)
#define PWM5_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1028)
#define PWM5_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x102C)
#define PWM5_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1030)
#define PWM5_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1034)
#define PWM5_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1038)

#define PWM6_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1040)
#define PWM6_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1044)
#define PWM6_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1048)
#define PWM6_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x104C)
#define PWM6_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1050)
#define PWM6_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1054)
#define PWM6_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1058)

#define PWM7_CFG_REG0             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1060)
#define PWM7_CFG_REG1             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1064)
#define PWM7_CFG_REG2             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1068)
#define PWM7_CTRL_REG             HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x106C)
#define PWM7_STATE_REG0           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1070)
#define PWM7_STATE_REG1           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1074)
#define PWM7_STATE_REG2           HI_IO_PWM_ADDRESS(PWM_ADDRESS_BASE + 0x1078)

#define  PWM_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  PWM_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))


#define PWM_NUM_MAX               0x08

#define PWM_ENABLE                0x01
#define PWM_DISABLE               0x00


void PWM_Init(void)
{
	unsigned int i, index = 0;
	unsigned int reg_value = 0;

	reg_peri_base_va = (void __iomem*)IO_ADDRESS(PERI_ADDRESS_BASE);
	reg_misc_base_va = (void __iomem*)IO_ADDRESS(MISC_ADDRESS_BASE);
	reg_pwm_base_va  = (void __iomem*)IO_ADDRESS(PWM_ADDRESS_BASE);
	
	index = 14*4;       // open pwm clock
	reg_value = PWM_READ_REG(PERI_REG(index));
	PWM_WRITE_REG(PERI_REG(index), reg_value | (0x1 << 1));

	index = 65*4;       // select pwm clock 3MHz
	reg_value = PWM_READ_REG(PERI_REG(index));
	PWM_WRITE_REG(PERI_REG(index), reg_value & ~(0x1 << 10));

	index = 5*4;
	for (i=0; i<4; i++) // pwm0,pwm1,pwm2,pwm3 normal output
	{
		reg_value = PWM_READ_REG(MISC_REG(index));
		PWM_WRITE_REG(MISC_REG(index), reg_value & ~(0x0f << 16));
	}
}

void PWM_Exit(void)
{
	reg_peri_base_va = NULL;
	reg_misc_base_va = NULL;
	reg_pwm_base_va  = NULL;
}

int PWM_Disable(unsigned char num)
{
	if (num >= PWM_NUM_MAX)
	{
		printk("The pwm number is big than the max value!\n");
		return -1;
	}

	switch (num)
	{
		case 0:
			PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);
			break;
		case 1:
			PWM_WRITE_REG(PWM1_CTRL_REG, PWM_DISABLE);
			break;
		case 2:
			PWM_WRITE_REG(PWM2_CTRL_REG, PWM_DISABLE);
			break;
		case 3:
			PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);
			break;
		case 4:
			PWM_WRITE_REG(PWM4_CTRL_REG, PWM_DISABLE);
			break;
		case 5:
			PWM_WRITE_REG(PWM5_CTRL_REG, PWM_DISABLE);
			break;
		case 6:
			PWM_WRITE_REG(PWM6_CTRL_REG, PWM_DISABLE);
			break;
		case 7:
			PWM_WRITE_REG(PWM7_CTRL_REG, PWM_DISABLE);
			break;
		default:
			break;
	}

	return 0;
}

int PWM_Write(unsigned char num, unsigned int duty, unsigned int period, unsigned int pwm_num)
{
	if (num >= PWM_NUM_MAX)
	{
		printk("The pwm number is big than the max value!\n");
		return -1;
	}

	switch (num)
	{
		case 0:
			PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM0_CFG_REG0, period);
			PWM_WRITE_REG(PWM0_CFG_REG1, duty);
			PWM_WRITE_REG(PWM0_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM0_CTRL_REG, (0 << 2 | PWM_ENABLE)); // keep pwm always output;
			//printk("The PWM0 state %x\n",PWM_READ_REG(PWM0_STATE_REG2));
			break;
		case 1:
			PWM_WRITE_REG(PWM1_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM1_CFG_REG0, period);
			PWM_WRITE_REG(PWM1_CFG_REG1, duty);
			PWM_WRITE_REG(PWM1_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM1_CTRL_REG, (0 << 2 | PWM_ENABLE)); // keep pwm always output;
			//printk("The PWM1 state %x\n",PWM_READ_REG(PWM1_STATE_REG2));
			break;
		case 2:
			PWM_WRITE_REG(PWM2_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM2_CFG_REG0, period);
			PWM_WRITE_REG(PWM2_CFG_REG1, duty);
			PWM_WRITE_REG(PWM2_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM2_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM2 state %x\n",PWM_READ_REG(PWM2_STATE_REG2));
			break;
		case 3:
			PWM_WRITE_REG(PWM3_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM3_CFG_REG0, period);
			PWM_WRITE_REG(PWM3_CFG_REG1, duty);
			PWM_WRITE_REG(PWM3_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM3_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM3 state %x\n",PWM_READ_REG(PWM3_STATE_REG2));
			break;
		case 4:
			PWM_WRITE_REG(PWM4_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM4_CFG_REG0, period);
			PWM_WRITE_REG(PWM4_CFG_REG1, duty);
			PWM_WRITE_REG(PWM4_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM4_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM4 state %x\n",PWM_READ_REG(PWM4_STATE_REG2));
			break;
		case 5:
			PWM_WRITE_REG(PWM5_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM5_CFG_REG0, period);
			PWM_WRITE_REG(PWM5_CFG_REG1, duty);
			PWM_WRITE_REG(PWM5_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM5_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM5 state %x\n",PWM_READ_REG(PWM5_STATE_REG2));
			break;
		case 6:
			PWM_WRITE_REG(PWM6_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM6_CFG_REG0, period);
			PWM_WRITE_REG(PWM6_CFG_REG1, duty);
			PWM_WRITE_REG(PWM6_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM6_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM6 state %x\n",PWM_READ_REG(PWM6_STATE_REG2));
			break;
		case 7:
			PWM_WRITE_REG(PWM7_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM7_CFG_REG0, period);
			PWM_WRITE_REG(PWM7_CFG_REG1, duty);
			PWM_WRITE_REG(PWM7_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM7_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM7 state %x\n",PWM_READ_REG(PWM7_STATE_REG2));
			break;
		default:
			PWM_WRITE_REG(PWM0_CTRL_REG, PWM_DISABLE);

			PWM_WRITE_REG(PWM0_CFG_REG0, period);
			PWM_WRITE_REG(PWM0_CFG_REG1, duty);
			PWM_WRITE_REG(PWM0_CFG_REG2, pwm_num); //pwm output number

			PWM_WRITE_REG(PWM0_CTRL_REG, (1 << 2 | PWM_ENABLE)); // keep pwm always output;
			printk("The PWM0 state %x\n",PWM_READ_REG(PWM0_STATE_REG2));
			break;
	}

	return 0;
}

static bool GetPwmState(unsigned char num)
{
	unsigned int reg_value = 0;

	switch (num)
	{
		case 0:
			reg_value = PWM_READ_REG(PWM0_STATE_REG2);
			break;
		case 1:
			reg_value = PWM_READ_REG(PWM1_STATE_REG2);
			break;
		case 2:
			reg_value = PWM_READ_REG(PWM2_STATE_REG2);
			break;
		case 3:
			reg_value = PWM_READ_REG(PWM3_STATE_REG2);
			break;
		case 4:
			reg_value = PWM_READ_REG(PWM4_STATE_REG2);
			break;
		case 5:
			reg_value = PWM_READ_REG(PWM5_STATE_REG2);
			break;
		case 6:
			reg_value = PWM_READ_REG(PWM6_STATE_REG2);
			break;
		case 7:
			reg_value = PWM_READ_REG(PWM7_STATE_REG2);
			break;
		default:
			reg_value = PWM_READ_REG(PWM0_STATE_REG2);
			break;
	}
	
	if (reg_value & (0x1 << 10)) return true;
	else return false;
} 

int PWM_Wait(unsigned char num)
{

	if (num >= PWM_NUM_MAX)
	{
		printk("The pwm number is big than the max value!\n");
		return -1;
	}

	//printk("state=%d\n", GetPwmState(num));
	while (GetPwmState(num) == false)
	{
		//msleep(1);
	}
	//while (GetPwmState(num) == true)
	//{
	//	msleep(1);
	//}
	//printk("state=%d\n", GetPwmState(num));

	return 0;
}

