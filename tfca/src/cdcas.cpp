#include "skycas_datatype.h"
#include "skyworth_tfca.h"
#include "skyworth_ca_api.h"
#include "cdcas.h"
#include "skycas_driver.h"


#define TRACE_FUNC_ENTRY	(1)	/*函数入口trace*/
#define TRACE_FUNC_EXIT	(0)	/*函数出口trace*/

#define CDCAS_FUNC_ENTRY(onOff)\
do {\
	if (onOff)\
		SKYCAS_Debugger_Printf("[CDCAS] %s-%d enter!\n", __FUNCTION__,__LINE__);\
}while(0)

#define SKYCAS_FUNC_EXIT(onOff)\
do {\
	if (onOff)\
		SKYCAS_Debugger_Printf("[CDCAS] %s-%d exit!\n", __FUNCTION__,__LINE__);\
}while(0)

CDCA_BOOL CDSTBCA_RegisterTask( const char* szName, CDCA_U8 byPriority,  void* pTaskFun,  void* pParam, CDCA_U16 wStackSize)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    SKYCAS_U32 u32TmpTaskId = 0;
    
    s32ret = SKYCAS_Task_Creat(&u32TmpTaskId,(const SKYCAS_PCHAR)szName,byPriority,wStackSize,(SKYCAS_TASK_FUNC)pTaskFun,pParam);

    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Creat Task fail! name:%s priority:%d stackSize:%d\n", szName,byPriority,wStackSize);
        return(CDCA_FALSE);
    }
    
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_TASK, u32TmpTaskId);
#endif

    SKYCAS_Debugger_Printf("Creat Task success! name:%s priority:%d stackSize:%d\n", szName,byPriority,wStackSize);
    return CDCA_TRUE;
}

void CDSTBCA_Sleep(CDCA_U16 wMilliSeconds)
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Task_Sleep(wMilliSeconds);
}

void CDSTBCA_SemaphoreInit( CDCA_Semaphore* pSemaphore, CDCA_BOOL bInitVal)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pSemaphore)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;

    *pSemaphore = SKYCAS_NULL;

    SKYCAS_U32 u32TmpSempId=0;
    SKYCAS_U8 u8TmpValue = 0;
    if (CDCA_TRUE == bInitVal)
    {
        u8TmpValue = 1;
    }
    else
    {
        u8TmpValue = 0;
    }
    
    s32ret = SKYCAS_Semp_Creat(&u32TmpSempId,u8TmpValue);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Creat Semp fail! u32TmpSempId:%d bInitVal:%d\n", u32TmpSempId, bInitVal);
        return;
    }
    
#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_SEMAP, u32TmpSempId);
#endif
    SKYCAS_Debugger_Printf("Creat Semp success! u32TmpSempId:%d bInitVal:%d\n", u32TmpSempId, bInitVal);
    *pSemaphore = u32TmpSempId;
    return;
}
void CDSTBCA_SemaphoreSignal( CDCA_Semaphore* pSemaphore )
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pSemaphore)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;

    SKYCAS_U32 u32TmpSempId=*pSemaphore;
    
    s32ret = SKYCAS_Semp_Release(u32TmpSempId);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Signal Semp fail! u32TmpSempId:%d\n", u32TmpSempId);
        return;
    }
    
    return;
}
void CDSTBCA_SemaphoreWait( CDCA_Semaphore* pSemaphore )
{
   //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pSemaphore)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;

    SKYCAS_U32 u32TmpSempId=*pSemaphore;
    
    s32ret = SKYCAS_Semp_Wait(u32TmpSempId,SKYCAS_U32_HANDLE_INVALID);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Wait Semp fail! u32TmpSempId:%d\n", u32TmpSempId);
        return;
    }
    
    return;
}

void* CDSTBCA_Malloc( CDCA_U32 byBufSize )
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    void *pU32TmpAddr = SKYCAS_NULL;

    if (0 == byBufSize)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_NULL;
    }
    
    pU32TmpAddr = SKYCAS_Mem_Alloc(byBufSize);
    if (SKYCAS_NULL == pU32TmpAddr)
    {
        SKYCAS_Debugger_Printf("Error! Malloc memory fail! byBufSize:%d\n", byBufSize);
        return SKYCAS_NULL;
    }
    
    return (void*)pU32TmpAddr;
}
void  CDSTBCA_Free(void* pBuf)
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if (SKYCAS_NULL == pBuf)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
   
    s32ret = SKYCAS_Mem_Free(pBuf);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Free memory fail! pBuf:0x%X\n", pBuf);
        return ;
    }
    
    return ;
}
void  CDSTBCA_Memset( void* pDestBuf,CDCA_U8  c, CDCA_U32 wSize )
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if (SKYCAS_NULL == pDestBuf)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
    
    s32ret = SKYCAS_Mem_set(pDestBuf,c,wSize);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Set memory fail! pDestBuf:0x%X, len:%d\n", pDestBuf, wSize);
        return ;
    }
    
    return ;
}
void  CDSTBCA_Memcpy( void* pDestBuf, const void* pSrcBuf, CDCA_U32    wSize )
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == pDestBuf)||(SKYCAS_NULL == pSrcBuf))
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
    
    s32ret = SKYCAS_Mem_copy(pDestBuf,pSrcBuf,wSize);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Copy memory fail! pDestBuf:0x%X, pSrcBuf:0x%X, len:%d\n", pDestBuf, pSrcBuf, wSize);
        return ;
    }
    
    return ;
}

void CDSTBCA_ReadBuffer( CDCA_U8 byBlockID, CDCA_U8*  pbyData, CDCA_U32* pdwLen )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == pbyData)||(SKYCAS_NULL == pdwLen) 
            || ((CDCA_FLASH_BLOCK_A != byBlockID)&&(CDCA_FLASH_BLOCK_B != byBlockID)))
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }

    SKYCAS_U32 u32TmpLen=*pdwLen;
    SKYCAS_CHAR tmpCasFile[256]={0,};
    SKYCAS_CHAR tmpCasFileName[256]={0,};
    CSkyworth_Cdca::GetCaInstance()->SKYCAS_GetFilePath(tmpCasFile);
    sprintf(tmpCasFileName, "%scdcas%d", tmpCasFile, (SKYCAS_U8)(byBlockID));
    s32ret = SKYCAS_File_Read(tmpCasFileName,0,&u32TmpLen,pbyData);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Read [%s] data fail! BlockID:%d, len:%d, actLen:%d\n", tmpCasFileName, byBlockID, *pdwLen,u32TmpLen);
        *pdwLen = 0;
        return ;
    }

    //SKYCAS_Debugger_Printf("u32TmpLen %d\n", u32TmpLen);
    if (0 != u32TmpLen)
    {
        *pdwLen = u32TmpLen; 
    }
    return ;
}
void CDSTBCA_WriteBuffer( CDCA_U8 byBlockID, const CDCA_U8* pbyData, CDCA_U32 dwLen )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if ((SKYCAS_NULL == pbyData)
            || ((CDCA_FLASH_BLOCK_A != byBlockID)&&(CDCA_FLASH_BLOCK_B != byBlockID)))
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
    
    SKYCAS_U32 u32TmpLen= dwLen;
    SKYCAS_CHAR tmpCasFile[256]={0,};
    SKYCAS_CHAR tmpCasFileName[256]={0,};
    CSkyworth_Cdca::GetCaInstance()->SKYCAS_GetFilePath(tmpCasFile);
    sprintf((SKYCAS_CHAR*)tmpCasFileName, "%scdcas%d", (SKYCAS_CHAR*)tmpCasFile, (SKYCAS_U8)(byBlockID));

    s32ret = SKYCAS_File_Write((SKYCAS_CHAR*)tmpCasFileName,0,&u32TmpLen,pbyData);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Write [%s] data fail! BlockID:%d, len:%d, actLen:%d\n", tmpCasFileName, byBlockID, dwLen,u32TmpLen);
        return ;
    }
    
    return ;
}

void CDSTBCA_ScrSetCW(CDCA_U16 wEcmPID, const CDCA_U8* pbyOddKey, const CDCA_U8* pbyEvenKey, CDCA_U8 byKeyLen, CDCA_BOOL bTapingEnabled)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    if (SKYCAS_NULL == pbyOddKey || SKYCAS_NULL == pbyEvenKey ||(0x1FFF == wEcmPID))
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_U16 i;
#if 1
    SKYCAS_Debugger_Printf("\n +++ CDSTBCA_ScrSetCW  wEcmPID:%d +++\n", wEcmPID);
    for(i=0;i<byKeyLen;i++)
        SKYCAS_Debugger_Printf("[%02x]", *(pbyOddKey+i));
    //SKYCAS_Debugger_Printf("\n Even:");
    for(i=0;i<byKeyLen;i++)
        SKYCAS_Debugger_Printf("[%02x]", *(pbyEvenKey+i));
    SKYCAS_Debugger_Printf("\n");
#endif

    s32ret = SKYCAS_Des_SetKey(wEcmPID,pbyOddKey,pbyEvenKey);
    if (SKYCAS_RET_SUCCESS != s32ret)
    {
        SKYCAS_Debugger_Printf("Error! Set CW fail! wEcmPID:0x%X(%d)\n", wEcmPID, wEcmPID);
        return ;
    }

    return;
}

CDCA_BOOL CDSTBCA_GetPrivateData(CDCA_U8 u8ReqID, const CDCA_U8* pU8Match, const CDCA_U8* pU8Mask, CDCA_U8 u8FilterDeep, CDCA_U16 u16DataPid, CDCA_U8 byWaitSeconds)
{
    
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pU8Match || SKYCAS_NULL == pU8Mask || 0x1FFF == u16DataPid)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return(CDCA_FALSE);
    }
    
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    s32ret = CSkyworth_Cdca::GetCaInstance()->SKYCAS_SetPrivateData(u8ReqID, pU8Match, pU8Mask, u8FilterDeep, u16DataPid, byWaitSeconds);
    if (SKYCAS_RET_SUCCESS == s32ret)
    {
        return CDCA_TRUE;
    }
    return CDCA_FALSE;
}

CDCA_BOOL CDSTBCA_SCReset(void * scHandle,  CDCA_U8* pbyATR,  CDCA_U8* pbyLen)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pbyATR || SKYCAS_NULL == pbyLen)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return(CDCA_FALSE);
    }

    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U8 u8ArrTmpATR[256]={0,};
    SKYCAS_U16 u16TmpATRLen = 256;
    SKYCAS_U8 retryCount = 3;

    while (retryCount --)
    {
        s32ret= SKYCAS_RET_SUCCESS;
        s32ret |= SKYCAS_SMC_Reset(0);
        s32ret |= SKYCAS_SMC_GetATR(0,u8ArrTmpATR,&u16TmpATRLen);
        if (SKYCAS_RET_SUCCESS != s32ret)
        {
            SKYCAS_Debugger_Printf("Error! Get ATR fail!\n");
            SKYCAS_Task_Sleep(500);
        }
        else
        {
            SKYCAS_Mem_copy(pbyATR,u8ArrTmpATR,u16TmpATRLen);
            *pbyLen = u16TmpATRLen;
            return CDCA_TRUE;
        }
    }

    *pbyLen = 0;
    return CDCA_FALSE;
}

CDCA_BOOL CDSTBCA_SCPBRun(void * scHandle,  const CDCA_U8* pbyCommand, CDCA_U16 wCommandLen,  CDCA_U8* pbyReply,  CDCA_U16* pwReplyLen)
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pbyCommand || SKYCAS_NULL == pbyReply || SKYCAS_NULL == pwReplyLen)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return(CDCA_FALSE);
    }
    
    SKYCAS_S32 s32ret= SKYCAS_RET_SUCCESS;
    SKYCAS_U8 retryCount = 3;
    SKYCAS_U8 u8ArrTmpRecvData[256]={0,};
    SKYCAS_U16 u16TmpRecvLen = sizeof(u8ArrTmpRecvData);
    SKYCAS_U16 u16TmpSendLen = wCommandLen;

    while (retryCount --)
    {
        SKYCAS_Mem_set(u8ArrTmpRecvData,0,sizeof(u8ArrTmpRecvData));
        s32ret = SKYCAS_SMC_SendRecv(0,pbyCommand,&u16TmpSendLen,u8ArrTmpRecvData,&u16TmpRecvLen);
        if (SKYCAS_RET_SUCCESS != s32ret)
        {
            SKYCAS_Debugger_Printf("Error! Send&Recv SMC data fail!\n");
            SKYCAS_SMC_Reset(0);
            *pwReplyLen = 0;
            return CDCA_FALSE;
        }
        else
        {
            SKYCAS_Mem_copy(pbyReply,u8ArrTmpRecvData,u16TmpRecvLen);
            *pwReplyLen = u16TmpRecvLen;
            return CDCA_TRUE;
        }
    }
    *pwReplyLen = 0;
    return CDCA_FALSE; 
}

void CDSTBCA_EntitleChanged(void * scHandle,  CDCA_U16 wTvsID)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### 授权变化[%d] ###########\n\n",wTvsID);
    
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_ENTITLECHANGED;
    eCAEvent.u32Param1 = wTvsID;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

void CDSTBCA_DetitleReceived( CDCA_U8 bstatus )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### 反授权确认[%d] ###########\n\n",bstatus);

    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_DETITLERECEIVED;
    eCAEvent.u32Param1 = bstatus;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;

    pTFCA->mpTVService(eCAEvent);
}

void CDSTBCA_GetSTBID( CDCA_U16* pwPlatformID,  CDCA_U32* pdwUniqueID)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pwPlatformID || SKYCAS_NULL == pdwUniqueID)
        return;

    * pwPlatformID = CDCASTB_GetPlatformID();
    * pdwUniqueID = 0;
}

CDCA_U16 CDSTBCA_SCFunction( CDCA_U8* pData)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pData)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return(0);
    }
    return(0x9100);
}

void CDSTBCA_ShowOSDMessage( CDCA_U8 byStyle, const char* szMessage )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == szMessage)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
    if ((CDCA_OSD_TOP != byStyle)&&(CDCA_OSD_BOTTOM != byStyle)&&(CDCA_OSD_FULLSCREEN != byStyle)&&(CDCA_OSD_HALFSCREEN != byStyle))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_Debugger_Printf("\n\n########### New osd[%d]:%s ###########\n\n", byStyle,szMessage);
    
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_SHOWOSDMESSAGE;
    eCAEvent.u32Param1 = byStyle;
    eCAEvent.u32Param2 = (SKYCAS_U32)szMessage;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}
void CDSTBCA_HideOSDMessage(CDCA_U8 byStyle)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if ((CDCA_OSD_TOP != byStyle)&&(CDCA_OSD_BOTTOM != byStyle)&&(CDCA_OSD_FULLSCREEN != byStyle)&&(CDCA_OSD_HALFSCREEN != byStyle))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_Debugger_Printf("\n\n########### Hide osd[%d] ###########\n\n", byStyle);
    
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_HIDEOSDMESSAGE;
    eCAEvent.u32Param1 = byStyle;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

void CDSTBCA_EmailNotifyIcon(CDCA_U8 byShow, CDCA_U32 dwEmailID)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);

    if(CDCA_Email_New == byShow)
    {
        SKYCAS_Debugger_Printf("\n\n########### New email[%d] ###########\n\n",byShow);
    }
    else if(CDCA_Email_SpaceExhaust == byShow)
    {
        SKYCAS_Debugger_Printf("\n\n########### Full email[%d] ###########\n\n",byShow);
    }
    else if(CDCA_Email_IconHide == byShow)
    {
        SKYCAS_Debugger_Printf("\n\n########### Hide email[%d] ###########\n\n",byShow);
    }
    
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_EMAILNOTIFYICON;
    eCAEvent.u32Param1 = byShow;
    eCAEvent.u32Param2 = dwEmailID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

void  CDSTBCA_RequestFeeding( CDCA_BOOL bReadStatus )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS request feeding ###########\n\n");
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_REQUESTFEEDING;
    eCAEvent.u32Param1 = bReadStatus;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

#define BCD2Dec1(x)                          ((((x) >> 4) * 10) + ((x) & 0x0F))
SKYCAS_U32 CA_BCD2DEC( SKYCAS_U32 u32BCD)
{
    SKYCAS_U32 result = (BCD2Dec1(((u32BCD>>28)& 0xf))*10000000)+
                 (BCD2Dec1((u32BCD>>24)& 0xf)*1000000)+
                 (BCD2Dec1((u32BCD>>20)& 0xf)*100000)+
                 (BCD2Dec1((u32BCD>>16)& 0xf)*10000)+
                 (BCD2Dec1((u32BCD>>12)& 0xf)*1000)+
                 (BCD2Dec1((u32BCD>>8)& 0xf)*100)+
                 (BCD2Dec1((u32BCD>>4)& 0xf)*10)+
                 (BCD2Dec1(u32BCD & 0xf));

    return result;
}

void CDSTBCA_LockService( const SCDCALockService* pLockService )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS lock service ###########\n\n");

    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_dwFrequency:%ld \n ",pLockService->m_dwFrequency);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_symbol_rate:%ld \n",pLockService->m_symbol_rate);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_wPcrPid:%d \n",pLockService->m_wPcrPid);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_Modulation:%d \n",pLockService->m_Modulation);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_ComponentNum:%d \n",pLockService->m_ComponentNum);
	for(SKYCAS_U8 i=0;i<pLockService->m_ComponentNum;i++)
	{
	    SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_wCompPID %d:%d \n",i,pLockService->m_CompArr[i].m_wCompPID);
		SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_wECMPID %d:%d \n ",i,pLockService->m_CompArr[i].m_wECMPID);
		SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_CompType %d:%d \n",i,pLockService->m_CompArr[i].m_CompType);
	}
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_fec_inner:%d \n",pLockService->m_fec_outer);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_fec_inner:%d \n",pLockService->m_fec_inner);


    SCDCALockService* pTmpLockService;
    pTmpLockService = (SCDCALockService*)SKYCAS_Mem_Alloc(sizeof(SCDCALockService));
    SKYCAS_Mem_copy(pTmpLockService,pLockService,sizeof(SCDCALockService));

    pTmpLockService->m_dwFrequency = CA_BCD2DEC(pLockService->m_dwFrequency);
    pTmpLockService->m_symbol_rate = CA_BCD2DEC(pLockService->m_symbol_rate);
    
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_dwFrequency:%d \n",pTmpLockService->m_dwFrequency);
	SKYCAS_Debugger_Printf("CDSTBCA_LockService,m_symbol_rate:%d \n",pTmpLockService->m_symbol_rate);
    
    
    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_LOCKSERVICE;
    eCAEvent.u32Param1 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param2 = (SKYCAS_U32)pTmpLockService;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID; 
    
    pTFCA->mpTVService(eCAEvent);
}
void CDSTBCA_UNLockService(void)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS unlock service ###########\n\n");
    
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_UNLOCKSERVICE;
    eCAEvent.u32Param1 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID; 
    
    pTFCA->mpTVService(eCAEvent);
}

void CDSTBCA_ShowProgressStrip(void * scHandle,CDCA_U8 byProgress, CDCA_U8 byMark)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_SHOWPROGRESSSTRIP;
    eCAEvent.u32Param1 = byProgress;
    eCAEvent.u32Param2 = byMark;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}
void CDSTBCA_ShowBuyMessage(CDCA_U16 wEcmPID, CDCA_U8 byMessageType)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("wEcmPID:0x%X, byMessageType:0x%02X, TIME:%d\n",wEcmPID,byMessageType ,SKYCAS_Time_Get_Times());
    
#if  1  
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_SHOWBUYMESSAGE;
    eCAEvent.u32Param1 = wEcmPID;
    eCAEvent.u32Param2 = byMessageType;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);

#else
    switch(byMessageType)
    {
        case CDCA_MESSAGE_CANCEL_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n########### 取消当前的显示 ###########\n\n\n\n");
           break;
        case CDCA_MESSAGE_BADCARD_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n###########无法识别卡 ###########\n\n\n\n");
            break;
        case CDCA_MESSAGE_EXPICARD_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n###########智能卡过期，请更换新卡 ###########\n\n\n\n");
            break;
        case CDCA_MESSAGE_INSERTCARD_TYPE:
            {
            SKYCAS_Debugger_Printf("\n\n\n\n###########加扰节目，请插入智能卡 ###########\n\n\n\n");
            //SKYCAS_CdcaMessageShowBuyParam_S buyMsg;
            SKYCAS_EVENT_S eCAEvent;
            eCAEvent.ecmPid = wEcmPID;
            eCAEvent.messageType = byMessageType;
            eCAEvent.u32EventId = CDCA_MESSAGE_INSERTCARD_TYPE;
            CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
            if(SKYCAS_NULL == pTFCA)
            {
                SKYCAS_Debugger_Printf("\nGetCaInstance fail\n");
                break;
            }
            
            //eCAEvent.u8EventContent = (SKYCAS_U8*)&buyMsg;
            SKYCAS_Debugger_Printf("\n\n\n\n#########pTFCA->mpTVService is %p#############\n\n\n\n",pTFCA->mpTVService);
            pTFCA->mpTVService(eCAEvent);
            
            SKYCAS_Debugger_Printf("\n\n\n\n#########pTFCA->mpTVService finish#############\n\n\n\n");
            break;
            }
        case CDCA_MESSAGE_NOOPER_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n###########卡中不存在节目运营商 ###########\n\n\n\n");
            break;
        case CDCA_MESSAGE_BLACKOUT_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n########### 条件禁播###########\n\n\n\n");
            break;
        case CDCA_MESSAGE_OUTWORKTIME_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n当前时段被设定为不能观看\n\n\n\n");
            break;
        case CDCA_MESSAGE_WATCHLEVEL_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n节目级别高于设定的观看级别\n\n\n\n");
            break;
        case CDCA_MESSAGE_PAIRING_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n智能卡与本机顶盒不对应\n\n\n\n");
            break;
        case CDCA_MESSAGE_NOENTITLE_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n 没有授权\n\n\n\n");
            break;
        case CDCA_MESSAGE_DECRYPTFAIL_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n节目解密失败\n\n\n\n");
            break;
         case CDCA_MESSAGE_NOMONEY_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n卡内金额不足 \n\n\n\n");
            break;
          case CDCA_MESSAGE_ERRREGION_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n区域不正确 \n\n\n\n");
            break;
           case CDCA_MESSAGE_UPDATE_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n智能卡升级中，请不要拔卡或者关机 \n\n\n\n");
            break;
           case CDCA_MESSAGE_LOWCARDVER_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n请升级智能卡\n\n\n\n");
            break;
          case CDCA_MESSAGE_VIEWLOCK_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n请勿频繁切换频道 \n\n\n\n");
            break;
           case CDCA_MESSAGE_MAXRESTART_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n智能卡暂时休眠，请5分钟后重新开机 \n\n\n\n");
            break;
           case CDCA_MESSAGE_FREEZE_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n 智能卡已冻结，请联系运营商\n\n\n\n");
            break;
          case CDCA_MESSAGE_ERRCARD_TYPE:
            SKYCAS_Debugger_Printf("\n\n\n\n 智能卡校验失败，请联系运营商\n\n\n\n");
            break;
          default :
            break;
    }
#endif    
 }

CDCA_U16 CDSTBCA_Strlen(const char* pString )
{
    return strlen(pString);
}
void CDSTBCA_Printf(CDCA_U8 byLevel,  const char* szMesssage)
{
    //CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == szMesssage)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return;
    }
    SKYCAS_Debugger_Printf("\n\n[CDSTBCA_Printf]");
    SKYCAS_Debugger_Printf((SKYCAS_PCHAR const)szMesssage);
    SKYCAS_Debugger_Printf("\n\n");
}

void CDSTBCA_StartIppvBuyDlg(CDCA_U8 byMessageType,CDCA_U16 wEcmPid, const SCDCAIppvBuyInfo *pIppvProgram)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_Debugger_Printf("[%s,%d][%d,%d]\n",__FUNCTION__,__LINE__,byMessageType,wEcmPid);
    SKYCAS_Debugger_Printf("[%s,%d][0x%x,0x%x,0x%x]\n",__FUNCTION__,__LINE__,pIppvProgram->m_dwProductID,pIppvProgram->m_wTvsID,pIppvProgram->m_bySlotID);
    SKYCAS_IppvBuyAllInfo_S stIppvBuyAllInfo;
    stIppvBuyAllInfo.u8MessageType = byMessageType;
    stIppvBuyAllInfo.u16EcmPid = wEcmPid;
    
    SKYCAS_Mem_copy(&stIppvBuyAllInfo.stIppvbuyinfo,&pIppvProgram->m_dwProductID,sizeof(SKYCAS_IppvBuyInfo_S));

    stIppvBuyAllInfo.stIppvbuyinfo.u32ProductID = pIppvProgram->m_dwProductID;
    stIppvBuyAllInfo.stIppvbuyinfo.u16TvsID = pIppvProgram->m_wTvsID;
    stIppvBuyAllInfo.stIppvbuyinfo.u8SlotID = pIppvProgram->m_bySlotID;
    stIppvBuyAllInfo.stIppvbuyinfo.u8PriceNum = pIppvProgram->m_byPriceNum;

    SKYCAS_Mem_copy((SKYCAS_VOID *) stIppvBuyAllInfo.stIppvbuyinfo.stPrice,(SKYCAS_VOID *)pIppvProgram->m_Price,sizeof(SCDCAIPPVPrice)*CDCA_MAXNUM_PRICE);
    
    CSkyworth_Cdca* pCDCA = CSkyworth_Cdca::GetCaInstance();
    pCDCA->CDDATATODATA((SKYCAS_U16)pIppvProgram->m_wIPPVTime.m_wExpiredDate,&stIppvBuyAllInfo.stIppvbuyinfo.SKYCAS_IPPVTime.stExpiredDate);
        
    SKYCAS_Debugger_Printf("[%s,%d][0x%x,0x%x,0x%x]\n",__FUNCTION__,__LINE__,stIppvBuyAllInfo.stIppvbuyinfo.u32ProductID,stIppvBuyAllInfo.stIppvbuyinfo.u16TvsID,stIppvBuyAllInfo.stIppvbuyinfo.u8SlotID);
    
    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_STARTIPPVBUYDLG;
    eCAEvent.u32Param1 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param2 = (SKYCAS_U32)&stIppvBuyAllInfo;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

void CDSTBCA_HideIPPVDlg(CDCA_U16 wEcmPid)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_HIDEIPPVDLG;
    eCAEvent.u32Param1 = wEcmPid;
    eCAEvent.u32Param2 = SKYCAS_U32_HANDLE_INVALID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}


void CDSTBCA_ReleasePrivateDataFilter( CDCA_U8 u8ReqID, CDCA_U16 wPid)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (0x1FFF == wPid)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return ;
    }
    
    CSkyworth_Cdca::GetCaInstance()->SKYCAS_ReleasePrivateDataFilter(u8ReqID, wPid);
}


CDCA_BOOL CDSTBCA_SetPrivateDataFilter( CDCA_U8 byReqID, const CDCA_U8* pbyFilter, const CDCA_U8* pbyMask, CDCA_U8 byLen, CDCA_U16 wPid, CDCA_U8 byWaitSeconds )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    if (SKYCAS_NULL == pbyFilter || SKYCAS_NULL == pbyMask || 0x1FFF == wPid)
    {
        SKYCAS_Debugger_Printf("[%s-%s]Error! para error!\n",__FUNCTION__,__LINE__);
        return(CDCA_FALSE);
    }
    
    SKYCAS_S32 s32ret= SKYCAS_RET_FAILURE;
    s32ret = CSkyworth_Cdca::GetCaInstance()->SKYCAS_SetPrivateData(byReqID, pbyFilter, pbyMask, byLen, wPid, byWaitSeconds);
    if (SKYCAS_RET_SUCCESS == s32ret)
    {
        return CDCA_TRUE;
    }
    return CDCA_FALSE;
}

void  CDSTBCA_ActionRequest( CDCA_U16 wTVSID, CDCA_U8  byActionType )
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS action request ###########\n\n");
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_ACTIONREQUEST;
    eCAEvent.u32Param1 = wTVSID;
    eCAEvent.u32Param2 = byActionType;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}
void CDSTBCA_ShowFingerMessage(CDCA_U16 wEcmPID, CDCA_U32 dwCardID)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS finger show ###########\n\n");
    CSkyworth_Cdca* pTFCA = CSkyworth_Cdca::GetCaInstance();
    if((SKYCAS_NULL == pTFCA)||(SKYCAS_NULL == pTFCA->mpTVService))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Error! pTFCA is NULL!\n",__FUNCTION__,__LINE__);
        return;
    }

    SKYCAS_EVENT_S eCAEvent;
    eCAEvent.u32EventId = SKYCAS_CA_SHOWFINGERMESSAGE;
    eCAEvent.u32Param1 = wEcmPID;
    eCAEvent.u32Param2 = dwCardID;
    eCAEvent.u32Param3 = SKYCAS_U32_HANDLE_INVALID;
    
    pTFCA->mpTVService(eCAEvent);
}

void  CDSTBCA_ShowCurtainNotify( CDCA_U16 wEcmPID,CDCA_U16  wCurtainCode)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("\n\n########### CDCAS unlock service ###########\n\n");
}

int CDSTBCA_FS_open(const char *pathname,int  flages)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    return 1;
}
int CDSTBCA_FS_close(int fd)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    return 1;
}
size_t CDSTBCA_FS_read(int fd,void *buf,size_t count)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    return 1;
}
int CDSTBCA_FS_lseek(int fd,int offset,int whence)
{
    CDCAS_FUNC_ENTRY(TRACE_FUNC_ENTRY);
    return 1;
}

