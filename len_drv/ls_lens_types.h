/******************************************************************************

  Copyright (C), 2014-2016, Longse Tech. Co., Ltd.

 ******************************************************************************
  File Name     : ls_lens_types.h
  Version       : Initial Draft
  Author        : tzq
  Created       : 2016/08/8
  Description   : 
  History       :

  Date          : 2016/08/8
  Author        : 60068
  Modification  : Created file

******************************************************************************/

#ifndef __LS_LENS_TYPES__
#define __LS_LENS_TYPES__

#define LENS_NULL              (0x00)
#define UNION_18X_T5095        (0x01)

#define LENS_TYPE              UNION_18X_T5095

#if (LENS_TYPE == UNION_18X_T5095)

#define FOC_HOME_POS           (1000)
#define FOC_POS_FRONT_D        (1044)
#define FOC_POS_REVER_D        (-184)
#define FOC_POS_RANGE          (FOC_POS_FRONT_D - FOC_POS_REVER_D)

#define FOC_MACH_RSV           (8)
#define FOC_POS_MAX            (FOC_POS_FRONT_D+FOC_HOME_POS-FOC_MACH_RSV)
#define FOC_POS_MIN            (FOC_POS_REVER_D+FOC_HOME_POS+FOC_MACH_RSV)

#define FOC_BACK_RUSH          (6)
#define FOC_MANU_MAX           (240)

#define ZOOM_HOME_POS          (1000)
#define ZOOM_POS_FRONT_D       (1345)
#define ZOOM_POS_REVER_D       (-65)
#define ZOOM_POS_RANGE         (ZOOM_POS_FRONT_D - ZOOM_POS_REVER_D)
/*
#define ZOOM_POINT_MAX         (18)

static const unsigned int ZOOM_TBL[ZOOM_POINT_MAX] = {
1334, 831, 607, 470, 372, 296,
233 , 180, 133, 92 , 57 , 28 ,
4   , -17, -32, -42, -52, -60,
};

static const unsigned int FOC_SCH_TBL[ZOOM_POINT_MAX] = {
8   , 10 , 23 , 42 , 65 , 64 ,
87  , 113, 141, 173, 206, 240,
240 , 240, 240, 240, 240, 240,
};

static const unsigned int FOC_INT_TBL[ZOOM_POINT_MAX] = {
1   , 1  , 1  , 2  , 2  , 2  ,
4   , 4  , 4  , 4  , 6  , 6  ,
6   , 6  , 6  , 6  , 6  , 6  ,
};
*/
#define ZOOM_POINT_MAX         (21)

static const unsigned int ZOOM_TBL[ZOOM_POINT_MAX] = {
1334, 1160, 998, 831, 718, 607, 470, 372, 296,
233 , 180 , 133, 92 , 57 , 28 ,
4   , -17 , -32, -42, -52, -60,
};

static const unsigned int FOC_SCH_TBL[ZOOM_POINT_MAX] = {
8   , 8   , 10 , 10 , 18 , 23 , 42 , 65 , 64 ,
87  , 113 , 141, 173, 206, 240,
240 , 240 , 240, 240, 240, 240,
};

static const unsigned int FOC_INT_TBL[ZOOM_POINT_MAX] = {
1   , 1   , 1  , 1  , 1  , 1  , 2  , 2  , 2  ,
4   , 4   , 4  , 4  , 6  , 6  ,
6   , 6   , 6  , 6  , 6  , 6  ,
};

static const unsigned int FOC_ADJ_TBL[ZOOM_POINT_MAX] = {
0   , 0   , 0  , 0  , 0  , 0  , 1  , 1  , 2 ,
2   , 3   , 4  , 5  , 6  , 8  ,
9   , 10  , 12 , 14 , 18 , 20 ,
};

static const unsigned int FOC_INF_TBL[ZOOM_POINT_MAX] = {
0   , 0   , 0  , 0  , 0  , 4  , 9  , 14 , 20 ,
27  , 36  , 45 , 55 , 66 , 80 ,
94  , 112 , 143, 165, 188, 207,
};

static const float FOC_CVR_TBL[ZOOM_POINT_MAX] = {
1.000, 1.100, 1.200, 1.300, 1.400, 1.500, 1.600,
2.000, 2.091, 2.723, 3.182, 3.818, 3.818, 4.545,
5.182, 6.723, 6.818, 7.273, 7.364, 7.455, 7.545,
};

#elif (LENS_TYPE == LENS_NULL)

#endif

#endif /* __LS_LENS_TYPES__ */

