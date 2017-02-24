#ifndef _DVNCAS_H_
#define _DVNCAS_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;

typedef enum
{
    DVN_RTOS_OK,
    DVN_RTOS_FAIL,
} DVN_RTOS_RESULT;

typedef enum
{
    DVN_RTOS_QUEUE_BLOCKING,
    DVN_RTOS_QUEUE_NON_BLOCKING
} DVN_RTOS_QUEUE_MODE;

typedef	unsigned long	DVN_RTOS_SEMAPHORE;
typedef	unsigned long	DVN_RTOS_MSG_QUEUE;

typedef enum {
    SC_COMPLETE_OK,
    SC_NO_CARD_ERROR,
    SC_MUTE_ERROR,
    SC_ERROR,
    SC_TIMEOUT
}SC_RESULT_CODE;

typedef enum {
    SC_CARD_IN,
    SC_CARD_OUT
}SC_EVENT_TYPE;




INT32 cak_Init(void);
void cak_SystemRun(void);
void cak_TimerTrigger(void);
INT32 cak_StopKernel(void);
INT32 cak_RestartKernel(void);
void cak_SetConfiguration(INT32 Mode);
INT32 cak_GetSTBAttribute(UINT8 *p);
INT32 cak_GetSmartCardID(UINT8 *p);
INT32 cak_GetAccountNo(UINT8 *p);
INT32 cak_ReadSysDescriptor (UINT8 index,UINT8 *p);
INT16 cak_GetCASID(void);
void cak_GetSmartCardVersion(UINT8 *p);
INT32 cak_GetUserType(void);
INT32 cak_PortingVendor(UINT8 *p);
INT32 cak_BuildDate(UINT8 *date);
INT32 cak_SmartCardPaired(void);
INT32 cak_VersionReport (UINT8 *p);
INT32 cak_GetTwoCASID (void);
INT32 cak_GetSizeofSubscriptionRecord(INT32 Select);
INT32 cak_GetSubscriptionRecord(INT8 Select , UINT8 *p);
INT32 cak_GetTokenValue(float *p);
INT32 cak_ConfirmChangeToken(UINT8 Index);
INT32 cak_CancelChangeToken(UINT8 Index);
INT32 cak_DeductToken(float Price, UINT8 Type, UINT32 ID);
INT32 cak_GetSizeofTransactionRecord(void);
INT32 cak_GetTransactionRecord(INT16 no_of_record, UINT8 *p);
INT32 cak_CheckSubscription(INT32 Product_ID);
INT32 cak_CheckProductID (INT32 Product_ID);
INT32 cak_StartGettingECM(INT16 PID, INT16 ECM_PID, INT16 Reserved);
INT32 cak_StopGettingECM(INT16 ECM_PID);
INT32 cak_PerformPairing(UINT8 *data);
INT32 cak_GetSmartCardUserData(UINT8 bankno, UINT8 *data);
INT32 cak_SetSmartCardUserData(UINT8 bankno, UINT8 *data);
INT32 cak_CryptoEngine(INT32 mode, UINT8 *sdata, INT32 slen,UINT8 *ddata, INT32 *dlen);
INT32 cak_SetPinCode(UINT8 *old_pin, UINT8 *new_pin);
INT32 cak_GetRating(UINT8 *rating);
INT32 cak_SetRating(UINT8 *pin, UINT8 rating);
typedef void (*Func)(int code);
void cak_InstallMSCallBack(Func MsgFunc);
INT32 cak_GetMSType(int *Type);
INT32 cak_GetMasterSmartCardID(unsigned char *ID);
INT32 cak_GetMasterSmartCardExpiryDate(unsigned char * DateTime);
INT32 cak_GetSlaveSmartCardExpiryDate(unsigned char * DateTime);
typedef void (*IPPV_CBFUNC_STATUS)(UINT8 *Data);
void cak_InstallCallBackFunc(INT32 Type, void *CBFunc);
INT32 cak_AddIPPVSubscription(INT32 productID, float price,UINT8 *startTime, UINT8 *endTime, UINT8 *transTime,UINT8 *Pin);
INT32 cak_GetSizeofIPPVSubscriptionRecord(void);
INT32 cak_GetIPPVSubscriptionRecord(UINT8* sub_record);
INT32 cak_SetLibActivationPassword (UINT8 *password);

/*********************** 以上为CAK提供给STB使用/以下为STB实现给CAK使用 ********************************/

DVN_RTOS_RESULT DVN_RTOS_Initialize(void);
DVN_RTOS_RESULT DVN_RTOS_CreateSemaphore(UINT8 *semaphoreName,UINT32 tokenCount,DVN_RTOS_SEMAPHORE *semaphoreID);
DVN_RTOS_RESULT DVN_RTOS_DeleteSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore);
DVN_RTOS_RESULT DVN_RTOS_AcquireSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore,UINT32 timeout);
DVN_RTOS_RESULT DVN_RTOS_ReleaseSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore);
DVN_RTOS_RESULT DVN_RTOS_CreateQueue(UINT8 *queueName,UINT16 maxElements,UINT16 elementSize,DVN_RTOS_MSG_QUEUE *queueID);
DVN_RTOS_RESULT DVN_RTOS_DeleteQueue(DVN_RTOS_MSG_QUEUE queueID);
DVN_RTOS_RESULT DVN_RTOS_EmptyQueue(DVN_RTOS_MSG_QUEUE whichQueue);
DVN_RTOS_RESULT DVN_RTOS_SendQueue(DVN_RTOS_MSG_QUEUE queueID,UINT8 *messageData);
DVN_RTOS_RESULT DVN_RTOS_ReceiveQueue (DVN_RTOS_MSG_QUEUE queueID,DVN_RTOS_QUEUE_MODE mode,UINT32 timeout,UINT8 *receiveData);
DVN_RTOS_RESULT DVN_RTOS_Sleep(UINT32 delay );
INT32 CAKCALL_SysInit( void );
SC_RESULT_CODE SC_initialization ( UINT8 *ATR,UINT32 Clock_frequency );
typedef void ( *TypeCardEventCB )( SC_EVENT_TYPE EventFlag );
SC_RESULT_CODE RegisterEventNotification(TypeCardEventCB EventFunc );
SC_RESULT_CODE SC_send(UINT8*pBuf, UINT16 Len,UINT32 Timeout );
SC_RESULT_CODE SC_receive( UINT8 *pBuf,UINT16 *SC_bytecount,UINT16 Len, UINT32 Timeout );
INT32 CAKCALL_StartSectionFilter (UINT8 FilterID,UINT16 PID,UINT16 TableID,UINT8 Period,UINT16 *SectByte,DVN_RTOS_MSG_QUEUE QueueID);
INT32 CAKCALL_StopSectionFilter( UINT8 FilterID );
INT32 CAKCALL_StartCAT(DVN_RTOS_MSG_QUEUE queueID );
INT32 CAKCALL_GetTotalDescrambler(void );
INT32 CAKCALL_AllocDescrambler( UINT16 PID,INT32 descrambler_no );
INT32 CAKCALL_FreeDescrambler( UINT16 PID,INT32 descrambler_no );
INT32 CAKCALL_InstallScramblingKey( UINT8 *Key,INT32 Index );
INT32 CAKCALL_ReadEEPROM (UINT32 Address, UINT32 Size,UINT8 *Data );
INT32 CAKCALL_WriteEEPROM( UINT32 Address, UINT32 Size,UINT8 *Data );
void* CAKCALL_MemAlloc( UINT32 Size );
void CAKCALL_FreeMemory( void * ptr );
INT32 CAKCALL_Memcpy ( void *ptr1, void *ptr2, INT32 Len );
INT32 CAKCALL_STBOn( void );
INT32 CAKCALL_STBOff( void );
INT32 CAKCALL_TokenChange( float p, INT32 Index );
INT32 CAKCALL_UserDefinedAction( UINT8 *DataPtr,INT32 DataLen );
void CAKCALL_UploadTransactionRecord( void );
void CAKCALL_ReportStatus( INT32 ReportCode );
INT32 CAKCALL_GetSystemDateAndTime(UINT8 *DateTime );
void CAK_DEBUG_PrintStr( INT8 *string );
void CAK_DEBUG_PrintHex( UINT32 data, INT32 size );
void CAK_DEBUG_PrintChar( INT8 data );


INT32 CAKCALL_OSUpdate(void); 
void CAKCALL_GetActivationSTBID(UINT8 *ID);











#ifdef  __cplusplus
}
#endif
#endif

