/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_spi.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/8
  Description   : 
  History       :

  Date          : 2016/08/8
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_SPI_H__
#define __LS_LENS_SPI_H__


#define    BUS_NUM        (1)
#define    CSN            (0)


int SPI_Init(void);
int SPI_Exit(void);

int SPI_Test(void);

int SPI_Write(unsigned char addr, unsigned short data);
int SPI_Read(unsigned char addr, unsigned short *data);

#endif /* __LS_LENS_SPI_H__ */

