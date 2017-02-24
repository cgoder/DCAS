//<Skyworth Software>
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SKYWORTH_CA_H_
#define _SKYWORTH_CA_H_

#include "skycas_datatype.h"	
#include "skycas_driver.h"	

#define SKYCAS_MAXNUM_PROGRAMBYCW   (4)  /* 一个控制字最多解的节目数 */
#define SKYCAS_MAXLEN_PINCODE			(8)		/*最长的PIN码长度*/
#define SKYCAS_MAXLEN_TVS_NAME			(32)		/*最长运营商名称长度*/
#define SKYCAS_MAXLEN_PRODUCT_NAME (21)	/*最长的产品名称,最多20个字符*/
#define SKYCAS_MAXNUM_ENTITLE	(300)	/*授权产品最大个数*/

#define SKYCAS_MAXLEN_EMAIL_SENDER_NAME		(11)		/*E-mail发送者姓名*/
#define SKYCAS_MAXLEN_EMAIL_CONTENT			(1025)	/*E-mail最大长度*/
#define SKYCAS_MAXLEN_EMAIL_TITLE				(37)		/*E-mail标题长度*/

#define SKYCAS_MAX_FILTER_COUNT 	(12)
#define SKYCAS_FILE_PATH_LEN_MAX 	(4096+1)

#define SKYCAS_MAXLEN_CAT_MAX 	(1024)

#define SKYCAS_MAXNUM_PRICE         (2)    /* 最多的IPPV价格个数 */

/*------------ 邮件图标显示方式 ------------*/
#define SKYCAS_Email_IconHide       (0x00)  /* 隐藏邮件通知图标 */
#define SKYCAS_Email_New            (0x01)  /* 新邮件通知，显示新邮件图标 */
#define SKYCAS_Email_SpaceExhaust   (0x02)  /* 磁盘空间以满，图标闪烁。 */

#define SKYCAS_MAXNUM_COMPONENT     (5)    /* 节目组件最大个数 */
#define SKYCAS_MAXLEN_LOCKMESS      (40)
#define SKYCAS_MAXNUM_CARD 			(4) // 同时插入卡的最大个数

#define SKYCAS_U32_HANDLE_INVALID 	(0xFFFFFFFF)
#define SKYCAS_U16_HANDLE_INVALID 	(0xFFFF)
#define SKYCAS_U8_HANDLE_INVALID 	(0xFF)
#define SKYCAS_PID_INVALID 			(0x1FFF)

/*---------- 返回值类型相关定义 ---------*/
#define SKYCAS_CA_OK                    0x00  /* 成功 */
#define SKYCAS_CA_UNKNOWN               0x01  /* 未知错误 */
#define SKYCAS_CA_POINTER_INVALID       0x02  /* 指针无效或参数无效 */
#define SKYCAS_CA_CARD_INVALID          0x03  /* 智能卡无效*/
#define SKYCAS_CA_PIN_INVALID           0x04  /* PIN码无效 */
#define SKYCAS_CA_DATASPACE_SMALL       0x06  /* 所给的空间不足 */
#define SKYCAS_CA_CARD_PAIROTHER        0x07  /* 智能卡已经对应别的机顶盒 */
#define SKYCAS_CA_DATA_NOT_FIND         0x08  /* 没有找到所要的数据 或CAT 没有收到*/
#define SKYCAS_CA_PROG_STATUS_INVALID   0x09  /* 要购买的节目状态无效 */
#define SKYCAS_CA_CARD_NO_ROOM          0x0A  /* 智能卡没有空间存放购买的节目 */
#define SKYCAS_CA_WORKTIME_INVALID      0x0B  /* 设定的工作时段无效 */
#define SKYCAS_CA_IPPV_CANNTDEL         0x0C  /* IPPV节目不能被删除 */
#define SKYCAS_CA_CARD_NOPAIR           0x0D  /* 智能卡没有对应任何的机顶盒 */
#define SKYCAS_CA_WATCHRATING_INVALID   0x0E  /* 设定的观看级别无效 */
#define SKYCAS_CA_CARD_NOTSUPPORT       0x0F  /* 当前智能卡不支持此功能 */
#define SKYCAS_CA_DATA_ERROR            0x10  /* 数据错误，智能卡拒绝或智能卡通讯错误  */
#define SKYCAS_CA_FEEDTIME_NOT_ARRIVE   0x11  /* 喂养时间未到，子卡不能被喂养 */
#define SKYCAS_CA_CARD_TYPEERROR        0x12  /* 子母卡喂养失败，插入智能卡类型错误 */
#define SKYCAS_CA_CARDHANDLE_INVALID    0x13  /* 卡句柄无效 */
#define SKYCAS_CA_CALIB_STOP    		0x14  /* ca库停止运行*/
#define SKYCAS_CA_SEMP_TIMEOUT			0x15  /* 信号量超时*/
#define SKYCAS_CA_ENTITLED				0x16  /* 节目已授权*/
#define SKYCAS_CA_NO_ENTITLED			0x17  /* 节目未授权*/
#define SKYCAS_CA_NO_ENTITLEDRECORD		0x18  /*没有节目授权记录*/
#define SKYCAS_CA_ENTITLED_OVERDUE		0x19  /* 授权过期*/
#define SKYCAS_CA_TV_LOCKED				0x1A  /* 电视或机顶盒被锁定*/
#define SKYCAS_CA_PAIRING_FAIL			0x1B  /* 配对失败*/
#define SKYCAS_CA_EMM_TIMEOUT			0x1C  /* 收取EMM超时*/
#define SKYCAS_CA_NOT_PARENTCHILDCARD	0x1D  /* 没有分配子母卡属性*/
#define SKYCAS_CA_PARENTCARD			0x1E  /*  该机顶盒是主机*/
#define SKYCAS_CA_MONEY_NOTENOUGH		0x1F  /*  余额不足*/




typedef enum skycas_camessagetype_e
{	
    SKYCAS_CA_SHOWBUYMESSAGE    = 0x00,
    SKYCAS_CA_SHOWFINGERMESSAGE  = 0x01,
    SKYCAS_CA_SHOWPROGRESSSTRIP = 0x02,
    SKYCAS_CA_ACTIONREQUEST     = 0x03,
    SKYCAS_CA_ENTITLECHANGED    = 0x04,
    SKYCAS_CA_DETITLERECEIVED   = 0x05,
    SKYCAS_CA_STARTIPPVBUYDLG   = 0x06,
    SKYCAS_CA_HIDEIPPVDLG        = 0x07,
    SKYCAS_CA_EMAILNOTIFYICON   = 0x08,
    SKYCAS_CA_SHOWOSDMESSAGE    = 0x09,
    SKYCAS_CA_HIDEOSDMESSAGE    = 0x0a,
    SKYCAS_CA_REQUESTFEEDING    = 0x0b,
    SKYCAS_CA_LOCKSERVICE       = 0x0c,
    SKYCAS_CA_UNLOCKSERVICE     = 0x0d,
}SKYCAS_CaMessageType_E;


/*-- 频道锁定信息 --*/

/*-- 节目组件信息 --*/
typedef struct skycas_component_s
{    
/* 组件用于通知机顶盒节目类型及PID等信息，一个节目可能包含多个组件 */    
	SKYCAS_U16   u16CompPID;     /* 组件PID */    
	SKYCAS_U16   u16ECMPID;      /* 组件对应的ECM包的PID，如果组件是不加扰的，则应取0。 */    
	SKYCAS_U8    u8CompType;     /* 组件类型 */    
	SKYCAS_U8    u8Reserved[3];/* 保留 */
}SKYCAS_Component_S;

/*-- 频道参数信息 --*/
typedef struct skycas_lockservice_s{        
	SKYCAS_U32   u32Frequency;              /* 频率，BCD码 */    
	SKYCAS_U32   u32symbol_rate;              /* 符号率，BCD码 */    
	SKYCAS_U16   u16PcrPid;                  /* PCR PID */    
	SKYCAS_U8    u8Modulation;               /* 调制方式 */    
	SKYCAS_U8    u8ComponentNum;             /* 节目组件个数 */    
	SKYCAS_Component_S stCompArr[SKYCAS_MAXNUM_COMPONENT];       /* 节目组件列表 */    
	SKYCAS_U8    u8fec_outer;                /* 前项纠错外码 */    
	SKYCAS_U8    u8fec_inner;                /* 前项纠错内码 */    
	SKYCAS_CHAR       s8BeforeInfo[SKYCAS_MAXLEN_LOCKMESS+1]; /* 保留 */    
	SKYCAS_CHAR       s8QuitInfo[SKYCAS_MAXLEN_LOCKMESS+1];   /* 保留 */    
	SKYCAS_CHAR       s8EndInfo[SKYCAS_MAXLEN_LOCKMESS+1];    /* 保留 */
}SKYCAS_LockService_S;

/*-- 高级预览节目信息 --*/
typedef struct skycas_curtaininfo_s
{    
	SKYCAS_U16   u16ProgramID;        /* 节目的ID */    
	SKYCAS_U32  u32StartWatchTime;   /* 起始观看时间 */    
	SKYCAS_U8    u8WatchTotalCount;  /* 累计观看次数 */    
	SKYCAS_U16   u16WatchTotalTime;    /* 累计观看时长,(cp周期数) */
}SKYCAS_CurtainInfo_S;


/* 初始化信息 */
typedef struct skycas_init_para_s
{
	SKYCAS_U16 u16StartPrio;		/*优先级 */
	SKYCAS_U16 u16CASType;			/* CA 类型选择*/
	SKYCAS_CHAR* pFilePath;		/*文件路径*/
	SKYCAS_U32 u32FlashAddr;		/*起始地址*/
	SKYCAS_U32 u32FlashSize;		/*长度*/
}SKYCAS_INIT_PARA_S;

/* 节目信息 */
typedef struct skycas_serviceinfo_s
{
    SKYCAS_U16 u16EcmPid;      /* ECM PID */
    SKYCAS_U8 u8ServiceNum;    /* 当前PID下的节目个数 */
    SKYCAS_U8 u8Reserved;      /* 保留 */
	SKYCAS_U16 u16ServiceID[SKYCAS_MAXNUM_PROGRAMBYCW]; 
} SKYCAS_SERVICEINFO_S;

/* pin码信息*/
typedef struct skycas_pininfo_s
{
	SKYCAS_U16	u8PinLen;
	SKYCAS_U8	u8Pin[SKYCAS_MAXLEN_PINCODE];
} SKYCAS_PININFO_S;

/* 日期时间信息*/
typedef struct skycas_datetime_s
{
	SKYCAS_U16	u16Year;
	SKYCAS_U8	u8Month;
	SKYCAS_U8	u8Day;
	SKYCAS_U8	u8Hour;
	SKYCAS_U8	u8Min;
	SKYCAS_U8	u8Sec;
	SKYCAS_U8	u8Reserve;
} SKYCAS_DATETIME_S;

/* 运营商信息 */
typedef struct skycas_operatorinfo_s
{
	SKYCAS_U16	u16TVSID; 									/*运营商的外部编号*/
	SKYCAS_U8	u8TVSName[SKYCAS_MAXLEN_TVS_NAME+1];		/*运营商名称*/
	SKYCAS_U8  u8Reserved[3];    						/* 保留 */
}SKYCAS_OPERATORINFO_S;

/* 钱包信息 */
typedef struct skycas_tvslotinfo_s
{
    SKYCAS_FLOAT fSlotLeft; 		/*钱包余额 */
	SKYCAS_FLOAT fSlotSpent;     /*已花费的金额*/ 
}SKYCAS_TVSLOTINFO_S;

/* 授权信息 */
typedef struct skycas_entitleinfo_s
{
    SKYCAS_U32  u32ProductID;   /* 普通授权的节目ID */    
	//SKYCAS_U16 stEntitleTime;    /*授权时间 */
	SKYCAS_DATETIME_S stBeginTime;    /* 授权的起始时间 */
	SKYCAS_DATETIME_S stEndTime;   /* 授权的结束时间 */
	//SKYCAS_U8	u8ProductName[SKYCAS_MAXLEN_PRODUCT_NAME];/*产品名称*/
    SKYCAS_U8  u8Video;    /* 是否录像：1－可以；0－不可以*/
    SKYCAS_U8  u8Reserved[3];	 /* 保留 */
} SKYCAS_ENTITLEINFO_S;

/*-- 授权信息集合 --*/
typedef struct skycas_entitles
{
    SKYCAS_U16      u16ProductCount;
    SKYCAS_U8       u8Reserved[2];    /* 保留 */
    SKYCAS_ENTITLEINFO_S  stEntitles[SKYCAS_MAXNUM_ENTITLE]; /* 授权列表 */
}SKYCAS_Entitles;


/* IPPV/IPPT节目的价格 */
typedef struct skycas_ippv_price_s
{
    SKYCAS_U16  u16Price;       /* 节目价格*/
    SKYCAS_U8   u8PriceCode;  /* 节目价格类型 */
    SKYCAS_U8   u8Reserved;   /* 保留 */
} SKYCAS_IPPV_PRICE_S;

/* IPPV节目信息 */
typedef struct
{
    SKYCAS_U32   u32ProductID;   /* 节目的ID */
    SKYCAS_FLOAT u32Price;        /* 节目价格 */
    SKYCAS_DATETIME_S  	 stEndDate;  /* 节目过期时间*/
    SKYCAS_DATETIME_S  	 stStartDate;  /* 节目开始时间*/
    SKYCAS_U8    u8Flag;  /* 产品状态：BOOKING，VIEWED */ 
    SKYCAS_U8    u8Video;      /* 是否可以录像：1－可以；0－不可以*/
    SKYCAS_U8    u8SlotID;      /* 钱包ID */
    SKYCAS_U8    u8Reserved;    /* 保留 */
} SKYCAS_IPPVINFO_S;

/*-- IPPV/IPPT节目购买提示信息 --*/
typedef struct skycas_ippvbuyinfo_s{    
	//SKYCAS_U8         u8scNum;				/*支持的卡的个数*/	
	//SKYCAS_VOID*      scHandle[SKYCAS_MAXNUM_CARD];             /* 使用的卡句柄 */    
	SKYCAS_U32        u32ProductID;          /* 节目的ID */    
	SKYCAS_U16        u16TvsID;               /* 运营商ID */    
	SKYCAS_U8         u8SlotID;             /* 钱包ID */    
	SKYCAS_U8         u8PriceNum;           /* 节目价格个数 */    
	SKYCAS_IPPV_PRICE_S  stPrice[SKYCAS_MAXNUM_PRICE]; /* 节目价格 */
    union {        
		SKYCAS_DATETIME_S   stExpiredDate;         /* 节目过期时间,IPPV用 */        
		SKYCAS_U16    u16IntervalMin;         /* 时间间隔，单位分钟,IPPT 用 */    
		}SKYCAS_IPPVTime;    
	SKYCAS_DATETIME_S	stStartDate;		   /* 节目开始时间,IPPV用 */		
}SKYCAS_IppvBuyInfo_S;

/*-- IPPV/IPPT节目购买提示信息 --*/
typedef struct skycas_ippvbuyallinfo
{
    SKYCAS_U8   u8MessageType;
    SKYCAS_U16  u16EcmPid;
    SKYCAS_IppvBuyInfo_S  stIppvbuyinfo;
}SKYCAS_IppvBuyAllInfo_S;


/* 邮件头部信息 */
typedef struct skycas_mailheadinfo_s
{
	SKYCAS_U32	u32EmailID;                 /* Email ID */
	SKYCAS_DATETIME_S	u32SendTime;						  /*发送时间*/
	SKYCAS_U8	u8SenderName[SKYCAS_MAXLEN_EMAIL_SENDER_NAME];	/*发送者姓名*/
	SKYCAS_U8  u8EmailHead[SKYCAS_MAXLEN_EMAIL_TITLE]; 	/* 邮件标题*/
	SKYCAS_U8  u8NewEmail;           /* 新邮件标记：0－已读；1－新邮件 */
	SKYCAS_U16  u8Importance;         /* 重要性： 0－普通，1－重要 */
	SKYCAS_U8  u8Reserve;         /* 保留位 */
}SKYCAS_EMAILHEADINFO_S;

/*邮件内容信息*/
typedef struct skycas_mailcontent_s
{
	SKYCAS_U8	u8MailContent[SKYCAS_MAXLEN_EMAIL_CONTENT];		/*邮件内容*/
	SKYCAS_U8	u8Reserve[3];		/*保留*/
	SKYCAS_U32	u32EmailLen;					  /*邮件内容长度*/
}SKYCAS_EMAILCONTENT_S;
#if 0
typedef struct skycas_event_s
{
	SKYCAS_U32 u32EventId;					/*消息ID*/
	//SKYCAS_U8 * u8EventContent;			/*消息内容*/
	SKYCAS_U16 	 u16EcmPid;
	SKYCAS_U8	 u8MessageType;
	SKYCAS_U8    u8Reserve;
}SKYCAS_EVENT_S;
#endif

/*第一个参数传消息类型，后面三个参数按照库传过来的参数顺序依次使用*/
/*比如CDSTBCA_ShowBuyMessage中，u32Param1传ecmpid,u32Param2传byMessageType，u32Param3不使用置为全F*/
/*如果是指针，直接强转用U32来存*/
typedef struct skycas_event_s
{
	SKYCAS_CaMessageType_E u32EventId;					/*消息类型*/
	SKYCAS_U32 	 u32Param1;
	SKYCAS_U32	 u32Param2;
	SKYCAS_U32   u32Param3;
}SKYCAS_EVENT_S;

typedef enum
{
	SKYCAS_TYPE_TF = 0,	/*永新视博/同方*/
	SKYCAS_TYPE_DVT,	/*数码视讯*/
	SKYCAS_TYPE_DVN,	/*天柏*/
	SKYCAS_TYPE_IRDETO,	/*爱迪德*/
	SKYCAS_TYPE_NDS,	/*NDS*/
	SKYCAS_TYPE_INVALID	/*无效值*/
}SKYCAS_TYPE_E;

/*交易信息*/
typedef struct skycas_transactrecord_s
{
	SKYCAS_U8 u8Type;
	SKYCAS_U8 u8ID[3];
	SKYCAS_FLOAT FPrice;
	SKYCAS_DATETIME_S stDateTime;
	SKYCAS_U8 u8ConfirmCode[8];
}SKYCAS_TRANSACTRECORD_S;

class skyworth_ca_api
{
public:

/***********************************************************************
函数名: 
skyworth_ca_api(SKYCAS_VOID)
功能:  
构造函数。 
参数:  
[IN]		无。  
[OUT]		无。  
[RET]		无。 
************************************************************************/
skyworth_ca_api(SKYCAS_VOID);

/***********************************************************************
函数名: 
~skyworth_ca_api(SKYCAS_VOID)
功能:  
析构函数。 
参数:  
[IN]		无。  
[OUT]		无。  
[RET]		无。 
************************************************************************/
~skyworth_ca_api(SKYCAS_VOID);

/***********************************************************************
函数名:
	static skyworth_ca_api* GetCaInstance(SKYCAS_VOID);
功能:  
得到类的一个实例。 
参数:  
[IN]		
无。  
[OUT]		无。  
[RET]		
返回一个类的实例。 
************************************************************************/
static skyworth_ca_api* GetCaInstance(SKYCAS_VOID);

/***********************************************************************
函数名:
	static SKYCAS_VOID DestroyInstance(SKYCAS_VOID);
功能:  
销毁一个类的实例。 
参数:  
[IN]		
[OUT]		无。  
[RET]		无。 
************************************************************************/
static SKYCAS_VOID DestroyInstance(SKYCAS_VOID);

/***********************************************************************
函数名: 
	SKYCAS_U32 SKYCAS_GetCAType(SKYCAS_VOID);
功能:  
获取当前CA类型
参数:  
[IN]		无。
[OUT]		无。  
[RET]		
	返回CA类型
************************************************************************/
SKYCAS_U32 SKYCAS_GetCAType(SKYCAS_VOID);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara );
功能:  
CA初始化。 
参数:  
[IN]		
SKYCAS_INIT_PARA_S * pInitPara：初始化参数。  
[OUT]		无。  
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara);

/***********************************************************************
函数名: 
virtual SKYCAS_ S32 SKYCAS_CaDeInit(SKYCAS_VOID);
功能:  
CA去初始化。 
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_CaDeInit(SKYCAS_VOID);

/***********************************************************************
函数名: 
virtual SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);
功能:  
判断参数SystemID是否是当前CA的SystemID。 
参数:  
[IN]		
SKYCAS_U16 u16SystemID：CA系统ID。  
[OUT]		无。  
[RET]		
是返回SKYCAS_TURE；否返回SKYCAS_FALSE。 
************************************************************************/
virtual SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);
功能:  
设置EMMPID。 
参数:  
[IN]		
SKYCAS_U16 u16EmmPid：EMMPID。
[OUT]		无。
[RET]		
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);
功能:  
CAT表变化时更新EMMPID。 
参数:  
[IN]		
SKYCAS_U8 *pbuffer:CAT表数据指针。  
[OUT]		无。  
[RET]		
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
功能:  
设置ECMPID。 
参数:  
[IN]		
pServiceInfo:service信息结构体指针。
[OUT]		无。
[RET]		
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
/***********************************************************************
函数名: 
    virtual SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
功能:  
    停ECM过滤。其中参数pServiceInfo中ecmPid，传入需要停止的ECMPID。
    用法与SKYCAS_SetEcmPid函数一样。
    特别的，当传入参数为SKYCAS_NULL时，将停止所有ECMPID过滤。
参数:  
[IN]		
    pServiceInfo:service信息结构体指针。
[OUT]		
    无。
[RET]		
    成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);
功能:  
上层传NIT数据给CA模块,主要用于同方CA
参数:  
[IN]		
SKYCAS_U8 *DataBuf:数据地址
[OUT]		无。
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);

/* 智能卡管理 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);
功能:  
获取智能卡的外部卡号。 
参数:  
[IN]		
SKYCAS_U8* pCardSN：卡号数据指针。  
[OUT]		
SKYCAS_U8* pCardSN：卡号数据指针。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);

/***********************************************************************
函数名: 
virtual SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
功能:  
智能卡主线程。
参数:  
[IN]		
	SKYCAS_VOID* para：预留参数
[OUT]		无
[RET]		
virtual SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para);
************************************************************************/

/* PIN码管理 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
												const SKYCAS_U8* pNewPin);
功能:  
更改PIN码。 
参数:  
[IN]		
const SKYCAS_U8* pOldPin：旧的PIN码
const SKYCAS_U8* pNewPin：新的PIN码
注:天柏CA传入为四个字节ASCII码
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:修改成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_PIN_INVALID:PIN码错误
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CARD_NOTSUPPORT:智能卡不支持此功能
************************************************************************/
virtual SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
                               				const SKYCAS_U8* pNewPin);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);
功能:  
校验用户输入pin码是否与卡中一致。 
参数:  
[IN]		
const SKYCAS_PinInfo *pPIN：用户输入pin码  
[OUT]		无。  
[RET]		
一致返回SKYCAS_RET_SUCCESS；不一致返回SKYCAS_RET_FAILURE
************************************************************************/
virtual SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);
功能:  
判断用户PIN码是否被锁住。 
参数:  
[IN]		
	  SKYCAS_BOOL * bLocked：true表示锁住了，false表示没有
[OUT]		无。  
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE
************************************************************************/
virtual SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);

/* 用户观看级别管理 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );
功能:  
设置观看级别。 
参数:  
[IN]		
const SKYCAS_U8* pPin：用户输入的pin码  
SKYCAS_U8 u8Rating：要设置的新的观看级别
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_PIN_INVALID:PIN码无效
SKYCAS_CA_WATCHRATING_INVALID:观看级别无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );
功能:  
获取当前观看级别 
参数:  
[IN]		
SKYCAS_U8* pRating：保存获取到的观看级别  
[OUT]		
	SKYCAS_U8* pRating：保存获取到的观看级别
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );

/* 智能卡工作时段管理 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
                                 SKYCAS_DATETIME_S *StartTime,
                                 SKYCAS_DATETIME_S *EndTime); 

功能:  
设置工作时间段。 
参数:  
[IN]		
  const SKYCAS_U8* pPin：用户输入的pin码
  SKYCAS_DATETIME_S *StartTime：工作时间起始时间
  SKYCAS_DATETIME_S *EndTime：工作时间结束时间
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_PIN_INVALID:PIN码无效或错误
SKYCAS_CA_WORKTIME_INVALID:工作时间无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
												SKYCAS_DATETIME_S *StartTime,
												SKYCAS_DATETIME_S *EndTime);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
                                 	SKYCAS_DATETIME_S *stEndTime);
功能:  
查询智能卡当前工作时段
参数:  
[IN]		
SKYCAS_DATETIME_S *stStartTime,
SKYCAS_DATETIME_S *stEndTime
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
												SKYCAS_DATETIME_S *stEndTime);

/* 基本信息查询 */
/***********************************************************************
函数名: 
virtual SKYCAS_U32 SKYCAS_GetVer(SKYCAS_VOID); 
功能:  
查询CA_LIB版本号
参数:  
[IN]		
SKYCAS_U8 *pData:数据指针，天柏时使用由6 个字节的ASCII 组成，
					数码CA为获取到数据的指针
					同方CA传0
[OUT]		
SKYCAS_U8 *pData:数据指针指向返回值
[RET]		
天柏CA和数码CA返回成功失败
同方CA返回CA库版本号
************************************************************************/
virtual SKYCAS_U32 SKYCAS_GetVer(SKYCAS_U8 *pData);

/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);
功能:  
查询当前机器平台编号
参数:  
[IN]		无。
[OUT]		无。
[RET]		
平台编号
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );
功能:  
查询运营商ID列表
参数:  
[IN]		
SKYCAS_U16* pTVSID:  保存返回运营商ID 的数组
[OUT]		
SKYCAS_U16* pTVSID:  保存返回运营商ID 的数组
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
                              SKYCAS_OPERATORINFO_S * stOperatorInfo ); 
功能:  
获取运营商信息
参数:  
[IN]		
SKYCAS_U16 u16TVSID：运营商ID
SKYCAS_U8 * pCount：数组的长度，返回时是运营商信息的个数
SKYCAS_OPERATORINFO_S * stOperatorInfo ：运营商信息数组指针
[OUT]		
SKYCAS_U8 * pCount：运营商信息的个数
SKYCAS_OPERATORINFO_S * stOperatorInfo ：运营商信息数组指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
										SKYCAS_OPERATORINFO_S* stOperatorInfo );

/***********************************************************************
函数名:
virtual SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );
功能:  
查询用户特征
参数:  
[IN]		
SKYCAS_U16 u16TVSID: 运营商ID
SKYCAS_U32* pACArray :用户特征信息数组指针
[OUT]		
SKYCAS_U32* pACArray :用户特征信息数组指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );

/*钱包信息查询 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID , SKYCAS_U32 *pSlotNum);
功能:  
查询钱包ID列表
参数:  
[IN]		
SKYCAS_U16 u16TVSID：运营商ID
SKYCAS_U8* pSlotID：钱包ID数组指针
SKYCAS_U32 *pSlotNum:钱包ID个数指针
[OUT]		
SKYCAS_U8* pSlotID：钱包ID数组指针
SKYCAS_U32 *pSlotNum:钱包ID个数指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID);//, SKYCAS_U32 *pSlotNum);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S * pSlotInfo );
功能:  
查询钱包的详细信息
参数:  
[IN]		
SKYCAS_U16  u16TVSID: 运营商ID
SKYCAS_U8   u8SlotID:钱包ID
SKYCAS_TVSLOTINFO_S * pSlotInfo：钱包信息数组指针
[OUT]		 
SKYCAS_TVSLOTINFO_S * pSlotInfo：钱包信息数组指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S* pSlotInfo );

/* 授权信息管理 */
/***********************************************************************
函数名:
virtual SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
									SKYCAS_ENTITLEINFO_S * pServiceEntitles );
功能:  
查询普通授权节目购买情况
参数:  
[IN]		
SKYCAS_U16 u16TVSID: 运营商ID
SKYCAS_Entitles * pServiceEntitles :授权信息结构体指针
[OUT]		 
SKYCAS_Entitles * pServiceEntitles :授权信息结构体指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
													SKYCAS_Entitles* pServiceEntitles );

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );
功能:  
查询授权ID列表
参数:  
[IN]		 
	SKYCAS_U16  u16TVSID: 运营商ID
SKYCAS_U32* pEntitleId：授权ID列表指针
[OUT]		  
SKYCAS_U32* pEntitleId：授权ID列表指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);
功能:  
查询反授权确认码
参数:  
[IN]		 
SKYCAS_U16 u16TVSID: 运营商ID
SKYCAS_BOOL* pReadFlag:读取状态，1—已读，0—未读
SKYCAS_U32*  pDetitleChkNums：反授权确认码数组指针
[OUT]		 
SKYCAS_U32*  pDetitleChkNums：反授权确认码数组指针
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);

/***********************************************************************
函数名: 
virtual SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID); 
功能:  
查询反授权信息读取状态
参数:  
[IN]		 
SKYCAS_U16 u16TVSID: 运营商ID
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到这个运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);

/***********************************************************************
函数名: 
virtual SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
                                  SKYCAS_U32 u32DetitleChkNum ); 
功能:  
删除反授权确认码
参数:  
[IN]		 
SKYCAS_U16 u16TVSID: 运营商ID 
SKYCAS_U32 u32DetitleChkNum:要删除的反授权确认码
[OUT]		无。  
[RET]		
SKYCAS_FALSE删除失败    SKYCAS_TRUE删除成功
************************************************************************/
virtual SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
													SKYCAS_U32 u32DetitleChkNum );

/* 机卡对应 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );
功能:  
查询机卡对应情况
参数:  
[IN]		 
SKYCAS_U8* pNum：
SKYCAS_U8* pSTBSerial：
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:已配对
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_CARD_PAIROTHER:智能卡已与其他终端配对
SKYCAS_CA_CARD_NOPAIR:智能卡没有与任何终端配对
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );

/* IPPV应用 */
/***********************************************************************
函数名: 
功能:  
	当用户对IPPV节目通知操作时，调用此接口将信息传递给CA库
参数:  
[IN]		
SKYCAS_BOOL bBuyProgram:用户是否购买
SKYCAS_U16 u16EcmPid:ecmpid
const SKYCAS_U8*  pbyPinCode:PIN码
const SKYCAS_IPPV_PRICE_S* pPrice :用户选择的节目价格
[OUT]		无。
[RET]	
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_CARD_NO_ROOM:智能卡空间不足
SKYCAS_CA _CARD_INVALID:智能卡无效
SKYCAS_CA_PROG_STATUS_INVALID:价格状态无效
SKYCAS_CA_PIN_INVALID:PIN码无效
SKYCAS_CA_DATA_NOT_FIND:没有找到该运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram,
												SKYCAS_U16 u16EcmPid,
                                    			const SKYCAS_U8*  pbyPinCode,
												const SKYCAS_IPPV_PRICE_S* pPrice );

/***********************************************************************
函数名: 
功能:  
	查询用户在某个运营商下IPPV节目购买情况
参数:  
[IN]		
SKYCAS_U16 u16TVSID:运营商ID
SKYCAS_IPPVINFO_S* pIppv:节目信息
SKYCAS_U16* u16Number :实际获得的节目个数
[OUT]		无。  
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA _CARD_INVALID:智能卡无效
SKYCAS_CA _POINTER_INVALID:指针为空
SKYCAS_CA_DATA_NOT_FIND:没有找到该运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetIPPVProgram( SKYCAS_U16 u16TVSID,
												SKYCAS_IPPVINFO_S* pIppv,
												SKYCAS_U16* u16Number );

/* 邮件管理 */
/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);
功能:  
查询邮件头部信息
参数:  
[IN]		 
SKYCAS_EMAILHEADINFO_S* pEmailHead：邮件头部信息数组
SKYCAS_U8* pCount:输入数组长度，返回是实际获取到的邮件个数
SKYCAS_U8* pIndex:输入是从第几个email空间开始取，返回下次取的位置，
					第一次调用传0，将获取到全部email信息
[OUT]		  
SKYCAS_U8* pCount:返回实际获取到的邮件个数
SKYCAS_U8* pIndex:返回下次取的位置，第一次调用传0，将获取到全部email信息

[RET]		
	SKYCAS_CA_OK:获取成功
	SKYCAS_CA _POINTER_INVALID:指针无效
	SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);
功能:  
查询邮件信息
参数:  
[IN]		 
SKYCAS_EMAILHEADINFO_S* pEmailHead：邮件头部信息指针
[OUT]		  
SKYCAS_EMAILHEADINFO_S* pEmailHead：邮件头部信息指针
[RET]		
	SKYCAS_CA_OK:获取成功
	SKYCAS_CA _POINTER_INVALID:指针无效
	SKYCAS_CA_DATA_NOT_FIND:没有找到该邮件
	SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);
功能:  
查询邮件信息
参数:  
[IN]		 
SKYCAS_U32 u32EmailId:邮件ID
SKYCAS_EMAILCONTENT_S * pEmailCont：邮件内容信息
[OUT]		  
[RET]		
	SKYCAS_CA_OK:获取成功
	SKYCAS_CA_POINTER_INVALID:指针无效
	SKYCAS_CA_DATA_NOT_FIND:没有找到该邮件
	SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);
功能:  
根据邮件ID删除邮件
参数:  
[IN]		 
	SKYCAS_U32 u32EmailID：邮件ID
[OUT]		无。  
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE 
************************************************************************/
virtual SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);

/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
													SKYCAS_U8* pEmptyNum );
功能:  
查询邮箱使用情况
参数:  
[IN]		 
		SKYCAS_U8* pEmailNum：已收邮件个数
SKYCAS_U8* pEmptyNum：还能接收邮件个数
[OUT]		 
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
													SKYCAS_U8* pEmptyNum );

/* 子母卡应用 */
/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16u16TVSID,
													SKYCAS_U8*   pIsChild,
													SKYCAS_U16*  pDelayTime,
													SKYCAS_DATETIME_S* pLastFeedTime,
													SKYCAS_U8* 	pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed );
功能:  
读取子母卡信息
参数:  
[IN]		
	SKYCAS_U16u16TVSID:运营商ID
[OUT]		
	SKYCAS_U8*   pIsChild：是否是子卡
	SKYCAS_U16*  pDelayTime：喂养周期
	SKYCAS_U32* pLastFeedTime：上次喂养时间
	SKYCAS_U8* 	pParentCardSN：母卡卡号
	SKYCAS_BOOL *pIsCanFeed：是否可喂养
[RET]		
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到该运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
													SKYCAS_U8*   pIsChild,
													SKYCAS_U16*  pDelayTime,
													SKYCAS_U32* pLastFeedTime,
													SKYCAS_U8* 	pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed );

/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
															SKYCAS_U8* pFeedData,
															SKYCAS_U8* pLen);
功能:  
读取母卡喂养数据
参数:  
[IN]		
	SKYCAS_U16 u16TVSID：运营商ID
 [OUT]		
	SKYCAS_U8* pFeedData：喂养数据
	SKYCAS_U8* pLen：喂养数据长度
[RET]		
	SKYCAS_CA_POINTER_INVALID:指针无效
	SKYCAS_CA_UNKNOWN:未知错误
	SKYCAS_RET_SUCCESS:获取成功
	SKYCAS_RET_FAILURE:获取失败
************************************************************************/

virtual SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
                                          SKYCAS_U8* pFeedData,
                                          SKYCAS_U8* pLen); 
/***********************************************************************
函数名: 
功能:  
喂养数据写入子卡
参数:  
[IN]	
	SKYCAS_U16 u16TVSID：运营商ID
	const SKYCAS_U8 *pFeedData：要写入的喂养数据
	SKYCAS_U8 u8Len	：数据长度
[OUT]		无。
[RET]		
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_RET_SUCCESS:获取成功
SKYCAS_RET_FAILURE:获取失败
************************************************************************/

virtual SKYCAS_U16 SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, 
													const SKYCAS_U8 *pFeedData, 
													SKYCAS_U8 u8Len);

/* 显示界面管理 */
/***********************************************************************
函数名: 
功能:  
刷新界面
参数:  
[IN]		无。
[OUT]		无。
[RET]		无。
************************************************************************/
virtual SKYCAS_VOID SKYCAS_RefreshInterface( SKYCAS_VOID );

/***********************************************************************
函数名: 
功能:  
回调函数。
参数:  
[IN]		
[OUT]		
[RET]		
************************************************************************/
typedef SKYCAS_VOID(*SKYCAS_PostEvent_CB)(SKYCAS_EVENT_S eCAEvent);

virtual SKYCAS_S32 SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ);

/* 获取长度信息 */
/***********************************************************************
函数名: 
	virtual SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);	
功能:  
获取pin码长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回pin码的长度
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);				

/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);	
功能:  
获取智能卡号长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回智能卡号长度 
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);			

/***********************************************************************
函数名: 
virtual SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);
功能:  
获取邮件发送者名字最大长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回邮件发送者名字最大长度
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);		

/***********************************************************************
函数名: 
	virtual SKYCAS_U16 SKYCAS_GetTitleMaxLen(SKYCAS_VOID);
功能:  
获取邮件标题最大长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回邮件标题最大长度
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetTitleMaxLen();				

/***********************************************************************
函数名: 
virtual SKYCAS_ U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);
功能:  
获取邮件内容最大长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回邮件内容最大长度
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);			

/***********************************************************************
函数名: 
virtual SKYCAS_ U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);
功能:  
获取osd最大长度
参数:  
[IN]		无。
[OUT]		无。
[RET]		
	返回osd最大长度
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);			

/***************以下接口针对天柏CA***************************/


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);
功能:  
获取机顶盒属性，数据由16 个字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pSTBAtr:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pSTBAtr:获取到的机顶盒属性指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);
功能:  
获取用户账号，数据由8 个字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pAcNo:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pAcNo:获取到的用户账号数据指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);
功能:  
获取系统描述，数据由32 个字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 u8index:要读取的系统描述的序号
SKYCAS_U8 *pSysDes:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pSysDes:获取到的系统描述数据指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);

/***********************************************************************
函数名: 
virtual SKYCAS_S16 SKYCAS_GetCASID(SKYCAS_VOID);
功能:  
获取CAS ID，数据由16 bit 组成
参数:  
[IN]		:无
[OUT]	:无	
[RET]	
	返回CAS ID
virtual SKYCAS_S16 SKYCAS_GetCASID(SKYCAS_VOID);
************************************************************************/

/***********************************************************************
函数名: 
virtual SKYCAS_S32  SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);
功能:  
获取智能卡版本，数据由3 个字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pSCVer:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pSCVer:获取到的智能卡版本数据指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取成功
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);
功能:  
获取厂商名，数据由20 字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pUser:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pUser:获取到的厂商名数据指针
[RET]	
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);
功能:  
获取CA库 的编译时间，数据由40 个字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pDate:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pDate:获取到的时间数据指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
功能:  
获取授权信息的个数
参数:  
[IN]		
SKYCAS_S32 s32TVSID:运营商ID
SKYCAS_S32 *pNum:指向返回数据的存储空间
[OUT]		
SKYCAS_S32 *pNum:获取到的授权信息个数指针
[RET]	
SKYCAS_CA_OK:获取成功并返回获取到的值
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
virtual SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
************************************************************************/

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);
功能:  
同意钱包金额变更
参数:  
[IN]		
SKYCAS_U8 u8Flag:为0表示拒绝钱包变更，为1表示同意钱包变更
SKYCAS_U8 u8Index:默认为0
[OUT]		
[RET]	
SKYCAS_CA_OK:变更成功
SKYCAS_CA_POINTER_INVALID:指针无效或参数无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);
功能:  
扣除钱包金额
参数:  
[IN]		
SKYCAS_FLOAT fPrice:某个节目/服务的价格
SKYCAS_U8 u8Type:节目/服务的类型，由运营商定义
skycas_u32 u32ProgID:节目/服务的ID，由运营商定义
[OUT]		
[RET]	
SKYCAS_CA_OK:扣除成功
SKYCAS_CA_POINTER_INVALID:指针无效或参数无效
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);
功能:  
获取节目状态信息
参数:  
[IN]		
SKYCAS_S32 s32ProductID:节目ID
[OUT]		
[RET]	
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_ENTITLED: 节目已授权
SKYCAS_CA_NO_ENTITLED:节目未授权
SKYCAS_CA_NO_ENTITLEDRECORD:没有节目授权记录
SKYCAS_CA_ENTITLED_OVERDUE:授权过期	
SKYCAS_CA_TV_LOCKED:电视或机顶盒被锁定		
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);
功能:  
执行手动配对
参数:  
[IN]		
SKYCAS_U8 *pData:保留，目前默认传NULL
[OUT]		
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_OK:配对成功
SKYCAS_CA_DATA_NOT_FIND:CAT 没有收到
SKYCAS_CA_PAIRING_FAIL	: 配对失败
SKYCAS_CA_EMM_TIMEOUT: 收取EMM超时
************************************************************************/
virtual SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);
功能:  
获取子母卡的类型,是子卡还是母卡
参数:  
[IN]		
SKYCAS_U8 *pType:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pType:获取到的卡类型指针
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取智能卡类型成功
SKYCAS_CA_NOT_PARENTCHILDCARD:没有分配子母卡属性
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);
功能:  
获取母卡卡号
参数:  
[IN]		
SKYCAS_U8 *pCardID:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pCardID:获取到的卡号指针
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取智能卡类型成功
SKYCAS_CA_NOT_PARENTCHILDCARD:没有分配子母卡属性
SKYCAS_CA_PARENTCARD:该机顶盒是主机
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);
功能:  
获取子卡或母卡过期时间
参数:  
[IN]		
SKYCAS_U8 u8Flag:0为母卡，1为子卡
SKYCAS_DATETIME_S stExpiryDate:指向返回时间的存储空间
[OUT]		
SKYCAS_DATETIME_S stExpiryDate:返回时间的数据指针
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:获取智能卡类型成功
SKYCAS_CA_NOT_PARENTCHILDCARD:没有分配子母卡属性
SKYCAS_CA_PARENTCARD:该机顶盒是主机
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);


/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S stStartTime
											SKYCAS_DATETIME_S stEndTime,SKYCAS_DATETIME_S stTrasTime);
功能:  
添加IPPV节目
参数:  
[IN]		
SKYCAS_S32 s32ProuctID: IPPV 节目的产品ID
SKYCAS_FLOAT fPrice: IPPV 节目的价格
SKYCAS_DATETIME_S *stStartTime: IPPV 节目的开始时间
SKYCAS_DATETIME_S *stEndTime: IPPV 节目的结束时间
SKYCAS_DATETIME_S *stTrasTime: IPPV 节目的购买时间，即当前时间
SKYCAS_U8 *pPin:PIN码
[OUT]		
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:订购成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_MONEY_NOTENOUGH: 余额不足
SKYCAS_CA_PIN_INVALID:错误的PIN
************************************************************************/
virtual SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);
功能:  
获取交易信息个数
参数:  
[IN]		
SKYCAS_U8 *pData:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pData:获取到的系统描述数据指针
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_OK:获取成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);

/***********************************************************************
函数名: 
virtual SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
功能:  
获取交易信息
参数:  
[IN]		
SKYCAS_U16 u16Num:交易信息个数
SKYCAS_U8 *pData:指向返回数据的存储空间
[OUT]	
SKYCAS_U8 *pData:获取到的系统描述数据指针
[RET]	
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_OK:订购成功
SKYCAS_CA_CALIB_STOP:ca库停止运行
SKYCAS_CA_SEMP_TIMEOUT:信号量超时
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_POINTER_INVALID:指针为空
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);

/***************以上接口针对天柏CA***************************/

private:

	SKYCAS_U32 SKYCAS_ReadCAType(SKYCAS_VOID);

public:
	 static skyworth_ca_api *pSkyworthCAApi;

private:
	SKYCAS_U32 u32CAType;
	
};

/****************************** 以下为各CAS公共部分，最好能独立成文件，封装在libskyworth内，不为外界获取 ************************************************/

#define FLAG_SKYCAS_RESOURCE_FOR_APK (1)  /* 用于APK方式测试CA库功能时用，与下面的宏FLAG_SKYCAS_RESOURCE_RECYCLING互斥。*/
#define FLAG_SKYCAS_RESOURCE_RECYCLING (0)  /* 是否需要回收CA资源。默认需要打开。*/

#if FLAG_SKYCAS_RESOURCE_RECYCLING
typedef enum
{
    SKYCAS_RESOURCE_TASK = 0,
    SKYCAS_RESOURCE_SEMAP,
    SKYCAS_RESOURCE_MUTEX,
    SKYCAS_RESOURCE_QUEUE,
    SKYCAS_RESOURCE_INVALID,
}SKYCAS_RESOURCE_TYPE_E;

typedef struct
{
    SKYCAS_RESOURCE_TYPE_E eType; 
    SKYCAS_U32 u32Id;
}SKYCAS_RESOURCE_T;

#define SKYCAS_RESOURCE_NUM_MAX (30)

extern SKYCAS_VOID SKYCAS_Lib_Resource_Init(SKYCAS_VOID);
extern SKYCAS_VOID SKYCAS_Lib_Resource_Release_All(SKYCAS_VOID);
extern SKYCAS_VOID SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_TYPE_E eType ,SKYCAS_U32 u32Id);
extern SKYCAS_VOID SKYCAS_Lib_Resource_Clear(SKYCAS_RESOURCE_TYPE_E eType ,SKYCAS_U32 u32Id);
#endif


#endif

