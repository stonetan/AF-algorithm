#include <linux/spi/spi.h>
#include <linux/device.h>
#include <linux/delay.h>

#include <asm/io.h>

#include "ls_lens_spi.h"


#define SPI0_ADDRESS_BASE         0x200C0000
#define SPI1_ADDRESS_BASE         0x200E0000

void __iomem* reg_spi0_base_va    = 0;
void __iomem* reg_spi1_base_va    = 0;

#define HI_IO_SPI0_ADDRESS(x)     (reg_spi0_base_va + ((x) - (SPI0_ADDRESS_BASE)))
#define HI_IO_SPI1_ADDRESS(x)     (reg_spi1_base_va + ((x) - (SPI1_ADDRESS_BASE)))

#define SPI0_REG(x)               HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + x)
#define SPI1_REG(x)               HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + x)

//SPI
#define SPI0_CR0                  HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x000)
#define SPI0_CR1                  HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x004)
#define SPI0_DR                   HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x008)
#define SPI0_SR                   HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x00C)
#define SPI0_CPSR                 HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x010)
#define SPI0_IMSC                 HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x014)
#define SPI0_RIS                  HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x018)
#define SPI0_MIS                  HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x01C)
#define SPI0_ICR                  HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x020)
#define SPI0_DMACR                HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x024)
#define SPI0_TXFIFOCR             HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0x028)
#define SPI0_RXFIFOCR             HI_IO_SPI0_ADDRESS(SPI0_ADDRESS_BASE + 0X02C)

#define SPI1_CR0                  HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x000)
#define SPI1_CR1                  HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x004)
#define SPI1_DR                   HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x008)
#define SPI1_SR                   HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x00C)
#define SPI1_CPSR                 HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x010)
#define SPI1_IMSC                 HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x014)
#define SPI1_RIS                  HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x018)
#define SPI1_MIS                  HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x01C)
#define SPI1_ICR                  HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x020)
#define SPI1_DMACR                HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x024)
#define SPI1_TXFIFOCR             HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0x028)
#define SPI1_RXFIFOCR             HI_IO_SPI1_ADDRESS(SPI1_ADDRESS_BASE + 0X02C)

#define  SPI_WRITE_REG(Addr, Value) ((*(volatile unsigned int *)(Addr)) = (Value))
#define  SPI_READ_REG(Addr)         (*(volatile unsigned int *)(Addr))

struct spi_master *hi_master;
struct spi_device *hi_spi;
extern struct bus_type spi_bus_type;


#define SPI_MSG_NUM    20
typedef struct hi_spi_message_s
{
	    struct spi_transfer	t;
		    struct spi_message	m;
			    unsigned char	buf[8];
} spi_message_s;

typedef struct hi_spi_message_info_s
{
	    int msg_idx;
		    spi_message_s spi_msg_array[SPI_MSG_NUM];
} spi_message_info_s;

static spi_message_info_s g_spi_msg = {0};


int SPI_Init(void)
{
	struct device *dev;
	char           spi_name[128] = {0};
	int            ret = 0;

	hi_master = spi_busnum_to_master(BUS_NUM);
	if (hi_master)
	{
		snprintf(spi_name, sizeof(spi_name), "%s.%u", dev_name(&hi_master->dev), CSN);
		// 通过每个片选在 SPI 核心层的名称得到指向 spi_device 的 device 成员的指针
		dev = bus_find_device_by_name(&spi_bus_type, NULL, spi_name);
		if (dev == NULL)
		{
			dev_err(NULL, "chipselect %d has not been used!\n", CSN);
			ret = -ENXIO;
			goto end1;
		}

		// 通过指向 spi_device 的 device 成员的指针得到描述 SPI 外围设备的结构体
		hi_spi = to_spi_device(dev);
		if (hi_spi == NULL)
		{
			dev_err(NULL, "to_spi_device() error!\n");
			ret = -ENXIO;
			goto end1;
		}
	}
	else
	{
		dev_err(NULL, "spi_busnum_to_master error!\n");
		ret = -ENXIO;
		goto end0;
	}

end1:
	put_device(dev);
end0:
	return ret;
}

int SPI_Exit(void)
{
	return 0;
}

int SPI_Test(void)
{
	unsigned int i, index;
	unsigned int reg_value = 0;

	reg_spi0_base_va  = (void __iomem*)IO_ADDRESS(SPI0_ADDRESS_BASE);
	reg_spi1_base_va  = (void __iomem*)IO_ADDRESS(SPI1_ADDRESS_BASE);

	index = 0;
	for (i=0; i<12; i++)
	{
		reg_value = SPI_READ_REG(SPI0_REG(index));
		index += 4;
		printk("%#x ", reg_value);
	}
	printk("\n");

	index = 0;
	for (i=0; i<12; i++)
	{
		reg_value = SPI_READ_REG(SPI1_REG(index));
		index += 4;
		printk("%#x ", reg_value);
	}
	printk("\n");

	reg_spi0_base_va = NULL;
	reg_spi1_base_va = NULL;

	return 0;
}

int SPI_Write(unsigned char addr, unsigned short data)
{
	struct spi_transfer *t;
	struct spi_message  *m;
	unsigned char              *buf = NULL;
	int                         buf_idx = 0;
	int                         idx = g_spi_msg.msg_idx;
	int                         ret = 0;
	unsigned long               flags;

	g_spi_msg.msg_idx++;
	if (g_spi_msg.msg_idx > SPI_MSG_NUM - 1)
	{
		g_spi_msg.msg_idx = 0;
	}

	buf = g_spi_msg.spi_msg_array[idx].buf;
	t	= &g_spi_msg.spi_msg_array[idx].t;
	m	= &g_spi_msg.spi_msg_array[idx].m;

	// check spi_message is or no finish
	spin_lock_irqsave(&hi_master->queue_lock, flags);
	// 该消息队列传输完成之后，在核心层会将 spi_message 的 state 成员设为空指针
	if (m->state != NULL)
	{
		dev_err(&hi_spi->dev, "%s, %s, %d line: spi_message no finish!\n",
				__FILE__, __func__, __LINE__);
		spin_unlock_irqrestore(&hi_master->queue_lock, flags);

		return -EFAULT;
	}
	spin_unlock_irqrestore(&hi_master->queue_lock, flags);

	hi_spi->mode = SPI_MODE_3 | SPI_LSB_FIRST | SPI_CS_HIGH; //设置SPI传输模式

	memset(buf, 0, sizeof(g_spi_msg.spi_msg_array[idx].buf));

	buf[buf_idx++] = (addr & 0x3F);
	buf[buf_idx++] = (data >> 0);
	buf[buf_idx++] = (data >> 8);

	t->tx_buf    = buf;
	t->rx_buf    = buf;
	t->len       = buf_idx;
	t->cs_change = 1;
	t->speed_hz  = 2000000;
	t->bits_per_word = 8;

	spi_message_init(m);
	spi_message_add_tail(t, m);

	m->state = m;
	ret = spi_async(hi_spi, m);
	if (ret)
	{
		dev_err(&hi_spi->dev, "%s: spi_async() error!\n", __func__);
		ret = -EFAULT;
	}

	return ret;
}

int SPI_Read(unsigned char addr, unsigned short *data)
{
	static struct spi_transfer t;
	static struct spi_message  m;
	static unsigned char       buf[8] = {0};
	int                        buf_idx = 0;
	int                        ret = 0;
	unsigned long              flags;

	// check spi_message is or no finish
	spin_lock_irqsave(&hi_master->queue_lock, flags);
	// 该消息队列传输完成之后，在核心层会将 spi_message 的 state 成员设为空指针
	if (m.state != NULL)
	{
		dev_err(&hi_spi->dev, "%s, %s, %d line: spi_message no finish!\n",
				__FILE__, __func__, __LINE__);
		spin_unlock_irqrestore(&hi_master->queue_lock, flags);

		return -EFAULT;
	}
	spin_unlock_irqrestore(&hi_master->queue_lock, flags);

	hi_spi->mode = SPI_MODE_3 | SPI_LSB_FIRST | SPI_CS_HIGH; //设置SPI传输模式

	memset(buf, 0, sizeof(buf));
	buf[buf_idx++] = (addr | 0x40);
	buf[buf_idx++] = 0;
	buf[buf_idx++] = 0;

	t.tx_buf    = buf;
	t.rx_buf    = buf;
	t.len       = buf_idx;
	t.cs_change = 1;
	t.speed_hz  = 2000000;
	t.bits_per_word = 8;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

	m.state = &m;
	ret = spi_async(hi_spi, &m);
	if (ret)
	{
		dev_err(&hi_spi->dev, "%s: spi_async() error!\n", __func__);
		ret = -EFAULT;
	}
	msleep(1);    // tzq@20160813 add for spi no receive data

	//printk("func:%s rx_buf = %#x, %#x, %#x\n", __func__, buf[0], buf[1], buf[2]);
	*data = (buf[1] + (buf[2] << 8));

	return ret;
}

