#include <time.h>
#include "skycas_datatype.h"
#include "skyworth_ca_api.h"
#include "skycas_driver.h"
#include "dvncas.h"
#include "skyworth_dvnca.h"


#define DVNCAS_ENTRY (1)	/*函数入口trace*/
#define DVNCAS_EXIT	(0)	/*函数出口trace*/

#define DVNCAS_FUNC_ENTRY(onOff)\
do {\
	if (onOff)\
		SKYCAS_Debugger_Printf("[DVNCAS] %s-%d enter!\n", __FUNCTION__,__LINE__);\
}while(0)

#define DVNCAS_FUNC_EXIT(onOff)\
do {\
	if (onOff)\
		SKYCAS_Debugger_Printf("[DVNCAS] %s-%d exit!\n", __FUNCTION__,__LINE__);\
}while(0)


typedef enum dvncas_retval
{
    DVNCAS_RET_FAIL=-1,
    DVNCAS_RET_SUCCESS=1
}DVNCAS_RETVAL_E;

#define DVNCAS_QUEUE_NAME_LEN_MAX (128)
typedef struct
{
    SKYCAS_U32 Mesgqueue;
    SKYCAS_U32 u32MesgSize;
    SKYCAS_U32 u32MesgCount;
    SKYCAS_CHAR u8QueueName[DVNCAS_QUEUE_NAME_LEN_MAX];
} DVNCAS_QUEUE_T;


DVN_RTOS_RESULT DVN_RTOS_Initialize(void)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_CreateSemaphore(UINT8 *semaphoreName,UINT32 tokenCount,DVN_RTOS_SEMAPHORE *semaphoreID)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == semaphoreID)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVN_RTOS_FAIL;
    }
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;

    SKYCAS_U32 u32TmpSempId=0;
    SKYCAS_U8 u8TmpValue = tokenCount;
    
    s32ret = SKYCAS_Semp_Creat(&u32TmpSempId,u8TmpValue);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Creat Semp fail!name:%s u32TmpSempId:0x%08X bInitVal:%d\n", __FUNCTION__,__LINE__,semaphoreName,u32TmpSempId, tokenCount);
        return DVN_RTOS_FAIL;
    }
    
    SKYCAS_Debugger_Printf("[%s-%d]Creat Semp success!name:%s u32TmpSempId:0x%08X bInitVal:%d\n", __FUNCTION__,__LINE__,semaphoreName,u32TmpSempId, tokenCount);
    *semaphoreID = u32TmpSempId;
        
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_SEMAP, u32TmpSempId);
#endif
    
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_DeleteSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U32 u32TmpSempId= whichSemaphore;
        
    s32ret = SKYCAS_Semp_Destory(u32TmpSempId);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Delete Semp fail! u32TmpSempId:0x%08X\n", __FUNCTION__,__LINE__,u32TmpSempId);
        return DVN_RTOS_FAIL;
    }
    
    SKYCAS_Debugger_Printf("[%s-%d]Delete Semp success! u32TmpSempId:0x%08X\n", __FUNCTION__,__LINE__,u32TmpSempId);
            
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Clear(SKYCAS_RESOURCE_SEMAP, u32TmpSempId);
#endif
        
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_AcquireSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore,UINT32 timeout)
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U32 u32TmpSempId= whichSemaphore;
    //SKYCAS_Debugger_Printf("[%s-%d]whichSemaphore:0x%08X, timeout:%d\n", __FUNCTION__,__LINE__,whichSemaphore,timeout);
        
    s32ret = SKYCAS_Semp_Wait(u32TmpSempId,timeout);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Wait Semp fail! u32TmpSempId:0x%08X\n", __FUNCTION__,__LINE__,u32TmpSempId);
        return DVN_RTOS_FAIL;
    }
    
    //SKYCAS_Debugger_Printf("[%s-%d] OK!whichSemaphore:0x%08X\n", __FUNCTION__,__LINE__,whichSemaphore);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_ReleaseSemaphore(DVN_RTOS_SEMAPHORE whichSemaphore)
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U32 u32TmpSempId= whichSemaphore;
    //SKYCAS_Debugger_Printf("[%s-%d]whichSemaphore:0x%08X\n", __FUNCTION__,__LINE__,whichSemaphore);
    
    s32ret = SKYCAS_Semp_Release(u32TmpSempId);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Signal Semp fail! u32TmpSempId:0x%08X\n", __FUNCTION__,__LINE__,u32TmpSempId);
        return DVN_RTOS_FAIL;
    }
    
    //SKYCAS_Debugger_Printf("[%s-%d] OK!whichSemaphore:0x%08X\n", __FUNCTION__,__LINE__,whichSemaphore);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_CreateQueue(UINT8 *queueName,UINT16 maxElements,UINT16 elementSize,DVN_RTOS_MSG_QUEUE *queueID)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U32 u32TmpQueueId= SKYCAS_U32_HANDLE_INVALID;

    DVNCAS_QUEUE_T* pTmpDvnQueue = SKYCAS_NULL;
    pTmpDvnQueue = (DVNCAS_QUEUE_T*)SKYCAS_Mem_Alloc(sizeof(DVNCAS_QUEUE_T));
    if (SKYCAS_NULL == pTmpDvnQueue)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Alloc Queue memory fail! name:%s, num:%d, oneSize:%d\n", __FUNCTION__,__LINE__,queueName, maxElements, elementSize);
        return DVN_RTOS_FAIL;
    }
    
    s32ret = SKYCAS_Queue_Creat(&u32TmpQueueId,elementSize,maxElements);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Creat Queue fail! name:%s, num:%d, oneSize:%d\n", __FUNCTION__,__LINE__,queueName, maxElements, elementSize);
        return DVN_RTOS_FAIL;
    }

    pTmpDvnQueue->Mesgqueue = u32TmpQueueId;
    pTmpDvnQueue->u32MesgCount = maxElements;
    pTmpDvnQueue->u32MesgSize = elementSize;
    SKYCAS_Mem_copy((SKYCAS_VOID* const)pTmpDvnQueue->u8QueueName, (const SKYCAS_VOID* const)queueName, (SKYCAS_U32)strlen((const SKYCAS_CHAR*)queueName));

    *queueID = (DVN_RTOS_MSG_QUEUE)pTmpDvnQueue;
    SKYCAS_Debugger_Printf("[%s-%d]Ok! Creat Queue success! QueueId:0x%08X[queueID:0x%08X], name:%s, num:%d, oneSize:%d\n",__FUNCTION__,__LINE__, u32TmpQueueId,(DVN_RTOS_MSG_QUEUE)pTmpDvnQueue, queueName, maxElements, elementSize);
                
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_QUEUE, (DVN_RTOS_MSG_QUEUE)pTmpDvnQueue);
#endif
            
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_DeleteQueue(DVN_RTOS_MSG_QUEUE queueID)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    
    DVNCAS_QUEUE_T* pTmpDvnQueue = (DVNCAS_QUEUE_T*)queueID;
    if (SKYCAS_NULL == pTmpDvnQueue)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Get QueueId fail! queueID:0x%08X\n", __FUNCTION__,__LINE__,queueID);
        return DVN_RTOS_FAIL;
    }

    s32ret = SKYCAS_Queue_Destory(pTmpDvnQueue->Mesgqueue);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Delete Queue fail! QueueId:0x%08X\n",__FUNCTION__,__LINE__, pTmpDvnQueue->Mesgqueue);
        return DVN_RTOS_FAIL;
    }

    SKYCAS_Debugger_Printf("[%s-%d]Ok! Delete Queue success! QueueId:0x%08X\n", __FUNCTION__,__LINE__, pTmpDvnQueue->Mesgqueue);

    SKYCAS_Mem_Free(pTmpDvnQueue);
    pTmpDvnQueue = SKYCAS_NULL;
                    
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Clear(SKYCAS_RESOURCE_QUEUE, (DVN_RTOS_MSG_QUEUE)pTmpDvnQueue);
#endif
                
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_EmptyQueue(DVN_RTOS_MSG_QUEUE whichQueue)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_U8 tmpMsg[4096];
    SKYCAS_S32 s32ret = 0;

    DVNCAS_QUEUE_T* pTmpDvnQueue = (DVNCAS_QUEUE_T*)whichQueue;
    if (SKYCAS_NULL == pTmpDvnQueue)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Get QueueId fail! queueID:0x%08X\n", __FUNCTION__,__LINE__,whichQueue);
        return DVN_RTOS_FAIL;
    }

    SKYCAS_U32 tmpRecvSize = pTmpDvnQueue->u32MesgSize;
    while(SKYCAS_RET_SUCCESS == s32ret)
    {
        s32ret = SKYCAS_Queue_RecvMsg(pTmpDvnQueue->Mesgqueue,tmpMsg,&tmpRecvSize,0);
    }
    SKYCAS_Debugger_Printf("[%s-%d]OK!queueID:0x%08X\n", __FUNCTION__,__LINE__,pTmpDvnQueue);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_SendQueue(DVN_RTOS_MSG_QUEUE queueID,UINT8 *messageData)
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;

    DVNCAS_QUEUE_T* pTmpDvnQueue = (DVNCAS_QUEUE_T*)queueID;
    if (SKYCAS_NULL == pTmpDvnQueue)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Get QueueId fail! queueID:0x%08X\n", __FUNCTION__,__LINE__,queueID);
        return DVN_RTOS_FAIL;
    }
    //SKYCAS_Debugger_Printf("[%s-%d]queueID:0x%08X\n", __FUNCTION__,__LINE__,pTmpDvnQueue);

    s32ret = SKYCAS_Queue_SendMsg(pTmpDvnQueue->Mesgqueue,messageData,pTmpDvnQueue->u32MesgSize,0);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        //SKYCAS_Debugger_Printf("[%s-%d]Error! Send Queue fail! QueueId:0x%08X, name:%s\n", __FUNCTION__,__LINE__,pTmpDvnQueue->Mesgqueue, pTmpDvnQueue->u8QueueName);
        return DVN_RTOS_FAIL;
    }

    //SKYCAS_Debugger_Printf("[%s-%d]OK!queueID:0x%08X\n", __FUNCTION__,__LINE__,pTmpDvnQueue);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_ReceiveQueue (DVN_RTOS_MSG_QUEUE queueID,DVN_RTOS_QUEUE_MODE mode,UINT32 timeout,UINT8 *receiveData)
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    
    DVNCAS_QUEUE_T* pTmpDvnQueue = (DVNCAS_QUEUE_T*)queueID;
    if (SKYCAS_NULL == pTmpDvnQueue)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Get QueueId fail! queueID:0x%08X\n", __FUNCTION__,__LINE__,queueID);
        return DVN_RTOS_FAIL;
    }
    //SKYCAS_Debugger_Printf("[%s-%d]queueID:0x%08X, timeout:%d, mode:%d\n", __FUNCTION__,__LINE__,pTmpDvnQueue,timeout,mode);
    
    SKYCAS_U32 u32TimeOut = 0;
    switch(mode)
    {
        case DVN_RTOS_QUEUE_NON_BLOCKING:
            u32TimeOut = timeout;
            break;
        case DVN_RTOS_QUEUE_BLOCKING:
            u32TimeOut = SKYCAS_U32_HANDLE_INVALID;
            break;
        default:
            break;
    }

    SKYCAS_U32 tmpRecvSize = pTmpDvnQueue->u32MesgSize;
    s32ret = SKYCAS_Queue_RecvMsg(pTmpDvnQueue->Mesgqueue,receiveData,&tmpRecvSize,u32TimeOut);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        //SKYCAS_Debugger_Printf("[%s-%d]Error! Recv Queue fail! QueueId:0x%08X, name:%s\n", __FUNCTION__,__LINE__,pTmpDvnQueue->Mesgqueue,pTmpDvnQueue->u8QueueName);
        return DVN_RTOS_FAIL;
    }

    //SKYCAS_Debugger_Printf("[%s-%d]OK!queueID:0x%08X, timeout:%d\n", __FUNCTION__,__LINE__,pTmpDvnQueue,timeout);
    return DVN_RTOS_OK;
}
DVN_RTOS_RESULT DVN_RTOS_Sleep(UINT32 delay )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_Task_Sleep(delay);
    return DVN_RTOS_OK;
}
INT32 CAKCALL_SysInit( void )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}

SC_RESULT_CODE SC_initialization ( UINT8 *ATR,UINT32 Clock_frequency )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == ATR)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SC_ERROR;
    }

    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U8 u8ArrTmpATR[256]={0,};
    SKYCAS_U16 u16TmpATRLen = 256;
    SKYCAS_U8 retryCount = 3;

    SKYCAS_Debugger_Printf("[%s-%d]Clock_frequency:%d\n",__FUNCTION__,__LINE__,Clock_frequency);

    while (retryCount --)
    {
        s32ret= SKYCAS_RET_SUCCESS;
        s32ret |= SKYCAS_SMC_Reset(0);
        s32ret |= SKYCAS_SMC_GetATR(0,u8ArrTmpATR,&u16TmpATRLen);
        if (SKYCAS_RET_SUCCESS != s32ret)
        {
            SKYCAS_Debugger_Printf("[%s-%d]Error! Get ATR fail!\n",__FUNCTION__,__LINE__);
            SKYCAS_Task_Sleep(500);
        }
        else
        {
            SKYCAS_Mem_copy(ATR,u8ArrTmpATR,u16TmpATRLen);
            return SC_COMPLETE_OK;
        }
    }

    return SC_NO_CARD_ERROR;
}

static TypeCardEventCB s_pFnSmc = SKYCAS_NULL;

SKYCAS_S32 dvncas_smc_nofity(SC_EVENT_TYPE EventFlag)
{
    if (SKYCAS_NULL == s_pFnSmc)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    if (SC_CARD_IN == EventFlag)
    {
        s_pFnSmc(SC_CARD_IN);
    }
    else
    {
        s_pFnSmc(SC_CARD_OUT);
    }
    return SKYCAS_RET_SUCCESS;
}

SC_RESULT_CODE RegisterEventNotification(TypeCardEventCB EventFunc )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == EventFunc)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SC_ERROR;
    }

    s_pFnSmc = EventFunc;
    return SC_COMPLETE_OK;
}

SC_RESULT_CODE SC_send(UINT8*pBuf, UINT16 Len,UINT32 Timeout )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == pBuf)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SC_ERROR;
    }
    
    SKYCAS_U8 retryCount = 3;
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    SKYCAS_U16 u16TmpSendLen = Len;
    SKYCAS_U16 u16TmpRecvLen = 0;

    while (retryCount --)
    {
        //SKYCAS_Mem_set(u8ArrTmpRecvData,0,sizeof(u8ArrTmpRecvData));
        s32ret = SKYCAS_SMC_SendRecv(0,pBuf,&u16TmpSendLen,SKYCAS_NULL,&u16TmpRecvLen);
        if (SKYCAS_RET_SUCCESS != s32ret)
        {
            SKYCAS_Debugger_Printf("[%s-%d]Error! Send&Recv SMC data fail!\n",__FUNCTION__,__LINE__);
        }
        else
        {
            return SC_COMPLETE_OK;
        }
    }
    return SC_ERROR;
}
SC_RESULT_CODE SC_receive( UINT8 *pBuf,UINT16 *SC_bytecount,UINT16 Len, UINT32 Timeout )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == pBuf)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SC_ERROR;
    }
    
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    SKYCAS_U8 retryCount = 3;
    SKYCAS_U8 u8ArrTmpRecvData[256]={0,};
    SKYCAS_U16 u16TmpRecvLen = Len;
    SKYCAS_U16 u16TmpSendLen = 0;

    while (retryCount --)
    {
        SKYCAS_Mem_set(u8ArrTmpRecvData,0,sizeof(u8ArrTmpRecvData));
        s32ret = SKYCAS_SMC_SendRecv(0,SKYCAS_NULL,&u16TmpSendLen,u8ArrTmpRecvData,&u16TmpRecvLen);
        if (SKYCAS_RET_SUCCESS != s32ret)
        {
            SKYCAS_Debugger_Printf("[%s-%d]Error! Send&Recv SMC data fail!\n",__FUNCTION__,__LINE__);
        }
        else
        {
            SKYCAS_Mem_copy(pBuf,u8ArrTmpRecvData,u16TmpRecvLen);
            *SC_bytecount = u16TmpRecvLen;
            return SC_COMPLETE_OK;
        }
    }
    return SC_ERROR;
}
INT32 CAKCALL_StartSectionFilter (UINT8 FilterID,UINT16 PID,UINT16 TableID,UINT8 Period,UINT16 *SectByte,DVN_RTOS_MSG_QUEUE QueueID)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if ((SKYCAS_PID_INVALID == PID))
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }
    
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    s32ret = CSkyworth_Dvnca::GetCaInstance()->SKYCAS_SetPrivateData(FilterID,PID,TableID,Period,SectByte,QueueID);
    if (SKYCAS_RET_SUCCESS == s32ret)
    {
        return DVNCAS_RET_SUCCESS;
    }
    return DVNCAS_RET_FAIL;
}
INT32 CAKCALL_StopSectionFilter( UINT8 FilterID )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    CSkyworth_Dvnca::GetCaInstance()->SKYCAS_ReleasePrivateDataFilter(FilterID, SKYCAS_PID_INVALID);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_StartCAT(DVN_RTOS_MSG_QUEUE queueID )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_U8 tmpCat[SKYCAS_MAXLEN_CAT_MAX]={0,};

    CSkyworth_Dvnca::GetCaInstance()->SKYCAS_SetCATQueueId(queueID);
    if (SKYCAS_RET_SUCCESS == CSkyworth_Dvnca::GetCaInstance()->SKYCAS_GetCATdata(tmpCat))
    {
        DVN_RTOS_SendQueue(queueID,tmpCat);
        return DVNCAS_RET_SUCCESS;
    }
    return DVNCAS_RET_SUCCESS;//DVNCAS_RET_FAIL;
}
INT32 CAKCALL_GetTotalDescrambler(void )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return 8;
}
INT32 CAKCALL_AllocDescrambler( UINT16 PID,INT32 descrambler_no )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_FreeDescrambler( UINT16 PID,INT32 descrambler_no )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_InstallScramblingKey( UINT8 *Key,INT32 Index )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if (SKYCAS_NULL == Key)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }

    SKYCAS_U16 i=0;
    SKYCAS_U16 u16TmpEcmPid = (Index>>16)&0xFFFF;
    SKYCAS_U8 eKey[8], oKey[8];
    
    SKYCAS_Debugger_Printf("\n +++ CAKCALL_InstallScramblingKey  Index:0x%08X +++\n", Index);
    for(i=0;i<8;i++)
    {
        eKey[i] = *(Key+i);
        SKYCAS_Debugger_Printf("[%02x]", eKey[i]);
    }
    for(i=0;i<8;i++)
    {
        oKey[i] = *(Key+8+i);
        SKYCAS_Debugger_Printf("[%02x]", oKey[i]);
    }
    SKYCAS_Debugger_Printf("\n");

    s32ret = SKYCAS_Des_SetKey(u16TmpEcmPid,oKey,eKey);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Set CW fail! wEcmPID:0x%X(%d)\n", u16TmpEcmPid, u16TmpEcmPid);
        return DVNCAS_RET_FAIL;
    }

    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_ReadEEPROM (UINT32 Address, UINT32 Size,UINT8 *Data )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == Data)||(SKYCAS_NULL == Size))
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }

    SKYCAS_U32 u32TmpLen=Size;
    SKYCAS_CHAR tmpCasFile[256]={0,};
    SKYCAS_CHAR tmpCasFileName[256]={0,};
    CSkyworth_Dvnca::GetCaInstance()->SKYCAS_GetFilePath(tmpCasFile);
    sprintf(tmpCasFileName, "%sdvncas%d", tmpCasFile, (SKYCAS_U8)(0));
    s32ret = SKYCAS_File_Read(tmpCasFileName,0,&u32TmpLen,Data);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Read [%s] data fail! BlockID:%d, len:%d, Address:%d\n", __FUNCTION__,__LINE__,tmpCasFileName, u32TmpLen,Address);
        return DVNCAS_RET_FAIL;
    }

    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_WriteEEPROM( UINT32 Address, UINT32 Size,UINT8 *Data )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == Data)||(SKYCAS_NULL == Size))
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }
    
    SKYCAS_U32 u32TmpLen= Size;
    SKYCAS_CHAR tmpCasFile[256]={0,};
    SKYCAS_CHAR tmpCasFileName[256]={0,};
    CSkyworth_Dvnca::GetCaInstance()->SKYCAS_GetFilePath(tmpCasFile);
    sprintf((SKYCAS_CHAR*)tmpCasFileName, "%sdvncas%d", (SKYCAS_CHAR*)tmpCasFile, (SKYCAS_U8)(0));

    s32ret = SKYCAS_File_Write((SKYCAS_CHAR*)tmpCasFileName,0,&u32TmpLen,Data);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Write [%s] data fail! BlockID:%d, len:%d, Address:%d\n", __FUNCTION__,__LINE__,tmpCasFileName, u32TmpLen,Address);
        return DVNCAS_RET_FAIL;
    }
    
    return DVNCAS_RET_SUCCESS;
}
void* CAKCALL_MemAlloc( UINT32 Size )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    void *pU32TmpAddr = SKYCAS_NULL;

    if (0 == Size)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_NULL;
    }
    
    pU32TmpAddr = SKYCAS_Mem_Alloc(Size);
    if (SKYCAS_NULL == pU32TmpAddr)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Malloc memory fail! byBufSize:%d\n", __FUNCTION__,__LINE__,Size);
        return SKYCAS_NULL;
    }
    
    return (void*)pU32TmpAddr;
}
void CAKCALL_FreeMemory( void * ptr )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if (SKYCAS_NULL == ptr)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
   
    s32ret = SKYCAS_Mem_Free(ptr);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! Free memory fail! pBuf:0x%X\n", __FUNCTION__,__LINE__,ptr);
        return ;
    }
    
    return ;
}
INT32 CAKCALL_Memcpy ( void *ptr1, void *ptr2, INT32 Len )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == ptr1)||(SKYCAS_NULL == ptr2))
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }
    
    s32ret = SKYCAS_Mem_copy(ptr1,ptr2,Len);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Copy memory fail! pDestBuf:0x%X, pSrcBuf:0x%X, len:%d\n", ptr1, ptr2, Len);
        return DVNCAS_RET_FAIL;
    }
    
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_STBOn( void )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_STBOff( void )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_TokenChange( float p, INT32 Index )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return DVNCAS_RET_SUCCESS;
}
INT32 CAKCALL_UserDefinedAction( UINT8 *DataPtr,INT32 DataLen )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    
    CSkyworth_Dvnca* pDvnCA = CSkyworth_Dvnca::GetCaInstance();
    if((SKYCAS_NULL == pDvnCA)||(SKYCAS_NULL == pDvnCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pDvnCA is NULL!\n",__FUNCTION__,__LINE__);
        return DVNCAS_RET_FAIL;
    }
    
    SKYCAS_EVENT_S eCAEvent;
    
    if (DataPtr[0] == 'A') /* email */
    {
        SKYCAS_Debugger_Printf("[%s-%d]New Email:%s\n",__FUNCTION__,__LINE__,DataPtr+1);
        eCAEvent.u32EventId = SKYCAS_CA_EMAILNOTIFYICON;
        eCAEvent.u32Param1 = (SKYCAS_U32)(DataPtr+1);
        eCAEvent.u32Param2 = DataLen-1;
        eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;        
        pDvnCA->mpTVService(eCAEvent);
    }
    else if (DataPtr[0] == 'B') /* OSD/Broadcasting */
    {
        eCAEvent.u32EventId = SKYCAS_CA_SHOWOSDMESSAGE;
        if (DataPtr[1] == 'T')
        {
            SKYCAS_Debugger_Printf("[%s-%d]New Broadcasting:%s\n",__FUNCTION__,__LINE__,DataPtr+2);
            if (DataPtr[2] == '0')
            {
                eCAEvent.u32EventId = SKYCAS_CA_LOCKSERVICE;
                eCAEvent.u32Param1 = SKYCAS_U32_HANDLE_INVALID;
                eCAEvent.u32Param2 = 0;/* TODO: set freq/qam/sym/svcId here! */
            }
            else if (DataPtr[2] == '1')
            {
                eCAEvent.u32EventId = SKYCAS_CA_UNLOCKSERVICE;
                eCAEvent.u32Param1 = SKYCAS_U32_HANDLE_INVALID;
                eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
            }
            eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;        
        }
        else
        {
            SKYCAS_Debugger_Printf("[%s-%d]New OSD:%s\n",__FUNCTION__,__LINE__,DataPtr+4);
             if (DataPtr[1] == 'L')
             {
                if (DataPtr[2] == '1')
                {
                    eCAEvent.u32Param1 = ((0x0001<<16)|(0x0001));
                }
                else if (DataPtr[2] == '2')
                {
                    eCAEvent.u32Param1 = ((0x0001<<16)|(0x0002));
                }
            }
            else
            {
                if (DataPtr[2] == '1')
                {
                    eCAEvent.u32Param1 = ((0x0000<<16)|(0x0001));
                }
                else if (DataPtr[2] == '2')
                {
                    eCAEvent.u32Param1 = ((0x0000<<16)|(0x0002));
                }
            }
            eCAEvent.u32Param2 = DataLen-4;
            eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
        }
        pDvnCA->mpTVService(eCAEvent);
    }
    else if (DataPtr[0] == '0')
    {
        SKYCAS_Debugger_Printf("[%s-%d]Show ID!\n",__FUNCTION__,__LINE__);
        eCAEvent.u32EventId = SKYCAS_CA_SHOWFINGERMESSAGE;
        eCAEvent.u32Param1 = 0; /* TODO: set CardId here! */
        eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
        eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
        pDvnCA->mpTVService(eCAEvent);
    }
    return DVNCAS_RET_SUCCESS;
}
void CAKCALL_UploadTransactionRecord( void )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
}
void CAKCALL_ReportStatus( INT32 ReportCode )
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_Debugger_Printf("[%s-%d]ReportCode:0x%X, TIME:%d\n",__FUNCTION__,__LINE__,ReportCode ,SKYCAS_Time_Get_Times());
        
    CSkyworth_Dvnca* pDvnCA = CSkyworth_Dvnca::GetCaInstance();
    if((SKYCAS_NULL == pDvnCA)||(SKYCAS_NULL == pDvnCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_SHOWBUYMESSAGE;
    eCAEvent.u32Param1 = ReportCode;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pDvnCA->mpTVService(eCAEvent);
}
INT32 CAKCALL_GetSystemDateAndTime(UINT8 *DateTime )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == DateTime)
    {
        return DVNCAS_RET_FAIL;
    }
#if 0
    //SKYCAS_LONG u64Ms = SKYCAS_Time_Get_Times();
    struct tm *tmNow;
    time_t rawtime;
    time( &rawtime );
    tmNow = localtime(&rawtime);
    SKYCAS_Debugger_Printf("\n[CAKCALL_GetSystemDateAndTime]:%s\n", asctime(tmNow));
    SKYCAS_Debugger_Printf("\n[CAKCALL_GetSystemDateAndTime]:%d,%d,%d,%d,%d,%d\n", tmNow->tm_year,tmNow->tm_mon,tmNow->tm_mday,tmNow->tm_hour,tmNow->tm_min,tmNow->tm_sec);
    DateTime[0] = ((tmNow->tm_year)>>8)&0xFF;
    DateTime[1] = (tmNow->tm_year)&0xFF;
    DateTime[2] = tmNow->tm_mon;
    DateTime[3] = tmNow->tm_mday;
    DateTime[4] = tmNow->tm_hour;
    DateTime[5] = tmNow->tm_min;
    DateTime[6] = tmNow->tm_sec;
    
    return DVNCAS_RET_SUCCESS;
#else
    time_t  tmUtc;
    struct tm* tmNow;
    //printf("DVN:[%d] [%s] start\n",__LINE__,__FUNCTION__);

    time(&tmUtc);
    tmNow = localtime(&tmUtc);
    if (DateTime)
    {
        DateTime[0] = (INT8)((tmNow->tm_year+1900)/100);
        DateTime[1] = (INT8)((tmNow->tm_year+1900)%100);
        DateTime[2] = (INT8)(tmNow->tm_mon+1);
        DateTime[3] = (INT8)(tmNow->tm_mday);
        DateTime[4] = (INT8)(tmNow->tm_hour);
        DateTime[5] = (INT8)(tmNow->tm_min);
        DateTime[6] = (INT8)(tmNow->tm_sec);
        //SKYCAS_Debugger_Printf("\n[CAKCALL_GetSystemDateAndTime]:%s\n", asctime(tmNow));
        //SKYCAS_Debugger_Printf("\n[CAKCALL_GetSystemDateAndTime]:%d,%d,%d,%d,%d,%d\n", tmNow->tm_year,tmNow->tm_mon,tmNow->tm_mday,tmNow->tm_hour,tmNow->tm_min,tmNow->tm_sec);
        return DVNCAS_RET_SUCCESS;
    }
    return DVNCAS_RET_FAIL;
#endif
}

void CAK_DEBUG_PrintStr( INT8 *string )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    if (SKYCAS_NULL == string)
    {
        SKYCAS_Debugger_Printf("[%s-%d]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_Debugger_Printf("\n\n[DVNCAK_Printf]");
    SKYCAS_Debugger_Printf((SKYCAS_PCHAR const)string);
    SKYCAS_Debugger_Printf("\n\n");
}
void CAK_DEBUG_PrintHex( UINT32 data, INT32 size )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_CHAR string[20];
    if (2 == size)
        sprintf(string,"%02x", data);
    else
        sprintf(string,"%08x", data);
    //SKYCAS_Debugger_Printf("\n\n[DVNCAK_Printf]");
    SKYCAS_Debugger_Printf((SKYCAS_PCHAR const)string);
    //SKYCAS_Debugger_Printf("\n\n");
}
void CAK_DEBUG_PrintChar( INT8 data )
{
    //DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_CHAR string[20];
    sprintf(string,"%c", data);
    //SKYCAS_Debugger_Printf("\n\n[DVNCAK_Printf]");
    SKYCAS_Debugger_Printf((SKYCAS_PCHAR const)string);
    //SKYCAS_Debugger_Printf("\n\n");
}

void CAKCALL_GetActivationSTBID(UINT8 *ID)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    SKYCAS_Mem_copy((SKYCAS_VOID * const )ID,(const SKYCAS_VOID * const)"1234567",8);  
}

INT32 CAKCALL_OSUpdate(void)
{
    DVNCAS_FUNC_ENTRY(DVNCAS_ENTRY);
    return 1;
}
