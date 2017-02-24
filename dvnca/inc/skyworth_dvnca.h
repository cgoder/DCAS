//<Skyworth Software>
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SKYWORTH_CA_DVN_
#define _SKYWORTH_CA_DVN_

#include "skycas_datatype.h"
#include "skyworth_ca_api.h"

typedef struct
{
	SKYCAS_FILTER_EVENT_E m_FilterEvent;
	SKYCAS_U32 	m_FilterId;
	SKYCAS_U8 *m_data;
	SKYCAS_U32 m_length;
}SKYCAS_DvncaSectionMessage_S;

typedef struct skycas_filter_s
{
	SKYCAS_BOOL	m_bUsed;
	SKYCAS_U32 m_StartTimeTicks;
	SKYCAS_U32	m_U32FilterId;
	SKYCAS_U16 m_Pid;
	SKYCAS_U8 m_RequestId;
	SKYCAS_U8 m_WaitSecond;
	SKYCAS_U8 m_Deep;
	SKYCAS_U8 match[SKYCAS_FILTER_DEEP];
	SKYCAS_U8 mask[SKYCAS_FILTER_DEEP];
	SKYCAS_U8 negate[SKYCAS_FILTER_DEEP];
	SKYCAS_U32 m_u32QueueId4Dvn;
	SKYCAS_U16 m_u16TableId4Dvn;
} SKYCAS_FILTER_S;


typedef struct
{
	SKYCAS_U16 	 ecmPid;
	SKYCAS_U8	 messageType;
}SKYCAS_DvncaMessageShowBuyParam_S;

typedef SKYCAS_VOID(*SKYCAS_PostEvent_CB)(SKYCAS_EVENT_S eCAEvent);


class CSkyworth_Dvnca : public  skyworth_ca_api
{
public:

/***********************************************************************
函数名:
	static CSkyworth_Dvnca* GetCaInstance(SKYCAS_VOID);
功能:
得到类的一个实例。
参数:
[IN]
[OUT]		无。
[RET]
返回一个类的实例。
************************************************************************/
static CSkyworth_Dvnca* GetCaInstance(SKYCAS_VOID);

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
CSkyworth_Dvnca(SKYCAS_VOID)
功能:
构造函数。
参数:
[IN]		无。
[OUT]		无。
[RET]		无。
************************************************************************/
CSkyworth_Dvnca(SKYCAS_VOID);

/***********************************************************************
函数名:
~CSkyworth_Dvnca(SKYCAS_VOID)
功能:
析构函数。
参数:
[IN]		无。
[OUT]		无。
[RET]		无。
************************************************************************/
~CSkyworth_Dvnca(SKYCAS_VOID);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara );
功能:
CA初始化。
参数:
[IN]
SKYCAS_INIT_PARA_S * pInitPara：初始化参数。
[OUT]		无。
[RET]
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara);


/***********************************************************************
函数名:
SKYCAS_ S32 SKYCAS_CaDeInit(SKYCAS_VOID);
功能:
CA去初始化。
参数:
[IN]		无。
[OUT]		无。
[RET]
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_CaDeInit(SKYCAS_VOID);


/***********************************************************************
函数名:
    SKYCAS_S32 SKYCAS_ReleaseFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 u16EcmPid);
功能:
    释放过滤器，包括物理及逻辑过滤器。
    当参数全部为无效值时，释放所有过滤器。
    否则，以传入参数为限制条件，释放符合对应要求的过滤器。
参数:
[IN]
    SKYCAS_U8 u8ReqID   请求ID。当为0XFF时，不以此参数为限制条件。
    SKYCAS_U16 u16EcmPid    要停止的过滤器PID。当为0x1FFF时，不以此参数为限制条件。
[OUT]		
    无。
[RET]
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_ReleaseFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 u16EcmPid);

/***********************************************************************
函数名:
SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);
功能:
判断参数SystemID是否是当前CA的SystemID。
参数:
[IN]
SKYCAS_U16 u16SystemID：CA系统ID。
[OUT]		无。
[RET]
是返回SKYCAS_TURE；否返回SKYCAS_FALSE。
************************************************************************/
SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);
功能:
设置EMMPID。
参数:
[IN]
SKYCAS_U16 u16EmmPid：EMMPID。
[OUT]		无。
[RET]
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);
功能:
CAT表变化时更新EMMPID。
参数:
[IN]
SKYCAS_U8 *pbuffer:CAT表数据指针。
[OUT]		无。
[RET]
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
功能:
设置ECMPID。
参数:
[IN]
pServiceInfo:service信息结构体指针,为空则表示stopecm,不为空为setecm。
[OUT]		无。
[RET]
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
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
SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_CASDataCALLBACK(SKYCAS_U8 u8ReqID,
	SKYCAS_BOOL bSuccess,SKYCAS_U16 u16Pid, SKYCAS_U16 u16Len,
	SKYCAS_U8 *DataBuf);
功能:
回调函数，通知CA模块收到过滤数据或过滤超时。
参数:
[IN]
u8ReqID:请求收取私有数据表的请求号
bSuccess：过滤到数据为true,超时为false
u16Pid：过滤到数据时的pid
u16Len：过滤数据长度
DataBuf：过滤数据指针
[OUT]		无。
[RET]
成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
************************************************************************/
SKYCAS_S32 SKYCAS_CASDataCALLBACK(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU32BufferAddr, 
	SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);

/***********************************************************************
函数名: 
	virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);
功能:  
上层传NIT数据给CA模块
参数:  
[IN]		
SKYCAS_U8 *DataBuf:数据地址
[OUT]		无。
[RET]		
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE?

************************************************************************/
SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);

/* 智能卡管理 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);
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
SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);

/***********************************************************************
函数名:
SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
功能:
智能卡主线程。
参数:
[IN]
	SKYCAS_VOID* para：预留参数
[OUT]		无
[RET]
SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para);
************************************************************************/


/* PIN码管理 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
												const SKYCAS_U8* pNewPin);
功能:
更改PIN码。
参数:
[IN]
const SKYCAS_U8* pOldPin：旧的PIN码
const SKYCAS_U8* pNewPin：新的PIN码
[OUT]		无。
[RET]
SKYCAS_CA_OK:修改成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_DATA_ERROR:智能卡通讯错误
SKYCAS_CA_PIN_INVALID:PIN码错误
SKYCAS_CA_UNKNOWN:未知错误
SKYCAS_CA_CARD_NOTSUPPORT:智能卡不支持此功能
************************************************************************/
SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
                               				const SKYCAS_U8* pNewPin);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PinInfo *pPIN);
功能:
校验用户输入pin码是否与卡中一致。
参数:
[IN]
const SKYCAS_PinInfo *pPIN：用户输入pin码
[OUT]		无。
[RET]
一致返回SKYCAS_RET_SUCCESS；不一致返回SKYCAS_RET_FAILURE
************************************************************************/
SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);
功能:
判断用户PIN码是否被锁住。
参数:
[IN]
	  SKYCAS_BOOL * bLocked：true表示锁住了，false表示没有
[OUT]		无。
[RET]
	成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE
************************************************************************/
SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);

/* 用户观看级别管理 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
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
SKYCAS_CA_PIN_INVALID:PIN码无效
SKYCAS_CA_WATCHRATING_INVALID:观看级别无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );
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
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );

/* 智能卡工作时段管理 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
                                 SKYCAS_DATETIME *StartTime,
                                 SKYCAS_DATETIME *EndTime);

功能:
设置工作时间段。
参数:
[IN]
  const SKYCAS_U8* pPin：用户输入的pin码
  SKYCAS_DATETIME *StartTime：工作时间起始时间
  SKYCAS_DATETIME *EndTime：工作时间结束时间
[OUT]		无。
[RET]
SKYCAS_CA_OK:获取成功
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_PIN_INVALID:PIN码无效或错误
SKYCAS_CA_WORKTIME_INVALID:工作时间无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
												SKYCAS_DATETIME_S *StartTime,
												SKYCAS_DATETIME_S *EndTime);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
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
SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
												SKYCAS_DATETIME_S *stEndTime);

/* 基本信息查询 */
/***********************************************************************
函数名:
SKYCAS_U32 SKYCAS_GetVer(SKYCAS_VOID);
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
SKYCAS_U32 SKYCAS_GetVer(SKYCAS_U8 *pData);

/***********************************************************************
函数名:
SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);
功能:
查询当前机器平台编号
参数:
[IN]		无。
[OUT]		无。
[RET]
平台编号
************************************************************************/
SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );
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
SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
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
SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
										SKYCAS_OPERATORINFO_S* stOperatorInfo );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );
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
SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );

/*钱包信息查询 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID , SKYCAS_U32 *pSlotNum);
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
SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID);//, SKYCAS_U32 *pSlotNum );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
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
SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S* pSlotInfo );

/* 授权信息管理 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
									SKYCAS_Entitles * pServiceEntitles );
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
SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
													SKYCAS_Entitles* pServiceEntitles );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
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
SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );

/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
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
SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);

/***********************************************************************
函数名:
SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);
功能:
查询反授权信息读取状态
参数:
[IN]
SKYCAS_U16 u16TVSID: 运营商ID
[OUT]		无。
[RET]
SKYCAS_FALSE未读    SKYCAS_TRUE已读
************************************************************************/
SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);

/***********************************************************************
函数名:
SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
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
SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
													SKYCAS_U32 u32DetitleChkNum );

/* 机卡对应 */
/***********************************************************************
函数名:
SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );
功能:
查询机卡对应情况
参数:
[IN]
SKYCAS_U8* pNum：
SKYCAS_U8* pSTBSerial：
[OUT]		无。
[RET]
SKYCAS_CA_OK:获取成功
SKYCAS_CA_CARD_PAIROTHER:智能卡已与其他终端配对
SKYCAS_CA_CARD_NOPAIR:智能卡没有与任何终端配对
SKYCAS_CA_CARD_INVALID:智能卡无效
SKYCAS_CA_POINTER_INVALID:指针为空
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );

/* IPPV应用 */
/***********************************************************************
函数名: 
功能:  
	当用户对IPPV节目通知操作时，调用此接口将信息传递给CA库
参数:  
[IN]		
SKYCAS_BOOL bBuyProgram:用户是否购买
SKYCAS_U16 u16EcmPid:
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
SKYCAS_S32 SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram,
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
SKYCAS_S32 SKYCAS_GetIPPVProgram( SKYCAS_U16 u16TVSID,
												SKYCAS_IPPVINFO_S* pIppv,
												SKYCAS_U16* u16Number );


/* 邮件管理 */
/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
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
SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetEmailInfos(SKYCAS_EMAILHEADINFO_S* pEmailHead,
SKYCAS_EMAILHEADINFO_S* pEmailCont);
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
SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetEmailInfos(SKYCAS_EMAILHEADINFO_S* pEmailHead,
SKYCAS_EMAILHEADINFO_S* pEmailCont);
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
SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);

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
SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);

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
SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
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
SKYCAS_CA _CARD_INVALID:智能卡无效
SKYCAS_CA_DATA_NOT_FIND:没有找到该运营商
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
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

SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
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

SKYCAS_U16 SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, 
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
SKYCAS_VOID SKYCAS_RefreshInterface( SKYCAS_VOID );

/***********************************************************************
函数名:
功能:
回调函数。
参数:
[IN]
[OUT]
[RET]
************************************************************************/

SKYCAS_S32 SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ);

/* 获取长度信息 */
/***********************************************************************
函数名:
 SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);
功能:
获取pin码长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回pin码的长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);

/***********************************************************************
函数名:
SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);
功能:
获取智能卡号长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回智能卡号长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);

/***********************************************************************
函数名:
 SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);
功能:
获取邮件发送者名字最大长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回邮件发送者名字最大长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);

/***********************************************************************
函数名:
SKYCAS_U16 SKYCAS_GetTitleMaxLen(SKYCAS_VOID);
功能:
获取邮件标题最大长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回邮件标题最大长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetTitleMaxLen();

/***********************************************************************
函数名:
SKYCAS_ U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);
功能:
获取邮件内容最大长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回邮件内容最大长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);

/***********************************************************************
函数名:
SKYCAS_ U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);
功能:
获取osd最大长度
参数:
[IN]		无。
[OUT]		无。
[RET]
	返回osd最大长度
************************************************************************/
SKYCAS_U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);


/***************以下接口针对天柏CA***************************/

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);
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
SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);
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
SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);
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
SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);


/***********************************************************************
函数名: 
SKYCAS_S32  SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);
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
SKYCAS_S32 SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);
功能:  
获取厂商名，数据由20 字节的ASCII 组成
参数:  
[IN]		
SKYCAS_U8 *pUser:指向返回数据的存储空间
[OUT]		
SKYCAS_U8 *pUser:获取到的厂商名数据指针
[RET]	
SKYCAS_CA_POINTER_INVALID:指针无效
SKYCAS_CA_UNKNOWN:未知错误
************************************************************************/
SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);
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
SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);


/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
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
SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
************************************************************************/

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);
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
SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);


/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);
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
SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);
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
SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);
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
SKYCAS_RET_DATA_NOT_FIND:CAT 没有收到
SKYCAS_CA_PAIRING_FAIL	: 配对失败
SKYCAS_CA_EMM_TIMEOUT: 收取EMM超时
************************************************************************/
SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);


/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);
功能:  
获取智能卡的类型,子母还是母卡
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
SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);
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
SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);
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
SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);


/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S stStartTime
											SKYCAS_DATETIME_S stEndTime,SKYCAS_DATETIME_S stTrasTime);
功能:  
获取子卡或母卡过期时间
参数:  
[IN]		
SKYCAS_S32 s32ProuctID: IPPV 节目的产品ID
SKYCAS_FLOAT fPrice: IPPV 节目的价格
SKYCAS_DATETIME_S *stStartTime: IPPV 节目的开始时间
SKYCAS_DATETIME_S *stEndTime: IPPV 节目的结束时间
SKYCAS_DATETIME_S *stTrasTime: IPPV 节目的购买时间，即当前时间
SKYCAS_U8 *pPin:pin码
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
SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin);


/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);
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
SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);

/***********************************************************************
函数名: 
SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
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
SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
/***************以上接口针对天柏CA***************************/


public:	
    SKYCAS_S32 SKYCAS_SetPrivateData(SKYCAS_U8 FilterID,SKYCAS_U16 PID,SKYCAS_U16 TableID,SKYCAS_U8 Period,const SKYCAS_U16 *SectByte,SKYCAS_LONG QueueID);
	SKYCAS_S32 SKYCAS_ReleasePrivateDataFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 wPid);
	SKYCAS_VOID CallbackSmartcardProcess(SKYCAS_VOID);
	SKYCAS_S32 SKYCAS_GetFilePath(SKYCAS_CHAR *pFilePath);//获取写文件的路径
	SKYCAS_S32 SKYCAS_GetCATdata(SKYCAS_U8* catData);
        SKYCAS_S32 SKYCAS_SetCATQueueId(SKYCAS_U32 u32QueueId);


	static SKYCAS_VOID CAPrivateDataTask(SKYCAS_VOID *param);
	static SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para);
public:	
	static CSkyworth_Dvnca *pSkyworthCAApi;
	static SKYCAS_SMC_EVENT_E m_eSmcEvent;

	static SKYCAS_PostEvent_CB mpTVService ;
	volatile SKYCAS_BOOL	m_DvnMainTaskStoped;
	volatile SKYCAS_BOOL	m_ScTaskStoped;
	volatile SKYCAS_BOOL	m_PriDataTaskStoped;
	SKYCAS_U32 m_u32SectionMsgQueueId;	
	SKYCAS_FILTER_S m_U32FilterIdArray[SKYCAS_MAX_FILTER_COUNT];
	SKYCAS_U32 m_U32PriDataMutexId;
	SKYCAS_U32 m_ScSemaphore;

private:
        SKYCAS_U32 m_DvnMainTaskId;
        SKYCAS_U32 m_ScTaskId;
        SKYCAS_U32 m_PriDataTaskId;
        SKYCAS_CHAR pDvncaFilePath[SKYCAS_FILE_PATH_LEN_MAX];//[256];
        SKYCAS_BOOL bInitflg;
        SKYCAS_U16 u16CATVersionNum;
        SKYCAS_U32 u32CATQueue4Dvn;
        SKYCAS_U8 u8ArrCAT[SKYCAS_MAXLEN_CAT_MAX];
        //SKYCAS_U16 u16CATTsId;
        //SKYCAS_U16 u16CATORNid;



private:
        SKYCAS_VOID SKYCAS_Filter_Logic_Init(SKYCAS_U8 logicIndex);
        SKYCAS_BOOL IsSame(SKYCAS_U8 index,SKYCAS_U8 FilterID,SKYCAS_U16 pid,const SKYCAS_U8 *filterData,const SKYCAS_U8 *filterMask,SKYCAS_U16 TableID);
        SKYCAS_S32 SKYCAS_Filter_data_cmp(const SKYCAS_FILTER_S* const pFilter, const SKYCAS_U8* const pData);
};

#endif
