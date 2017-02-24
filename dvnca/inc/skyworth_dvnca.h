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
������:
	static CSkyworth_Dvnca* GetCaInstance(SKYCAS_VOID);
����:
�õ����һ��ʵ����
����:
[IN]
[OUT]		�ޡ�
[RET]
����һ�����ʵ����
************************************************************************/
static CSkyworth_Dvnca* GetCaInstance(SKYCAS_VOID);

/***********************************************************************
������:
	static SKYCAS_VOID DestroyInstance(SKYCAS_VOID);
����:
����һ�����ʵ����
����:
[IN]
[OUT]		�ޡ�
[RET]		�ޡ�
************************************************************************/
static SKYCAS_VOID DestroyInstance(SKYCAS_VOID);

/***********************************************************************
������:
CSkyworth_Dvnca(SKYCAS_VOID)
����:
���캯����
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		�ޡ�
************************************************************************/
CSkyworth_Dvnca(SKYCAS_VOID);

/***********************************************************************
������:
~CSkyworth_Dvnca(SKYCAS_VOID)
����:
����������
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		�ޡ�
************************************************************************/
~CSkyworth_Dvnca(SKYCAS_VOID);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara );
����:
CA��ʼ����
����:
[IN]
SKYCAS_INIT_PARA_S * pInitPara����ʼ��������
[OUT]		�ޡ�
[RET]
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara);


/***********************************************************************
������:
SKYCAS_ S32 SKYCAS_CaDeInit(SKYCAS_VOID);
����:
CAȥ��ʼ����
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_CaDeInit(SKYCAS_VOID);


/***********************************************************************
������:
    SKYCAS_S32 SKYCAS_ReleaseFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 u16EcmPid);
����:
    �ͷŹ����������������߼���������
    ������ȫ��Ϊ��Чֵʱ���ͷ����й�������
    �����Դ������Ϊ�����������ͷŷ��϶�ӦҪ��Ĺ�������
����:
[IN]
    SKYCAS_U8 u8ReqID   ����ID����Ϊ0XFFʱ�����Դ˲���Ϊ����������
    SKYCAS_U16 u16EcmPid    Ҫֹͣ�Ĺ�����PID����Ϊ0x1FFFʱ�����Դ˲���Ϊ����������
[OUT]		
    �ޡ�
[RET]
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_ReleaseFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 u16EcmPid);

/***********************************************************************
������:
SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);
����:
�жϲ���SystemID�Ƿ��ǵ�ǰCA��SystemID��
����:
[IN]
SKYCAS_U16 u16SystemID��CAϵͳID��
[OUT]		�ޡ�
[RET]
�Ƿ���SKYCAS_TURE���񷵻�SKYCAS_FALSE��
************************************************************************/
SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);
����:
����EMMPID��
����:
[IN]
SKYCAS_U16 u16EmmPid��EMMPID��
[OUT]		�ޡ�
[RET]
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);
����:
CAT��仯ʱ����EMMPID��
����:
[IN]
SKYCAS_U8 *pbuffer:CAT������ָ�롣
[OUT]		�ޡ�
[RET]
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
����:
����ECMPID��
����:
[IN]
pServiceInfo:service��Ϣ�ṹ��ָ��,Ϊ�����ʾstopecm,��Ϊ��Ϊsetecm��
[OUT]		�ޡ�
[RET]
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
/***********************************************************************
������: 
    virtual SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
����:  
    ͣECM���ˡ����в���pServiceInfo��ecmPid��������Ҫֹͣ��ECMPID��
    �÷���SKYCAS_SetEcmPid����һ����
    �ر�ģ����������ΪSKYCAS_NULLʱ����ֹͣ����ECMPID���ˡ�
����:  
[IN]		
    pServiceInfo:service��Ϣ�ṹ��ָ�롣
[OUT]		
    �ޡ�
[RET]		
    �ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_CASDataCALLBACK(SKYCAS_U8 u8ReqID,
	SKYCAS_BOOL bSuccess,SKYCAS_U16 u16Pid, SKYCAS_U16 u16Len,
	SKYCAS_U8 *DataBuf);
����:
�ص�������֪ͨCAģ���յ��������ݻ���˳�ʱ��
����:
[IN]
u8ReqID:������ȡ˽�����ݱ�������
bSuccess�����˵�����Ϊtrue,��ʱΪfalse
u16Pid�����˵�����ʱ��pid
u16Len���������ݳ���
DataBuf����������ָ��
[OUT]		�ޡ�
[RET]
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
SKYCAS_S32 SKYCAS_CASDataCALLBACK(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU32BufferAddr, 
	SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);
����:  
�ϲ㴫NIT���ݸ�CAģ��
����:  
[IN]		
SKYCAS_U8 *DataBuf:���ݵ�ַ
[OUT]		�ޡ�
[RET]		
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE?

************************************************************************/
SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);

/* ���ܿ����� */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);
����:
��ȡ���ܿ����ⲿ���š�
����:
[IN]
SKYCAS_U8* pCardSN����������ָ�롣
[OUT]
SKYCAS_U8* pCardSN����������ָ�롣
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);

/***********************************************************************
������:
SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
����:
���ܿ����̡߳�
����:
[IN]
	SKYCAS_VOID* para��Ԥ������
[OUT]		��
[RET]
SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para);
************************************************************************/


/* PIN����� */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
												const SKYCAS_U8* pNewPin);
����:
����PIN�롣
����:
[IN]
const SKYCAS_U8* pOldPin���ɵ�PIN��
const SKYCAS_U8* pNewPin���µ�PIN��
[OUT]		�ޡ�
[RET]
SKYCAS_CA_OK:�޸ĳɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_PIN_INVALID:PIN�����
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CARD_NOTSUPPORT:���ܿ���֧�ִ˹���
************************************************************************/
SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
                               				const SKYCAS_U8* pNewPin);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PinInfo *pPIN);
����:
У���û�����pin���Ƿ��뿨��һ�¡�
����:
[IN]
const SKYCAS_PinInfo *pPIN���û�����pin��
[OUT]		�ޡ�
[RET]
һ�·���SKYCAS_RET_SUCCESS����һ�·���SKYCAS_RET_FAILURE
************************************************************************/
SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);
����:
�ж��û�PIN���Ƿ���ס��
����:
[IN]
	  SKYCAS_BOOL * bLocked��true��ʾ��ס�ˣ�false��ʾû��
[OUT]		�ޡ�
[RET]
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE
************************************************************************/
SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);

/* �û��ۿ�������� */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );
����:
���ùۿ�����
����:
[IN]
const SKYCAS_U8* pPin���û������pin��
SKYCAS_U8 u8Rating��Ҫ���õ��µĹۿ�����
[OUT]		�ޡ�
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_PIN_INVALID:PIN����Ч
SKYCAS_CA_WATCHRATING_INVALID:�ۿ�������Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );
����:
��ȡ��ǰ�ۿ�����
����:
[IN]
SKYCAS_U8* pRating�������ȡ���Ĺۿ�����
[OUT]
	SKYCAS_U8* pRating�������ȡ���Ĺۿ�����
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );

/* ���ܿ�����ʱ�ι��� */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
                                 SKYCAS_DATETIME *StartTime,
                                 SKYCAS_DATETIME *EndTime);

����:
���ù���ʱ��Ρ�
����:
[IN]
  const SKYCAS_U8* pPin���û������pin��
  SKYCAS_DATETIME *StartTime������ʱ����ʼʱ��
  SKYCAS_DATETIME *EndTime������ʱ�����ʱ��
[OUT]		�ޡ�
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_PIN_INVALID:PIN����Ч�����
SKYCAS_CA_WORKTIME_INVALID:����ʱ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
												SKYCAS_DATETIME_S *StartTime,
												SKYCAS_DATETIME_S *EndTime);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
                                 	SKYCAS_DATETIME_S *stEndTime);
����:
��ѯ���ܿ���ǰ����ʱ��
����:
[IN]
SKYCAS_DATETIME_S *stStartTime,
SKYCAS_DATETIME_S *stEndTime
[OUT]		�ޡ�
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
												SKYCAS_DATETIME_S *stEndTime);

/* ������Ϣ��ѯ */
/***********************************************************************
������:
SKYCAS_U32 SKYCAS_GetVer(SKYCAS_VOID);
����:
��ѯCA_LIB�汾��
����:
[IN]		
SKYCAS_U8 *pData:����ָ�룬���ʱʹ����6 ���ֽڵ�ASCII ��ɣ�
					����CAΪ��ȡ�����ݵ�ָ��
					ͬ��CA��0
[OUT]		
SKYCAS_U8 *pData:����ָ��ָ�򷵻�ֵ
[RET]		
���CA������CA���سɹ�ʧ��
ͬ��CA����CA��汾��
************************************************************************/
SKYCAS_U32 SKYCAS_GetVer(SKYCAS_U8 *pData);

/***********************************************************************
������:
SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);
����:
��ѯ��ǰ����ƽ̨���
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
ƽ̨���
************************************************************************/
SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );
����:
��ѯ��Ӫ��ID�б�
����:
[IN]
SKYCAS_U16* pTVSID:  ���淵����Ӫ��ID ������
[OUT]
SKYCAS_U16* pTVSID:  ���淵����Ӫ��ID ������
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
                              SKYCAS_OPERATORINFO_S * stOperatorInfo );
����:
��ȡ��Ӫ����Ϣ
����:
[IN]
SKYCAS_U16 u16TVSID����Ӫ��ID
SKYCAS_U8 * pCount������ĳ��ȣ�����ʱ����Ӫ����Ϣ�ĸ���
SKYCAS_OPERATORINFO_S * stOperatorInfo ����Ӫ����Ϣ����ָ��
[OUT]
SKYCAS_U8 * pCount����Ӫ����Ϣ�ĸ���
SKYCAS_OPERATORINFO_S * stOperatorInfo ����Ӫ����Ϣ����ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
										SKYCAS_OPERATORINFO_S* stOperatorInfo );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );
����:
��ѯ�û�����
����:
[IN]
SKYCAS_U16 u16TVSID: ��Ӫ��ID
SKYCAS_U32* pACArray :�û�������Ϣ����ָ��
[OUT]
SKYCAS_U32* pACArray :�û�������Ϣ����ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );

/*Ǯ����Ϣ��ѯ */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID , SKYCAS_U32 *pSlotNum);
����:
��ѯǮ��ID�б�
����:
[IN]
SKYCAS_U16 u16TVSID����Ӫ��ID
SKYCAS_U8* pSlotID��Ǯ��ID����ָ��
SKYCAS_U32 *pSlotNum:Ǯ��ID����ָ��
[OUT]
SKYCAS_U8* pSlotID��Ǯ��ID����ָ��
SKYCAS_U32 *pSlotNum:Ǯ��ID����ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID);//, SKYCAS_U32 *pSlotNum );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S * pSlotInfo );
����:
��ѯǮ������ϸ��Ϣ
����:
[IN]
SKYCAS_U16  u16TVSID: ��Ӫ��ID
SKYCAS_U8   u8SlotID:Ǯ��ID
SKYCAS_TVSLOTINFO_S * pSlotInfo��Ǯ����Ϣ����ָ��
[OUT]
SKYCAS_TVSLOTINFO_S * pSlotInfo��Ǯ����Ϣ����ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S* pSlotInfo );

/* ��Ȩ��Ϣ���� */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
									SKYCAS_Entitles * pServiceEntitles );
����:
��ѯ��ͨ��Ȩ��Ŀ�������
����:
[IN]
SKYCAS_U16 u16TVSID: ��Ӫ��ID
SKYCAS_Entitles * pServiceEntitles :��Ȩ��Ϣ�ṹ��ָ��
[OUT]
SKYCAS_Entitles * pServiceEntitles :��Ȩ��Ϣ�ṹ��ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
													SKYCAS_Entitles* pServiceEntitles );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );
����:
��ѯ��ȨID�б�
����:
[IN]
	SKYCAS_U16  u16TVSID: ��Ӫ��ID
SKYCAS_U32* pEntitleId����ȨID�б�ָ��
[OUT]
SKYCAS_U32* pEntitleId����ȨID�б�ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );

/***********************************************************************
������:
SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);
����:
��ѯ����Ȩȷ����
����:
[IN]
SKYCAS_U16 u16TVSID: ��Ӫ��ID
SKYCAS_BOOL* pReadFlag:��ȡ״̬��1���Ѷ���0��δ��
SKYCAS_U32*  pDetitleChkNums������Ȩȷ��������ָ��
[OUT]
SKYCAS_U32*  pDetitleChkNums������Ȩȷ��������ָ��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);

/***********************************************************************
������:
SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);
����:
��ѯ����Ȩ��Ϣ��ȡ״̬
����:
[IN]
SKYCAS_U16 u16TVSID: ��Ӫ��ID
[OUT]		�ޡ�
[RET]
SKYCAS_FALSEδ��    SKYCAS_TRUE�Ѷ�
************************************************************************/
SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);

/***********************************************************************
������:
SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
                                  SKYCAS_U32 u32DetitleChkNum );
����:
ɾ������Ȩȷ����
����:
[IN]
SKYCAS_U16 u16TVSID: ��Ӫ��ID
SKYCAS_U32 u32DetitleChkNum:Ҫɾ���ķ���Ȩȷ����
[OUT]		�ޡ�
[RET]
SKYCAS_FALSEɾ��ʧ��    SKYCAS_TRUEɾ���ɹ�
************************************************************************/
SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
													SKYCAS_U32 u32DetitleChkNum );

/* ������Ӧ */
/***********************************************************************
������:
SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );
����:
��ѯ������Ӧ���
����:
[IN]
SKYCAS_U8* pNum��
SKYCAS_U8* pSTBSerial��
[OUT]		�ޡ�
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_CARD_PAIROTHER:���ܿ����������ն����
SKYCAS_CA_CARD_NOPAIR:���ܿ�û�����κ��ն����
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );

/* IPPVӦ�� */
/***********************************************************************
������: 
����:  
	���û���IPPV��Ŀ֪ͨ����ʱ�����ô˽ӿڽ���Ϣ���ݸ�CA��
����:  
[IN]		
SKYCAS_BOOL bBuyProgram:�û��Ƿ���
SKYCAS_U16 u16EcmPid:
const SKYCAS_U8*  pbyPinCode:PIN��
const SKYCAS_IPPV_PRICE_S* pPrice :�û�ѡ��Ľ�Ŀ�۸�
[OUT]		�ޡ�
[RET]	
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_CARD_NO_ROOM:���ܿ��ռ䲻��
SKYCAS_CA _CARD_INVALID:���ܿ���Ч
SKYCAS_CA_PROG_STATUS_INVALID:�۸�״̬��Ч
SKYCAS_CA_PIN_INVALID:PIN����Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ�����Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram,
												SKYCAS_U16 u16EcmPid,
                                    			const SKYCAS_U8*  pbyPinCode,
												const SKYCAS_IPPV_PRICE_S* pPrice );

/***********************************************************************
������: 
����:  
	��ѯ�û���ĳ����Ӫ����IPPV��Ŀ�������
����:  
[IN]		
SKYCAS_U16 u16TVSID:��Ӫ��ID
SKYCAS_IPPVINFO_S* pIppv:��Ŀ��Ϣ
SKYCAS_U16* u16Number :ʵ�ʻ�õĽ�Ŀ����
[OUT]		�ޡ�  
[RET]		
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA _CARD_INVALID:���ܿ���Ч
SKYCAS_CA _POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_DATA_NOT_FIND:û���ҵ�����Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetIPPVProgram( SKYCAS_U16 u16TVSID,
												SKYCAS_IPPVINFO_S* pIppv,
												SKYCAS_U16* u16Number );


/* �ʼ����� */
/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);
����:  
��ѯ�ʼ�ͷ����Ϣ
����:  
[IN]		 
SKYCAS_EMAILHEADINFO_S* pEmailHead���ʼ�ͷ����Ϣ����
SKYCAS_U8* pCount:�������鳤�ȣ�������ʵ�ʻ�ȡ�����ʼ�����
SKYCAS_U8* pIndex:�����Ǵӵڼ���email�ռ俪ʼȡ�������´�ȡ��λ�ã�
					��һ�ε��ô�0������ȡ��ȫ��email��Ϣ
[OUT]		  
SKYCAS_U8* pCount:����ʵ�ʻ�ȡ�����ʼ�����
SKYCAS_U8* pIndex:�����´�ȡ��λ�ã���һ�ε��ô�0������ȡ��ȫ��email��Ϣ

[RET]		
	SKYCAS_CA_OK:��ȡ�ɹ�
	SKYCAS_CA _POINTER_INVALID:ָ����Ч
	SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetEmailInfos(SKYCAS_EMAILHEADINFO_S* pEmailHead,
SKYCAS_EMAILHEADINFO_S* pEmailCont);
����:  
��ѯ�ʼ���Ϣ
����:  
[IN]		 
SKYCAS_EMAILHEADINFO_S* pEmailHead���ʼ�ͷ����Ϣָ��
[OUT]		  
SKYCAS_EMAILHEADINFO_S* pEmailHead���ʼ�ͷ����Ϣָ��
[RET]		
	SKYCAS_CA_OK:��ȡ�ɹ�
	SKYCAS_CA _POINTER_INVALID:ָ����Ч
	SKYCAS_CA_DATA_NOT_FIND:û���ҵ����ʼ�
	SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetEmailInfos(SKYCAS_EMAILHEADINFO_S* pEmailHead,
SKYCAS_EMAILHEADINFO_S* pEmailCont);
����:  
��ѯ�ʼ���Ϣ
����:  
[IN]		 
SKYCAS_U32 u32EmailId:�ʼ�ID
SKYCAS_EMAILCONTENT_S * pEmailCont���ʼ�������Ϣ
[OUT]		  
[RET]		
	SKYCAS_CA_OK:��ȡ�ɹ�
	SKYCAS_CA_POINTER_INVALID:ָ����Ч
	SKYCAS_CA_DATA_NOT_FIND:û���ҵ����ʼ�
	SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);

/***********************************************************************
������:
virtual SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);
����:
�����ʼ�IDɾ���ʼ�
����:
[IN]
	SKYCAS_U32 u32EmailID���ʼ�ID
[OUT]		�ޡ�
[RET]
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE
************************************************************************/
SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);

/***********************************************************************
������:
virtual SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
													SKYCAS_U8* pEmptyNum );
����:
��ѯ����ʹ�����
����:
[IN]
		SKYCAS_U8* pEmailNum�������ʼ�����
SKYCAS_U8* pEmptyNum�����ܽ����ʼ�����
[OUT]
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
													SKYCAS_U8* pEmptyNum );


/* ��ĸ��Ӧ�� */
/***********************************************************************
������:
virtual SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16u16TVSID,
													SKYCAS_U8*   pIsChild,
													SKYCAS_U16*  pDelayTime,
													SKYCAS_DATETIME_S* pLastFeedTime,
													SKYCAS_U8* 	pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed );
����:
��ȡ��ĸ����Ϣ
����:
[IN]		
	SKYCAS_U16u16TVSID:��Ӫ��ID
[OUT]		
	SKYCAS_U8*   pIsChild���Ƿ����ӿ�
	SKYCAS_U16*  pDelayTime��ι������
	SKYCAS_U32* pLastFeedTime���ϴ�ι��ʱ��
	SKYCAS_U8* 	pParentCardSN��ĸ������
	SKYCAS_BOOL *pIsCanFeed���Ƿ��ι��
[RET]
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA _CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ�����Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
													SKYCAS_U8*   pIsChild,
													SKYCAS_U16*  pDelayTime,
													SKYCAS_U32* pLastFeedTime,
													SKYCAS_U8* 	pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed );

/***********************************************************************
������:
virtual SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
															SKYCAS_U8* pFeedData,
															SKYCAS_U8* pLen);
����:
��ȡĸ��ι������
����:
[IN]		
	SKYCAS_U16 u16TVSID����Ӫ��ID
 [OUT]		
	SKYCAS_U8* pFeedData��ι������
	SKYCAS_U8* pLen��ι�����ݳ���
[RET]
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_RET_SUCCESS:��ȡ�ɹ�
SKYCAS_RET_FAILURE:��ȡʧ��
************************************************************************/

SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
                                          SKYCAS_U8* pFeedData,
                                          SKYCAS_U8* pLen);
/***********************************************************************
������:
����:
ι������д���ӿ�
����:
[IN]	
	SKYCAS_U16 u16TVSID����Ӫ��ID
	const SKYCAS_U8 *pFeedData��Ҫд���ι������
	SKYCAS_U8 u8Len	�����ݳ���
[OUT]		�ޡ�
[RET]
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_RET_SUCCESS:��ȡ�ɹ�
SKYCAS_RET_FAILURE:��ȡʧ��
************************************************************************/

SKYCAS_U16 SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, 
													const SKYCAS_U8 *pFeedData, 
													SKYCAS_U8 u8Len);

/* ��ʾ������� */
/***********************************************************************
������:
����:
ˢ�½���
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		�ޡ�
************************************************************************/
SKYCAS_VOID SKYCAS_RefreshInterface( SKYCAS_VOID );

/***********************************************************************
������:
����:
�ص�������
����:
[IN]
[OUT]
[RET]
************************************************************************/

SKYCAS_S32 SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ);

/* ��ȡ������Ϣ */
/***********************************************************************
������:
 SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);
����:
��ȡpin�볤��
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	����pin��ĳ���
************************************************************************/
SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);

/***********************************************************************
������:
SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);
����:
��ȡ���ܿ��ų���
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	�������ܿ��ų���
************************************************************************/
SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);

/***********************************************************************
������:
 SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);
����:
��ȡ�ʼ�������������󳤶�
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	�����ʼ�������������󳤶�
************************************************************************/
SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);

/***********************************************************************
������:
SKYCAS_U16 SKYCAS_GetTitleMaxLen(SKYCAS_VOID);
����:
��ȡ�ʼ�������󳤶�
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	�����ʼ�������󳤶�
************************************************************************/
SKYCAS_U16 SKYCAS_GetTitleMaxLen();

/***********************************************************************
������:
SKYCAS_ U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);
����:
��ȡ�ʼ�������󳤶�
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	�����ʼ�������󳤶�
************************************************************************/
SKYCAS_U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);

/***********************************************************************
������:
SKYCAS_ U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);
����:
��ȡosd��󳤶�
����:
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]
	����osd��󳤶�
************************************************************************/
SKYCAS_U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);


/***************���½ӿ�������CA***************************/

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);
����:  
��ȡ���������ԣ�������16 ���ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pSTBAtr:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pSTBAtr:��ȡ���Ļ���������ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);
����:  
��ȡ�û��˺ţ�������8 ���ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pAcNo:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pAcNo:��ȡ�����û��˺�����ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);
����:  
��ȡϵͳ������������32 ���ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 u8index:Ҫ��ȡ��ϵͳ���������
SKYCAS_U8 *pSysDes:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pSysDes:��ȡ����ϵͳ��������ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);


/***********************************************************************
������: 
SKYCAS_S32  SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);
����:  
��ȡ���ܿ��汾��������3 ���ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pSCVer:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pSCVer:��ȡ�������ܿ��汾����ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ�ɹ�
************************************************************************/
SKYCAS_S32 SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);
����:  
��ȡ��������������20 �ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pUser:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pUser:��ȡ���ĳ���������ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);
����:  
��ȡCA�� �ı���ʱ�䣬������40 ���ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pDate:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pDate:��ȡ����ʱ������ָ��
[RET]	
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);


/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
����:  
��ȡ��Ȩ��Ϣ�ĸ���
����:  
[IN]		
SKYCAS_S32 s32TVSID:��Ӫ��ID
SKYCAS_S32 *pNum:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_S32 *pNum:��ȡ������Ȩ��Ϣ����ָ��
[RET]	
SKYCAS_CA_OK:��ȡ�ɹ������ػ�ȡ����ֵ
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
************************************************************************/

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);
����:  
ͬ��Ǯ�������
����:  
[IN]		
SKYCAS_U8 u8Flag:Ϊ0��ʾ�ܾ�Ǯ�������Ϊ1��ʾͬ��Ǯ�����
SKYCAS_U8 u8Index:Ĭ��Ϊ0
[OUT]		
[RET]	
SKYCAS_CA_OK:����ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч�������Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);


/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);
����:  
�۳�Ǯ�����
����:  
[IN]		
SKYCAS_FLOAT fPrice:ĳ����Ŀ/����ļ۸�
SKYCAS_U8 u8Type:��Ŀ/��������ͣ�����Ӫ�̶���
skycas_u32 u32ProgID:��Ŀ/�����ID������Ӫ�̶���
[OUT]		
[RET]	
SKYCAS_CA_OK:�۳��ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч�������Ч
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);
����:  
��ȡ��Ŀ״̬��Ϣ
����:  
[IN]		
SKYCAS_S32 s32ProductID:��ĿID
[OUT]		
[RET]	
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_ENTITLED: ��Ŀ����Ȩ
SKYCAS_CA_NO_ENTITLED:��Ŀδ��Ȩ
SKYCAS_CA_NO_ENTITLEDRECORD:û�н�Ŀ��Ȩ��¼
SKYCAS_CA_ENTITLED_OVERDUE:��Ȩ����	
SKYCAS_CA_TV_LOCKED:���ӻ�����б�����		
************************************************************************/
SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);
����:  
ִ���ֶ����
����:  
[IN]		
SKYCAS_U8 *pData:������ĿǰĬ�ϴ�NULL
[OUT]		
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_OK:��Գɹ�
SKYCAS_RET_DATA_NOT_FIND:CAT û���յ�
SKYCAS_CA_PAIRING_FAIL	: ���ʧ��
SKYCAS_CA_EMM_TIMEOUT: ��ȡEMM��ʱ
************************************************************************/
SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);


/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);
����:  
��ȡ���ܿ�������,��ĸ����ĸ��
����:  
[IN]		
SKYCAS_U8 *pType:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pType:��ȡ���Ŀ�����ָ��
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ���ܿ����ͳɹ�
SKYCAS_CA_NOT_PARENTCHILDCARD:û�з�����ĸ������
************************************************************************/
SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);
����:  
��ȡĸ������
����:  
[IN]		
SKYCAS_U8 *pCardID:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pCardID:��ȡ���Ŀ���ָ��
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ���ܿ����ͳɹ�
SKYCAS_CA_NOT_PARENTCHILDCARD:û�з�����ĸ������
SKYCAS_CA_PARENTCARD:�û�����������
************************************************************************/
SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);
����:  
��ȡ�ӿ���ĸ������ʱ��
����:  
[IN]		
SKYCAS_U8 u8Flag:0Ϊĸ����1Ϊ�ӿ�
SKYCAS_DATETIME_S stExpiryDate:ָ�򷵻�ʱ��Ĵ洢�ռ�
[OUT]		
SKYCAS_DATETIME_S stExpiryDate:����ʱ�������ָ��
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:��ȡ���ܿ����ͳɹ�
SKYCAS_CA_NOT_PARENTCHILDCARD:û�з�����ĸ������
SKYCAS_CA_PARENTCARD:�û�����������
************************************************************************/
SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);


/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S stStartTime
											SKYCAS_DATETIME_S stEndTime,SKYCAS_DATETIME_S stTrasTime);
����:  
��ȡ�ӿ���ĸ������ʱ��
����:  
[IN]		
SKYCAS_S32 s32ProuctID: IPPV ��Ŀ�Ĳ�ƷID
SKYCAS_FLOAT fPrice: IPPV ��Ŀ�ļ۸�
SKYCAS_DATETIME_S *stStartTime: IPPV ��Ŀ�Ŀ�ʼʱ��
SKYCAS_DATETIME_S *stEndTime: IPPV ��Ŀ�Ľ���ʱ��
SKYCAS_DATETIME_S *stTrasTime: IPPV ��Ŀ�Ĺ���ʱ�䣬����ǰʱ��
SKYCAS_U8 *pPin:pin��
[OUT]		
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:�����ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_MONEY_NOTENOUGH: ����
SKYCAS_CA_PIN_INVALID:�����PIN
************************************************************************/
SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin);


/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);
����:  
��ȡ������Ϣ����
����:  
[IN]		
SKYCAS_U8 *pData:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pData:��ȡ����ϵͳ��������ָ��
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
************************************************************************/
SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);

/***********************************************************************
������: 
SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
����:  
��ȡ������Ϣ
����:  
[IN]		
SKYCAS_U16 u16Num:������Ϣ����
SKYCAS_U8 *pData:ָ�򷵻����ݵĴ洢�ռ�
[OUT]	
SKYCAS_U8 *pData:��ȡ����ϵͳ��������ָ��
[RET]	
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_OK:�����ɹ�
SKYCAS_CA_CALIB_STOP:ca��ֹͣ����
SKYCAS_CA_SEMP_TIMEOUT:�ź�����ʱ
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
************************************************************************/
SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
/***************���Ͻӿ�������CA***************************/


public:	
    SKYCAS_S32 SKYCAS_SetPrivateData(SKYCAS_U8 FilterID,SKYCAS_U16 PID,SKYCAS_U16 TableID,SKYCAS_U8 Period,const SKYCAS_U16 *SectByte,SKYCAS_LONG QueueID);
	SKYCAS_S32 SKYCAS_ReleasePrivateDataFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 wPid);
	SKYCAS_VOID CallbackSmartcardProcess(SKYCAS_VOID);
	SKYCAS_S32 SKYCAS_GetFilePath(SKYCAS_CHAR *pFilePath);//��ȡд�ļ���·��
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
