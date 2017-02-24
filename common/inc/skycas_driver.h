#ifndef __SKYCAS_DRIVER_H__
#define __SKYCAS_DRIVER_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif     /* __cplusplus */
#endif  /* __cplusplus */

/**************************************************
	SmartCard
**************************************************/

typedef enum skycas_smc_event_e
{
	SKYCAS_SMC_EVENT_IN = 0,		/*�������¼�*/
	SKYCAS_SMC_EVENT_OUT,		/*���γ��¼�*/
	SKYCAS_SMC_EVENT_INVALID	/*��Ч*/
} SKYCAS_SMC_EVENT_E;


typedef enum skycas_smc_status_e
{
	SKYCAS_SMC_STATUS_NOCARD = 0,	/*δ��⵽��*/
	SKYCAS_SMC_STATUS_BUSY,		/*����æ/���ڳ�ʼ��/�շ�����*/
	SKYCAS_SMC_STATUS_ACTIVE,	/*������ȷ��ʼ�����*/
	SKYCAS_SMC_STATUS_INACTIVE,	/*��������ȷ��ʼ��/��Ч��*/
	SKYCAS_SMC_STATUS_FAIL,		/*��ȡ״̬ʧ��/δ֪״̬*/
	SKYCAS_SMC_STATUS_INVALID	/*��Ч*/
} SKYCAS_SMC_STATUS_E;
typedef enum skycas_smc_mode_e
{
	SKYCAS_SMC_MODE_T0 = 0,	/*T0Э��*/
	SKYCAS_SMC_MODE_T1,
	SKYCAS_SMC_MODE_T14,
	SKYCAS_SMC_MODE_INVALID	/*��Ч*/
}SKYCAS_SMC_MODE_E;

/******************************************************************************************
	���ܿ��ص��������塣
******************************************************************************************/
typedef	SKYCAS_VOID (*SKY_CAS_SMC_EVENT_CB)(SKYCAS_U8 cardSlot, SKYCAS_SMC_EVENT_E eSmcEvent);

/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_Init(SKYCAS_U8 cardSlot)
����:
	���ܿ�ģ���ʼ����
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
	[OUT]
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Init(SKYCAS_U8 cardSlot);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_Deinit(SKYCAS_U8 cardSlot)
����:
	���ܿ�ģ��ȥ��ʼ����
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Deinit(SKYCAS_U8 cardSlot);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_Open(SKYCAS_U8 cardSlot, SKYCAS_SMC_MODE_E smcOpenMode)
����:
	�����ܿ�ģ�顣
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
		SKYCAS_SMC_MODE_E smcOpenMode	���ܿ���ʼ��Э�顣T0/T14/T1�ȡ�
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Open(SKYCAS_U8 cardSlot, SKYCAS_SMC_MODE_E smcOpenMode);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_Close(SKYCAS_U8 cardSlot)
����:
	�ر����ܿ�ģ�顣
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Close(SKYCAS_U8 cardSlot);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_Reset(SKYCAS_U8 cardSlot)
����:
	��λ���ܿ�ģ�顣
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Reset(SKYCAS_U8 cardSlot);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_SendRecv (SKYCAS_U8 cardSlot, const SKYCAS_U8* const pU8SendData, SKYCAS_U16* const pU16SendLen, 
								SKYCAS_U8* const pU8RecvData, SKYCAS_U16* const pU16RecvLen);
����:
	���ܿ�ͨѶ/�շ����ݡ�
����:
	[IN]	
		SKYCAS_U8 cardSlot					���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
		const SKYCAS_U8* const pU8SendData		����ָ�롣�������ݻ�������ַ��
		SKYCAS_U16* const pU16SendLen			����ָ�롣�ñ����洢��Ҫ�������ݵ��ֽڳ��ȡ�
		SKYCAS_U8* const pU8RecvData			����ָ�롣�������ݻ�������ַ��
		SKYCAS_U16* const pU16RecvLen			����ָ�롣�ñ����洢���ɽ��յ������ֽڳ��ȡ�
	[OUT]	
		SKYCAS_U16* const pU16SendLen			����ָ�롣�ñ����洢ʵ���ѷ������ݵ��ֽڳ��ȡ�
		SKYCAS_U16* const pU16RecvLen			����ָ�롣�ñ����洢ʵ���ѽ��յ������ֽڳ��ȡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_SendRecv (SKYCAS_U8 cardSlot, const SKYCAS_U8* const pU8SendData, SKYCAS_U16* const pU16SendLen, 
											SKYCAS_U8* const pU8RecvData, SKYCAS_U16* const pU16RecvLen);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_GetATR (SKYCAS_U8 cardSlot, SKYCAS_U8* const pU8AtrData, SKYCAS_U16* const pU16AtrLen);
����:
	��ȡ���ܿ�ATR����ֵ��
����:
	[IN]	
		SKYCAS_U8 cardSlot			���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
		SKYCAS_U8* pU8AtrData const	����ָ�롣ATRֵ��������ַ��
		SKYCAS_U16* pU16AtrLen const	����ָ�롣�ñ����洢���ɽ��յ������ֽڳ��ȡ�
	[OUT]	
		SKYCAS_U16* pU16AtrLen const	����ָ�롣�ñ����洢ʵ���յ��������ֽڳ��ȡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_GetATR (SKYCAS_U8 cardSlot, SKYCAS_U8* const pU8AtrData, SKYCAS_U16* const pU16AtrLen);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_SMC_RegistCB(SKYCAS_U8 cardSlot, SKY_CAS_SMC_EVENT_CB pFnSmcCB)
����:
	ע�����ܿ��ص�������
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
		SKY_CAS_SMC_EVENT_CB pFnSmcCB	����ָ�롣ָ�����ϲ㴫�ݹ��������ܿ��¼��ص�������
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_RegistCB(SKYCAS_U8 cardSlot, SKY_CAS_SMC_EVENT_CB pFnSmcCB);
/******************************************************************************************
������:	
	SKYCAS_SMC_STATUS_E SKYCAS_SMC_GetStatus(SKYCAS_U8 cardSlot)
����:
	��ȡ���ܿ�״̬��
����:
	[IN]	
		SKYCAS_U8 cardSlot	���ܿ����ͨ���š������ۻ���Ĭ��Ϊ0��
	[OUT]	
		�ޡ�
	[RET]		
		����ö��ֵSKYCAS_SMC_STATUS_E��
******************************************************************************************/
SKYCAS_SMC_STATUS_E SKYCAS_SMC_GetStatus(SKYCAS_U8 cardSlot);


/**************************************************
	Demux/Filter
**************************************************/
#define SKYCAS_FILTER_DEEP (16)
typedef enum skycas_filter_mode_e
{
	SKYCAS_FILTER_MODE_ONESHOT = 0,
	SKYCAS_FILTER_MODE_ONESHOT_CRC,
	SKYCAS_FILTER_MODE_CONTINUE,
	SKYCAS_FILTER_MODE_CONTINUE_CRC,
	SKYCAS_FILTER_MODE_INVALID
}SKYCAS_FILTER_MODE_E;

typedef enum skycas_filter_event_e
{
	SKYCAS_FILTER_EVENT_READY = 0,
	SKYCAS_FILTER_EVENT_TIMEOUT,
	SKYCAS_FILTER_EVENT_OVERFLOW,
	SKYCAS_FILTER_EVENT_CRCERROR,
	SKYCAS_FILTER_EVENT_ERRORUNKNOWN,
	SKYCAS_FILTER_EVENT_INVALID
}SKYCAS_FILTER_EVENT_E;

typedef enum skycas_filter_section_size_e
{
	SKYCAS_FILTER_SECTION_SIZE_1K = 0,
	SKYCAS_FILTER_SECTION_SIZE_4K,
	SKYCAS_FILTER_SECTION_INVALID
}SKYCAS_FILTER_SECTION_SIZE_E;
typedef struct skycas_Fliter_para_s
{
	SKYCAS_U16 u16FilterDataPid;	/*���������˵�ECMPID����*/
	SKYCAS_FILTER_MODE_E eFilterMode;
	SKYCAS_U8 u8FilterDeep;		/*���������*/
	SKYCAS_U32 u32FilterTimeout;	/*��������ʱʱ�䡣��λ���롣Ϊ0ʱ����һֱ�ȴ�����Ҫ��ʱ*/
	SKYCAS_U8 u8ArrFilterMacth[SKYCAS_FILTER_DEEP];	/*Match*/
	SKYCAS_U8 u8ArrFilterMask[SKYCAS_FILTER_DEEP];	/*Mask*/
	SKYCAS_U8 u8ArrFilterNeg[SKYCAS_FILTER_DEEP];	/*Negate*/
	SKYCAS_S32 (*SKY_CAS_FILTER_EVENT_CB)(SKYCAS_U32 u32FilterId,  const SKYCAS_U8* const pU8Buffer, SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);	/*�������ص�����ָ��*/
}SKYCAS_FILTER_PARA_S;

/******************************************************************************************
	�������ص��������塣����ֵԤ�������ֽ׶�ֱ�ӷ���SKYCAS_RET_SUCCESS
******************************************************************************************/
typedef	SKYCAS_S32 (*SKY_CAS_FILTER_EVENT_CB)(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU8Buffer, 
															SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Filter_Alloc(SKYCAS_U32* const pU32FilterId, SKYCAS_U32 u32FilterSectionSize)
����:
	�����������
����:
	[IN]	
		SKYCAS_U32* const pU32FilterId		����ָ�롣�洢Ĭ��ֵ��
		SKYCAS_FILTER_SECTION_SIZE_E eFilterSectionSize		����������֮һ����ʾ������section��С��
	[OUT]	
		SKYCAS_U32* const pU32FilterId		����ָ�롣�洢���䵽�Ĺ�����ID��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Alloc(SKYCAS_U32* const pU32FilterId, SKYCAS_FILTER_SECTION_SIZE_E eFilterSectionSize);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Filter_Free(SKYCAS_U32 u32FilterId)
����:
	�ͷŹ�������
����:
	[IN]	
		SKYCAS_U32 u32FilterId	������ID��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Free(SKYCAS_U32 u32FilterId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Filter_Start(SKYCAS_U32 u32FilterId)
����:
	������������
����:
	[IN]	
		SKYCAS_U32 u32FilterId	������ID��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Start(SKYCAS_U32 u32FilterId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Filter_Stop(SKYCAS_U32 u32FilterId)
����:
	ֹͣ��������
����:
	[IN]	
		SKYCAS_U32 u32FilterId	������ID��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Stop(SKYCAS_U32 u32FilterId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Filter_Set(SKYCAS_U32 u32FilterId, const SKYCAS_FILTER_PARA_S* const pFilterPara);
����:
	���ù��������ԡ�
����:
	[IN]	
		SKYCAS_U32 u32FilterId	������ID��
		const SKYCAS_FILTER_PARA_S* const pFilterPara	�ṹ��ָ�롣��Ա���Բμ��ṹ��SKYCAS_FILTER_PARA_S��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Set(SKYCAS_U32 u32FilterId, const SKYCAS_FILTER_PARA_S* const pFilterPara);


/**************************************************
	Descramber
**************************************************/
//SKYCAS_S32 SKYCAS_Des_Init(SKYCAS_VOID);
//SKYCAS_S32 SKYCAS_Des_Deinit(SKYCAS_VOID);
//SKYCAS_S32 SKYCAS_Des_Alloc(SKYCAS_U32* pU32DesId);
//SKYCAS_S32 SKYCAS_Des_Free(SKYCAS_U32 u32DesId);
//SKYCAS_S32 SKYCAS_Des_Attach(SKYCAS_U32 u32DesId, SKYCAS_U16 u16ComponentPid);
//SKYCAS_S32 SKYCAS_Des_Detach(SKYCAS_U32 u32DesId, SKYCAS_U16 u16ComponentPid);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Des_SetKey(SKYCAS_U16 u16EcmPid, const SKYCAS_U8* const pU8OddKey, const SKYCAS_U8* const pU8EvenKey);
����:
	���ÿ����֡�
����:
	[IN]	
		SKYCAS_U16 u16EcmPid				�����ֶ�Ӧ��EcmPid��
		const SKYCAS_U8* const pU8OddKey	����ָ�롣ָ��������ִ���
		const SKYCAS_U8* const pU8EvenKey	����ָ�롣ָ��ż�����ִ���
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Des_SetKey(SKYCAS_U16 u16EcmPid, const SKYCAS_U8* const pU8OddKey, const SKYCAS_U8* const pU8EvenKey);


/**************************************************
	Flash/File
**************************************************/
//typedef enum skshcas_flash_rw_mode_e
//{
//	SKYCAS_FLASH_RW_MODE_OS,
//	SKYCAS_FLASH_RW_MODE_OS_NONE,
//	SKYCAS_FLASH_RW_MODE_INVALID
//}SKYCAS_FLASH_RW_MODE_E;
//SKYCAS_FLASH_RW_MODE_E SKYCAS_Flash_RW_Mode_Get(SKYCAS_VOID);
//SKYCAS_S32 SKYCAS_Flash_Init(SKYCAS_VOID);
//SKYCAS_S32 SKYCAS_Flash_Deinit(SKYCAS_VOID);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Flash_Read(SKYCAS_U32 readAddr, SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
����:
	��Flash����(������ַ��)
����:
	[IN]	
		SKYCAS_U32 readAddr			Flash��ַ��
		SKYCAS_U32* const pReadLen		����ָ�롣�ñ����洢��Ҫ��ȡ�������ֽڳ��ȡ�
		SKYCAS_U8* const pReadBuf		����ָ�롣������洢��ȡ�������ݡ�
	[OUT]	
		SKYCAS_U8* const pReadLen		����ָ�롣�ñ����洢ʵ�ʶ�ȡ���������ֽڳ��ȡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Flash_Read(SKYCAS_U32 readAddr, SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Flash_Write(SKYCAS_U32 writeAddr, SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
����:
	дFlash����(������ַ��)
����:
	[IN]	
		SKYCAS_U32 writeAddr				Flash��ַ��
		SKYCAS_U32* const pWriteLen		����ָ�롣�ñ����洢��Ҫд��������ֽڳ��ȡ�
		const SKYCAS_U8* const pWriteBuf	����ָ�롣������洢�ȴ�д������ݡ�
	[OUT]	
		SKYCAS_U32* const pWriteLen		����ָ�롣�ñ����洢ʵ��д��������ֽڳ��ȡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Flash_Write(SKYCAS_U32 writeAddr, SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
/******************************************************************************************
������:	
    SKYCAS_S32 SKYCAS_File_Init(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 u32FileSize);
����:
	��ʼ��CAS��Ҫ���ļ�(�ļ���)��
	�ж��ļ�������������ڣ������κ����飻
	�������ڣ��������ļ�����ʼ��Ϊ����ָ����С��������Ϊȫ0xFF��
����:
	[IN]	
		const SKYCAS_CHAR* const pFileName	�ַ�����������Ҫ��ʼ�����ļ�·����
		SKYCAS_U32 u32FileSize		�ļ���С��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Init(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 u32FileSize);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_File_Read(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 readOffsetAddr, 
										SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
����:
	��Flash����(�ļ���)
����:
	[IN]	
		const SKYCAS_CHAR* const pFileName	�ַ�����������Ҫ��ȡ���ݵ��ļ�·����
		SKYCAS_U32 readOffsetAddr		�ļ�ƫ�Ƶ�ַ��
		SKYCAS_U32* const pReadLen		����ָ�롣�ñ����洢��Ҫ��ȡ�������ֽڳ��ȡ�
		SKYCAS_U8* const pReadBuf		����ָ�롣������洢������ȡ�������ݡ�
	[OUT]	
		SKYCAS_U32* const pReadLen		����ָ�롣�ñ����洢ʵ�ʶ�ȡ���������ֽڳ��ȡ�
            SKYCAS_U8* const pReadBuf       ����ָ�롣������洢������ȡ�������ݡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Read(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 readOffsetAddr, 
									SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_File_Write(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 writeOffsetAddr, 
										SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
����:
	дFlash����(�ļ���)
����:
	[IN]	
		const SKYCAS_CHAR* const pFileName		�ַ�����������Ҫд�����ݵ��ļ�·����
		SKYCAS_U32 writeOffsetAddr			�ļ�ƫ�Ƶ�ַ��
		SKYCAS_U32* const pWriteLen		����ָ�롣�ñ����洢��Ҫд��������ֽڳ��ȡ�
		const SKYCAS_U8* const pWriteBuf	����ָ�롣������洢�ȴ�д������ݡ�
	[OUT]	
		SKYCAS_U32* const pWriteLen		����ָ�롣�ñ����洢ʵ��д��������ֽڳ��ȡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Write(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 writeOffsetAddr, 
									SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);


/**************************************************
	OS/Timer
**************************************************/
/******************************************************************************************
	Task�������塣
******************************************************************************************/
typedef SKYCAS_VOID* (*SKYCAS_TASK_FUNC)(SKYCAS_VOID* pTaskPara);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Task_Creat(SKYCAS_U32* const pU32TaskId, const SKYCAS_CHAR* const pU8TaskName, SKYCAS_U16 u16Priority, 
											SKYCAS_U32 u32StackSize, SKYCAS_TASK_FUNC const pTaskFunc, const SKYCAS_VOID* pTaskPara);
����:
	��������/�̡߳�
����:
	[IN]	
		SKYCAS_U32* const pU32TaskId			����ָ�롣�洢Ĭ��taskID��
		const SKYCAS_CHAR* const pU8TaskName		�ַ�������������
		SKYCAS_U16 u16Priority				�������ȼ���
		SKYCAS_U32 u32StackSize				�����ջ��С��
		SKYCAS_TASK_FUNC const pTaskFunc		����ָ�롣ָ��������������
		const SKYCAS_VOID* const pTaskPara		ָ�롣����������������
	[OUT]	
		SKYCAS_U32* const pU32TaskId			����ָ�롣�洢�����ɹ���taskID��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Creat(SKYCAS_U32* const pU32TaskId, const SKYCAS_CHAR* const pU8TaskName, SKYCAS_U16 u16Priority, 
										SKYCAS_U32 u32StackSize, SKYCAS_TASK_FUNC const pTaskFunc,  const SKYCAS_VOID* const pTaskPara);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Task_Destory(SKYCAS_U32 u32TaskId)
����:
	��������/�̡߳�
����:
	[IN]	
		SKYCAS_U32 u32TaskId		����/�߳�ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Destory(SKYCAS_U32 u32TaskId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Task_Sleep(SKYCAS_U32 u32TimeofMS)
����:
	��ʱ����/�̡߳�
����:
	[IN]	
		SKYCAS_U32 u32TimeofMS	��ʱʱ�䡣��λ���롣
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Sleep(SKYCAS_U32 u32TimeofMS);

/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Semp_Creat(SKYCAS_U32* const pU32SempId, SKYCAS_U8 u8InitValue);
����:
	�����ź���
����:
	[IN]	
		SKYCAS_U32* const pU32SempId	����ָ�롣�洢Ĭ��SempID��
		SKYCAS_U8 u8InitValue			�ź�����ʼ��ֵ����ֵΪ0ʱ�����ź����ֲ����á�
	[OUT]	
		SKYCAS_U32* const pU32SempId	����ָ�롣�洢�����ɹ���SempID��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Creat(SKYCAS_U32* const pU32SempId, SKYCAS_U8 u8InitValue);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Semp_Destory(SKYCAS_U32 u32SempId)
����:
	�����ź���
����:
	[IN]	
		SKYCAS_U32 u32SempId		�ź���ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Destory(SKYCAS_U32 u32SempId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Semp_Wait(SKYCAS_U32 u32SempId, SKYCAS_U32 u32WaitTime)
����:
	�ȴ��ź���
����:
	[IN]	
		SKYCAS_U32 u32SempId		�ź���ID
		SKYCAS_U32 u32WaitTime	�ȴ���ʱʱ�䡣��λ���롣����0xFFFFFFFF����Ϊһֱ�ȴ���
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Wait(SKYCAS_U32 u32SempId, SKYCAS_U32 u32WaitTime);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Semp_Release(SKYCAS_U32 u32SempId)
����:
	�ͷ��ź���
����:
	[IN]	
		SKYCAS_U32 u32SempId		�ź���ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Release(SKYCAS_U32 u32SempId);

/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mutex_Creat(SKYCAS_U32* const pU32MutexId);
����:
	����������
����:
	[IN]	
		SKYCAS_U32* const pU32MutexId		����ָ�롣�洢Ĭ��MutexId��
	[OUT]	
		SKYCAS_U32* const pU32MutexId		����ָ�롣�洢�����ɹ���MutexId��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_Creat(SKYCAS_U32* const pU32MutexId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
����:
	���ٻ�����
����:
	[IN]	
		SKYCAS_U32 u32MutexId		������ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mutex_lock(SKYCAS_U32 u32MutexId);
����:
	��ȡ������
����:
	[IN]	
		SKYCAS_U32 u32MutexId		������ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_lock(SKYCAS_U32 u32MutexId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
����:
	�ͷŻ�����
����:
	[IN]	
		SKYCAS_U32 u32MutexId		������ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_unlock(SKYCAS_U32 u32MutexId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
����:
	���Ի�ȡ������
����:
	[IN]	
		SKYCAS_U32 u32MutexId		������ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_trylock(SKYCAS_U32 u32MutexId);

/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Queue_Creat(SKYCAS_U32* const pU32QueueId, SKYCAS_U32 u32MsgSize, SKYCAS_U32 u32QueueLen)
����:
	������Ϣ���С�
����:
	[IN]	
		SKYCAS_U32* const pU32QueueId	����ָ�롣�洢������QueueId��
		SKYCAS_U32 u32MsgSize		��Ϣ��С��
		SKYCAS_U32 u32QueueLen	      ��Ϣ���г��ȡ�
	[OUT]	
		SKYCAS_U32* const pU32QueueId	����ָ�롣�洢�����ɹ���QueueId��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_Creat(SKYCAS_U32* const pU32QueueId, SKYCAS_U32 u32MsgSize, SKYCAS_U32 u32QueueLen);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Queue_Destory(SKYCAS_U32 u32QueueId)
����:
	������Ϣ����
����:
	[IN]	
		SKYCAS_U32 u32QueueId		��Ϣ����ID
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_Destory(SKYCAS_U32 u32QueueId);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Queue_SendMsg(SKYCAS_U32 u32QueueId, SKYCAS_U8* pU8SendMsg, 
								SKYCAS_U32 u32SendMsgSize, SKYCAS_U32 u32Timeout)
����:
	������Ϣ
����:
	[IN]	
		SKYCAS_U32 u32QueueId				��Ϣ����ID
		const SKYCAS_U8* const pU8SendMsg	ָ�롣ָ������͵���Ϣ��
		SKYCAS_U32 u32SendMsgSize			�ȴ����͵���Ϣ��С��
		SKYCAS_U32 u32Timeout				��ʱʱ�䡣0xFFFFFFFF����һֱ�ȴ���
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_SendMsg(SKYCAS_U32 u32QueueId, const SKYCAS_U8* const pU8SendMsg, 
											SKYCAS_U32 u32SendMsgSize, SKYCAS_U32 u32Timeout);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Queue_RecvMsg(SKYCAS_U32 u32QueueId, SKYCAS_U8* const pU8RecvMsg, 
												SKYCAS_U32* const u32RecvMsgSize, SKYCAS_U32 u32Timeout);
����:
	������Ϣ
����:
	[IN]	
		SKYCAS_U32 u32QueueId				��Ϣ����ID
		SKYCAS_U8* const pU8RecvMsg	      ָ�롣ָ����յ�����Ϣ��
		SKYCAS_U32* const u32RecvMsgSize	�������յ�����Ϣ��С�����п��ܲ�ֹ����һ����Ϣ��
		                                        �ʴ˴�С��ʾ��Ҫ���յĶ�������Ϣ��������Ϣ��С�Ļ�.
		SKYCAS_U32 u32Timeout				��ʱʱ�䡣0xFFFFFFFF����һֱ�ȴ���
	[OUT]	
		SKYCAS_U32* const u32RecvMsgSize	 ʵ�ʽ��յ�����Ϣ��С��
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_RecvMsg(SKYCAS_U32 u32QueueId,  SKYCAS_U8* const pU8RecvMsg, 
											SKYCAS_U32* const u32RecvMsgSize, SKYCAS_U32 u32Timeout);

/******************************************************************************************
������:	
    SKYCAS_VOID* SKYCAS_Mem_Alloc(SKYCAS_U32 u32AllocSize);
����:
	�����ڴ�
����:
	[IN]	
		SKYCAS_U32 u32AllocSize			�������С��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ����ط���ĵ�ַ��ʧ�ܷ���SKYCAS_NULL��
******************************************************************************************/
SKYCAS_VOID* SKYCAS_Mem_Alloc(SKYCAS_U32 u32AllocSize);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mem_Free(const SKYCAS_VOID* const pAllocBufAddr);
����:
	�ͷ��ڴ�
����:
	[IN]	
		const SKYCAS_VOID* const pAllocBufAddr	ָ�롣ָ����ͷŵ��ڴ��ַ��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_Free(const SKYCAS_VOID* const pAllocBufAddr);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mem_set(SKYCAS_VOID* const pBufAddr, SKYCAS_U8 value, SKYCAS_U32 u32MemSetSize)
����:
	set�ڴ�
����:
	[IN]	
		SKYCAS_VOID* const pBufAddr	ָ�롣ָ���set���ڴ��ַ��
		SKYCAS_U8 value			��Ҫset��ֵ��
		SKYCAS_U32 u32MemSetSize	��Ҫset���ڴ��С��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_set(SKYCAS_VOID* const pBufAddr, SKYCAS_U8 value, SKYCAS_U32 u32MemSetSize);
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Mem_copy(SKYCAS_VOID* const pDesBufAddr, const SKYCAS_VOID* const pSrcBufAddr);
����:
	�ڴ濽��
����:
	[IN]	
		SKYCAS_VOID* const pDesBufAddr		ָ�롣ָ��Ŀ���ڴ��ַ��
		const SKYCAS_VOID* const pSrcBufAddr		ָ�롣ָ��Դ�ڴ��ַ��
		SKYCAS_U32 u32MemCpySize		������С��
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_copy(SKYCAS_VOID* const pDesBufAddr, const SKYCAS_VOID* const pSrcBufAddr, SKYCAS_U32 u32MemCpySize);

/******************************************************************************************
������:	
	SKYCAS_U32 SKYCAS_Time_Get_Times(SKYCAS_VOID)
����:
	��ȡ��ǰϵͳ��ʱ��ֵ���Ժ���Ϊ��λ��
	��Linuxϵͳ�뽫ϵͳʱ�ӽ���ֵת��Ϊ����ֵ���ء�
����:
	[IN]	
		�ޡ�
	[OUT]	
		�ޡ�
	[RET]		
		����ϵͳʱ��ֵ��
******************************************************************************************/
SKYCAS_LONG SKYCAS_Time_Get_Times(SKYCAS_VOID);


/**************************************************
	Debugger
**************************************************/
typedef enum skycas_debugger_status_e
{
	SKYCAS_DEBUGGER_STATUS_ON = 0,
	SKYCAS_DEBUGGER_STATUS_OFF,
	SKYCAS_DEBUGGER_STATUS_INVALID
}SKYCAS_DEBUGGER_STATUS_E;
/******************************************************************************************
������:	
	SKYCAS_S32 SKYCAS_Debugger_OnOff(SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff)
����:
	��ӡ����
����:
	[IN]	
		SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff	��ӡ����ö�١�
	[OUT]	
		�ޡ�
	[RET]		
		�ɹ�����SKYCAS_RET_SUCCESS��ʧ�ܷ���SKYCAS_RET_FAILURE��
******************************************************************************************/
SKYCAS_S32 SKYCAS_Debugger_OnOff(SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff);
/******************************************************************************************
������:	
	SKYCAS_VOID SKYCAS_Debugger_Printf(const SKYCAS_CHAR* const fmt, ...)
����:
	��ӡ��Ϣ
����:
	[IN]	
		const SKYCAS_CHAR* const fmt	��ӡ��Ϣ�����ɱ������
	[OUT]	
		�ޡ�
	[RET]		
		�ޡ�
******************************************************************************************/
SKYCAS_VOID SKYCAS_Debugger_Printf(const SKYCAS_CHAR* const fmt, ...);


#ifdef __cplusplus
 #if __cplusplus
	}
 #endif     /* __cplusplus */
#endif  /* __cplusplus */
	
#endif /* __SKYCAS_DRIVER_H__ */

