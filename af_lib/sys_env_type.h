/* ===========================================================================
* @path $(IPNCPATH)\include
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
/**
* @file sys_env_type.h
* @brief System evironment structure and Global definition.
*/
#ifndef __SYS_ENV_TYPE_H__
#define __SYS_ENV_TYPE_H__

#include <asm/types.h>
#include <netinet/in.h>
#include <time.h>


#define MAGIC_NUM			0x9A9B000D 
#define REL_UNIQUE_ID 		(026000)		//八进制表示
#define REL_UNIQUE_ID_1 	(0x3ff)
#define REL_UNIQUE_ID_2 	(0x400)


#define SYS_ENV_SIZE			sizeof(SysInfo)
#define SYS_ENV_SIZE0			sizeof(SysInfo_0)
#define SYS_ENV_SIZE1			sizeof(SysInfo_1)
#define SYS_ENV_SIZE2			sizeof(SysInfo_2)
#define SYS_ENV_SIZE_RES		32 * 1024
#define MAX_LOG_PAGE_NUM		40 
#define NUM_LOG_PER_PAGE		10
#define LOG_ENTRY_SIZE			sizeof(LogEntry_t)


#define MAX_DOMAIN_NAME			40 		///< Maximum length of domain name. Ex: www.xxx.com
#define MAX_LANCAM_TITLE_LEN	11 		///< Maximum length of LANCAM title.
#define OSDTEXTLEN				256
#define MAX_NAME_LENGTH			200 	///< Maximum length of normal string.
#define MAX_OSD_STREAMS			3 		///< Maximum length of normal string.
#define MAX_CODEC_STREAMS		3 		///< Maximum length of normal string.
#define MAX_STREAM_NAME			75  	///< Maximum length of normal string.
#define MAC_LENGTH				6 		///< Length of MAC address.
#define MAX_FILE_NAME			128		///< Maximum length of file name.

#define ACOUNT_NUM				16 		///< How many acounts which are stored in system.
#define USER_LEN				32 		///< Maximum of acount username length.
#define PASSWORD_LEN			32 		///< Maximum of acount password length. //old is 16 may short
#define SMTP_USER_LEN			64
#define SMTP_PASSWD_LEN		64
#define IP_STR_LEN				20		///< IP string length

#define MOTION_BLK_LEN			(8) 		///< Motion block size
#define MOTION_BLK_LEN_EX		(32) 	///< Motion block size

#define MAX_PRIVACYMASK_NUMS	12

/**
* @brief Infomation of network status and user settings.
*/
typedef struct
{
	struct in_addr		ip; 							///< IP address in static IP mode
	struct in_addr		netmask; 					///< netmask in static IP mode
	struct in_addr		gateway; 					///< gateway in static IP mode
	struct in_addr		dns; 						///< DNS IP in static IP mode
	__u16 			http_port; 					///< HTTP port in web site.
	__u16			rtsp_port; 					///< HTTP port in web site.
	int				dhcp_enable; 				///< current DHCP status.
	__u8 			ntp_enable;		
	char				ntp_server[MAX_DOMAIN_NAME+1]; 	///< NTP server FQDN
	__u8			ntp_timezone; 				///< current time zone
	__u8			time_format; 				///< time display format
	__u8			MAC[MAC_LENGTH]; 			///< hardware MAC address
	__u16       		mobileport;  					///< mobile port
	__u8        		ucOnvif_Enable;   		
	__u8 			Authentication;
	__u8 			NvrModifyTimeEnable;
	__u8 			wifi_dhcp_enable;
	struct in_addr		wifi_ip; 						///< IP address in static IP mode
	struct in_addr		wifi_netmask; 				///< netmask in static IP mode
	struct in_addr		wifi_gateway; 				///< gateway in static IP mode
	struct in_addr 	wifi_dns;					///< DNS IP in static IP mode			
	__u8			wifi_mac[MAC_LENGTH]; 		///< hardware MAC address
	__u8   			Wifi_ConnectStatus;			///0:disconnect 1:connect
	__u8 			ucRes1[1];
	__u16 			usHttps_Port;
	__u16 			usOnvif_Port;
	__u16 			usHkPrivate_Port;
	__u8			ucHkPrivate_Enable;
	__u8 			ucRes3[1];
	__u16 			usXmPrivate_Port;
	__u8			ucXmPrivate_Enable;
	__u8 			ucRes4[1];	
	
	char 			reserve[90]; 				
} Network_Config_Data;


/**
* @brief Infomation about ftp configuration.
*/
typedef struct
{
	__u8       bFtpEnable;						///< FTP enable
	char		servier_ip[MAX_DOMAIN_NAME+1]; 	///< FTP server address
	char		username[USER_LEN]; 				///< FTP login username
	char		password[PASSWORD_LEN]; 		///< FTP login password
	char		foldername[MAX_FILE_NAME];		///< FTP upload folder
	__u8	ucAnonymousEnable;				///< FTP Anonymous enable,0:not enable 1:enable 
	__u8      ucAutoCoverEnable;			      ///< FTP AutoCover enable,0:not enable 1:enable 
	__u8      ucFtptestStatus;				      /// 0:success 1:fail			
	__u8 	ucFtpUploadStatus;
	//int	image_acount; 				      ///< Image count
	int		pid; 							      ///< PID
	__u16	port; 						      ///< FTP port
	__u8       ftpfileformat; 				      ///< file format
} Ftp_Config_Data;


/**
* @brief Infomation about SMTP configuration.
*/
typedef struct
{
	__u8		bSmtpEnable;				///< SMTP enable
	char		servier_ip[MAX_DOMAIN_NAME+1]; 	///< SMTP server address
	__u16		server_port; 					///< SMTP server port
	char		username[SMTP_USER_LEN]; 		///< SMTP username
	char		password[SMTP_PASSWD_LEN]; 		///< SMTP password
	__u8		authentication; 				///< SMTP authentication
	char		sender_email[MAX_NAME_LENGTH]; 	///< sender E-mail address
	char		receiver_email[MAX_NAME_LENGTH]; ///< receiver E-mail address
	char		receiver_email1[100];				///< receiver E-mail address
	char		receiver_email2[100]; 				///< receiver E-mail address
	char		CC[MAX_NAME_LENGTH]; 			///< CC E-mail address
	char		subject[MAX_NAME_LENGTH]; 		///< mail subject
	char		text[MAX_NAME_LENGTH]; 			///< mail text
	__u8 	ucSmtpTestEnable;				/// 0:not enable 1:enable	
	__u8 	ucSmtpTestStatus;				/// 0:success 1:fail					
	//__u8		attachments; 					///< mail attachment
	//__u8		view; 						///< smtp view
	__u8		asmtpattach; 				///< attatched file numbers
	__u8       	attfileformat; 				///< attachment file format
} Smtp_Config_Data;


/**
* @brief Infomation about Cloud Storage configuration.
*/
typedef struct
{
	__u8       bCloudStorageEnable;			///< Cloud Storage enable
	__u8       CloudStorageFileFormat; 		///< file format
	__u8 	CloudStorageType;
	__u8 	ucRes[29];
	
} CloudStorage_Config_Data;


/**
* @brief IPCAM configuration data.
*/
typedef struct
{
	__u8		nBrightness;					///< brightness value
	__u8		nContrast;						///< contrast value
	__u8		nSaturation;					///< saturation value
	__u8		nSharpness;						///< sharpness value

	__u8		ircutswitchtime;
	__u8		agc;
	__u8		noisefilter;
	__u8		nWhiteBalance;					///< white balance mode, 0自动 1手动 2:锁定白平衡 3:日光灯 4:白炽灯 5:暖光灯 6:自然光 
	__u8		nMwbRgain;						///< manual white balance r gain
	__u8		nMwbGgain;						///< manual white balance g gain
	__u8		nMwbBgain;						///< manual white balance b gain

	__u8		nBacklightControl;				///< backlight control setting
	__u8		nBackLight;						///< backlight value
	__u8		imagesource; 					///< image source (NTSC/PAL)
	__u8 		irismode;
	__u8 		irisopen;
	__u8		ircutmode;
	__u8		night2day;
	__u8		irled;
	__u8		nElectronicShutter;
	__u8 		nExposureCtl;
	__u8		maxexposure;
	__u8		nWideDynamicEable;
	__u8		nWideDynamicLevel;
	__u8 		nDNRModel;
	__u8 		nDNR2DStrength;
	__u8		nDNR3DStrength;
	__u8		nDefogEnable;
	__u8        	nDefogMode;
	__u8        	nDefogStrength;
	__u8		nRotateAngle;
	__u8 		nSLInhibition;				 	 
	__u8		nSLInhibitionStreangth;			 
 
	__u8 		nStreamType;
	__u8		nVideocodecmode;				///< selected codec mode
	__u8		nEncodingType1;					///< selected encoding type, 0:jpeg 1:MPEG4 2:h264 3:h265 4:MJPEG
	__u8 		nEncodingType2; 
	__u8 		nEncodingType3;
	int			nFrameRate1;					///< MPEG4_1 frame rate
	int			nFrameRate2;					///< MPEG4_2 frame rate
	int			nFrameRate3;					///< JPEG frame rate
	unsigned long			nBitrate1;						///< MPEG4_1 bitrate
	unsigned long			nBitrate2;						///< MPEG4_2 bitrate
	unsigned long			nBitrate3;						///< MPEG4_3 bitrate
	__u8		nRateControl1;					///< stream1 bitrate control setting
	__u8		nRateControl2;					///< stream2 bitrate control setting
	__u8		nRateControl3;					///< stream3 bitrate control setting

	__u8		mirror; 						///< mirroring	0,off,  1,H  2,V 3both
	__u8		premirror; 						///< mirroring	0,off,  1,H  2,V 3both
	
	__u8		nImageQuality1;					///<	 image quality
	__u8		nImageQuality2;					///<	 image quality
	__u8		nImageQuality3;					///<	 image quality

	__u8 		nProfile1;						///0:baseline profile 1:main profile 2:high profile
	__u8 		nProfile2;
	__u8 		nProfile3;
	
	//__u8		Supportstream1;					///< support stream1 (JPEG)
	//__u8		Supportstream2;					///< support stream2 (MPEG4_1)
	//__u8		Supportstream3;					///< support stream3 (MPEG4_2)
	
	__u8		Supportstream4;					///< support stream4
	__u8		Supportstream5;					///< support stream5 (AVC_1)
	__u8		Supportstream6;					///< support stream6 (AVC_2)

	/*IO*/
	__u8		gioinenable;						///< GIO input enable
	__u8		giointype;						///< GIO input type,  0低电平 1高电平
	__u8		giooutenable;					///< GIO output enable
	__u8		gioouttype;						///< GIO output type, 0低电平 1高电平

    	__u8		ucElectronicAntiShakeEnable;  		//电子防抖: 0:关闭 1:使能
    	
	//__u8		bSdaEnable;						///< alarm save into SD card
	__u8        	aviduration;						///< avi record duration
	__u8		aviformat;						///< avirecord format.
	__u8		chipConfig;
	__u8		nAlarmAudioFile; 			    		///< alarm audio file
	
	__u8		nDayNight;						///< Daynight mode
	__u8		nBinning;						///< binning/skipping setting
	__u8		linearwdr;
	__u8		nRateControl;					///< bitrate control setting

	__u8		nAEWswitch;						///< 2A engine selection
	__u8		AdvanceMode; 					///< video stabilization (ON/OFF)
	__u8		nAEWtype;						///< 2A engine selection

	__u8		maxgain;
	__u8        	dateposition;
	__u8        	timeposition;
	__u8		nAlarmAudioPlay; 			    ///< alarm audio play enable/disable
	__u8		daylight_time; 					///< daylight saving time mode
	__u8		imageformat; 					///< image format
	__u8		nPreRotateAngle; 				//add 20150303 
	__u8 		nHdrEnable;
	__u8 		reserve[510];
} Lancam_Config_Data;


/**
* @brief SD card configuration data.
*/
typedef struct
{
	__u8			sdfileformat;	///< file format saved into SD card
	__u8			sdrenable;		///< enable SD card recording
	__u8			sdinsert;		///< SD card inserted
	__u8			sdstrategy;   	///< SD strategy      
	__u8			sdmaxsize;  	///< SD  avi file  maxsize
}Sdcard_Config_Data;


/**
* @brief IPCAM user account data.
*/
typedef struct
{
	char		user[USER_LEN];				///< username
	char		password[PASSWORD_LEN];		///< password
	int	authority;							///< user authority
}Acount_t;


/**
* @brief motion detection configuration data.
*/
typedef struct
{
	__u8      		motionenable;				///< motion detection enable
	__u8      		motioncenable;				///< customized sensitivity enable
	__u8      		motionlevel;					///< predefined sensitivity level
	__u8      		motioncvalue;				///< customized sensitivity value
	__u8		motionblock[MOTION_BLK_LEN];	///< motion detection block data
	unsigned long motionblockEx[MOTION_BLK_LEN_EX];
	//char 		reserve[128];
}Motion_Config_Data;


typedef struct
{
	__u8	maskenable;
	unsigned long 	x1[MAX_PRIVACYMASK_NUMS];
	unsigned long		y1[MAX_PRIVACYMASK_NUMS];
	unsigned long		x2[MAX_PRIVACYMASK_NUMS];
	unsigned long		y2[MAX_PRIVACYMASK_NUMS];
}PrivacyMask_Config_Data;

typedef struct
{
	__u8 x;
	__u8 y;
}POINT_T;

typedef struct
{
	POINT_T pos[4];
}REGION_T;

typedef struct
{
	__u8      		audioenable;									
	__u8		codectype;
	__u8      		samplerate;						
	__u8      		bitrate;						
	__u32     	bitrateValue;					
	__u8		audioinvolume;
	__u8		audiooutvolume;
	__u8 		audioInputMode;
	__u8      		audiorecvenable;    			
	__u8      		audioServerIp[MAX_NAME_LENGTH];  
}Audio_Config_Data;

typedef struct
{
	__u8		dateformat;
	__u8		nTimeEnable;				///< time  enable (ON/OFF)
	__u8		nTimePosition;   				///< time position
	__u8		nTextEnable;					///< text enable (ON/OFF)
	__u8		nTextPosition;   				///< text position
	__u8		overlaytext[OSDTEXTLEN];		///< osd text name
}OSD_config;

typedef struct
{
	__u32 		ipRatio;
	__u8 		fIframe;
	__u8 		qpInit;
	__u8 		qpMin;
	__u8 		qpMax;
	__u8 		meConfig;
	__u8 		packetSize;
	__u8 		unkown1;
	__u8 		unkown2;
}CodecAdvParam;


typedef struct
{
  __u16     	width;
  __u16     	height;
  __u16			portnum;
  __u8			name[MAX_STREAM_NAME];
  __u8 			portname[MAX_STREAM_NAME];
  __u8 			iEnable;
}StreamParam;


/*ptz 串口属性*/
typedef struct 
{
	unsigned char ucAddr;                	/*地址位*/
	unsigned char ucDataBit;            	/*数据位*/
	unsigned char ucStopBit;            	/*停止位*/
	unsigned char ucCheckType;       	/*检验类型*/
	unsigned char ucProtocol;          	/*协议*/
	unsigned char ucRes1[2];       		/*保留1*/
	unsigned long ulBaudRate;			/*比特率*/
}Ptz_Serial_Config;


/**
* @brief PTZ configuration data.
*/
typedef struct
{
  	__u8 		ucFocusMode;     
	__u8 		ucZoomPos; 
   	__u8  		ucApertureMode;   	/*光圈模式  0自动  1控制*/
   	__u8  		ucApertureVal;    		/*光圈打开量 0-100 (在光圈模式为控制的情况下有效)*/
      __u8 		ucStep;				/*速度*/
	__u8 		reserve[3];
	__u32       	uiZoomPosReal;
	__u8 		reserve1[4];
	Ptz_Serial_Config ptz_serial;
	__u8 		ucRes[36];
}Ptz_Config_Data;   //64


/*
@ weblanguage
*/
typedef struct
{
	char		language[32];	///< zoom-in
}Ipnc_web_language;


/**
* @brief event log data structure.
*/
typedef struct LogEntry_t{
	char 			event[100];	///< event description
	struct tm 	time;		///< event log time
}LogEntry_t;


/**
* @brief user settings structure.
*/
typedef struct{
	char 		id[USER_LEN];
	char 		password[PASSWORD_LEN];
	char 		remote_ip[IP_STR_LEN];
}login_data_t;


#if 1
typedef struct{
	char 		user_id[USER_LEN];
	int  		authority;
}get_user_authority_t;


typedef struct{
	char		user_id[USER_LEN];
	char		password[PASSWORD_LEN];
	__u8		authority;
}add_user_t;
#endif

/*DDNS*/
typedef struct{
	unsigned long 	enable;
	unsigned long 	type;	//ddns service type
	char 			user[32];
	char 			pwd[32];
	char 			site[32];	
}Ddns_Config_Data;


/*HTTP*/
typedef struct{
	int 		cmd;
	int 		internalport;
	int 		externalport;
}Http_Config_Data;


typedef struct 
{
	__u32 	update_interval;
	char 	device_id[16];
	char 	update_server[IP_STR_LEN];
	__u16 	update_port;

	__u8 	pppoe_enable;
	char 	pppoe_account[32];
	char 	pppoe_password[32];
	__u8 	pppoe_status;
	__u8 	pppoe_ip[32];
}HY_Config_Data;


typedef struct
{
	__u8 	freeptopenable;
	__u32  	maxwaitmsfreep2p;	
	char 	freeptopserverip[32];
	char 	freeptopsn[32];
	char 	freeptopiosapp[128];
	char 	freeptopandroidapp[128];

	__u8 	ucFreeIpRegion;			
	__u8 	ucRes[295];	
}P2P_Config_Data;


/**
* @brief custom data structure for time.
*/
typedef struct{
	__u8		nHour;	///< Hour from 0 to 23.
	__u8		nMin;	///< Minute from 0 to 59.
	__u8		nSec;	///< Second from 0 to 59.
} Time_t;


//自动维护
typedef struct
{
	__u8	AutoRebootEnable;
	__u8	Day;
	Time_t	Time;	
}AutoMainTain_Config_Data;


typedef struct 
{
	unsigned char ucEnable;                 	/*是否开启定时重启, 0关闭, 1开启*/
  	unsigned char ucRebootModel;	    	/*重启模式, 0每天, 1每周, 2每月*/
	unsigned char ucDay;			    	/*重启时间, 单位日*/ 
	unsigned char ucHour;                   	/*重启时间, 单位时*/
	unsigned char ucMinute;		    	/*重启时间, 单位分*/
	unsigned char ucSecond;                  /*重启时间, 单位秒*/
	unsigned char ucWeekData;             /*0-6 分别为星期天到星期六*/
	unsigned char ucRes[25];			/*保留*/
}AutoReboot_Config_Data;


//报警配置
typedef struct 
{
	__u8   	nAlarmEnable;                   	///< alarm enable/disable setting
	__u16	alarmstatus;					///< Current alarm status.
	__u8	nAlarmDuration;				///< alarm duration
	__u8   	lostalarm;					///< ethernet lost alarm
	__u8	nExtAlarm; 			            	///< external trigger alarm
	char		SnapshotPath[MAX_FILE_NAME]; 	///< recordpath path
	char		recordpath[MAX_FILE_NAME]; 		///< record path
	__u8 	nAlarmType;					///0:none 1:IO 2:motion
}Alarm_Config_Data;


//厂家参数
typedef struct
{
    	char		strDevName[32];            	//设备名称
	char		strFactoryName[32];		   	//厂家名称
	char		strHardVer[64];			   	//固件版本号
	char		strSoftVer[64];			   	//软件版本号
}Factory_config_Data;


/*矩形结构体*/
typedef struct {
    unsigned long x1;                 /*左上角x坐标*/
    unsigned long y1;                 /*左上角y坐标*/
    unsigned long x2;                 /*右下角x坐标*/
    unsigned long y2;                 /*右下角y坐标*/
}ROI_RECT;

typedef struct {
	ROI_RECT      area;               /*百分比区域*/
	unsigned char ucQpType;       /*QP类型: 填写 ROI_QP_TYPE*/
	char cQpVal;             		/*QP数值: 相对模式下填[-51,51] 绝对模式下填[0,51]*/
	unsigned char ucEnbale;           /*使能: 0关闭 1打开*/
	unsigned char ucRes[1];         /*保留*/
}ROI_Region;

typedef struct {
    unsigned char ucCh;                 /*通道*/
    unsigned char ucEnable;           /*0关闭RIO, 1开启ROI*/
    unsigned char ucBgFrameRate;      /*背景帧率: 0关闭背景帧率, 1-255数值为背景帧率.并开启背景帧率*/
    ROI_Region    region[8];
    unsigned char ucRes2[16];         /*保留2*/
}ROI_Config_data;


typedef struct {
	unsigned char  ucViewType;    /*畸变矫正类型: 0裁剪 1全模式*/
	long  	lCenterXOffset;		    /*畸变中心点相对图象中心点水平偏移: -75到75*/
	long  	lCenterYOffset;		    /*畸变中心点相对图象中心点垂直偏移: -75到75*/
	unsigned long  ulRatio;           /*畸变程度: 0-511*/
}LDC_Config_data;


typedef struct {
	unsigned char  ucMonth;                /*月*/
	unsigned char  ucDay;                  /*日*/
	unsigned char  ucHour;                 /*时*/
	unsigned char  ucweekDay;                  /*周*/
	unsigned char  ucWeekNumber;          /*周序号*/
	unsigned char  ucRes[16];               /*保留*/
 
}DST_Property;


typedef struct {    
	unsigned char  ucEnDST;                /*DST开关, 1开启 0关闭*/	
	unsigned char  ucDSTType;                /*DST模式, 0日期  1周*/
	DST_Property ucDSTStart;              /*开始时间*/	
	DST_Property ucDSTEnd;              /*结束时间*/	
	unsigned char  offsetTime;          /*偏移时间，默认为一个小时*/
	unsigned char  ucRes[16];               /*保留*/
}DST_Config_Data;


typedef enum 
{
	SCOPE_NULL       = 0,                  /*无时间段*/
	SCOPE_ALL,                             /*全天时间段*/
	SCOPE_SAME,                            /*按第一个时间段*/
	SCOPE_DIFF,                            /*按每个具体时间段*/
}Scope_Mode;

typedef struct 
{
	unsigned char   ucHour;               /*时: 0-23*/
	unsigned char   ucMinute;             /*分: 0-59*/
	unsigned char   ucSecond;             /*秒: 0-59*/
	unsigned char   ucRes[1];             /*保留*/
}Hour_Info;

typedef struct 
{
	Hour_Info  beginHour;
	Hour_Info  endHour;
}Hour_Scope;


/*天小时范围*/
typedef struct
{
    unsigned char         ucSocpeMode;          /*时间模式*/
    unsigned char         ucRes1[3];            	   /*保留*/
    Hour_Scope            hourScope[6];         /*一天最多有6个时段*/
}Day_Hour_Scope;


/*周时间表*/
typedef struct
{
	Day_Hour_Scope   dayScope[7];          /*一周7天. 0到6 表示星期天到星期六*/
}Week_Scope;


/*录像存储*/
typedef struct 
{
	unsigned char     	ucEnable;               	 	/*是否启动录像: 0关闭 1启动*/
	unsigned char     	ucStorageType;          	/*存储模式: 填写 ARG_STORAGE_TYPE*/
	unsigned char		ucPackingModel;    	 	/*打包模式 0按时间 1按大小 2无*/
	unsigned char 	ucPackingTime;     		/*打包时间 0-255 单位分钟(按时间打包模式下有效)*/

	unsigned char 	ucPackingSize;     		/*打包大小 0-125M, 1-256M, 2-512M, 3-1G, 4-2G, 5-4G (按大小打包模式下有效)*/
	unsigned char 	ucLackSpaceModel; 	 	/*0 为不覆盖1为覆盖*/
	unsigned char     	ucRes1[10];            	 	/*保留*/
	Week_Scope       	weekScope;       	 		/*周时间*/
}Record_Config_Data;


//////////////////////////////视频编码参数配置//////////////////////////
#define	MAX_CODEC_CHANNEL	5
#define   MAX_CODEC_MODE		4

typedef struct 
{
	int width;
	int height;
}Resolution_Param;


typedef struct 
{
	int sizeResolutions;
	Resolution_Param Resolutions[5];
}Resoulution_Available_Param;


/*视频编码属性*/
typedef struct 
{
	unsigned char  ucEncodingType;      /*视频编码 填写 (ARG_ENCODING_TYPE)的视频部分*/
	unsigned char  ucPicQuality;        /*图象质量 0-最好 1-次好 2-较好 3-一般 4-较差 5-差*/
	unsigned char  ucStreamType;	/*码流类型: 0:主码流,1:子码流*/
	unsigned char  ucProfile;		/*编码复杂度 0-低 1-中 3-高*/

	unsigned short usWidth;             /*宽度*/
	unsigned short usHeight;            /*高度*/
	unsigned short ucFrameRate;     	/*帧率*/
	unsigned short ucBitRate;           /*码率 单位k byte(大B)*/

	unsigned char  ucKeyFrameInterval;  /* I帧间隔,(ucEncodingType为h264时有效)*/
	unsigned char  ucBitrateType;       /*码率类型 0:定码率，1:变码率*/

	unsigned char  ucVideoType;		/*视频类型: 0:视频流 1:复合流*/

	unsigned char  ucRes1[21];

	Resoulution_Available_Param ReSolution; /*分辨率有效值*/
	
}Video_Encoding_Property;


typedef struct 
{
	unsigned char  ucCh;              /*通道号 ARG_CHANNEL_TYPE*/

	unsigned char  ucChStreamMode;    /*通道码流模式 0:单码流,1:双码流,2:三码流*/
	unsigned char  ucMaxFrameRate;
	unsigned char  ucMinFrameRate;
	unsigned char  ucEncodingScope;   /*视频编码支持类型,填写 ARG_ENCODING_SCOPE_TYPE*/	 

	unsigned char  ucRes[11];

	Video_Encoding_Property  Video[3];	

}Encoding_Config_Data;


//////////////////////////////鱼眼参数配置//////////////////////////
typedef enum 
{
	STREAM_MODE_ONE = 0,          /*模式1: 鱼眼+全景+3*PTZ*/
	STREAM_MODE_TWO,     	      /*模式2: 鱼眼或者4*PTZ*/
	STREAM_MODE_THREE,     	      /*模式3: 鱼眼主码流+鱼眼子码流+3PTZ*/
	STREAM_MODE_FOUR,   	      /*模式4: 全景主码流+全景子码流*/
}STREAM_MODE_TYPE;


typedef struct 
{
	unsigned char ucCh;

	unsigned char ucStreamMode;     	/*码流模式 填写 ARG_STREAM_MODE_TYPE*/
	unsigned char ucInstallMode;		/*安装方式 0:墙面 1:桌面 2:吸顶*/

	unsigned char ucRes[61];
}FishEye_Config_Data;


/////////////////////////////巡航参数配置//////////////////////////
#define	MAX_CHANNEL					4			/*最大通道*/
#define 	MAX_CRUISE_ROUTE 				32			/*最大巡航路径*/
#define 	MAX_CRUISE_ROUTE_PRESENT	16			/*每条巡航路径最多预置点数*/

#define 	MAX_CRUISE_POINT				256			/*最大预置点数*/


/*ptz 操作*/
typedef struct 
{
	unsigned char ucCh;                 		/*通道*/
	unsigned char ucOptType;            	/*操作类型: PTZ_OPTION_TYPE*/
	unsigned char ucRes1[2];           	 	/*保留*/
	union{
		unsigned char ucPresetPointIndex;   /*预置点序号*/
		unsigned char ucCruiseLineIndex;    	/*巡航线序号*/
	}opt;
}Ptz_Option;


//预置点属性
typedef struct 
{
	unsigned char      ucEnable;            				/*是否使能,0:不使能 1:使能*/
	unsigned char      ucRes[3];
	int      	            iPanPos;			
	int      	            iTiltPos;
	int 			     iZoomPos;
}Point_Param;


typedef struct 
{
	Point_Param 		point[MAX_CHANNEL][MAX_CRUISE_POINT];
}Preset_Point_Config_Data;


/*巡航点操作*/
typedef struct 
{
	unsigned char      ucCh;                               /*通道号*/
	unsigned char      ucCruiseLineIndex;           /*巡航线序号*/
	unsigned char      ucCruisePointIndex;         /*巡航点序号*/
	unsigned char      ucRes1[1];                       /*保留*/

	unsigned char      ucEnable;                         /*是否使能, 0不使能, 1使能*/
	unsigned char      ucPresetPointIndex;         /*预置点序号*/
	unsigned char      ucStep;                            /*速度，范围1~8，8控制效果最明显,从这个点移动到下个点的速度*/
	unsigned char      ucRes2[1];                        /*保留*/
	unsigned long      ulDwelltime;                      /*停留时间: 单位秒数, 最大30分钟*/
	unsigned char      ucRes3[16];                       /*保留*/
}Cruise_Point_Option;


/*巡航线操作*/
typedef struct 
{
	unsigned char              ucCh;                                       					/*通道*/
	unsigned char              ucIndex;                                    					/*巡航线序号*/
	unsigned char              ucEnable;                                   					/*是否启用, 0启用, 1不启用, 只读*/
	unsigned char              ucLineName[64];                		/*巡航线名称*/
	Cruise_Point_Option      ucPoints[32];   	/*巡航线下标, 最多 32 个预置点*/
	unsigned char              ucRes1[16];                                 				/*保留*/
}Cruise_Line_Option;


//巡航线属性
typedef struct 
{
	int    		index; 				//预置点序号		
	unsigned char 	ucCruiseTime;
	unsigned char       	ucCruiseSpeed;
	unsigned char		ucEnable;
	unsigned char    	ucCruisePointIndex; 	//巡航点序号	
}Cruise_Point_Param;


typedef struct 
{
	Cruise_Point_Param  cruise_point[MAX_CRUISE_ROUTE_PRESENT];
}Cruise_Route_Param;


typedef struct 
{
	Cruise_Route_Param   	cruise_route[MAX_CHANNEL][MAX_CRUISE_ROUTE];
}Cruise_Config_Data;


typedef struct
{
	unsigned char		ucCh;
	unsigned char		ucZoomPos;                      /*0-100 倍率*/
	unsigned char 	ucRes[2];
	int				iPanpos;
	int 				iTiltPos;
	unsigned char		ucRes1[8];                          /*保留*/
}Ptz_Info;


typedef struct 
{
	Ptz_Info			Info[MAX_CHANNEL];
}Ptz_Info_Config_Data;


typedef struct
{
	unsigned char 	ucCh;             /*通道*/
	unsigned char 	ucRes[3];
	Ptz_Info_Config_Data 		Info_Config;
	Cruise_Config_Data		Cruise_Config;			//云台巡航路线配置
	Preset_Point_Config_Data	Preset_Point_Config;		//云台预置点配置
}Ptz_Config_Data_Ex;



typedef struct 
{
	unsigned char ucCh;
	unsigned char ucOsdNameVisiable;	/*显示名称:0:不显示 1:显示*/
	unsigned char ucOsdDateVisiable;		/*显示日期:0:不显示 1:显示*/
	unsigned char ucOsdWeekVisiable;	/*显示星期:0:不显示 1:显示*/

	unsigned char ucOsdTimeFormat;		/*时间格式:0:12小时制 1:24小时制*/
	unsigned char ucOsdDateFormat;		/*日期格式:填写 ARG_DATE_FORMAT_TYPE*/
	unsigned char ucOsdDisplayMode;		/*OSD属性:填写 ARG_DISPLAY_MODE_TYPE*/
	unsigned char ucOsdFontSizeMode;	/*OSD字体大小:填写ARG_FONTSIZE_MODE_TYPE*/

	unsigned char ucOsdFontSizeScope;	/*OSD字体大小支持列表: 填写 ARG_FONTSIZE_MODE_TYPE*/

	unsigned char ucOsdTime_X_Coordinate;	/*OSD时间X坐标位置: 0-100 百分比*/
	unsigned char ucOsdTime_Y_Coordinate; 	/*OSD时间Y坐标位置: 0-100 百分比*/  	
	unsigned char ucOsdText_X_Coordinate;	/*OSD字体X坐标位置: 0-100 百分比*/ 
	unsigned char ucOsdText_Y_Coordinate;	/*OSD字体Y坐标位置: 0-100 百分比*/ 

	unsigned char ucRes[51];

	unsigned char ucOsdText[64];	/*OSD显示文本*/

}OSD_Config_Ex;



/********************************************************************************************/
/**********************************      报警布防配置   **********************************/
/********************************************************************************************/
typedef struct 
{
	unsigned char 	ucAlarmSmtpEnable;		   /*SMTP联动使能*/
	unsigned char 	ucAlarmFtpEnable;		   /*FTP联动使能*/
	unsigned char 	ucAlarmCloudEnalbe;		   /*上传云使能*/
	unsigned char 	ucAlarmRecordEnable;	   /*录像使能*/
	unsigned char 	ucAlarmIOEnable;		   	   /*IO联动使能*/
	unsigned char 	ucRes[11];
}Alarm_Process;


typedef struct 
{
	unsigned char       ucCh;                   		/*通道*/
	unsigned char       ucAlarmType;            		/*报警类型: 填写 ARG_ALARM_TYPE*/
	unsigned char       ucAlarmLinkScope;		/*报警联动类型,填写ARG_ALARM_LINK_TYPE*/
	unsigned char       ucRes1[29];             		/*保留1*/
	Hour_Scope		timeSocpeTab[7][8];     	/*时间表: 7天每天8个时段*/
	Alarm_Process	    	process;	    				/*报警联动处理*/
	unsigned char       ucRes3[64];             		/*保留3*/
}Alarm_Config_Data_Ex;


/********************************************************************************************/
/**********************************      遮挡报警配置   *********************************/
/********************************************************************************************/
/*矩形结构体*/
typedef struct 
{
    unsigned char	x1;                         /*左上角x坐标百分比*/
    unsigned char 	y1;                         /*左上角y坐标百分比*/
    unsigned char 	x2;                         /*右下角x坐标百分比*/
    unsigned char 	y2;                         /*右下角y坐标百分比*/
}Arg_Rect;


typedef struct 
{
	unsigned char 	ucCH;               		/*通道*/
	unsigned char 	ucEnable;           		/*0:关闭遮盖报警, 1:开启遮盖报警*/
	unsigned char 	ucSensitivity;      		/*灵敏度 0:低 1:中 2:高*/ 
	unsigned char   	ucAlarmLinkScope;   	/*报警联动类型,填写ARG_ALARM_LINK_TYPE*/  
	unsigned char 	ucRes[4];           		/*保留*/   
	Arg_Rect     		ucArea[6];          		/*6组百分比区域*/
	Hour_Scope 		timeSocpeTab[7][8];   /*时间表: 7天每天8个时段*/
	Alarm_Process		process;	    			/*报警联动处理*/
	unsigned char 	ucRes1[16];          	/*保留*/
}VideoCover_Alarm_Config_Data;


/********************************************************************************************/
/*******************************       TCP/IP网络配置扩展   ******************************/
/********************************************************************************************/
/*TCP/IP配置*/
typedef struct 
{
	unsigned char ucCh;                              		/*通道号*/
	unsigned char ucNetCardType;                     	/*网卡类型,填写ARG_NETCARD_TYPE*/
	unsigned char ucEnDHCP;                          	/*使能dhcp, 0开启 1关闭*/
	unsigned char ucIPAddrTestEnable;			/*使能IP地址测试, 0:关闭 1:开启*/
	unsigned char ucIPAddrStatus;				/*IP地址状态,0:可用 1:不可用*/
	unsigned char ucRes1[11];                         	/*保留*/
	unsigned char ucIPV4[16];          		/*IPV4地址*/
	unsigned char ucSubNetMaskV4[16];     	/*IPV4子网掩码*/
	unsigned char ucGatewayV4[16];        	/*IPV4网关*/
	unsigned char ucDNS1Ip[16];           		/*dns 服务器1*/
	unsigned char ucDNS2Ip[16];           		/*dns 服务器2*/
	unsigned char ucIPV6Mode;			     	/*IPV6模式:填写ARG_IPV6MODE_TYPE*/
	unsigned char ucRes2[3];
	int 	  		iMaxTransUnit;		     	/*MTU*/
	unsigned char ucIPV6[40];           		/*IPV6地址*/
	int		 	  iSubNetMaskV6;   		/*IPV6子网掩码*/
	unsigned char ucGatewayV6[40];      		/*IPV6网关*/
	unsigned char ucMulticast[40];      		/*多播地址*/
	unsigned char ucMacIp[20];                     /*Mac服务器地址: 填写字符串例如 "A1:B2:C3:D4:E5:F6"*/
	unsigned char ucRes3[36];                       /*保留*/
}Net_Config_Data_Ex;



/********************************************************************************************/
/***********************************      异常事件    **************************************/
/********************************************************************************************/
typedef struct 
{
    unsigned char 	ucCh;                      		/*通道号*/	
    unsigned char 	ucAbnormalType;			/*异常类型,填写ARG_ABNORMAL_TYPE*/
    unsigned char   	ucAlarmLinkScope;	   	/*报警联动类型,填写ARG_ALARM_LINK_TYPE*/
    unsigned char 	ucRes[45];			       /*保留*/
    Alarm_Process	process;		           		/*报警联动处理*/		
}Abnormal_Event_Config_Data;



/**
* @brief system info main data structure.
*/
typedef struct SysInfo_0{	
	unsigned int 				relUniqueId;						///< IPCAM device type
	Acount_t     				acounts[ACOUNT_NUM];				///< user account data
	Network_Config_Data		net;								///< network status and user settings
	Lancam_Config_Data 		lan_config;							///< IPCAM configuration data
	Factory_config_Data 		factory_config;
	Motion_Config_Data 		motion_config;						///< motion detection configuration data
	PrivacyMask_Config_Data	privacy_mask_config;
	Audio_Config_Data 		audio_config;
	OSD_config 				osd_config[MAX_OSD_STREAMS];
	CodecAdvParam 			codec_advconfig[MAX_CODEC_STREAMS];
	StreamParam 				stream_config[MAX_CODEC_STREAMS];
	Ftp_Config_Data 			ftp_config;							///< Ftp configuration data
	Smtp_Config_Data 			smtp_config;   						///< Smtp configuration data
	LogEntry_t				tCurLog;		    				///< event log
	AutoReboot_Config_Data     autoreboot_config;		    			
	Ddns_Config_Data 			ddns_config;  						//DDNS
	Http_Config_Data 			http_config;  						//http config
	P2P_Config_Data  			p2p_config;
	AutoMainTain_Config_Data	AutoMainTain_config;
	Alarm_Config_Data 		alarm_config;
	char 					reserve1[3];
	ROI_Config_data			Roi_config;
	LDC_Config_data			Ldc_config;
	Sdcard_Config_Data 		sdcard_config;
	CloudStorage_Config_Data	cloudstorage_config;
	Ptz_Config_Data			Ptz_config;
	Record_Config_Data            Record_config;
	DST_Config_Data			DST_config;
	
	//char 					reserve[548];
	char 					reserve[487];
}SysInfo_0;


/**
* @brief system info main data structure.
*/
typedef struct SysInfo_1{	
	unsigned int 					relUniqueId1;	
	Net_Config_Data_Ex			Net_Config_Ex;
	char 						reRes0[72];
	VideoCover_Alarm_Config_Data	VideoCoverAlarm_Config;
	char 						reRes1[152];		//1024
	Alarm_Config_Data_Ex			Alarm_Config_Ex[2];
	Abnormal_Event_Config_Data	Abnormal_Config[4]; 
	char 						reRes2[112];		//1024
	
	char 						reRes[14 * 1024];
}SysInfo_1;



/**
* @brief system info main data structure.
*/
typedef struct SysInfo_2{	
	unsigned int 			relUniqueId2;	

	Encoding_Config_Data     	Video_Encoding_Config[MAX_CODEC_MODE][MAX_CODEC_CHANNEL];  //5K
	OSD_Config_Ex			Osd_Config_Ex[MAX_CODEC_CHANNEL];
	FishEye_Config_Data		FishEye_Config;
	char 					reRes1[320];	
	char 					reRes2[4 * 1024];	

	char 					reRes3[768];
	Ptz_Config_Data_Ex		Ptz_Config_Ex;
	char 					reRes4[5 * 1024];
}SysInfo_2;


/**
* @brief system info main data structure.
*/
typedef struct SysInfo_3{	
	unsigned int 			relUniqueId2;	
	char 				reRes[32 * 1024];	
}SysInfo_3;


/**
* @brief system info main data structure.
*/
typedef struct SysInfo{	
	unsigned int 				relUniqueId;						///< IPCAM device type
	Acount_t     				acounts[ACOUNT_NUM];				///< user account data
	Network_Config_Data		net;								///< network status and user settings
	Lancam_Config_Data 		lan_config;							///< IPCAM configuration data
	Factory_config_Data 		factory_config;
	Motion_Config_Data 		motion_config;						///< motion detection configuration data
	PrivacyMask_Config_Data	privacy_mask_config;
	Audio_Config_Data 		audio_config;
	OSD_config 				osd_config[MAX_OSD_STREAMS];
	CodecAdvParam 			codec_advconfig[MAX_CODEC_STREAMS];
	StreamParam 				stream_config[MAX_CODEC_STREAMS];
	Ftp_Config_Data 			ftp_config;							///< Ftp configuration data
	Smtp_Config_Data 			smtp_config;   						///< Smtp configuration data
	LogEntry_t				tCurLog;		    				///< event log
	AutoReboot_Config_Data     autoreboot_config;		    			
	Ddns_Config_Data 			ddns_config;  						//DDNS
	Http_Config_Data 			http_config;  						//http config
	P2P_Config_Data  			p2p_config;
	AutoMainTain_Config_Data	AutoMainTain_config;
	Alarm_Config_Data 		alarm_config;
	char 					reserve0[3];
	ROI_Config_data			Roi_config;
	LDC_Config_data			Ldc_config;
	Sdcard_Config_Data 		sdcard_config;
	CloudStorage_Config_Data	cloudstorage_config;
	Ptz_Config_Data			Ptz_config;
	Record_Config_Data            Record_config;
	DST_Config_Data			DST_config;

	//char 					reserve[548];
	char 					reserve[487];

	unsigned int 					relUniqueId1;	
	Net_Config_Data_Ex			Net_Config_Ex;
	char 						reRes0[72];
	VideoCover_Alarm_Config_Data	VideoCoverAlarm_Config;
	char 						reRes1[152];		//1024
	Alarm_Config_Data_Ex			Alarm_Config_Ex[2];
	Abnormal_Event_Config_Data	Abnormal_Config[4]; 
	char 						reRes2[112];		//1024
	
	char 						reRes[14 * 1024];

	char 				reserver3[32 * 1024];

	///use for fish platform
	unsigned int 				relUniqueId2;	
	Encoding_Config_Data     	Video_Encoding_Config[MAX_CODEC_MODE][MAX_CODEC_CHANNEL];  //5K
	OSD_Config_Ex			Osd_Config_Ex[MAX_CODEC_CHANNEL];
	FishEye_Config_Data		FishEye_Config;
	char 					reRes11[320];	
	char 					reRes12[4 * 1024];	

	char 					reRes13[768];
	Ptz_Config_Data_Ex		Ptz_Config_Ex;
	char 					reRes14[5 * 1024];
}SysInfo;

#endif /* __SYS_ENV_TYPE_H__ */



