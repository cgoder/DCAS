//<Skyworth Software>
//******************************************************************************

#include <string.h>
#include <new>
#include <errno.h>
#include <iostream>
#include <stdlib.h>

#include "dvncas.h"
#include "skycas_datatype.h"
#include "skyworth_ca_api.h"
#include "skyworth_dvnca.h"

#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
    if ( ! ( _x_ ) )                                                                    \
    {                                                                                   \
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);     \
        abort();                                                                        \
    }                                                                                   \
} while (0)
        
#define FLAG_SKYWORTH_DVNCA_FUNC_ENTRY (1)
#define FLAG_SKYWORTH_DVNCA_FUNC_EXIT (0)
#define SKYWORTH_DVNCA_FUNC_ENTRY(onOff)\
    do {\
        if (onOff)\
            SKYCAS_Debugger_Printf("[DVNCA] %s-%d enter!\n", __FUNCTION__,__LINE__);\
    }while(0)
    
#define SKYWORTH_DVNCA_FUNC_EXIT(onOff)\
    do {\
        if (onOff)\
            SKYCAS_Debugger_Printf("[DVNCA] %s-%d exit!\n", __FUNCTION__,__LINE__);\
    }while(0)

CSkyworth_Dvnca *CSkyworth_Dvnca::pSkyworthCAApi = SKYCAS_NULL;
SKYCAS_SMC_EVENT_E CSkyworth_Dvnca::m_eSmcEvent = SKYCAS_SMC_EVENT_INVALID;
SKYCAS_PostEvent_CB CSkyworth_Dvnca::mpTVService = SKYCAS_NULL;


#define SKYCAS_SMC_SLOT (0)		/*smartCard 卡槽号码*/
//#define SKYCAS_SC_TASK_PRIORITY (2)
#define SKYCAS_SC_TASK_STACK_SIZE (4096)
//#define SKYCAS_SF_TASK_PRIORITY (4)
#define SKYCAS_SF_TASK_STACK_SIZE (4096)

#define SKYCAS_DVNCAS_FILTER_SOFTFILTER_JUDGE(req) (0x80 == (req&0x80))

extern SKYCAS_S32 dvncas_smc_nofity(SC_EVENT_TYPE EventFlag);
static SKYCAS_VOID DVNCA_Main_Task(void *data)
{
    while (1)
    {
        cak_SystemRun();
        cak_TimerTrigger();
        SKYCAS_Task_Sleep(100);
    }
}


SKYCAS_U32 u32SMCHandle = 0;//SKYCAS_HANDLE_INVALID;
static SKYCAS_VOID SKYCAS_SmartCard_CB(SKYCAS_U8 cardSlot, SKYCAS_SMC_EVENT_E eSmcEvent)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[SKYCAS_SmartCard_CB][%s,%d] cardSlot = %d,eSmcEvent = %d\n",__FUNCTION__,__LINE__,cardSlot,eSmcEvent);

    CSkyworth_Dvnca* pDVNCA = CSkyworth_Dvnca::GetCaInstance();
    pDVNCA->m_eSmcEvent=eSmcEvent;
    pDVNCA->CallbackSmartcardProcess();
}

static SKYCAS_S32 SKYCAS_FilterEvent_CB(SKYCAS_U32 u32FilterId,  const SKYCAS_U8* const pU32BufferAddr, SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent)
{
    //SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    CSkyworth_Dvnca* pDVNCA = CSkyworth_Dvnca::GetCaInstance();
    return pDVNCA->SKYCAS_CASDataCALLBACK(u32FilterId,pU32BufferAddr,u32BufferLen,eFilterEvent);
}

CSkyworth_Dvnca* CSkyworth_Dvnca::GetCaInstance(SKYCAS_VOID)
{
    if(SKYCAS_NULL == pSkyworthCAApi)
    {
        //SKYCAS_Debugger_Printf("[DVNCA][%s,%d] pSkyworthCAApi SKYCAS_NULL\n",__FUNCTION__,__LINE__);
        pSkyworthCAApi =new (std::nothrow) CSkyworth_Dvnca;
        ASSERT(pSkyworthCAApi);
    }
    return pSkyworthCAApi;
}
SKYCAS_VOID CSkyworth_Dvnca::DestroyInstance(SKYCAS_VOID)
{
    if(pSkyworthCAApi != SKYCAS_NULL)
    {
        delete pSkyworthCAApi;
        pSkyworthCAApi = SKYCAS_NULL;
    }
}

CSkyworth_Dvnca::CSkyworth_Dvnca()
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    bInitflg = SKYCAS_FALSE;
    u16CATVersionNum = SKYCAS_U16_HANDLE_INVALID;
    u32CATQueue4Dvn = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_Mem_set(u8ArrCAT,0,SKYCAS_MAXLEN_CAT_MAX);

    m_DvnMainTaskId = SKYCAS_U32_HANDLE_INVALID;
    m_DvnMainTaskStoped = SKYCAS_TRUE;
    
    m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
    m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
    m_ScTaskStoped = SKYCAS_TRUE;
    m_eSmcEvent= SKYCAS_SMC_EVENT_INVALID;

    m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;	
    m_PriDataTaskStoped = SKYCAS_TRUE;
    m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
    m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;

    mpTVService = SKYCAS_NULL;
    
    for (SKYCAS_U16 i=0; i<SKYCAS_MAX_FILTER_COUNT; ++i)
    {
        SKYCAS_Filter_Logic_Init(i);
    }
    SKYCAS_Mem_set(pDvncaFilePath,'\0',sizeof(pDvncaFilePath));
}

CSkyworth_Dvnca::~CSkyworth_Dvnca()
{
	SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
}

SKYCAS_VOID CSkyworth_Dvnca::SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_SMC_EVENT_E scStatus = SKYCAS_SMC_EVENT_INVALID;
    CSkyworth_Dvnca* pDVNCA = CSkyworth_Dvnca::GetCaInstance();
    SKYCAS_BOOL bRet = SKYCAS_FALSE;
    
    while(SKYCAS_FALSE == pDVNCA->m_ScTaskStoped)
    {
        SKYCAS_Semp_Wait(pDVNCA->m_ScSemaphore,SKYCAS_U32_HANDLE_INVALID);

        scStatus = pDVNCA->m_eSmcEvent;
        if(SKYCAS_SMC_EVENT_IN == scStatus)
        {
            dvncas_smc_nofity(SC_CARD_IN);
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d %d] smartcard insert\n",__FUNCTION__,__LINE__,bRet);
        }
        else if(SKYCAS_SMC_EVENT_OUT== scStatus)
        {
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d,%d] smartcard out\n",__FUNCTION__,__LINE__,bRet);
            dvncas_smc_nofity(SC_CARD_OUT);
        }
    }
}

SKYCAS_VOID CSkyworth_Dvnca::SKYCAS_Filter_Logic_Init(SKYCAS_U8 logicIndex)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    m_U32FilterIdArray[logicIndex].m_Pid = SKYCAS_PID_INVALID;
    m_U32FilterIdArray[logicIndex].m_bUsed = SKYCAS_FALSE;
    m_U32FilterIdArray[logicIndex].m_RequestId = SKYCAS_U8_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_StartTimeTicks = SKYCAS_U32_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_u32QueueId4Dvn = SKYCAS_U32_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_u16TableId4Dvn = SKYCAS_U16_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_WaitSecond = 0;
    m_U32FilterIdArray[logicIndex].m_Deep = 0;
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].match,0,SKYCAS_FILTER_DEEP);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].mask,0,SKYCAS_FILTER_DEEP);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].negate,0,SKYCAS_FILTER_DEEP);
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pInitPara)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;

#if FLAG_SKYCAS_RESOURCE_FOR_APK
    /*File*/
    if(strcmp((char *)pInitPara->pFilePath,"") != 0)
    {
        strcpy(pDvncaFilePath, pInitPara->pFilePath);
    }
    else if ((0 != pInitPara->u32FlashAddr)&&(SKYCAS_U32_HANDLE_INVALID != pInitPara->u32FlashAddr)&&(0 != pInitPara->u32FlashSize))
    {
        SKYCAS_Debugger_Printf("[%s--%d] Init dvncas Flash! addr:0x%08X, size:0x%08X\n",__FUNCTION__,__LINE__, pInitPara->u32FlashAddr, pInitPara->u32FlashSize);
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s--%d]Error!!! Init dvncas File/Flash para error! u16StartPrio = %d,u16CASType = %d,pFilePath = %s,u32FlashAddr = 0x%08X,u32FlashSize = 0x%08X\n",
                                                __FUNCTION__,__LINE__,pInitPara->u16StartPrio,pInitPara->u16CASType,pInitPara->pFilePath,pInitPara->u32FlashAddr,pInitPara->u32FlashSize);
        return SKYCAS_RET_FAILURE;
    }

    /*Filter*/
    s32Ret = SKYCAS_Queue_Creat(&m_u32SectionMsgQueueId,sizeof(SKYCAS_DvncaSectionMessage_S),1);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Mutex_Creat(&m_U32PriDataMutexId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Mutex_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Task_Creat(&m_PriDataTaskId,"SectionFilterTask",pInitPara->u16StartPrio,SKYCAS_SF_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)CAPrivateDataTask,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_PriDataTaskStoped = SKYCAS_FALSE;
    
    if(SKYCAS_FALSE == bInitflg)
    {
        /*SMC*/
        s32Ret = SKYCAS_Semp_Creat(&m_ScSemaphore,0);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Semp_Creat fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        s32Ret = SKYCAS_Task_Creat(&m_ScTaskId,"SmartCardTask",pInitPara->u16StartPrio,SKYCAS_SC_TASK_STACK_SIZE,
                                                    (SKYCAS_TASK_FUNC)SKYCAS_SmartCard_Task,SKYCAS_NULL);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        m_ScTaskStoped = SKYCAS_FALSE;
        s32Ret = SKYCAS_SMC_Init(SKYCAS_SMC_SLOT);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            m_ScTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_ScTaskId);
            m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Init fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        s32Ret=SKYCAS_SMC_RegistCB(SKYCAS_SMC_SLOT,SKYCAS_SmartCard_CB);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
            m_ScTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_ScTaskId);
            m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_RegistCB fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        
        s32Ret =SKYCAS_SMC_Open(SKYCAS_SMC_SLOT,SKYCAS_SMC_MODE_T0);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
            m_ScTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_ScTaskId);
            m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Open fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
    }

    if(SKYCAS_TRUE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[%s,%d] repet enter CAS!\n",__FUNCTION__,__LINE__);
    }
    else
    {
        /*CAK*/
        cak_SetConfiguration(0x688F);
        //cak_InstallMSCallBack(DVNCAS_MS_ALTER);
        SKYCAS_S32 initRet = cak_Init();
        if (initRet < 0)
        {
            SKYCAS_SMC_Close(SKYCAS_SMC_SLOT);
            SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
            m_ScTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_ScTaskId);
            m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] ca init fail!initRet:%d\n",__FUNCTION__,__LINE__,initRet);
            return SKYCAS_RET_FAILURE;
        }
        cak_SetLibActivationPassword((UINT8*)"13803187"); /* for DVNCAS test libs*/
        
        s32Ret = SKYCAS_Task_Creat(&m_DvnMainTaskId,"DvnMainTask",pInitPara->u16StartPrio,SKYCAS_SC_TASK_STACK_SIZE,
                                                    (SKYCAS_TASK_FUNC)DVNCA_Main_Task,SKYCAS_NULL);
        if(SKYCAS_RET_FAILURE == s32Ret)
        {
            m_DvnMainTaskStoped = SKYCAS_TRUE;
            cak_StopKernel();
            SKYCAS_SMC_Close(SKYCAS_SMC_SLOT);
            SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
            m_ScTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_ScTaskId);
            m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Semp_Destory(m_ScSemaphore);
            m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
            m_PriDataTaskStoped = SKYCAS_TRUE;
            SKYCAS_Task_Destory(m_PriDataTaskId);
            m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
            m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
            m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
            SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        m_DvnMainTaskStoped = SKYCAS_FALSE;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] ca already init OK!\n",__FUNCTION__,__LINE__);
    }

    bInitflg = SKYCAS_TRUE;
    
#else

    if(SKYCAS_TRUE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] ca already init\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_SUCCESS;
    }
    
    #if FLAG_SKYCAS_RESOURCE_RECYCLING
        SKYCAS_Lib_Resource_Init();
    #endif

    /*File*/
    if(strcmp((char *)pInitPara->pFilePath,"") != 0)
    {
        strcpy(pDvncaFilePath, pInitPara->pFilePath);
    }
    else if ((0 != pInitPara->u32FlashAddr)&&(SKYCAS_U32_HANDLE_INVALID != pInitPara->u32FlashAddr)&&(0 != pInitPara->u32FlashSize))
    {
        SKYCAS_Debugger_Printf("[%s--%d] Init dvncas Flash! addr:0x%08X, size:0x%08X\n",__FUNCTION__,__LINE__, pInitPara->u32FlashAddr, pInitPara->u32FlashSize);
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s--%d]Error!!! Init dvncas File/Flash para error! u16StartPrio = %d,u16CASType = %d,pFilePath = %s,u32FlashAddr = 0x%08X,u32FlashSize = 0x%08X\n",
                                                __FUNCTION__,__LINE__,pInitPara->u16StartPrio,pInitPara->u16CASType,pInitPara->pFilePath,pInitPara->u32FlashAddr,pInitPara->u32FlashSize);
        return SKYCAS_RET_FAILURE;
    }

    /*Filter*/
    s32Ret = SKYCAS_Queue_Creat(&m_u32SectionMsgQueueId,sizeof(SKYCAS_DvncaSectionMessage_S),1);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Mutex_Creat(&m_U32PriDataMutexId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Mutex_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Task_Creat(&m_PriDataTaskId,"SectionFilterTask",pInitPara->u16StartPrio,SKYCAS_SF_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)CAPrivateDataTask,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_PriDataTaskStoped = SKYCAS_FALSE;
    
    
    /*SMC*/
    s32Ret = SKYCAS_Semp_Creat(&m_ScSemaphore,0);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Semp_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Task_Creat(&m_ScTaskId,"SmartCardTask",pInitPara->u16StartPrio,SKYCAS_SC_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)SKYCAS_SmartCard_Task,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_ScTaskStoped = SKYCAS_FALSE;
    s32Ret = SKYCAS_SMC_Init(SKYCAS_SMC_SLOT);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        m_ScTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_ScTaskId);
        m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Init fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret=SKYCAS_SMC_RegistCB(SKYCAS_SMC_SLOT,SKYCAS_SmartCard_CB);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
        m_ScTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_ScTaskId);
        m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_RegistCB fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret =SKYCAS_SMC_Open(SKYCAS_SMC_SLOT,SKYCAS_SMC_MODE_T0);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
        m_ScTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_ScTaskId);
        m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Open fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
        
    /*CAK*/
    cak_SetConfiguration(0x688F);
    //cak_InstallMSCallBack(DVNCAS_MS_ALTER);
    SKYCAS_S32 initRet = cak_Init();
    if (initRet < 0)
    {
        SKYCAS_SMC_Close(SKYCAS_SMC_SLOT);
        SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
        m_ScTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_ScTaskId);
        m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] ca init fail!initRet:%d\n",__FUNCTION__,__LINE__,initRet);
        return SKYCAS_RET_FAILURE;
    }
    cak_SetLibActivationPassword((UINT8*)"13803187"); /* for DVNCAS test libs*/

    s32Ret = SKYCAS_Task_Creat(&m_DvnMainTaskId,"DvnMainTask",pInitPara->u16StartPrio,SKYCAS_SC_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)DVNCA_Main_Task,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        m_DvnMainTaskStoped = SKYCAS_TRUE;
        cak_StopKernel();
        SKYCAS_SMC_Close(SKYCAS_SMC_SLOT);
        SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
        m_ScTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_ScTaskId);
        m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Semp_Destory(m_ScSemaphore);
        m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
        m_PriDataTaskStoped = SKYCAS_TRUE;
        SKYCAS_Task_Destory(m_PriDataTaskId);
        m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
        m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
        m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_DvnMainTaskStoped = SKYCAS_FALSE;
    SKYCAS_Debugger_Printf("[DVNCA][%s,%d] ca already init OK!\n",__FUNCTION__,__LINE__);

    bInitflg = SKYCAS_TRUE;
#endif
    
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_CaDeInit(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_S32 s32Ret = SKYCAS_RET_FAILURE;

#if FLAG_SKYCAS_RESOURCE_FOR_APK
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_ReleaseFilter(SKYCAS_U8_HANDLE_INVALID, SKYCAS_PID_INVALID);
    u16CATVersionNum = SKYCAS_U16_HANDLE_INVALID;
    u32CATQueue4Dvn = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_Mem_set(u8ArrCAT,0,SKYCAS_MAXLEN_CAT_MAX);

    m_PriDataTaskStoped = SKYCAS_TRUE;
    s32Ret = SKYCAS_Task_Destory(m_PriDataTaskId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
    
    s32Ret = SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
    
    s32Ret = SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Mutex_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;

    bInitflg = SKYCAS_TRUE;
    
#else
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_ReleaseFilter(SKYCAS_U8_HANDLE_INVALID, SKYCAS_PID_INVALID);
    u16CATVersionNum = SKYCAS_U16_HANDLE_INVALID;
    u32CATQueue4Dvn = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_Mem_set(u8ArrCAT,0,SKYCAS_MAXLEN_CAT_MAX);

    cak_StopKernel();
    m_DvnMainTaskStoped = SKYCAS_TRUE;
    s32Ret = SKYCAS_Task_Destory(m_DvnMainTaskId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_DvnMainTaskId = SKYCAS_U32_HANDLE_INVALID;

    #if FLAG_SKYCAS_RESOURCE_RECYCLING
        SKYCAS_Lib_Resource_Release_All();
    #endif

    m_ScTaskStoped=SKYCAS_TRUE;
    s32Ret = SKYCAS_SMC_Close(SKYCAS_SMC_SLOT);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Close fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_SMC_Deinit(SKYCAS_SMC_SLOT);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Deinit fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    s32Ret = SKYCAS_Task_Destory(m_ScTaskId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_ScTaskId = SKYCAS_U32_HANDLE_INVALID;
    
    s32Ret = SKYCAS_Semp_Destory(m_ScSemaphore);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Semp_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_ScSemaphore = SKYCAS_U32_HANDLE_INVALID;
    
    m_PriDataTaskStoped = SKYCAS_TRUE;
    s32Ret = SKYCAS_Task_Destory(m_PriDataTaskId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_PriDataTaskId = SKYCAS_U32_HANDLE_INVALID;
    
    s32Ret = SKYCAS_Queue_Destory(m_u32SectionMsgQueueId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_u32SectionMsgQueueId = SKYCAS_U32_HANDLE_INVALID;
    
    s32Ret = SKYCAS_Mutex_Destory(m_U32PriDataMutexId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Mutex_Destory fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    m_U32PriDataMutexId = SKYCAS_U32_HANDLE_INVALID;

    bInitflg = SKYCAS_FALSE;
#endif

    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_ReleaseFilter(SKYCAS_U8 FilterID,SKYCAS_U16 u16EcmPid)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_S32 s32Ret = SKYCAS_RET_FAILURE;
    SKYCAS_U32 i = 0;
    SKYCAS_U32 U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    
    SKYCAS_Mutex_lock(m_U32PriDataMutexId);
    if (SKYCAS_U8_HANDLE_INVALID == FilterID)
    {
        if (SKYCAS_PID_INVALID == u16EcmPid)    /* Release all */
        {
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                if (SKYCAS_U32_HANDLE_INVALID != m_U32FilterIdArray[i].m_U32FilterId)
                {
                    U32FilterId=m_U32FilterIdArray[i].m_U32FilterId;
                    s32Ret = SKYCAS_Filter_Stop(U32FilterId);	
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        //SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
                        SKYCAS_Debugger_Printf("[%s,%d]stop filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                        //return SKYCAS_RET_FAILURE;   /*这里是正确的吗?*/
                    }
                    s32Ret = SKYCAS_Filter_Free(U32FilterId);
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        //SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
                        SKYCAS_Debugger_Printf("[%s,%d]free filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                        //return SKYCAS_RET_FAILURE;   /*这里是正确的吗?*/
                    }
                    SKYCAS_Filter_Logic_Init(i);
                }	
            }            
        }
        else 
        {
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                if (u16EcmPid == m_U32FilterIdArray[i].m_Pid)   /* Release by u16EcmPid */
                {
                    U32FilterId=m_U32FilterIdArray[i].m_U32FilterId;
                    s32Ret = SKYCAS_Filter_Stop(U32FilterId);	
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]stop filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    s32Ret = SKYCAS_Filter_Free(U32FilterId);
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]free filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    SKYCAS_Filter_Logic_Init(i);
                }
            }
        }
    }
    else
    {
        if (SKYCAS_PID_INVALID == u16EcmPid)
        {
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                if (m_U32FilterIdArray[i].m_RequestId==FilterID)    /* Release by FilterID*/
                {
                    U32FilterId=m_U32FilterIdArray[i].m_U32FilterId;
                    s32Ret = SKYCAS_Filter_Stop(U32FilterId);   
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]stop filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    s32Ret = SKYCAS_Filter_Free(U32FilterId);
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]free filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    SKYCAS_Filter_Logic_Init(i);
                }
            }
        }
        else
        {
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                if((m_U32FilterIdArray[i].m_RequestId==FilterID)&&(u16EcmPid ==m_U32FilterIdArray[i].m_Pid))    /* Release by FilterID & u16EcmPid */
                {
                    U32FilterId=m_U32FilterIdArray[i].m_U32FilterId;
                    s32Ret = SKYCAS_Filter_Stop(U32FilterId);   
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]stop filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    s32Ret = SKYCAS_Filter_Free(U32FilterId);
                    if (SKYCAS_RET_SUCCESS != s32Ret)
                    {
                        SKYCAS_Debugger_Printf("[%s,%d]free filter fail! filterId:0x%X\n",__FUNCTION__,__LINE__,U32FilterId);
                    }
                    SKYCAS_Filter_Logic_Init(i);
                }
            }
        }
    }
    SKYCAS_Mutex_unlock(m_U32PriDataMutexId);

    return SKYCAS_RET_SUCCESS;
}

SKYCAS_BOOL CSkyworth_Dvnca::SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    //SKYCAS_BOOL ret = DVNCASTB_IsCDCa((DVNCA_U16)u16SystemID);
    INT16 dvnCasId = cak_GetCASID();
    if(dvnCasId != u16SystemID)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] u16SystemID = 0x%x\n",__FUNCTION__,__LINE__,u16SystemID);
        return SKYCAS_FALSE;
    }

    return SKYCAS_TRUE;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    if(SKYCAS_PID_INVALID <= u16EmmPid)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] emmpid illegic\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_NULL == pbuffer)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_U16 length =0;
    SKYCAS_U8 catVer = (SKYCAS_U8)((pbuffer[5]&0x3E)>>1);
    SKYCAS_Debugger_Printf("[%s,%d] catVer:%d\n",__FUNCTION__,__LINE__, catVer);

    if (catVer != u16CATVersionNum)
    {
        u16CATVersionNum = ((pbuffer[5]&0x3E)>>1);
        length = (((pbuffer[1] & 0x0F) << 8) | pbuffer[2]) +3; /* Total section len. */
        SKYCAS_Mem_copy((SKYCAS_VOID *)u8ArrCAT,(const SKYCAS_VOID * const)(pbuffer),length);

        if (u32CATQueue4Dvn != SKYCAS_U32_HANDLE_INVALID)
        {
            DVN_RTOS_SendQueue(u32CATQueue4Dvn,u8ArrCAT);
            SKYCAS_Debugger_Printf("[%s,%d] Send CAT to CAK!u32CATQueue4Dvn:0x%08X\n",__FUNCTION__,__LINE__, u32CATQueue4Dvn);
        }
        SKYCAS_Debugger_Printf("[%s,%d] CAT update!len:%d\n",__FUNCTION__,__LINE__, length);
        return SKYCAS_RET_SUCCESS;
    }

    return SKYCAS_RET_FAILURE;
}

/* 需要多路解扰时怎么办? */
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    INT32 s32Ret = cak_StopGettingECM(pServiceInfo->u16EcmPid);
    if (s32Ret >= 0)
    {
        return SKYCAS_RET_SUCCESS;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] dvncas set ecmPid:0x%04X fail! s32Ret:%i\n",__FUNCTION__,__LINE__,pServiceInfo->u16EcmPid,s32Ret);
    }
    
    return SKYCAS_RET_FAILURE;
}
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(SKYCAS_NULL == pServiceInfo)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    INT32 s32Ret = cak_StartGettingECM(pServiceInfo->u16ServiceID[0],pServiceInfo->u16EcmPid,0);
    if (s32Ret >= 0)
    {
        return SKYCAS_RET_SUCCESS;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] dvncas set ecmPid:0x%04X fail! s32Ret:%i\n",__FUNCTION__,__LINE__,pServiceInfo->u16EcmPid,s32Ret);
    }
    
    return SKYCAS_RET_FAILURE;
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_CASDataCALLBACK(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU32BufferAddr, 
	SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent)
{
    //SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    //SKYCAS_Debugger_Printf("[DVNCA][%s,%d],u32FilterId = %d,u32BufferLen = %d,eFilterEvent = %d\n",__FUNCTION__,__LINE__,u32FilterId,u32BufferLen,eFilterEvent);
    
    SKYCAS_U8 *u8data = SKYCAS_NULL;
    SKYCAS_DvncaSectionMessage_S Msg;			
    SKYCAS_Mem_set(&Msg,0,sizeof(SKYCAS_DvncaSectionMessage_S));

    if(eFilterEvent==SKYCAS_FILTER_EVENT_READY)
    {
        if ((SKYCAS_NULL == pU32BufferAddr)||(0 == u32BufferLen))
        {
            SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
            return SKYCAS_RET_FAILURE;
        }
        
        u8data = (SKYCAS_U8*)SKYCAS_Mem_Alloc(u32BufferLen);
        if (SKYCAS_NULL == u8data)
        {
            SKYCAS_Debugger_Printf("[%s,%d]Can't alloc memory for send filter[%d] data!\n",__FUNCTION__,__LINE__, u32FilterId);
            return SKYCAS_RET_FAILURE;
        }
        SKYCAS_Mem_copy(u8data,pU32BufferAddr,u32BufferLen);
        Msg.m_data = u8data;
        Msg.m_length = u32BufferLen;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d]TIMECOUNT:%d!!!\n",__FUNCTION__,__LINE__, SKYCAS_Time_Get_Times());
        Msg.m_data = SKYCAS_NULL;
        Msg.m_length = 0;
    }
    Msg.m_FilterId = u32FilterId;
    Msg.m_FilterEvent = eFilterEvent;

    if (SKYCAS_RET_SUCCESS != SKYCAS_Queue_SendMsg(m_u32SectionMsgQueueId,(SKYCAS_U8*)&Msg,sizeof(SKYCAS_DvncaSectionMessage_S),1000))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Filter[%d] sendMsg[%d] fail! \n",__FUNCTION__,__LINE__,u32FilterId,eFilterEvent);
        return SKYCAS_RET_FAILURE;
    }
    
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetNITData(SKYCAS_U8 *DataBuf)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetCardSN(SKYCAS_U8* pCardSN)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pCardSN)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }


    INT32 s32ret = 0;
    SKYCAS_U8 cardID[9] = "";
    s32ret = cak_GetSmartCardID(cardID);
    
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN success,cardID = [0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x]\n",
                        cardID[0],cardID[1],cardID[2],cardID[3],cardID[4],cardID[5],cardID[6],cardID[7]);

        SKYCAS_U8 u8Temp = 0;
        for (SKYCAS_U8 i=0; i<8; i++)
        {
            u8Temp = cardID[i] / 16;
            sprintf((char *)&pCardSN[2*i], "%X", u8Temp);
            u8Temp = cardID[i]%16;
            sprintf((char *)&pCardSN[2*i+1], "%X", u8Temp);
        }
        
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN success,pCardSN = %s\n",pCardSN);
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN card invalid,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN calib stop,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN semphore timeout,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN error unknown,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_ChangePin( const SKYCAS_U8* pOldPin, const SKYCAS_U8* pNewPin)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pOldPin)||(SKYCAS_NULL == pNewPin))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;

    
    SKYCAS_U8 u8OldPin[4] = {0};
    SKYCAS_U8 u8NewPin[4] = {0};
    for(SKYCAS_U8 i = 0;i<4;i++)
    {
        u8OldPin[i] = (pOldPin[i] + 0x30);
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin,u8OldPin:[0x%2X]\n",u8OldPin[i]);
    }

    for(SKYCAS_U8 i = 0;i<4;i++)
    {
        u8NewPin[i] = (pNewPin[i] + 0x30);
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin,u8NewPin:[0x%2X]\n",u8NewPin[i]);
    }
    s32ret = cak_SetPinCode((UINT8*)u8OldPin,(UINT8*)u8NewPin);
    
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin success\n");
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin card communication error,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin not support,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_NOTSUPPORT;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin pin error,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangePin error unknown,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetRating( const SKYCAS_U8* pPin,SKYCAS_U8 u8Rating )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating,pPin:[%d,%d,%d,%d] ,u8Rating:%d\n",pPin[0],pPin[1],pPin[2],pPin[3],u8Rating);
    if ((SKYCAS_NULL == pPin))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    SKYCAS_U8 u8Pin[4] = {0};
    for(SKYCAS_U8 i = 0;i<4;i++)
    {
        u8Pin[i] = (pPin[i] + 0x30);
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating,u8Pin:[0x%2X]\n",u8Pin[i]);
    }
    
    s32ret = cak_SetRating(u8Pin,u8Rating);

    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating success\n");
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating card invalid,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating not support,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_NOTSUPPORT;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating pin error,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_SetRating error unknown,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetRating( SKYCAS_U8* pRating )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pRating))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;  
    UINT8 u8Rating = 0;
    s32ret = cak_GetRating(&u8Rating);
    
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetRating success,pRating = %d\n",u8Rating);
        *pRating = u8Rating;
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetRating card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetRating not support,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_NOTSUPPORT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetRating error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,SKYCAS_DATETIME_S *StartTime,SKYCAS_DATETIME_S *EndTime)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,SKYCAS_DATETIME_S *stEndTime)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_U32 CSkyworth_Dvnca::SKYCAS_GetVer(SKYCAS_U8 *pData)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrCAVer[6+1]={0,};
    INT32 s32ret = cak_VersionReport(u8ArrCAVer);
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetVer success,u8CAVer:%s\n",u8ArrCAVer);
        SKYCAS_Mem_copy((char *)pData,u8ArrCAVer,6);
        return SKYCAS_CA_OK;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetVer fail\n");
        return SKYCAS_CA_UNKNOWN;	

    }

}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetPlatformID(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_U16 u16ArrID[2] = {0,1};
    SKYCAS_Mem_copy((SKYCAS_VOID *) pTVSID,(SKYCAS_VOID *)u16ArrID,2*sizeof(SKYCAS_U16));
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID, SKYCAS_U8 * pCount, SKYCAS_OPERATORINFO_S* stOperatorInfo )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

/** para add num **/SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID)//, SKYCAS_U32 *pSlotNum )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSlotID))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    pSlotID[0] = 1;
    return SKYCAS_CA_OK;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,SKYCAS_U8 u8SlotID,SKYCAS_TVSLOTINFO_S* pSlotInfo )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSlotInfo))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    float fTmpValue = 0.0;
    INT32 s32ret = cak_GetTokenValue(&fTmpValue);
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSlotInfo success\n");
        pSlotInfo->fSlotLeft = fTmpValue;
        pSlotInfo->fSlotSpent = 0.0;
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSlotInfo card communication error,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSlotInfo calib stop,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSlotInfo semp timeout,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSlotInfo error unknown,u16ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID, SKYCAS_Entitles* pServiceEntitles )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pServiceEntitles)||((u16TVSID != 0)&&(u16TVSID != 1)))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret0 = 0;
	INT32 s32ret1 = 0 ; 	
    UINT8 *pTemp0 = SKYCAS_NULL;
	UINT8 *pTemp1 = SKYCAS_NULL ;	
	UINT8 *pTemp = SKYCAS_NULL ; 
    INT32 s32SubNum0 = 0;	
    INT32 s32SubNum1 = 0 ; 
	INT32 s32SubTotalNum = 0 ; 
	SKYCAS_BOOL GetRecord0 = SKYCAS_FALSE; 
	SKYCAS_BOOL GetRecord1 = SKYCAS_FALSE ; 

    s32SubNum0 = cak_GetSizeofSubscriptionRecord( 0 ) ; 
	s32SubNum1 = cak_GetSizeofSubscriptionRecord( 1 ) ;
	s32SubTotalNum = std::max(0,s32SubNum0) + std::max(0,s32SubNum1);
    pTemp = (UINT8*)SKYCAS_Mem_Alloc( s32SubTotalNum*20 ) ;
	
	if( s32SubNum0 > 0 || s32SubNum1 > 0 )
	{
		 if( s32SubNum0 > 0 )
		 {
		 	pTemp0 = (UINT8*)SKYCAS_Mem_Alloc(s32SubNum0 * 20) ;
		 	s32ret0 = cak_GetSubscriptionRecord(0,pTemp0);
			if( s32ret0 >= 0 )
			{
			    SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles success 0,s32ret0 = 0x%x\n",s32ret0);
				pServiceEntitles->u16ProductCount = s32SubNum0;		

				SKYCAS_Mem_copy(pTemp , pTemp0 , s32SubNum0*20);
				SKYCAS_Mem_Free(pTemp0);
				GetRecord0 = SKYCAS_TRUE; 				
			}
			else 
			{
				// todo  case false 
				if(-1 == s32ret0)
        		{
           		 	SKYCAS_Mem_Free(pTemp0);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles card invalid,s32ret0 = 0x%x\n",s32ret0);
            		//return SKYCAS_CA_CARD_INVALID;
        		}
        		else if(-2 == s32ret0)
       			{
            		SKYCAS_Mem_Free(pTemp0);
           			SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles calib stop,s32ret0 = 0x%x\n",s32ret0);
            		//return SKYCAS_CA_CALIB_STOP;
        		}
        		else if(-3 == s32ret0)
        		{
            		SKYCAS_Mem_Free(pTemp0);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles semphore timeout,s32ret0 = 0x%x\n",s32ret0);
            		//return SKYCAS_CA_SEMP_TIMEOUT;
        		}
        		else
        		{
            		SKYCAS_Mem_Free(pTemp0);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles error unknown,s32ret0 = 0x%x\n",s32ret0);
            		//return SKYCAS_CA_UNKNOWN;
        		}
			}
		 }
		 if( s32SubNum1 > 0 )
		 {
		 	pTemp1 = (UINT8*)SKYCAS_Mem_Alloc(s32SubNum1 * 20) ; 
		 	s32ret1 = cak_GetSubscriptionRecord(1,pTemp1);
			if( s32ret1 >=0 )
			{
				pServiceEntitles->u16ProductCount += s32SubNum1 ; 
				if(s32SubNum0 < 0)
				{
					s32SubNum0 = 0 ; 
				}
					
				SKYCAS_Mem_copy(pTemp + s32SubNum0*20,pTemp1,s32SubNum1*20) ; 
				SKYCAS_Mem_Free(pTemp1);
				GetRecord1 = SKYCAS_TRUE ; 
			}
			else 
			{
			   //todo  case false
			   if(-1 == s32ret1)
        		{
           		 	SKYCAS_Mem_Free(pTemp1);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles card invalid,s32ret1 = 0x%x\n",s32ret1);
                    if( !GetRecord0 )
                    {
                    	return SKYCAS_CA_CARD_INVALID;
                    }					
        		}
        		else if(-2 == s32ret1)
       			{
            		SKYCAS_Mem_Free(pTemp1);
           			SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles calib stop,s32ret1 = 0x%x\n",s32ret1);
            		
					if( !GetRecord0 )
                    {
                    	return SKYCAS_CA_CALIB_STOP;
                    }	
        		}
        		else if(-3 == s32ret1)
        		{
            		SKYCAS_Mem_Free(pTemp1);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles semphore timeout,s32ret1 = 0x%x\n",s32ret1);
                    if( !GetRecord0 )
                    {
                    	return SKYCAS_CA_SEMP_TIMEOUT;
                    }					
        		}
        		else
        		{
            		SKYCAS_Mem_Free(pTemp1);
            		SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles error unknown,s32ret1 = 0x%x\n",s32ret1);
					if(!GetRecord0)
					{
						return SKYCAS_CA_UNKNOWN;
					}
				}
			}
			
		 }
		 
	
	}
	else 
	{
	   SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles entitle num 0\n");
       return SKYCAS_CA_UNKNOWN;		
	}
    
	if( GetRecord0 || GetRecord1 )
	{
        
		for( INT32 i = 0 ; i < s32SubTotalNum; ++i )
		{
			pServiceEntitles->stEntitles[i].u32ProductID = ((pTemp[0]<<8)|(pTemp[1]));
            pServiceEntitles->stEntitles[i].stBeginTime.u16Year = ((pTemp[4]*100)+(pTemp[5]));
            pServiceEntitles->stEntitles[i].stBeginTime.u8Month = pTemp[6];
            pServiceEntitles->stEntitles[i].stBeginTime.u8Day = pTemp[7];
            pServiceEntitles->stEntitles[i].stBeginTime.u8Hour = pTemp[8];
           	pServiceEntitles->stEntitles[i].stBeginTime.u8Min = pTemp[9];
            pServiceEntitles->stEntitles[i].stBeginTime.u8Sec = pTemp[10];
            
            pServiceEntitles->stEntitles[i].stEndTime.u16Year = (pTemp[12]*100)+(pTemp[13]);
            pServiceEntitles->stEntitles[i].stEndTime.u8Month = pTemp[14];
            pServiceEntitles->stEntitles[i].stEndTime.u8Day = pTemp[15];
            pServiceEntitles->stEntitles[i].stEndTime.u8Hour = pTemp[16];
            pServiceEntitles->stEntitles[i].stEndTime.u8Min = pTemp[17];
            pServiceEntitles->stEntitles[i].stEndTime.u8Sec = pTemp[18];	

            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles u32ProductID = %d\n",pServiceEntitles->stEntitles[i].u32ProductID);
            pTemp = pTemp + 20;
		}
		//SKYCAS_Mem_Free(pTemp);
		return SKYCAS_CA_OK ; 
	}
	else 
	{
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetServiceEntitles success 10\n");
		SKYCAS_Mem_Free(pTemp);
       return SKYCAS_CA_UNKNOWN;		
	}
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,SKYCAS_U32* pEntitleId )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if ((SKYCAS_NULL == pEntitleId))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    pEntitleId[0] = 0;
    pEntitleId[1] = 1;
    return SKYCAS_CA_OK;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,SKYCAS_BOOL *pReadFlag,SKYCAS_U32 *pDetitleChkNums)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_BOOL CSkyworth_Dvnca::SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_FALSE;
}

SKYCAS_BOOL CSkyworth_Dvnca::SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,SKYCAS_U32 u32DetitleChkNum )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_FALSE;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_IsPaired( SKYCAS_U8* pNum, SKYCAS_U8* pSTBSerial )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSTBSerial)||(SKYCAS_NULL == pNum))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_SmartCardPaired();

    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_IsPaired no pair\n");
        return SKYCAS_CA_CARD_NOPAIR;	
    }
    else if(s32ret == 1)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_IsPaired has pair\n");
        return SKYCAS_CA_OK;	
    }
    else if(s32ret == -1)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_IsPaired card communication error\n");
        return SKYCAS_CA_DATA_ERROR;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_IsPaired unknown error\n");
        return SKYCAS_CA_UNKNOWN;	
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram, SKYCAS_U16 u16EcmPid, const SKYCAS_U8 *pbyPinCode,const SKYCAS_IPPV_PRICE_S *pPrice )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetIPPVProgram(SKYCAS_U16 u16TVSID,SKYCAS_IPPVINFO_S *pIppv,SKYCAS_U16 *u16Number)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pIppv)||(SKYCAS_NULL == u16Number))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_GetSizeofIPPVSubscriptionRecord();
    if(s32ret > 0)
    {
        *u16Number = s32ret;

        UINT8 *pTemp = SKYCAS_NULL;
        pTemp = (UINT8 *)SKYCAS_Mem_Alloc(s32ret * 24);
        s32ret = cak_GetIPPVSubscriptionRecord(pTemp);
        if(s32ret >= 0)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetIPPVProgram success\n");
            pIppv->u32ProductID = ((pTemp[0]>>8)|(pTemp[1]));
            pIppv->u32Price = ((pTemp[4]<<24)|(pTemp[5]<<16)|(pTemp[6]<<8)|(pTemp[7]));
            
            pIppv->stStartDate.u16Year = ((pTemp[8]<<8)|(pTemp[9]));
            pIppv->stStartDate.u8Month = pTemp[10];
            pIppv->stStartDate.u8Day = pTemp[11];
            pIppv->stStartDate.u8Hour = pTemp[12];
            pIppv->stStartDate.u8Min = pTemp[13];
            pIppv->stStartDate.u8Sec = pTemp[14];

            pIppv->stStartDate.u16Year = ((pTemp[16]<<8)|(pTemp[17]));
            pIppv->stStartDate.u8Month = pTemp[18];
            pIppv->stStartDate.u8Day = pTemp[19];
            pIppv->stStartDate.u8Hour = pTemp[20];
            pIppv->stStartDate.u8Min = pTemp[21];
            pIppv->stStartDate.u8Sec = pTemp[22];

            SKYCAS_Mem_Free(pTemp);
            pTemp = SKYCAS_NULL;
            return SKYCAS_CA_OK;
        }
        else
        {
            SKYCAS_Mem_Free(pTemp);
            pTemp = SKYCAS_NULL;
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetIPPVProgram unknown error\n");
            return SKYCAS_CA_UNKNOWN;   
        }
    }
    else if(s32ret == 0)
    {
        *u16Number = s32ret;
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetIPPVProgram success\n");
        return SKYCAS_CA_OK;
    }
    else
    {
        *u16Number = 0;
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetIPPVProgram unknown error\n");
        return SKYCAS_CA_UNKNOWN;   
    }
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead, SKYCAS_U8* pCount, SKYCAS_U8* pIndex)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_CA_UNKNOWN;
}
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_DelEmail(SKYCAS_U32 u32EmailID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetEmailSpaceInfo( SKYCAS_U8 *pEmailNum,SKYCAS_U8 *pEmptyNum )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
													SKYCAS_U8 *pIsChild,
													SKYCAS_U16 *pDelayTime,
													SKYCAS_U32 *pLastFeedTime,
													SKYCAS_U8 *pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed )
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID, SKYCAS_U8 *pFeedData, SKYCAS_U8 *pLen)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, const SKYCAS_U8 *pFeedData, SKYCAS_U8 u8Len)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    return SKYCAS_CA_UNKNOWN;
}

SKYCAS_VOID CSkyworth_Dvnca::SKYCAS_RefreshInterface(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);


    return;
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pServ))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    mpTVService = pServ;
    SKYCAS_Debugger_Printf("[%s,%d] mpTVService is %p\n",__FUNCTION__,__LINE__,mpTVService);
    
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetPinLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return 4;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetCardNumLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return 16;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetSendNameMaxLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_SENDER_NAME;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetTitleMaxLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_TITLE;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetContentMaxLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_CONTENT;
}

SKYCAS_U16 CSkyworth_Dvnca::SKYCAS_GetOSDMaxLen(SKYCAS_VOID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    return 128;
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    
    if (SKYCAS_NULL == pSTBAtr)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrTmp[16+1]={0,};
    INT32 s32ret = cak_GetSTBAttribute(u8ArrTmp);
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSTBAttribute success,Attribute = %s\n",u8ArrTmp);
        SKYCAS_Mem_copy(pSTBAtr,u8ArrTmp,16);
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSTBAttribute card invalid,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSTBAttribute calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSTBAttribute semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSTBAttribute error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
        
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pAcNo)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrTmp[8+1]={0,};
    INT32 s32ret = cak_GetAccountNo(u8ArrTmp);
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetAccountNo success,pAcNo = %s\n",u8ArrTmp);
        SKYCAS_Mem_copy(pAcNo,u8ArrTmp,8);
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetAccountNo card invalid,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetAccountNo calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetAccountNo semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetAccountNo error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pSysDes)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrTmp[32+1]={0,};
    INT32 s32ret = 0;
    s32ret = cak_ReadSysDescriptor(u8index,u8ArrTmp);
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor success,pSysDes = %s\n",u8ArrTmp);
        SKYCAS_Mem_copy(pSysDes,u8ArrTmp,32);
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor card invalid,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pSCVer)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrTmp[3+1]={0,};
    cak_GetSmartCardVersion(u8ArrTmp);
    SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetSysDescriptor success,pSysDes = %s\n",u8ArrTmp);
    SKYCAS_Mem_copy(pSCVer,u8ArrTmp,3);
    return SKYCAS_CA_OK;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_PortingVendor(SKYCAS_U8 *pUser)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pUser)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 u8ArrTmp[20+1]={0,};
    INT32 s32ret = 0;
    s32ret = cak_PortingVendor(u8ArrTmp);
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PortingVendor success,pUser = %s\n",u8ArrTmp);
        SKYCAS_Mem_copy(pUser,u8ArrTmp,20);
        return SKYCAS_CA_OK;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PortingVendor unknown error\n");
        return SKYCAS_CA_UNKNOWN;	
    }
}

/** para be string **/SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pDate)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    
    SKYCAS_U8 u8ArrTmp[40+1]={0,};
    INT32 s32ret = 0;
    s32ret = cak_BuildDate(u8ArrTmp);
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_CALIBBuildDate success,pDate = %s\n",u8ArrTmp);
        SKYCAS_Mem_copy(pDate,u8ArrTmp,40);
        return SKYCAS_CA_OK;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_CALIBBuildDate unknown error\n");
        return SKYCAS_CA_UNKNOWN;	
    }

}

#if 0
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pNum)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_GetSizeofSubscriptionRecord(s32TVSID);
    
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetEntitleNum success,s32ret = 0x%x\n",s32ret);
        *pNum = s32ret;
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetEntitleNum card invalid,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetEntitleNum calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetEntitleNum semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetEntitleNum error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}
#endif 

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    if(u8Flag == 1)
    {
        s32ret = cak_ConfirmChangeToken(u8Index);
    }
    if(u8Flag == 0)
    {
        s32ret = cak_CancelChangeToken(u8Index);
    }

    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot success\n");
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot PARA ERROR,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else if(-4 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_ChangeSlot error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_DeductToken(fPrice,u8Type,u32ProgID);

    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot success\n");
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot PARA ERROR,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else if(-4 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_DeductSlot error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    INT32 s32ret = 0;
    s32ret = cak_CheckSubscription(s32ProductID);
    
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus success\n");
        return SKYCAS_CA_ENTITLED;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus no entitled,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_NO_ENTITLED;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus no entitle record,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_NO_ENTITLEDRECORD;
    }
    else if(-4 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus entitle overdue,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_ENTITLED_OVERDUE;
    }
    else if(-5 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus tv locked,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_TV_LOCKED;
    }
    else if(-6 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-7 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetProgramStatus error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_PerformPairing(SKYCAS_U8 *pData)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
#if 0 // del by CAK doc
    if (SKYCAS_NULL == pData)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
#endif    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_PerformPairing(pData);
    
    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing success\n");
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing Pairing Fail error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_PAIRING_FAIL;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing Emm_Timeout, s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_EMM_TIMEOUT;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing Data not Find,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_NOT_FIND;
    }
    else if(-4 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing CaLib_Stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-5 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing Semp TimeOut ,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_PerformPairing error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetPCCardType(SKYCAS_S32 *pType)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pType)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    INT32 msType = 0;
    s32ret = cak_GetMSType(&msType);
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardType success! type:%s\n", (msType==0)?"Mother":"Child");
        *pType = msType;
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardType not parentChildCard error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_NOT_PARENTCHILDCARD;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardType error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetPCardID(SKYCAS_U8 *pCardID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pCardID)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    SKYCAS_U8 cardID[9] = "";
    INT32 s32ret = 0;
    s32ret = cak_GetMasterSmartCardID(pCardID);

    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN success,cardID = [0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x]\n",
                        cardID[0],cardID[1],cardID[2],cardID[3],cardID[4],cardID[5],cardID[6],cardID[7]);

        SKYCAS_U8 u8Temp = 0;
        for (SKYCAS_U8 i=0; i<8; i++)
        {
            u8Temp = cardID[i] / 16;
            sprintf((char *)&pCardID[2*i], "%X", u8Temp);
            u8Temp = cardID[i]%16;
            sprintf((char *)&pCardID[2*i+1], "%X", u8Temp);
        }
        
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardSN success,pCardSN = %s\n",pCardID);
        return SKYCAS_CA_OK;	
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetPCardID Not parentChildCard,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_NOT_PARENTCHILDCARD;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetPCardID parent card ,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_PARENTCARD;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetPCardID error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == stExpiryDate)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    if(u8Flag == 0)
    {
        s32ret = cak_GetMasterSmartCardExpiryDate((unsigned char *)stExpiryDate);
        if(s32ret == 0)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate success\n");
            return SKYCAS_CA_OK;    
        }
        else if(-1 == s32ret)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate NOT PARENTCHILDCARD,s32ret = 0x%x\n",s32ret);
            return SKYCAS_CA_NOT_PARENTCHILDCARD;
        }
        else
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate error unknown,s32ret = 0x%x\n",s32ret);
            return SKYCAS_CA_UNKNOWN;
        }
    }
    
    if(u8Flag == 1)
    {
        s32ret = cak_GetSlaveSmartCardExpiryDate((unsigned char *)stExpiryDate);
        if(s32ret == 0)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate success\n");
            return SKYCAS_CA_OK;    
        }
        else if(-1 == s32ret)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate NOT PARENTCHILDCARD,s32ret = 0x%x\n",s32ret);
            return SKYCAS_CA_NOT_PARENTCHILDCARD;
        }
        else if(-2 == s32ret)
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate parent card ,s32ret = 0x%x\n",s32ret);
            return SKYCAS_CA_PARENTCARD;
        }
        else
        {
            SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetCardExpiryDate error unknown,s32ret = 0x%x\n",s32ret);
            return SKYCAS_CA_UNKNOWN;
        }
    }

    return SKYCAS_CA_UNKNOWN;
    
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if( SKYCAS_NULL == pPin )
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    INT32 s32ret = 0;
    s32ret = cak_AddIPPVSubscription(s32ProuctID,fPrice,(UINT8*)stStartTime,(UINT8*)stEndTime,(UINT8*)stTrasTime,pPin);
    if(s32ret == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV success\n");
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else if(-4 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV money not enough,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_MONEY_NOTENOUGH;
    }
    else if(-5 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV pin error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_AddIPPV error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pData)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_GetSizeofTransactionRecord();

    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecordNum success\n");
        *pData = s32ret;
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecordNum card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecordNum calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecordNum semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecordNum error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);

    if (SKYCAS_NULL == pData)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    INT32 s32ret = 0;
    s32ret = cak_GetTransactionRecord(u16Num,pData);

    if(s32ret >= 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecord success\n");
        *pData = s32ret;
        return SKYCAS_CA_OK;    
    }
    else if(-1 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecord card communication error,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_DATA_ERROR;
    }
    else if(-2 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecord calib stop,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_CALIB_STOP;
    }
    else if(-3 == s32ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecord semphore timeout,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_SEMP_TIMEOUT;
    }
    else
    {
        SKYCAS_Debugger_Printf("[DVNCA]SKYCAS_GetTransRecord error unknown,s32ret = 0x%x\n",s32ret);
        return SKYCAS_CA_UNKNOWN;
    }
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetFilePath(SKYCAS_CHAR *pFilePath)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pFilePath))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(strcmp((const char *)pDvncaFilePath,"") == 0)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d]file path is SKYCAS_NULL\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    strcpy(pFilePath, pDvncaFilePath);
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_GetCATdata(SKYCAS_U8* catData)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == catData))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    if (SKYCAS_U16_HANDLE_INVALID != u16CATVersionNum)
    {
        SKYCAS_Mem_copy(catData,u8ArrCAT,SKYCAS_MAXLEN_CAT_MAX);
        return SKYCAS_RET_SUCCESS;
    }
    SKYCAS_Debugger_Printf("[%s,%d] Get CAT error!u16CATVersionNum:0x%08X\n",__FUNCTION__,__LINE__,u16CATVersionNum);
    return SKYCAS_RET_FAILURE;
}
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetCATQueueId(SKYCAS_U32 u32QueueId)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    u32CATQueue4Dvn = u32QueueId;
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_SetPrivateData(SKYCAS_U8 FilterID,SKYCAS_U16 PID,SKYCAS_U16 TableID,SKYCAS_U8 Period,const SKYCAS_U16 *SectByte,SKYCAS_LONG QueueID)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_U32 i = 0;
    SKYCAS_U8 u8Match[16],u8Mask[16],u8Deep=0;

    SKYCAS_Debugger_Printf("+++[%s]FilterID:%d pid:0x%04X TableId:0x%02X QueueID:0x%08X TO:%d+++\n", __FUNCTION__,FilterID,PID,TableID,QueueID,Period);

    if (SKYCAS_NULL == SectByte)
    {
        SKYCAS_Debugger_Printf("\nSectByte[0]\n");
        u8Match[0]=TableID;/* [Warning!] u16 to u8 */
        u8Mask[0] = 0xFF;
        u8Deep = 1;
    }
    else
    {
#if 1
        SKYCAS_Debugger_Printf("\nSectByte[%d]:\n",8);
        for (i=0;i<8;i++)
            SKYCAS_Debugger_Printf("%04X ", *(SectByte+i));
        SKYCAS_Debugger_Printf("\n");
        SKYCAS_Debugger_Printf("\n");
#endif
        u8Match[0]=TableID;/* [Warning!] u16 to u8 */
        u8Mask[0] = 0xFF;

        for (i=0;i<8;i++)
        {
            u8Match[i+1] = (SKYCAS_U8)(SectByte[i]&0xff);
            u8Mask[i+1] = (SKYCAS_U8)(SectByte[i]>>8);
        }
        u8Deep = 9;
    }
    
    
    SKYCAS_U32 logicFilterIndex = SKYCAS_U32_HANDLE_INVALID;
    
    SKYCAS_Mutex_lock(m_U32PriDataMutexId);
    
    for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)  //如果有过滤器正在使用并且过滤条件相同则不处理
    {
        if(SKYCAS_TRUE == IsSame(i,FilterID,PID,u8Match,u8Mask,TableID))
        {
            m_U32FilterIdArray[i].m_bUsed = SKYCAS_TRUE;
            m_U32FilterIdArray[i].m_WaitSecond = Period;
            m_U32FilterIdArray[i].m_StartTimeTicks = SKYCAS_Time_Get_Times();
            SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
            SKYCAS_Debugger_Printf("\n[%s,%d]filter[%d]:0x%X has been used!\n",__FUNCTION__,__LINE__,i,m_U32FilterIdArray[i].m_U32FilterId);
            return SKYCAS_RET_SUCCESS;
        }
    }

    for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
    {
        //SKYCAS_Debugger_Printf("~~~[%d]:m_bUsed[%d] U32FilterId[0x%X] pid[0x%X]\n",i,m_U32FilterIdArray[i].m_bUsed,m_U32FilterIdArray[i].m_U32FilterId,m_U32FilterIdArray[i].m_Pid);
        if(SKYCAS_FALSE == m_U32FilterIdArray[i].m_bUsed)
        {
            //SKYCAS_Debugger_Printf("\nGot new logic filter:%d\n",i);
            logicFilterIndex = i;
            break;
        }
    }
    
    if(i>=SKYCAS_MAX_FILTER_COUNT)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] all filter is in use\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    SKYCAS_U32 U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;
    SKYCAS_FILTER_PARA_S FilterPara;
    SKYCAS_Mem_set(&FilterPara,0,sizeof(SKYCAS_FILTER_PARA_S));

    s32Ret = SKYCAS_Filter_Alloc(&U32FilterId,SKYCAS_FILTER_SECTION_SIZE_4K);
    if(SKYCAS_RET_SUCCESS != s32Ret)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Debugger_Printf("[%s,%d]Alloc filter fail!FilterID:%d, pid:0x%X\n",__FUNCTION__,__LINE__, FilterID, PID);
        return SKYCAS_RET_FAILURE;		
    }
    //SKYCAS_Debugger_Printf("\n ljc the new filter id is %d \n",U32FilterId);

    FilterPara.SKY_CAS_FILTER_EVENT_CB = &SKYCAS_FilterEvent_CB;
    FilterPara.u16FilterDataPid = PID;
    FilterPara.u32FilterTimeout = Period*1000;
    FilterPara.u8FilterDeep = u8Deep;
    FilterPara.eFilterMode = SKYCAS_FILTER_MODE_CONTINUE;
    FilterPara.u8ArrFilterMacth[0] = u8Match[0];
    FilterPara.u8ArrFilterMask[0] = u8Mask[0];
    if (SKYCAS_NULL != SectByte)
    {
        SKYCAS_Mem_copy(&(FilterPara.u8ArrFilterMacth[3]),&(u8Match[1]),FilterPara.u8FilterDeep-1);
        SKYCAS_Mem_copy(&(FilterPara.u8ArrFilterMask[3]),&(u8Mask[1]),FilterPara.u8FilterDeep-1);
        //SKYCAS_Mem_set(FilterPara.u8ArrFilterNeg,0,u8FilterDeep);
        FilterPara.u8FilterDeep = u8Deep+2;
    }

    
    s32Ret = SKYCAS_Filter_Set(U32FilterId,&FilterPara);
    if(SKYCAS_RET_SUCCESS != s32Ret)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Filter_Free(U32FilterId);
        SKYCAS_Debugger_Printf("[%s,%d]Set filter fail!reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__, FilterID, PID,U32FilterId);
        return SKYCAS_RET_FAILURE;		
    }
    s32Ret = SKYCAS_Filter_Start(U32FilterId);
    if(SKYCAS_RET_SUCCESS != s32Ret)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Filter_Free(U32FilterId);
        SKYCAS_Debugger_Printf("[%s,%d]Start filter fail!reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__, FilterID, PID,U32FilterId);
        return SKYCAS_RET_FAILURE;		
    }

    m_U32FilterIdArray[logicFilterIndex].m_RequestId = FilterID;
    m_U32FilterIdArray[logicFilterIndex].m_U32FilterId = U32FilterId;
    m_U32FilterIdArray[logicFilterIndex].m_StartTimeTicks = SKYCAS_Time_Get_Times();
    m_U32FilterIdArray[logicFilterIndex].m_Pid = PID;
    m_U32FilterIdArray[logicFilterIndex].m_WaitSecond = Period;
    m_U32FilterIdArray[logicFilterIndex].m_Deep = FilterPara.u8FilterDeep;
    m_U32FilterIdArray[logicFilterIndex].m_bUsed = SKYCAS_TRUE;
    m_U32FilterIdArray[logicFilterIndex].m_u16TableId4Dvn = TableID;
    m_U32FilterIdArray[logicFilterIndex].m_u32QueueId4Dvn = QueueID;
    SKYCAS_Mem_copy(m_U32FilterIdArray[logicFilterIndex].match,&(FilterPara.u8ArrFilterMacth[0])/*u8Match*/,FilterPara.u8FilterDeep);
    SKYCAS_Mem_copy(m_U32FilterIdArray[logicFilterIndex].mask,&(FilterPara.u8ArrFilterMask[0])/*u8Mask*/,FilterPara.u8FilterDeep);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicFilterIndex].negate,0,FilterPara.u8FilterDeep);

    SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
    SKYCAS_Debugger_Printf("[%s,%d]Start filter data... index:%d reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__,logicFilterIndex, FilterID, PID,U32FilterId);

#if 1
    SKYCAS_Debugger_Printf("[%s,%d]TIMECOUNT:%d, second:%d!!!\n",__FUNCTION__,__LINE__, SKYCAS_Time_Get_Times(), Period);
    for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
    {
        SKYCAS_Debugger_Printf("[%d]:m_RequestId[0x%X] U32FilterId[0x%X] pid[0x%X]\n",i,m_U32FilterIdArray[i].m_RequestId,m_U32FilterIdArray[i].m_U32FilterId,m_U32FilterIdArray[i].m_Pid);
    }
#endif

    return SKYCAS_RET_SUCCESS;
}


/*当(SKYCAS_U8_HANDLE_INVALID==FilterID)时表示只根据PID来释放过滤器，否则必须判断u8ReqID和PID*/
SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_ReleasePrivateDataFilter(SKYCAS_U8 FilterID,SKYCAS_U16 wPid)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[%s,%d]FilterID:%d,pid:0x%X\n",__FUNCTION__,__LINE__, FilterID,wPid);

    return SKYCAS_ReleaseFilter(FilterID,wPid);
}


SKYCAS_VOID CSkyworth_Dvnca::CallbackSmartcardProcess(SKYCAS_VOID)
{
	SKYCAS_Debugger_Printf("[DVNCA]CallbackSmartcardProcess in\n");
	SKYCAS_Semp_Release(m_ScSemaphore);
}


SKYCAS_S32 CSkyworth_Dvnca::SKYCAS_Filter_data_cmp(const SKYCAS_FILTER_S* const pFilter, const SKYCAS_U8* const pData)
{
	if ((SKYCAS_NULL == pData)||(SKYCAS_NULL == pFilter))
		return SKYCAS_RET_FAILURE;

	SKYCAS_U16 i =0;
	SKYCAS_U8 u8Match,u8Data,u8DataPos;

	for ( i = 0 ; i < pFilter->m_Deep; i++)
	{
		u8DataPos = (i > 0) ? (i+2) : 0 ;
		u8Match = (pFilter->match[i]) & (~(pFilter->mask[i])) ;
		u8Data = (*(pData+u8DataPos)) & (~(pFilter->mask[i])) ;
		if ( u8Match != u8Data)
		{
			SKYCAS_Debugger_Printf("Error! data_cmpare! u8ReqId:%d->[%d][0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X-0x%02X] \n",pFilter->m_RequestId,i,pData[0],pData[1],pData[2],pData[3],pData[4],pData[5],pData[6],pData[7]);
			return SKYCAS_RET_FAILURE;
		}
	}
	
	return SKYCAS_RET_SUCCESS;
}

SKYCAS_VOID CSkyworth_Dvnca::CAPrivateDataTask(SKYCAS_VOID* param)
{
    SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    SKYCAS_U32 i = 0;
    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;
    SKYCAS_U32 U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_U8 u8RequestId = SKYCAS_U8_HANDLE_INVALID;
    SKYCAS_U16 u16Pid = SKYCAS_PID_INVALID;
    SKYCAS_U32 sectionlength = sizeof(SKYCAS_DvncaSectionMessage_S);
    CSkyworth_Dvnca* pDVNCA = CSkyworth_Dvnca::GetCaInstance();
    SKYCAS_U32 U32DvnQueueId = SKYCAS_U32_HANDLE_INVALID;

    SKYCAS_DvncaSectionMessage_S Msg;
    Msg.m_FilterEvent = SKYCAS_FILTER_EVENT_INVALID;
    Msg.m_FilterId = SKYCAS_U32_HANDLE_INVALID;
    Msg.m_data = SKYCAS_NULL;
    Msg.m_length = 0;
    
    while (SKYCAS_FALSE == pDVNCA->m_PriDataTaskStoped)
    {
        s32Ret = SKYCAS_Queue_RecvMsg(pDVNCA->m_u32SectionMsgQueueId,(SKYCAS_U8 *)&Msg,&sectionlength,SKYCAS_U32_HANDLE_INVALID);
        if (SKYCAS_RET_SUCCESS != s32Ret)
        {
            //SKYCAS_Debugger_Printf("[DVNCA][%s,%d]SKYCAS_Queue_RecvMsg fail\n",__FUNCTION__,__LINE__);
            continue;
        }
        //SKYCAS_Debugger_Printf("\nMsg.m_length %d Msg.m_FilterId 0x%02X Msg.m_FilterEvent %d  \n",Msg.m_length,Msg.m_FilterId,Msg.m_FilterEvent);

        U32FilterId = Msg.m_FilterId;
        SKYCAS_Mutex_lock(pDVNCA->m_U32PriDataMutexId);
        for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
        {
            //SKYCAS_Debugger_Printf("ljc pDVNCA->m_U32FilterIdArray[i].m_bUsed %d U32FilterId %d i %d\n",pDVNCA->m_U32FilterIdArray[i].m_bUsed,U32FilterId,i);
            if((SKYCAS_TRUE == pDVNCA->m_U32FilterIdArray[i].m_bUsed)&&(U32FilterId == pDVNCA->m_U32FilterIdArray[i].m_U32FilterId)
                && (SKYCAS_TRUE == pDVNCA->IsSame(i,pDVNCA->m_U32FilterIdArray[i].m_RequestId,pDVNCA->m_U32FilterIdArray[i].m_Pid,pDVNCA->m_U32FilterIdArray[i].match,pDVNCA->m_U32FilterIdArray[i].mask,pDVNCA->m_U32FilterIdArray[i].m_u16TableId4Dvn)))
            {
                u8RequestId = pDVNCA->m_U32FilterIdArray[i].m_RequestId;
                u16Pid = pDVNCA->m_U32FilterIdArray[i].m_Pid;
                U32DvnQueueId = pDVNCA->m_U32FilterIdArray[i].m_u32QueueId4Dvn;
                break;
            }
        }
        SKYCAS_Mutex_unlock(pDVNCA->m_U32PriDataMutexId);

        if(i>=SKYCAS_MAX_FILTER_COUNT)
        {
            SKYCAS_Debugger_Printf("[%s,%d]Got msg, but can't find filter[%d]:reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, U32FilterId, u8RequestId, u16Pid);
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                SKYCAS_Debugger_Printf("[%d]:m_bUsed[%d] U32FilterId[0x%X] pid[0x%X]\n",i,pDVNCA->m_U32FilterIdArray[i].m_bUsed,pDVNCA->m_U32FilterIdArray[i].m_U32FilterId,pDVNCA->m_U32FilterIdArray[i].m_Pid);
            }
            continue;
        }
        
        if(SKYCAS_FILTER_EVENT_READY == Msg.m_FilterEvent)
        {
            if ((SKYCAS_NULL != Msg.m_data)&&(0 != Msg.m_length))
            {
                if(SKYCAS_DVNCAS_FILTER_SOFTFILTER_JUDGE(u8RequestId))
                {
                    s32Ret = pDVNCA->SKYCAS_Filter_data_cmp(&(pDVNCA->m_U32FilterIdArray[i]),Msg.m_data); /* for soft filter */
                    if (SKYCAS_RET_SUCCESS == s32Ret)
                    {
                        //SKYCAS_Debugger_Printf("[%s,%d]Soft filter Got dat! filter[%d]:reqId:%d, pid:0x%X, U32DvnQueueId:0x%08X\n",__FUNCTION__,__LINE__, U32FilterId, u8RequestId, u16Pid,U32DvnQueueId);
                        DVN_RTOS_SendQueue(U32DvnQueueId,Msg.m_data);
                    }
                }
                else
                {
                    //SKYCAS_Debugger_Printf("[%s,%d]Normal filter Got dat! filter[%d]:reqId:%d, pid:0x%X, U32DvnQueueId:0x%08X\n",__FUNCTION__,__LINE__, U32FilterId, u8RequestId, u16Pid,U32DvnQueueId);
                    DVN_RTOS_SendQueue(U32DvnQueueId,Msg.m_data);
                }
            }
            else
            {
                SKYCAS_Debugger_Printf("[%s,%d]Got dat, but ther buffer len[%d]! filter[%d]:reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, Msg.m_length,U32FilterId, u8RequestId, u16Pid);
                continue;
            }
        }
        else
        {
            DVN_RTOS_SendQueue(U32DvnQueueId,SKYCAS_NULL);
        }
        
        if(SKYCAS_NULL != Msg.m_data)
        {
            SKYCAS_Mem_Free(Msg.m_data);
            Msg.m_data=SKYCAS_NULL;
        }
        
        Msg.m_FilterEvent = SKYCAS_FILTER_EVENT_INVALID;
        Msg.m_FilterId = SKYCAS_U32_HANDLE_INVALID;
        Msg.m_data = SKYCAS_NULL;
        Msg.m_length = 0;

        U32FilterId = SKYCAS_U32_HANDLE_INVALID;
        u16Pid = SKYCAS_PID_INVALID;
        u8RequestId = SKYCAS_U8_HANDLE_INVALID;
        U32DvnQueueId = SKYCAS_U32_HANDLE_INVALID;
    }
}

SKYCAS_BOOL CSkyworth_Dvnca::IsSame(SKYCAS_U8 index,SKYCAS_U8 FilterID,SKYCAS_U16 pid,const SKYCAS_U8 *filterData,const SKYCAS_U8 *filterMask,SKYCAS_U16 TableID)
{
    //SKYWORTH_DVNCA_FUNC_ENTRY(FLAG_SKYWORTH_DVNCA_FUNC_ENTRY);
    
    if(filterData == SKYCAS_NULL)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filterData is null\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }
    
    if(index >= SKYCAS_MAX_FILTER_COUNT)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filter count > 12\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    if(m_U32FilterIdArray[index].m_RequestId!=FilterID) return SKYCAS_FALSE;	
    if(m_U32FilterIdArray[index].m_Pid!=pid) return SKYCAS_FALSE;	
    if(m_U32FilterIdArray[index].m_u16TableId4Dvn!=TableID) return SKYCAS_FALSE;	
    //if(memcmp(filterData,m_U32FilterIdArray[index].match,8)!=0) return SKYCAS_FALSE;	
    //if(memcmp(filterMask,m_U32FilterIdArray[index].mask,8)!=0) return SKYCAS_FALSE;

    return SKYCAS_TRUE;
}

