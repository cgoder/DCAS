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

#define SKYCAS_MAXNUM_PROGRAMBYCW   (4)  /* һ������������Ľ�Ŀ�� */
#define SKYCAS_MAXLEN_PINCODE			(8)		/*���PIN�볤��*/
#define SKYCAS_MAXLEN_TVS_NAME			(32)		/*���Ӫ�����Ƴ���*/
#define SKYCAS_MAXLEN_PRODUCT_NAME (21)	/*��Ĳ�Ʒ����,���20���ַ�*/
#define SKYCAS_MAXNUM_ENTITLE	(300)	/*��Ȩ��Ʒ������*/

#define SKYCAS_MAXLEN_EMAIL_SENDER_NAME		(11)		/*E-mail����������*/
#define SKYCAS_MAXLEN_EMAIL_CONTENT			(1025)	/*E-mail��󳤶�*/
#define SKYCAS_MAXLEN_EMAIL_TITLE				(37)		/*E-mail���ⳤ��*/

#define SKYCAS_MAX_FILTER_COUNT 	(12)
#define SKYCAS_FILE_PATH_LEN_MAX 	(4096+1)

#define SKYCAS_MAXLEN_CAT_MAX 	(1024)

#define SKYCAS_MAXNUM_PRICE         (2)    /* ����IPPV�۸���� */

/*------------ �ʼ�ͼ����ʾ��ʽ ------------*/
#define SKYCAS_Email_IconHide       (0x00)  /* �����ʼ�֪ͨͼ�� */
#define SKYCAS_Email_New            (0x01)  /* ���ʼ�֪ͨ����ʾ���ʼ�ͼ�� */
#define SKYCAS_Email_SpaceExhaust   (0x02)  /* ���̿ռ�������ͼ����˸�� */

#define SKYCAS_MAXNUM_COMPONENT     (5)    /* ��Ŀ��������� */
#define SKYCAS_MAXLEN_LOCKMESS      (40)
#define SKYCAS_MAXNUM_CARD 			(4) // ͬʱ���뿨��������

#define SKYCAS_U32_HANDLE_INVALID 	(0xFFFFFFFF)
#define SKYCAS_U16_HANDLE_INVALID 	(0xFFFF)
#define SKYCAS_U8_HANDLE_INVALID 	(0xFF)
#define SKYCAS_PID_INVALID 			(0x1FFF)

/*---------- ����ֵ������ض��� ---------*/
#define SKYCAS_CA_OK                    0x00  /* �ɹ� */
#define SKYCAS_CA_UNKNOWN               0x01  /* δ֪���� */
#define SKYCAS_CA_POINTER_INVALID       0x02  /* ָ����Ч�������Ч */
#define SKYCAS_CA_CARD_INVALID          0x03  /* ���ܿ���Ч*/
#define SKYCAS_CA_PIN_INVALID           0x04  /* PIN����Ч */
#define SKYCAS_CA_DATASPACE_SMALL       0x06  /* �����Ŀռ䲻�� */
#define SKYCAS_CA_CARD_PAIROTHER        0x07  /* ���ܿ��Ѿ���Ӧ��Ļ����� */
#define SKYCAS_CA_DATA_NOT_FIND         0x08  /* û���ҵ���Ҫ������ ��CAT û���յ�*/
#define SKYCAS_CA_PROG_STATUS_INVALID   0x09  /* Ҫ����Ľ�Ŀ״̬��Ч */
#define SKYCAS_CA_CARD_NO_ROOM          0x0A  /* ���ܿ�û�пռ��Ź���Ľ�Ŀ */
#define SKYCAS_CA_WORKTIME_INVALID      0x0B  /* �趨�Ĺ���ʱ����Ч */
#define SKYCAS_CA_IPPV_CANNTDEL         0x0C  /* IPPV��Ŀ���ܱ�ɾ�� */
#define SKYCAS_CA_CARD_NOPAIR           0x0D  /* ���ܿ�û�ж�Ӧ�κεĻ����� */
#define SKYCAS_CA_WATCHRATING_INVALID   0x0E  /* �趨�Ĺۿ�������Ч */
#define SKYCAS_CA_CARD_NOTSUPPORT       0x0F  /* ��ǰ���ܿ���֧�ִ˹��� */
#define SKYCAS_CA_DATA_ERROR            0x10  /* ���ݴ������ܿ��ܾ������ܿ�ͨѶ����  */
#define SKYCAS_CA_FEEDTIME_NOT_ARRIVE   0x11  /* ι��ʱ��δ�����ӿ����ܱ�ι�� */
#define SKYCAS_CA_CARD_TYPEERROR        0x12  /* ��ĸ��ι��ʧ�ܣ��������ܿ����ʹ��� */
#define SKYCAS_CA_CARDHANDLE_INVALID    0x13  /* �������Ч */
#define SKYCAS_CA_CALIB_STOP    		0x14  /* ca��ֹͣ����*/
#define SKYCAS_CA_SEMP_TIMEOUT			0x15  /* �ź�����ʱ*/
#define SKYCAS_CA_ENTITLED				0x16  /* ��Ŀ����Ȩ*/
#define SKYCAS_CA_NO_ENTITLED			0x17  /* ��Ŀδ��Ȩ*/
#define SKYCAS_CA_NO_ENTITLEDRECORD		0x18  /*û�н�Ŀ��Ȩ��¼*/
#define SKYCAS_CA_ENTITLED_OVERDUE		0x19  /* ��Ȩ����*/
#define SKYCAS_CA_TV_LOCKED				0x1A  /* ���ӻ�����б�����*/
#define SKYCAS_CA_PAIRING_FAIL			0x1B  /* ���ʧ��*/
#define SKYCAS_CA_EMM_TIMEOUT			0x1C  /* ��ȡEMM��ʱ*/
#define SKYCAS_CA_NOT_PARENTCHILDCARD	0x1D  /* û�з�����ĸ������*/
#define SKYCAS_CA_PARENTCARD			0x1E  /*  �û�����������*/
#define SKYCAS_CA_MONEY_NOTENOUGH		0x1F  /*  ����*/




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


/*-- Ƶ��������Ϣ --*/

/*-- ��Ŀ�����Ϣ --*/
typedef struct skycas_component_s
{    
/* �������֪ͨ�����н�Ŀ���ͼ�PID����Ϣ��һ����Ŀ���ܰ��������� */    
	SKYCAS_U16   u16CompPID;     /* ���PID */    
	SKYCAS_U16   u16ECMPID;      /* �����Ӧ��ECM����PID���������ǲ����ŵģ���Ӧȡ0�� */    
	SKYCAS_U8    u8CompType;     /* ������� */    
	SKYCAS_U8    u8Reserved[3];/* ���� */
}SKYCAS_Component_S;

/*-- Ƶ��������Ϣ --*/
typedef struct skycas_lockservice_s{        
	SKYCAS_U32   u32Frequency;              /* Ƶ�ʣ�BCD�� */    
	SKYCAS_U32   u32symbol_rate;              /* �����ʣ�BCD�� */    
	SKYCAS_U16   u16PcrPid;                  /* PCR PID */    
	SKYCAS_U8    u8Modulation;               /* ���Ʒ�ʽ */    
	SKYCAS_U8    u8ComponentNum;             /* ��Ŀ������� */    
	SKYCAS_Component_S stCompArr[SKYCAS_MAXNUM_COMPONENT];       /* ��Ŀ����б� */    
	SKYCAS_U8    u8fec_outer;                /* ǰ��������� */    
	SKYCAS_U8    u8fec_inner;                /* ǰ��������� */    
	SKYCAS_CHAR       s8BeforeInfo[SKYCAS_MAXLEN_LOCKMESS+1]; /* ���� */    
	SKYCAS_CHAR       s8QuitInfo[SKYCAS_MAXLEN_LOCKMESS+1];   /* ���� */    
	SKYCAS_CHAR       s8EndInfo[SKYCAS_MAXLEN_LOCKMESS+1];    /* ���� */
}SKYCAS_LockService_S;

/*-- �߼�Ԥ����Ŀ��Ϣ --*/
typedef struct skycas_curtaininfo_s
{    
	SKYCAS_U16   u16ProgramID;        /* ��Ŀ��ID */    
	SKYCAS_U32  u32StartWatchTime;   /* ��ʼ�ۿ�ʱ�� */    
	SKYCAS_U8    u8WatchTotalCount;  /* �ۼƹۿ����� */    
	SKYCAS_U16   u16WatchTotalTime;    /* �ۼƹۿ�ʱ��,(cp������) */
}SKYCAS_CurtainInfo_S;


/* ��ʼ����Ϣ */
typedef struct skycas_init_para_s
{
	SKYCAS_U16 u16StartPrio;		/*���ȼ� */
	SKYCAS_U16 u16CASType;			/* CA ����ѡ��*/
	SKYCAS_CHAR* pFilePath;		/*�ļ�·��*/
	SKYCAS_U32 u32FlashAddr;		/*��ʼ��ַ*/
	SKYCAS_U32 u32FlashSize;		/*����*/
}SKYCAS_INIT_PARA_S;

/* ��Ŀ��Ϣ */
typedef struct skycas_serviceinfo_s
{
    SKYCAS_U16 u16EcmPid;      /* ECM PID */
    SKYCAS_U8 u8ServiceNum;    /* ��ǰPID�µĽ�Ŀ���� */
    SKYCAS_U8 u8Reserved;      /* ���� */
	SKYCAS_U16 u16ServiceID[SKYCAS_MAXNUM_PROGRAMBYCW]; 
} SKYCAS_SERVICEINFO_S;

/* pin����Ϣ*/
typedef struct skycas_pininfo_s
{
	SKYCAS_U16	u8PinLen;
	SKYCAS_U8	u8Pin[SKYCAS_MAXLEN_PINCODE];
} SKYCAS_PININFO_S;

/* ����ʱ����Ϣ*/
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

/* ��Ӫ����Ϣ */
typedef struct skycas_operatorinfo_s
{
	SKYCAS_U16	u16TVSID; 									/*��Ӫ�̵��ⲿ���*/
	SKYCAS_U8	u8TVSName[SKYCAS_MAXLEN_TVS_NAME+1];		/*��Ӫ������*/
	SKYCAS_U8  u8Reserved[3];    						/* ���� */
}SKYCAS_OPERATORINFO_S;

/* Ǯ����Ϣ */
typedef struct skycas_tvslotinfo_s
{
    SKYCAS_FLOAT fSlotLeft; 		/*Ǯ����� */
	SKYCAS_FLOAT fSlotSpent;     /*�ѻ��ѵĽ��*/ 
}SKYCAS_TVSLOTINFO_S;

/* ��Ȩ��Ϣ */
typedef struct skycas_entitleinfo_s
{
    SKYCAS_U32  u32ProductID;   /* ��ͨ��Ȩ�Ľ�ĿID */    
	//SKYCAS_U16 stEntitleTime;    /*��Ȩʱ�� */
	SKYCAS_DATETIME_S stBeginTime;    /* ��Ȩ����ʼʱ�� */
	SKYCAS_DATETIME_S stEndTime;   /* ��Ȩ�Ľ���ʱ�� */
	//SKYCAS_U8	u8ProductName[SKYCAS_MAXLEN_PRODUCT_NAME];/*��Ʒ����*/
    SKYCAS_U8  u8Video;    /* �Ƿ�¼��1�����ԣ�0��������*/
    SKYCAS_U8  u8Reserved[3];	 /* ���� */
} SKYCAS_ENTITLEINFO_S;

/*-- ��Ȩ��Ϣ���� --*/
typedef struct skycas_entitles
{
    SKYCAS_U16      u16ProductCount;
    SKYCAS_U8       u8Reserved[2];    /* ���� */
    SKYCAS_ENTITLEINFO_S  stEntitles[SKYCAS_MAXNUM_ENTITLE]; /* ��Ȩ�б� */
}SKYCAS_Entitles;


/* IPPV/IPPT��Ŀ�ļ۸� */
typedef struct skycas_ippv_price_s
{
    SKYCAS_U16  u16Price;       /* ��Ŀ�۸�*/
    SKYCAS_U8   u8PriceCode;  /* ��Ŀ�۸����� */
    SKYCAS_U8   u8Reserved;   /* ���� */
} SKYCAS_IPPV_PRICE_S;

/* IPPV��Ŀ��Ϣ */
typedef struct
{
    SKYCAS_U32   u32ProductID;   /* ��Ŀ��ID */
    SKYCAS_FLOAT u32Price;        /* ��Ŀ�۸� */
    SKYCAS_DATETIME_S  	 stEndDate;  /* ��Ŀ����ʱ��*/
    SKYCAS_DATETIME_S  	 stStartDate;  /* ��Ŀ��ʼʱ��*/
    SKYCAS_U8    u8Flag;  /* ��Ʒ״̬��BOOKING��VIEWED */ 
    SKYCAS_U8    u8Video;      /* �Ƿ����¼��1�����ԣ�0��������*/
    SKYCAS_U8    u8SlotID;      /* Ǯ��ID */
    SKYCAS_U8    u8Reserved;    /* ���� */
} SKYCAS_IPPVINFO_S;

/*-- IPPV/IPPT��Ŀ������ʾ��Ϣ --*/
typedef struct skycas_ippvbuyinfo_s{    
	//SKYCAS_U8         u8scNum;				/*֧�ֵĿ��ĸ���*/	
	//SKYCAS_VOID*      scHandle[SKYCAS_MAXNUM_CARD];             /* ʹ�õĿ���� */    
	SKYCAS_U32        u32ProductID;          /* ��Ŀ��ID */    
	SKYCAS_U16        u16TvsID;               /* ��Ӫ��ID */    
	SKYCAS_U8         u8SlotID;             /* Ǯ��ID */    
	SKYCAS_U8         u8PriceNum;           /* ��Ŀ�۸���� */    
	SKYCAS_IPPV_PRICE_S  stPrice[SKYCAS_MAXNUM_PRICE]; /* ��Ŀ�۸� */
    union {        
		SKYCAS_DATETIME_S   stExpiredDate;         /* ��Ŀ����ʱ��,IPPV�� */        
		SKYCAS_U16    u16IntervalMin;         /* ʱ��������λ����,IPPT �� */    
		}SKYCAS_IPPVTime;    
	SKYCAS_DATETIME_S	stStartDate;		   /* ��Ŀ��ʼʱ��,IPPV�� */		
}SKYCAS_IppvBuyInfo_S;

/*-- IPPV/IPPT��Ŀ������ʾ��Ϣ --*/
typedef struct skycas_ippvbuyallinfo
{
    SKYCAS_U8   u8MessageType;
    SKYCAS_U16  u16EcmPid;
    SKYCAS_IppvBuyInfo_S  stIppvbuyinfo;
}SKYCAS_IppvBuyAllInfo_S;


/* �ʼ�ͷ����Ϣ */
typedef struct skycas_mailheadinfo_s
{
	SKYCAS_U32	u32EmailID;                 /* Email ID */
	SKYCAS_DATETIME_S	u32SendTime;						  /*����ʱ��*/
	SKYCAS_U8	u8SenderName[SKYCAS_MAXLEN_EMAIL_SENDER_NAME];	/*����������*/
	SKYCAS_U8  u8EmailHead[SKYCAS_MAXLEN_EMAIL_TITLE]; 	/* �ʼ�����*/
	SKYCAS_U8  u8NewEmail;           /* ���ʼ���ǣ�0���Ѷ���1�����ʼ� */
	SKYCAS_U16  u8Importance;         /* ��Ҫ�ԣ� 0����ͨ��1����Ҫ */
	SKYCAS_U8  u8Reserve;         /* ����λ */
}SKYCAS_EMAILHEADINFO_S;

/*�ʼ�������Ϣ*/
typedef struct skycas_mailcontent_s
{
	SKYCAS_U8	u8MailContent[SKYCAS_MAXLEN_EMAIL_CONTENT];		/*�ʼ�����*/
	SKYCAS_U8	u8Reserve[3];		/*����*/
	SKYCAS_U32	u32EmailLen;					  /*�ʼ����ݳ���*/
}SKYCAS_EMAILCONTENT_S;
#if 0
typedef struct skycas_event_s
{
	SKYCAS_U32 u32EventId;					/*��ϢID*/
	//SKYCAS_U8 * u8EventContent;			/*��Ϣ����*/
	SKYCAS_U16 	 u16EcmPid;
	SKYCAS_U8	 u8MessageType;
	SKYCAS_U8    u8Reserve;
}SKYCAS_EVENT_S;
#endif

/*��һ����������Ϣ���ͣ����������������տ⴫�����Ĳ���˳������ʹ��*/
/*����CDSTBCA_ShowBuyMessage�У�u32Param1��ecmpid,u32Param2��byMessageType��u32Param3��ʹ����ΪȫF*/
/*�����ָ�룬ֱ��ǿת��U32����*/
typedef struct skycas_event_s
{
	SKYCAS_CaMessageType_E u32EventId;					/*��Ϣ����*/
	SKYCAS_U32 	 u32Param1;
	SKYCAS_U32	 u32Param2;
	SKYCAS_U32   u32Param3;
}SKYCAS_EVENT_S;

typedef enum
{
	SKYCAS_TYPE_TF = 0,	/*�����Ӳ�/ͬ��*/
	SKYCAS_TYPE_DVT,	/*������Ѷ*/
	SKYCAS_TYPE_DVN,	/*���*/
	SKYCAS_TYPE_IRDETO,	/*���ϵ�*/
	SKYCAS_TYPE_NDS,	/*NDS*/
	SKYCAS_TYPE_INVALID	/*��Чֵ*/
}SKYCAS_TYPE_E;

/*������Ϣ*/
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
������: 
skyworth_ca_api(SKYCAS_VOID)
����:  
���캯���� 
����:  
[IN]		�ޡ�  
[OUT]		�ޡ�  
[RET]		�ޡ� 
************************************************************************/
skyworth_ca_api(SKYCAS_VOID);

/***********************************************************************
������: 
~skyworth_ca_api(SKYCAS_VOID)
����:  
���������� 
����:  
[IN]		�ޡ�  
[OUT]		�ޡ�  
[RET]		�ޡ� 
************************************************************************/
~skyworth_ca_api(SKYCAS_VOID);

/***********************************************************************
������:
	static skyworth_ca_api* GetCaInstance(SKYCAS_VOID);
����:  
�õ����һ��ʵ���� 
����:  
[IN]		
�ޡ�  
[OUT]		�ޡ�  
[RET]		
����һ�����ʵ���� 
************************************************************************/
static skyworth_ca_api* GetCaInstance(SKYCAS_VOID);

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
	SKYCAS_U32 SKYCAS_GetCAType(SKYCAS_VOID);
����:  
��ȡ��ǰCA����
����:  
[IN]		�ޡ�
[OUT]		�ޡ�  
[RET]		
	����CA����
************************************************************************/
SKYCAS_U32 SKYCAS_GetCAType(SKYCAS_VOID);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara );
����:  
CA��ʼ���� 
����:  
[IN]		
SKYCAS_INIT_PARA_S * pInitPara����ʼ��������  
[OUT]		�ޡ�  
[RET]		
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara);

/***********************************************************************
������: 
virtual SKYCAS_ S32 SKYCAS_CaDeInit(SKYCAS_VOID);
����:  
CAȥ��ʼ���� 
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_CaDeInit(SKYCAS_VOID);

/***********************************************************************
������: 
virtual SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);
����:  
�жϲ���SystemID�Ƿ��ǵ�ǰCA��SystemID�� 
����:  
[IN]		
SKYCAS_U16 u16SystemID��CAϵͳID��  
[OUT]		�ޡ�  
[RET]		
�Ƿ���SKYCAS_TURE���񷵻�SKYCAS_FALSE�� 
************************************************************************/
virtual SKYCAS_BOOL SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);
����:  
����EMMPID�� 
����:  
[IN]		
SKYCAS_U16 u16EmmPid��EMMPID��
[OUT]		�ޡ�
[RET]		
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);
����:  
CAT��仯ʱ����EMMPID�� 
����:  
[IN]		
SKYCAS_U8 *pbuffer:CAT������ָ�롣  
[OUT]		�ޡ�  
[RET]		
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
����:  
����ECMPID�� 
����:  
[IN]		
pServiceInfo:service��Ϣ�ṹ��ָ�롣
[OUT]		�ޡ�
[RET]		
�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);
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
virtual SKYCAS_S32 SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);
����:  
�ϲ㴫NIT���ݸ�CAģ��,��Ҫ����ͬ��CA
����:  
[IN]		
SKYCAS_U8 *DataBuf:���ݵ�ַ
[OUT]		�ޡ�
[RET]		
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetNITData(SKYCAS_U8 *DataBuf);

/* ���ܿ����� */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);
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
virtual SKYCAS_S32 SKYCAS_GetCardSN(SKYCAS_U8* pCardSN);

/***********************************************************************
������: 
virtual SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
����:  
���ܿ����̡߳�
����:  
[IN]		
	SKYCAS_VOID* para��Ԥ������
[OUT]		��
[RET]		
virtual SKYCAS_VOID SKYCAS_SmartCard_Task(SKYCAS_VOID* para);
************************************************************************/

/* PIN����� */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
												const SKYCAS_U8* pNewPin);
����:  
����PIN�롣 
����:  
[IN]		
const SKYCAS_U8* pOldPin���ɵ�PIN��
const SKYCAS_U8* pNewPin���µ�PIN��
ע:���CA����Ϊ�ĸ��ֽ�ASCII��
[OUT]		�ޡ�  
[RET]		
SKYCAS_CA_OK:�޸ĳɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_PIN_INVALID:PIN�����
SKYCAS_CA_UNKNOWN:δ֪����
SKYCAS_CA_CARD_NOTSUPPORT:���ܿ���֧�ִ˹���
************************************************************************/
virtual SKYCAS_S32 SKYCAS_ChangePin( const SKYCAS_U8* pOldPin,
                               				const SKYCAS_U8* pNewPin);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);
����:  
У���û�����pin���Ƿ��뿨��һ�¡� 
����:  
[IN]		
const SKYCAS_PinInfo *pPIN���û�����pin��  
[OUT]		�ޡ�  
[RET]		
һ�·���SKYCAS_RET_SUCCESS����һ�·���SKYCAS_RET_FAILURE
************************************************************************/
virtual SKYCAS_S32 SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN);

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);
����:  
�ж��û�PIN���Ƿ���ס�� 
����:  
[IN]		
	  SKYCAS_BOOL * bLocked��true��ʾ��ס�ˣ�false��ʾû��
[OUT]		�ޡ�  
[RET]		
	�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE
************************************************************************/
virtual SKYCAS_S32 SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked);

/* �û��ۿ�������� */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
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
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_PIN_INVALID:PIN����Ч
SKYCAS_CA_WATCHRATING_INVALID:�ۿ�������Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetRating( const SKYCAS_U8* pPin,
											SKYCAS_U8 u8Rating );

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );
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
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_S32 SKYCAS_GetRating( SKYCAS_U8* pRating );

/* ���ܿ�����ʱ�ι��� */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
                                 SKYCAS_DATETIME_S *StartTime,
                                 SKYCAS_DATETIME_S *EndTime); 

����:  
���ù���ʱ��Ρ� 
����:  
[IN]		
  const SKYCAS_U8* pPin���û������pin��
  SKYCAS_DATETIME_S *StartTime������ʱ����ʼʱ��
  SKYCAS_DATETIME_S *EndTime������ʱ�����ʱ��
[OUT]		�ޡ�  
[RET]		
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_PIN_INVALID:PIN����Ч�����
SKYCAS_CA_WORKTIME_INVALID:����ʱ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_S32 SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,
												SKYCAS_DATETIME_S *StartTime,
												SKYCAS_DATETIME_S *EndTime);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
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
virtual SKYCAS_S32 SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,
												SKYCAS_DATETIME_S *stEndTime);

/* ������Ϣ��ѯ */
/***********************************************************************
������: 
virtual SKYCAS_U32 SKYCAS_GetVer(SKYCAS_VOID); 
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
virtual SKYCAS_U32 SKYCAS_GetVer(SKYCAS_U8 *pData);

/***********************************************************************
������: 
virtual SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);
����:  
��ѯ��ǰ����ƽ̨���
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
ƽ̨���
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetPlatformID(SKYCAS_VOID);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );
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
virtual SKYCAS_S32 SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID );

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
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
virtual SKYCAS_S32 SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID,
										SKYCAS_U8 * pCount,
										SKYCAS_OPERATORINFO_S* stOperatorInfo );

/***********************************************************************
������:
virtual SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );
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
virtual SKYCAS_S32 SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray );

/*Ǯ����Ϣ��ѯ */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID , SKYCAS_U32 *pSlotNum);
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
virtual SKYCAS_S32 SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID);//, SKYCAS_U32 *pSlotNum);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
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
virtual SKYCAS_S32 SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,
											SKYCAS_U8   u8SlotID,
											SKYCAS_TVSLOTINFO_S* pSlotInfo );

/* ��Ȩ��Ϣ���� */
/***********************************************************************
������:
virtual SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
									SKYCAS_ENTITLEINFO_S * pServiceEntitles );
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
virtual SKYCAS_S32 SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID,
													SKYCAS_Entitles* pServiceEntitles );

/***********************************************************************
������: 
	virtual SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
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
virtual SKYCAS_S32 SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,
												SKYCAS_U32* pEntitleId );

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
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
virtual SKYCAS_S32 SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,
													SKYCAS_BOOL* pReadFlag,
													SKYCAS_U32*  pDetitleChkNums);

/***********************************************************************
������: 
virtual SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID); 
����:  
��ѯ����Ȩ��Ϣ��ȡ״̬
����:  
[IN]		 
SKYCAS_U16 u16TVSID: ��Ӫ��ID
[OUT]		�ޡ�  
[RET]		
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ������Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_BOOL SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID);

/***********************************************************************
������: 
virtual SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
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
virtual SKYCAS_BOOL SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,
													SKYCAS_U32 u32DetitleChkNum );

/* ������Ӧ */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );
����:  
��ѯ������Ӧ���
����:  
[IN]		 
SKYCAS_U8* pNum��
SKYCAS_U8* pSTBSerial��
[OUT]		�ޡ�  
[RET]		
SKYCAS_CA_OK:�����
SKYCAS_CA_DATA_ERROR:���ܿ�ͨѶ����
SKYCAS_CA_CARD_PAIROTHER:���ܿ����������ն����
SKYCAS_CA_CARD_NOPAIR:���ܿ�û�����κ��ն����
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_POINTER_INVALID:ָ��Ϊ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_S32 SKYCAS_IsPaired( SKYCAS_U8* pNum,
										SKYCAS_U8* pSTBSerial );

/* IPPVӦ�� */
/***********************************************************************
������: 
����:  
	���û���IPPV��Ŀ֪ͨ����ʱ�����ô˽ӿڽ���Ϣ���ݸ�CA��
����:  
[IN]		
SKYCAS_BOOL bBuyProgram:�û��Ƿ���
SKYCAS_U16 u16EcmPid:ecmpid
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
virtual SKYCAS_S32 SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram,
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
virtual SKYCAS_S32 SKYCAS_GetIPPVProgram( SKYCAS_U16 u16TVSID,
												SKYCAS_IPPVINFO_S* pIppv,
												SKYCAS_U16* u16Number );

/* �ʼ����� */
/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
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
virtual SKYCAS_S32 SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead,
												SKYCAS_U8* pCount, SKYCAS_U8* pIndex);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);
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
virtual SKYCAS_S32 SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);
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
virtual SKYCAS_S32 SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont);


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
virtual SKYCAS_S32 SKYCAS_DelEmail(SKYCAS_U32 u32EmailID);

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
virtual SKYCAS_U16 SKYCAS_GetEmailSpaceInfo( SKYCAS_U8* pEmailNum,
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
SKYCAS_CA_CARD_INVALID:���ܿ���Ч
SKYCAS_CA_DATA_NOT_FIND:û���ҵ�����Ӫ��
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
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

virtual SKYCAS_U16 SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID,
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

virtual SKYCAS_U16 SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, 
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
virtual SKYCAS_VOID SKYCAS_RefreshInterface( SKYCAS_VOID );

/***********************************************************************
������: 
����:  
�ص�������
����:  
[IN]		
[OUT]		
[RET]		
************************************************************************/
typedef SKYCAS_VOID(*SKYCAS_PostEvent_CB)(SKYCAS_EVENT_S eCAEvent);

virtual SKYCAS_S32 SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ);

/* ��ȡ������Ϣ */
/***********************************************************************
������: 
	virtual SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);	
����:  
��ȡpin�볤��
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	����pin��ĳ���
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetPinLen(SKYCAS_VOID);				

/***********************************************************************
������: 
virtual SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);	
����:  
��ȡ���ܿ��ų���
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	�������ܿ��ų��� 
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetCardNumLen(SKYCAS_VOID);			

/***********************************************************************
������: 
virtual SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);
����:  
��ȡ�ʼ�������������󳤶�
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	�����ʼ�������������󳤶�
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetSendNameMaxLen(SKYCAS_VOID);		

/***********************************************************************
������: 
	virtual SKYCAS_U16 SKYCAS_GetTitleMaxLen(SKYCAS_VOID);
����:  
��ȡ�ʼ�������󳤶�
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	�����ʼ�������󳤶�
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetTitleMaxLen();				

/***********************************************************************
������: 
virtual SKYCAS_ U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);
����:  
��ȡ�ʼ�������󳤶�
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	�����ʼ�������󳤶�
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetContentMaxLen(SKYCAS_VOID);			

/***********************************************************************
������: 
virtual SKYCAS_ U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);
����:  
��ȡosd��󳤶�
����:  
[IN]		�ޡ�
[OUT]		�ޡ�
[RET]		
	����osd��󳤶�
************************************************************************/
virtual SKYCAS_U16 SKYCAS_GetOSDMaxLen(SKYCAS_VOID);			

/***************���½ӿ�������CA***************************/


/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);
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
virtual SKYCAS_S32 SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);
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
virtual SKYCAS_S32 SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);
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
virtual SKYCAS_S32 SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes);

/***********************************************************************
������: 
virtual SKYCAS_S16 SKYCAS_GetCASID(SKYCAS_VOID);
����:  
��ȡCAS ID��������16 bit ���
����:  
[IN]		:��
[OUT]	:��	
[RET]	
	����CAS ID
virtual SKYCAS_S16 SKYCAS_GetCASID(SKYCAS_VOID);
************************************************************************/

/***********************************************************************
������: 
virtual SKYCAS_S32  SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);
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
virtual SKYCAS_S32 SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);
����:  
��ȡ��������������20 �ֽڵ�ASCII ���
����:  
[IN]		
SKYCAS_U8 *pUser:ָ�򷵻����ݵĴ洢�ռ�
[OUT]		
SKYCAS_U8 *pUser:��ȡ���ĳ���������ָ��
[RET]	
SKYCAS_CA_OK:��ȡ�ɹ�
SKYCAS_CA_POINTER_INVALID:ָ����Ч
SKYCAS_CA_UNKNOWN:δ֪����
************************************************************************/
virtual SKYCAS_S32 SKYCAS_PortingVendor(SKYCAS_U8 *pUser);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);
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
virtual SKYCAS_S32 SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate);


/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
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
virtual SKYCAS_S32 SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum);
************************************************************************/

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);
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
virtual SKYCAS_S32 SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index);


/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);
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
virtual SKYCAS_S32 SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);
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
virtual SKYCAS_S32 SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);
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
SKYCAS_CA_DATA_NOT_FIND:CAT û���յ�
SKYCAS_CA_PAIRING_FAIL	: ���ʧ��
SKYCAS_CA_EMM_TIMEOUT: ��ȡEMM��ʱ
************************************************************************/
virtual SKYCAS_S32 SKYCAS_PerformPairing(SKYCAS_U8 *pData);


/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);
����:  
��ȡ��ĸ��������,���ӿ�����ĸ��
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
virtual SKYCAS_S32 SKYCAS_GetPCCardType(SKYCAS_S32 *pType);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);
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
virtual SKYCAS_S32 SKYCAS_GetPCardID(SKYCAS_U8 *pCardID);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);
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
virtual SKYCAS_S32 SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate);


/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S stStartTime
											SKYCAS_DATETIME_S stEndTime,SKYCAS_DATETIME_S stTrasTime);
����:  
���IPPV��Ŀ
����:  
[IN]		
SKYCAS_S32 s32ProuctID: IPPV ��Ŀ�Ĳ�ƷID
SKYCAS_FLOAT fPrice: IPPV ��Ŀ�ļ۸�
SKYCAS_DATETIME_S *stStartTime: IPPV ��Ŀ�Ŀ�ʼʱ��
SKYCAS_DATETIME_S *stEndTime: IPPV ��Ŀ�Ľ���ʱ��
SKYCAS_DATETIME_S *stTrasTime: IPPV ��Ŀ�Ĺ���ʱ�䣬����ǰʱ��
SKYCAS_U8 *pPin:PIN��
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
virtual SKYCAS_S32 SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);
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
virtual SKYCAS_S32 SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData);

/***********************************************************************
������: 
virtual SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);
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
virtual SKYCAS_S32 SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData);

/***************���Ͻӿ�������CA***************************/

private:

	SKYCAS_U32 SKYCAS_ReadCAType(SKYCAS_VOID);

public:
	 static skyworth_ca_api *pSkyworthCAApi;

private:
	SKYCAS_U32 u32CAType;
	
};

/****************************** ����Ϊ��CAS�������֣�����ܶ������ļ�����װ��libskyworth�ڣ���Ϊ����ȡ ************************************************/

#define FLAG_SKYCAS_RESOURCE_FOR_APK (1)  /* ����APK��ʽ����CA�⹦��ʱ�ã�������ĺ�FLAG_SKYCAS_RESOURCE_RECYCLING���⡣*/
#define FLAG_SKYCAS_RESOURCE_RECYCLING (0)  /* �Ƿ���Ҫ����CA��Դ��Ĭ����Ҫ�򿪡�*/

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

