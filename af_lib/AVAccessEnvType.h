
#ifndef AV_ACCESS_ENV_TYPE_H_INCLUDE
#define AV_ACCESS_ENV_TYPE_H_INCLUDE

#include <pthread.h>

typedef enum _AVAccessErrorCode
{
	AVACCESS_OK 					= 0,
	AVACCESS_UNKNOWN				= 201,
	AVACCESS_NOINIT,
	AVACCESS_VIDEO_NOOPEN,
	AVACCESS_PARAM_ERROR,
	AVACCESS_PARAM_NULL,
	AVACCESS_ERRORCODE_END
} AVAccessErrorCode;

#ifdef FISHEYE_MULTI_CHANNEL_SUPPORT
typedef enum _AVAccessVideoType
{
	AVACCESS_1ST_STREAM				= 0,
	AVACCESS_2ND_STREAM				= 1,
	AVACCESS_3RD_STREAM				= 2,
	AVACCESS_4TH_STREAM				= 3,
	AVACCESS_5TH_STREAM				= 4,
	AVACCESS_6TH_STREAM				= 5,
	AVACCESS_7TH_STREAM				= 6,
	AVACCESS_8TH_STREAM				= 7,
	AVACCESS_9TH_STREAM				= 8,
	AVACCESS_10TH_STREAM				= 9,
	AVACCESS_11TH_STREAM				= 10,
	AVACCESS_12TH_STREAM				= 11,
	AVACCESS_13TH_STREAM				= 12,
	AVACCESS_14TH_STREAM				= 13,
	AVACCESS_15TH_STREAM				= 14,
	AVACCESS_MAX_VIDEO_STREAM
} AVAccessVideoType;
#else
typedef enum _AVAccessVideoType
{
	AVACCESS_1ST_STREAM				= 0,
	AVACCESS_2ND_STREAM				= 1,
	AVACCESS_3RD_STREAM				= 2,
	AVACCESS_MAX_VIDEO_STREAM
} AVAccessVideoType;
#endif

typedef enum _AVAccessVideoInversionType
{
	AVACCESS_VIDEO_INVERSION_NORMAL	= 0,
	AVACCESS_VIDEO_INVERSION_MIRROR	= 1,
	AVACCESS_VIDEO_INVERSION_FLIP	= 2,
	AVACCESS_VIDEO_INVERSION_BOTH	= 3,
} AVAccessVideoInversionType;

typedef enum _AVAccessVideoStatus
{
	AVACCESS_VIDEO_STATUS_NOUSE		= 0,
	AVACCESS_VIDEO_STATUS_USE		= 1,
	AVACCESS_VIDEO_STATUS_END
} AVAccessVideoStatus;


typedef struct _AVAccessAVSysInfo
{
	AVAccessVideoStatus 	VideoStatus[AVACCESS_MAX_VIDEO_STREAM];
	AVAccessVideoStatus	AudioEncodeStatus;
	pthread_t 				VideoThreadId[AVACCESS_MAX_VIDEO_STREAM];
	pthread_t				AudioThreadId;
} AVAccessAVSysInfo;


typedef struct __AVAccessPtzOption
{
	unsigned char ucCh;				
	unsigned char ucCmd;				
	unsigned char ucPresetPointIndex;   
	unsigned char ucCruiseLineIndex;    	
	
	unsigned char ucRes[12];
}AVAccessPtzOption;


typedef enum _AVAccessAbnormalEventType
{
	AVACCESS_ABNORMAL_EVNENT_NULL	= 0,
	AVACCESS_ABNORMAL_EVNENT_NETLINKDOWN	= 1,
	AVACCESS_ABNORMAL_EVNENT_IPCONFLICT	= 2,

} AVAccessAbnormalEventType;


#endif



