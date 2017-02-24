//<Skyworth Software>
//******************************************************************************

#include <string.h>
#include <new>
#include <errno.h>
#include <iostream>
#include <stdlib.h>

#include "cdcas.h"
#include "skycas_datatype.h"
#include "skyworth_ca_api.h"
#include "skyworth_tfca.h"


#define ASSERT(_x_)                                                                         \
    do  {                                                                                   \
    if ( ! ( _x_ ) )                                                                    \
    {                                                                                   \
        printf("ASSERT FAIL: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);     \
        abort();                                                                        \
    }                                                                                   \
} while (0)
        
#define FLAG_SKYWORTH_TFCA_FUNC_ENTRY (1)
#define FLAG_SKYWORTH_TFCA_FUNC_EXIT (0)
#define SKYWORTH_TFCA_FUNC_ENTRY(onOff)\
    do {\
        if (onOff)\
            SKYCAS_Debugger_Printf("[CDCA] %s-%d enter!\n", __FUNCTION__,__LINE__);\
    }while(0)
    
#define SKYWORTH_TFCA_FUNC_EXIT(onOff)\
    do {\
        if (onOff)\
            SKYCAS_Debugger_Printf("[CDCA] %s-%d exit!\n", __FUNCTION__,__LINE__);\
    }while(0)

CSkyworth_Cdca *CSkyworth_Cdca::pSkyworthCAApi = SKYCAS_NULL;
SKYCAS_SMC_EVENT_E CSkyworth_Cdca::m_eSmcEvent = SKYCAS_SMC_EVENT_INVALID;
SKYCAS_PostEvent_CB CSkyworth_Cdca::mpTVService = SKYCAS_NULL;


#define SKYCAS_SMC_SLOT (0)		/*smartCard 卡槽号码*/
//#define SKYCAS_SC_TASK_PRIORITY (2)
#define SKYCAS_SC_TASK_STACK_SIZE (4096)
//#define SKYCAS_SF_TASK_PRIORITY (4)
#define SKYCAS_SF_TASK_STACK_SIZE (4096)

#define SKYCAS_CDCAS_FILTER_ONESHOT_JUDGE(req) (0x80 == (req&0x80))
#define isLeap(years) (((years%4==0 && years%100!=0) || years%400==0)?1:0)

SKYCAS_U32 u32SMCHandle = 0;//SKYCAS_HANDLE_INVALID;

SKYCAS_VOID CSkyworth_Cdca::CDDATATODATA(SKYCAS_U16 days,SKYCAS_DATETIME_S *pDATATIME)
{
	SKYCAS_S32 s32leap;
	SKYCAS_U8 m;
	SKYCAS_U32 mon_lengths[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
	SKYCAS_U32 year_lengths[2] = {365, 366};
	SKYCAS_U16 y = 2000;
	days = days + 1;
	while (true) 
    {
		s32leap = isLeap(y);
		if (days <= year_lengths[s32leap])
			break;
		y++;
        days -= ((s32leap == 0) ? year_lengths[0] : year_lengths[1]);
	}
    
	SKYCAS_U32 ip[12] = {0};
    if(s32leap == 0)
    {
        SKYCAS_Mem_copy((SKYCAS_VOID *) ip, (SKYCAS_VOID *) mon_lengths[0],sizeof(SKYCAS_U32)*12);
    }
    if(s32leap == 1)
    {
        SKYCAS_Mem_copy((SKYCAS_VOID *) ip, (SKYCAS_VOID *) mon_lengths[1],sizeof(SKYCAS_U32)*12);
    }
    
	for (m = 0; days > ip[m]; ++m)
    {
		days -= ip[m];
	}

    pDATATIME->u16Year = y;
    pDATATIME->u8Month = (m+1);
    pDATATIME->u8Day = days;
	//return y+"-"+(m+1)+"-"+days;
}    

#if 0
static void CDTIME2GMT(CDCA_TIME caTime,TimeGMT *ptGMT,int flag)
{
    DateGMT date;
	int i=0;
	U32 u32Days = 0;
	int second =24*3600;
	printf("\n\n caTime%u\n\n",caTime);
	if(flag==0)
   {
	   CDCA_DATE caDate = (CDCA_DATE)((caTime >> 16) & 0xFFFF);
	    
	    CDDATE2GMT(caDate, &date);//原函数CDTIME2GMT

	    ptGMT->u16year = date.u16year;
	    ptGMT->u8month = date.u8month;
	    ptGMT->u8day = date.u8day;
	    
	    ptGMT->u8second = (U8)((caTime & 0x1F)*2);
	    ptGMT->u8minute = (U8)((caTime >> 5) & 0x3F);
	    ptGMT->u8hour = (U8)((caTime >> 11) & 0x1F);
	}
	else if(flag==1)
	{
		while (i < 60)
	    {
	        if (caTime < sg_emailyear_days[i])
	            break;
	        i++;
	    }

	    if (i >= 60)
	    {
	        //ca_adp_trace(CAADP_TRACE_ERROR, "invalid DAE:%d\n", caDate);
	        ptGMT->u16year = 0;
	        ptGMT->u8month = 0;
	        ptGMT->u8day = 0;
	        return;
	    }
		printf("\n\ni %d\n\n",i);
	    ptGMT->u16year = 1970 + i - 1;
		printf("\n\n++ptGMT->u16year %d++\n\n",ptGMT->u16year);
	    u32Days = caTime - sg_emailyear_days[i - 1]+1;
		printf("\n\n++u32Days %d++\n\n",u32Days);
	    i = 0;
		if (TRUE == isLeapYear(ptGMT->u16year))
	    {
	        while (i < 12)
	        {
	            if (u32Days < sg_leep_mon_days[i]*second)
	                break;
	            i++;
	        }        
	    }
	    else
	    {
	        while (i < 12)
	        {
	            if (u32Days <sg_mon_days[i]*second)
	                break;
	            i++;
	        } 
			printf("\n\n i%d\n\n",i);
	    }

	    if (i >= 12)
	    {
	        ca_adp_trace(CAADP_TRACE_ERROR, "invalid u32Days:%d\n", u32Days);
	        ptGMT->u8month = 0;
	        ptGMT->u8day = 0;
	        return;
	    }

	    ptGMT->u8month = i+1;

	    if (ptGMT->u8month == 1)
	     {
	     		ptGMT->u8day = u32Dayscond;
			ptGMT->u8hour= u32Days%second/3600+8;
			ptGMT->u8minute = u32Days%second%3600/60;
			ptGMT->u8second = u32Days%second%3600%60/60;
	    }
	    else
	    {	
	    	
	        if (TRUE == isLeapYear(ptGMT->u16year))
	        {	
	        		printf("%u %d%u ",u32Days,i,ptGMT->u8day);
	        		ptGMT->u8day = (u32Days - sg_leep_mon_days[i-1]*second)cond;
				ptGMT->u8hour= (u32Days - sg_leep_mon_days[i-1]*second)%second/3600+8;
				ptGMT->u8minute = (u32Days - sg_leep_mon_days[i-1]*second)%second%3600/60;
				ptGMT->u8second = (u32Days - sg_leep_mon_days[i-1]*second)%second%3600%60/60;
				printf("\n\nptGMT->u8day%d\n\n",ptGMT->u8day);
				
		}
	        else
	      	{    
	      			ptGMT->u8day = (u32Days - sg_mon_days[i-1]*second)cond;
				ptGMT->u8hour= (u32Days - sg_mon_days[i-1]*second)%second/3600+8;
				ptGMT->u8minute = (u32Days - sg_mon_days[i-1]*second)%second%3600/60;
				ptGMT->u8second = (u32Days - sg_mon_days[i-1]*second)%second%3600%60/60;
	        }
			
	    }
		
	}
    return;
}
#endif


static SKYCAS_VOID SKYCAS_SmartCard_CB(SKYCAS_U8 cardSlot, SKYCAS_SMC_EVENT_E eSmcEvent)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[CDCASKYCAS_SmartCard_CB][%s,%d] cardSlot = %d,eSmcEvent = %d\n",__FUNCTION__,__LINE__,cardSlot,eSmcEvent);

    CSkyworth_Cdca* pCDCA = CSkyworth_Cdca::GetCaInstance();
    pCDCA->m_eSmcEvent=eSmcEvent;
    pCDCA->CallbackSmartcardProcess();
}

static SKYCAS_S32 SKYCAS_FilterEvent_CB(SKYCAS_U32 u32FilterId,  const SKYCAS_U8* const pU32BufferAddr, SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);

    CSkyworth_Cdca* pCDCA = CSkyworth_Cdca::GetCaInstance();
    return pCDCA->SKYCAS_CASDataCALLBACK(u32FilterId,pU32BufferAddr,u32BufferLen,eFilterEvent);
}


CSkyworth_Cdca* CSkyworth_Cdca::GetCaInstance(SKYCAS_VOID)
{
    if(SKYCAS_NULL == pSkyworthCAApi)
    {
        //SKYCAS_Debugger_Printf("[CDCA][%s,%d] pSkyworthCAApi SKYCAS_NULL\n",__FUNCTION__,__LINE__);
        pSkyworthCAApi =new (std::nothrow) CSkyworth_Cdca;
        ASSERT(pSkyworthCAApi);
    }
    return pSkyworthCAApi;
}
SKYCAS_VOID CSkyworth_Cdca::DestroyInstance(SKYCAS_VOID)
{
    if(pSkyworthCAApi != SKYCAS_NULL)
    {
        delete pSkyworthCAApi;
        pSkyworthCAApi = SKYCAS_NULL;
    }
}

CSkyworth_Cdca::CSkyworth_Cdca()
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    bInitflg = SKYCAS_FALSE;
    u16CATVersionNum = SKYCAS_U16_HANDLE_INVALID;
    //u16CATTsId = SKYCAS_U16_HANDLE_INVALID;
    //u16CATORNid = SKYCAS_U16_HANDLE_INVALID;

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
    SKYCAS_Mem_set(pCDcaFilePath,'\0',sizeof(pCDcaFilePath));
}

CSkyworth_Cdca::~CSkyworth_Cdca()
{
	SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
}


SKYCAS_VOID CSkyworth_Cdca::SKYCAS_SmartCard_Task(SKYCAS_VOID* para)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_SMC_EVENT_E scStatus = SKYCAS_SMC_EVENT_INVALID;
    CSkyworth_Cdca* pCDCA = CSkyworth_Cdca::GetCaInstance();
    CDCA_BOOL bRet = SKYCAS_FALSE;
    
    while(SKYCAS_FALSE == pCDCA->m_ScTaskStoped)
    {
        SKYCAS_Semp_Wait(pCDCA->m_ScSemaphore,SKYCAS_U32_HANDLE_INVALID);

        scStatus = pCDCA->m_eSmcEvent;
        if(SKYCAS_SMC_EVENT_IN == scStatus)
		{
			bRet=(SKYCAS_BOOL)CDCASTB_SCInsert(&u32SMCHandle);
            SKYCAS_Debugger_Printf("[CDCA][%s,%d %d] smartcard insert\n",__FUNCTION__,__LINE__,bRet);

		}
		else
		{
			SKYCAS_Debugger_Printf("[CDCA][%s,%d,%d] smartcard out\n",__FUNCTION__,__LINE__,bRet);
			CDCASTB_SCRemove(&u32SMCHandle);
		}
	}

}

SKYCAS_VOID CSkyworth_Cdca::SKYCAS_Filter_Logic_Init(SKYCAS_U8 logicIndex)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    m_U32FilterIdArray[logicIndex].m_Pid = SKYCAS_PID_INVALID;
    m_U32FilterIdArray[logicIndex].m_bUsed = SKYCAS_FALSE;
    m_U32FilterIdArray[logicIndex].m_RequestId = SKYCAS_U8_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_StartTimeTicks = SKYCAS_U32_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    m_U32FilterIdArray[logicIndex].m_WaitSecond = 0;
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].match,0,SKYCAS_FILTER_DEEP);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].mask,0,SKYCAS_FILTER_DEEP);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicIndex].negate,0,SKYCAS_FILTER_DEEP);
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pInitPara)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;

    if(SKYCAS_TRUE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] ca already init\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_SUCCESS;
    }

#if FLAG_SKYCAS_RESOURCE_RECYCLING
    SKYCAS_Lib_Resource_Init();
#endif
        
    //文件初始化
    if(strcmp((char *)pInitPara->pFilePath,"") != 0)
    {
        strcpy(pCDcaFilePath, pInitPara->pFilePath);
#if 0        
        SKYCAS_CHAR* pU8TmpFilePath = SKYCAS_NULL;
        SKYCAS_U32 u32TmpFilePathMax = SKYCAS_FILE_PATH_LEN_MAX;
        SKYCAS_U32 u32TmpFileSize = SKYCAS_CDCAS_FILE_SIZE_MAX;

        pU8TmpFilePath = (SKYCAS_CHAR*)SKYCAS_Mem_Alloc(u32TmpFilePathMax);
        if (SKYCAS_NULL == pU8TmpFilePath)
        {
            SKYCAS_Debugger_Printf("[%s--%d]Alloc file path tmpBuffer fail! size:%d\n",__FUNCTION__,__LINE__, u32TmpFilePathMax);
            return SKYCAS_RET_FAILURE;
        }
        s32Ret |= SKYCAS_Mem_set(pU8TmpFilePath,'\0',u32TmpFilePathMax);
        sprintf(pU8TmpFilePath, "%scdcas%d", pCDcaFilePath, (SKYCAS_U8)(CDCA_FLASH_BLOCK_A));
        s32Ret |= SKYCAS_File_Init((SKYCAS_CHAR*)pU8TmpFilePath,u32TmpFileSize);

        s32Ret |= SKYCAS_Mem_set(pU8TmpFilePath,'\0',u32TmpFilePathMax);
        sprintf(pU8TmpFilePath, "%scdcas%d", pCDcaFilePath, (SKYCAS_U8)(CDCA_FLASH_BLOCK_B));
        s32Ret |= SKYCAS_File_Init((SKYCAS_CHAR*)pU8TmpFilePath,u32TmpFileSize);
        if (SKYCAS_RET_SUCCESS != s32Ret)
        {
            SKYCAS_Debugger_Printf("[%s--%d]Warning!!! Init cdcas file has some error! path:%s\n",__FUNCTION__,__LINE__, pU8TmpFilePath);
        }
        SKYCAS_Mem_Free(pU8TmpFilePath);
#endif        
    }
    else if ((0 != pInitPara->u32FlashAddr)&&(SKYCAS_U32_HANDLE_INVALID != pInitPara->u32FlashAddr)&&(0 != pInitPara->u32FlashSize))
    {
        SKYCAS_Debugger_Printf("[%s--%d] Init cdcas Flash! addr:0x%08X, size:0x%08X\n",__FUNCTION__,__LINE__, pInitPara->u32FlashAddr, pInitPara->u32FlashSize);
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s--%d]Error!!! Init cdcas File/Flash para error! u16StartPrio = %d,u16CASType = %d,pFilePath = %s,u32FlashAddr = 0x%08X,u32FlashSize = 0x%08X\n",
                                                __FUNCTION__,__LINE__,pInitPara->u16StartPrio,pInitPara->u16CASType,pInitPara->pFilePath,pInitPara->u32FlashAddr,pInitPara->u32FlashSize);
        return SKYCAS_RET_FAILURE;
    }

    s32Ret = SKYCAS_Queue_Creat(&m_u32SectionMsgQueueId,sizeof(SKYCAS_CdcaSectionMessage_S),1);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    s32Ret = SKYCAS_Mutex_Creat(&m_U32PriDataMutexId);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Queue_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    s32Ret = SKYCAS_Task_Creat(&m_PriDataTaskId,"SectionFilterTask",pInitPara->u16StartPrio,SKYCAS_SF_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)CAPrivateDataTask,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    m_PriDataTaskStoped = SKYCAS_FALSE;
    
    //库初始化
    CDCA_BOOL bRet = CDCASTB_Init(pInitPara->u16StartPrio);
    if(CDCA_FALSE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] CDCASTB_Init fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    

    //SMC Init
    s32Ret = SKYCAS_Semp_Creat(&m_ScSemaphore,0);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Semp_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    s32Ret = SKYCAS_Task_Creat(&m_ScTaskId,"SmartCardTask",pInitPara->u16StartPrio,SKYCAS_SC_TASK_STACK_SIZE,
                                                (SKYCAS_TASK_FUNC)SKYCAS_SmartCard_Task,SKYCAS_NULL);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_Task_Creat fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    m_ScTaskStoped = SKYCAS_FALSE;
    s32Ret = SKYCAS_SMC_Init(SKYCAS_SMC_SLOT);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Init fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    s32Ret=SKYCAS_SMC_RegistCB(SKYCAS_SMC_SLOT,SKYCAS_SmartCard_CB);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_RegistCB fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    s32Ret =SKYCAS_SMC_Open(SKYCAS_SMC_SLOT,SKYCAS_SMC_MODE_T0);
    if(SKYCAS_RET_FAILURE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] SKYCAS_SMC_Open fail\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    

    bInitflg = SKYCAS_TRUE;
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_CaDeInit(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_S32 s32Ret = SKYCAS_RET_FAILURE;

    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    CDCASTB_SetEcmPid(CDCA_LIST_FIRST,SKYCAS_NULL);
    CDCASTB_SetEcmPid(CDCA_LIST_OK,SKYCAS_NULL);
    CDCASTB_SetEmmPid(0);
    SKYCAS_ReleaseFilter(SKYCAS_U8_HANDLE_INVALID, SKYCAS_PID_INVALID);
    u16CATVersionNum = SKYCAS_U16_HANDLE_INVALID;

    SKYCAS_Debugger_Printf("[%s,%d]start close CDCAS LIB!\n",__FUNCTION__,__LINE__);
    CDCASTB_Close();
    SKYCAS_Debugger_Printf("[%s,%d]end close CDCAS LIB!\n",__FUNCTION__,__LINE__);

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

    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_ReleaseFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 u16EcmPid)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_S32 s32Ret = SKYCAS_RET_FAILURE;
    SKYCAS_U32 i = 0;
    SKYCAS_U32 U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    
    SKYCAS_Mutex_lock(m_U32PriDataMutexId);
    if (SKYCAS_U8_HANDLE_INVALID == u8ReqID)
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
                if (m_U32FilterIdArray[i].m_RequestId==u8ReqID)    /* Release by u8ReqID*/
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
                if((m_U32FilterIdArray[i].m_RequestId==u8ReqID)&&(u16EcmPid ==m_U32FilterIdArray[i].m_Pid))    /* Release by u8ReqID & u16EcmPid */
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

SKYCAS_BOOL CSkyworth_Cdca::SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    CDCA_BOOL ret = CDCASTB_IsCDCa((CDCA_U16)u16SystemID);
    if(CDCA_FALSE == ret)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] u16SystemID = 0x%x\n",__FUNCTION__,__LINE__,u16SystemID);
        return SKYCAS_FALSE;
    }

    return SKYCAS_TRUE;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    if(SKYCAS_PID_INVALID <= u16EmmPid)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] emmpid illegic\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_Debugger_Printf("[CDCA][%s,%d] emmpid = 0x%x\n",__FUNCTION__,__LINE__,u16EmmPid);
    CDCASTB_SetEmmPid(u16EmmPid);
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_NULL == pbuffer)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_U16 CA_SYSTEMID=0, CA_EMM_PID=0;
    SKYCAS_U16 i = 0;
    SKYCAS_U16 length =0, lengthDEC = 0;
    //SKYCAS_U8* pCatSecBuf = pbuffer;
    //ASSERT(pCatSecBuf);

    SKYCAS_U8 u8TmpBuffer[256];
    SKYCAS_Mem_set((SKYCAS_VOID *)u8TmpBuffer,0,256);
    
    SKYCAS_U8 catVer = (SKYCAS_U8)((pbuffer[5]&0x3E)>>1);
    SKYCAS_Debugger_Printf("[%s,%d] catVer:%d\n",__FUNCTION__,__LINE__, catVer);

    if (catVer != u16CATVersionNum)
    {
        u16CATVersionNum = ((pbuffer[5]&0x3E)>>1);
        length = (((pbuffer[1] & 0x0F) << 8) | pbuffer[2]) +3; /* Total section len. */
        length = length-8-4; /* not header & CRC */
        SKYCAS_Mem_copy((SKYCAS_VOID *)u8TmpBuffer,(const SKYCAS_VOID * const)(pbuffer+8),length);
        SKYCAS_Debugger_Printf("[%s,%d] CAT update!len:%d\n",__FUNCTION__,__LINE__, length);
        for(i=0;i<length;++i)   SKYCAS_Debugger_Printf("\n### u8TmpBuffer[%d]:0x%02X",i,u8TmpBuffer[i]);
        //pCatSecBuf = pCatSecBuf + 8;
        

#if 1
        for (i=0;i<length;)
        {
            lengthDEC = u8TmpBuffer[i+1];
            if (u8TmpBuffer[i] == 0x09)
            {
                SKYCAS_SetEmmPid(0);
                CA_SYSTEMID = (SKYCAS_U16)((SKYCAS_U16)(u8TmpBuffer[i+2]) << 8 | (SKYCAS_U16)(u8TmpBuffer[i+3]));
                //SKYCAS_Debugger_Printf("\nu8TmpBuffer[2],u8TmpBuffer[3]:0x%X,0x%X\n",__FUNCTION__,__LINE__, CA_SYSTEMID);
                SKYCAS_Debugger_Printf("\n[%s,%d] CAS SystemID:0x%X\n",__FUNCTION__,__LINE__, CA_SYSTEMID);
                if(SKYCAS_TRUE == SKYCAS_CheckSystem(CA_SYSTEMID))
                {
                    CA_EMM_PID = (SKYCAS_U16)((SKYCAS_U16)(u8TmpBuffer[i+4] << 8) | (SKYCAS_U16)(u8TmpBuffer[i+5]))&SKYCAS_PID_INVALID;
                    SKYCAS_Debugger_Printf("[%s,%d] SystemID = 0x%X, EmmPid = 0x%X\n",__FUNCTION__,__LINE__,CA_SYSTEMID,CA_EMM_PID);
                    SKYCAS_SetEmmPid(CA_EMM_PID);
                }
            }
            i = (i+lengthDEC+2);            
        }
#else
        while((length - 4) > 0)
        {
            lengthDEC = pCatSecBuf[1];
            if(pCatSecBuf[0] == 0x09)
            {
                CA_SYSTEMID = pCatSecBuf[2] << 8 | pCatSecBuf[3] ;
                SKYCAS_Debugger_Printf("\nCAS SystemID:0x%X\n",__FUNCTION__,__LINE__, CA_SYSTEMID);
                if(pCDCA->SKYCAS_CheckSystem(CA_SYSTEMID))
                {
                    CA_EMM_PID = ( pCatSecBuf[4] << 8 | pCatSecBuf[5] ) & SKYCAS_PID_INVALID;
                    SKYCAS_Debugger_Printf("[CDCA][%s,%d] SystemID = 0x%X, EmmPid = 0x%X\n",__FUNCTION__,__LINE__,CA_SYSTEMID,CA_EMM_PID);
                    pCDCA->SKYCAS_SetEmmPid(CA_EMM_PID);
                }
            }
            pCatSecBuf = pCatSecBuf + lengthDEC + 2;
            length = length - lengthDEC - 2;
        }
#endif        
        return SKYCAS_RET_SUCCESS;
    }

    return SKYCAS_RET_FAILURE;
}

/* 需要多路解扰时怎么办? */
SKYCAS_S32 CSkyworth_Cdca::SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    CDCASTB_SetEcmPid(CDCA_LIST_FIRST,SKYCAS_NULL);
    CDCASTB_SetEcmPid(CDCA_LIST_OK,SKYCAS_NULL);

    if(SKYCAS_NULL != pServiceInfo)
    {
        return SKYCAS_ReleaseFilter(SKYCAS_U8_HANDLE_INVALID, pServiceInfo->u16EcmPid);
    }
    else
    {
         return SKYCAS_ReleaseFilter(SKYCAS_U8_HANDLE_INVALID, SKYCAS_PID_INVALID);
    }
}
SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(SKYCAS_NULL == pServiceInfo)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_U8 i = 0;
    SCDCASServiceInfo tmpService;
    SKYCAS_Mem_set((SKYCAS_VOID* const)&tmpService,0,sizeof(SCDCASServiceInfo));

    CDCASTB_SetEcmPid(CDCA_LIST_FIRST,SKYCAS_NULL);
    SKYCAS_Debugger_Printf("[CDCA][%s,%d],u16EcmPid = 0x%x,u8ServiceNum = %d\n",__FUNCTION__,__LINE__,pServiceInfo->u16EcmPid,pServiceInfo->u8ServiceNum);
    tmpService.m_wEcmPid = pServiceInfo->u16EcmPid;
    tmpService.m_byServiceNum = pServiceInfo->u8ServiceNum;
    for (i=0;i<pServiceInfo->u8ServiceNum;++i)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d],u16ServiceID[%d] = 0x%x\n",__FUNCTION__,__LINE__,i,pServiceInfo->u16ServiceID[i]);
        tmpService.m_wServiceID[i] = pServiceInfo->u16ServiceID[i];
    }
    CDCASTB_SetEcmPid(CDCA_LIST_ADD,&tmpService);
    CDCASTB_SetEcmPid(CDCA_LIST_OK,SKYCAS_NULL);

    return SKYCAS_RET_SUCCESS;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_CASDataCALLBACK(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU32BufferAddr, 
	SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(SKYCAS_FALSE == bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    //SKYCAS_Debugger_Printf("[CDCA][%s,%d],u32FilterId = %d,u32BufferLen = %d,eFilterEvent = %d\n",__FUNCTION__,__LINE__,u32FilterId,u32BufferLen,eFilterEvent);
    
    SKYCAS_U8 *u8data = SKYCAS_NULL;
    SKYCAS_CdcaSectionMessage_S Msg;			
    SKYCAS_Mem_set(&Msg,0,sizeof(SKYCAS_CdcaSectionMessage_S));

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

    if (SKYCAS_RET_SUCCESS != SKYCAS_Queue_SendMsg(m_u32SectionMsgQueueId,(SKYCAS_U8*)&Msg,sizeof(SKYCAS_CdcaSectionMessage_S),1000))
    {
        SKYCAS_Debugger_Printf("[%s,%d]Filter[%d] sendMsg[%d] fail! \n",__FUNCTION__,__LINE__,u32FilterId,eFilterEvent);
        return SKYCAS_RET_FAILURE;
    }
    
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetNITData(SKYCAS_U8 *DataBuf)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if (SKYCAS_NULL == DataBuf)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetCardSN(SKYCAS_U8* pCardSN)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pCardSN)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    CDCA_U16 u16ret = CDCA_RC_OK;

    SKYCAS_U8 pTempCardSN[CDCA_MAXLEN_SN+1]={0};

    u16ret = CDCASTB_GetCardSN(&u32SMCHandle,(char *)pTempCardSN);
    if(CDCA_RC_OK == u16ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetCardSN success,pTempCardSN = %s\n",pTempCardSN);
        SKYCAS_Mem_copy(pCardSN,pTempCardSN,CDCA_MAXLEN_SN+1);
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetCardSN pionit null,u16ret = 0x%x\n",u16ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetCardSN card invalid,u16ret = 0x%x\n",u16ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetCardSN error unknown,u16ret = 0x%x\n",u16ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_ChangePin( const SKYCAS_U8* pOldPin, const SKYCAS_U8* pNewPin)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pOldPin)||(SKYCAS_NULL == pNewPin))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_ChangePin(&u32SMCHandle,pOldPin,pNewPin);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ChangePin success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ChangePin pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ChangePin card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(CDCA_RC_PIN_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ChangePin PIN invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ChangePin error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pPIN))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == bLocked))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetRating( const SKYCAS_U8* pPin,SKYCAS_U8 u8Rating )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating,pPin:%s ,u8Rating:%d\n",pPin,u8Rating);
    if ((SKYCAS_NULL == pPin))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_SetRating(&u32SMCHandle,pPin,u8Rating);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(CDCA_RC_PIN_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating PIN invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else if(CDCA_RC_WATCHRATING_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating WATCHRATING invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_WATCHRATING_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetRating error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetRating( SKYCAS_U8* pRating )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pRating))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetRating(&u32SMCHandle,pRating);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetRating success,pRating:%d\n",*pRating);
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetRating pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetRating card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetRating error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }


}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,SKYCAS_DATETIME_S *StartTime,SKYCAS_DATETIME_S *EndTime)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pPin)||(SKYCAS_NULL == StartTime)||(SKYCAS_NULL == EndTime))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_SetWorkTime(&u32SMCHandle,pPin,StartTime->u8Hour,StartTime->u8Min,StartTime->u8Sec,EndTime->u8Hour,EndTime->u8Min,EndTime->u8Sec);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(CDCA_RC_PIN_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime PIN invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_PIN_INVALID;
    }
    else if(CDCA_RC_WORKTIME_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime worktime invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_WORKTIME_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_SetWorkTime error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,SKYCAS_DATETIME_S *stEndTime)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == stStartTime)||(SKYCAS_NULL == stEndTime))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetWorkTime(&u32SMCHandle,&stStartTime->u8Hour,&stStartTime->u8Min,&stStartTime->u8Sec,&stEndTime->u8Hour,&stEndTime->u8Min,&stEndTime->u8Sec);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetWorkTime success\n");
        SKYCAS_Debugger_Printf("[CDCA]starttime:%d:%d:%d,endtime:%d:%d:%d\n",stStartTime->u8Hour,stStartTime->u8Min,stStartTime->u8Sec,stEndTime->u8Hour,stEndTime->u8Min,stEndTime->u8Sec);
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetWorkTime pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetWorkTime card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetWorkTime error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_U32 CSkyworth_Cdca::SKYCAS_GetVer(SKYCAS_U8 *pData)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

#if 1
    SKYCAS_U32 u32Ver = SKYCAS_U32_HANDLE_INVALID;
    u32Ver = CDCASTB_GetVer();
    SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetVer u32Ver = 0x%x\n",u32Ver);
#else
    SKYCAS_U8 initIndex=0;
    while(1)
    {
        SKYCAS_Task_Sleep(500);
        SKYCAS_Debugger_Printf("[CDCASTB_Close]initIndex:%d\n",initIndex);
        CDCASTB_SCRemove(&u32SMCHandle);
        CDCASTB_Close();
        SKYCAS_Task_Sleep(500);
        SKYCAS_Debugger_Printf("[CDCASTB_Init]initIndex:%d\n",initIndex);
        if (CDCA_TRUE != CDCASTB_Init(4))
        {
            break;
        }
        CDCASTB_SCInsert(&u32SMCHandle);
        initIndex++;
    }
#endif
    return u32Ver;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetPlatformID(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    SKYCAS_U16 u32PlatID = 0;
    u32PlatID = CDCASTB_GetPlatformID();

    SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetPlatformID u32PlatID = 0x%x\n",u32PlatID);	
    return u32PlatID;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pTVSID))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetOperatorIds(&u32SMCHandle,pTVSID);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorIds success,tsid = %d\n",*pTVSID);
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorIds pionit null,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorIds card invalid,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorIds error unknown,u16Ret = 0x%x\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }


}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID, SKYCAS_U8 * pCount, SKYCAS_OPERATORINFO_S* stOperatorInfo )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pCount)||(SKYCAS_NULL == stOperatorInfo))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    SCDCAOperatorInfo* tmpOperInfo;
    tmpOperInfo = (SCDCAOperatorInfo*)malloc(sizeof(SCDCAOperatorInfo));
    //SCDCAOperatorInfo* tmpOperInfo=(SCDCAOperatorInfo*)SKYCAS_Mem_Alloc(sizeof(SCDCAOperatorInfo));

    u16Ret = CDCASTB_GetOperatorInfo(&u32SMCHandle,u16TVSID,tmpOperInfo);
    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorInfo success\n");
         stOperatorInfo->u16TVSID = u16TVSID;
         SKYCAS_Mem_copy(stOperatorInfo->u8TVSName,tmpOperInfo->m_szTVSPriInfo,SKYCAS_MAXLEN_TVS_NAME+1);
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorInfo pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorInfo card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorInfo tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorInfo error unknown,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pACArray))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetACList(&u32SMCHandle,u16TVSID,(CDCA_U32 *)pACArray);
    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetACList success,AC = %s\n",pACArray);
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetACList pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetACList card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetACList tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetACList error unknown,s32Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID)//, SKYCAS_U32 *pSlotNum )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSlotID))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetSlotIDs(&u32SMCHandle,u16TVSID,pSlotID);

    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotIDs success\n");
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotIDs pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotIDs card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotIDs tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotIDs error unknown,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,SKYCAS_U8 u8SlotID,SKYCAS_TVSLOTINFO_S* pSlotInfo )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSlotInfo))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetSlotInfo(&u32SMCHandle,u16TVSID,u8SlotID,(SCDCATVSSlotInfo*)pSlotInfo);

    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotInfo success\n");
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotInfo pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotInfo card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotInfo tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetSlotInfo error unknown,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID, SKYCAS_Entitles* pServiceEntitles )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pServiceEntitles))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    SCDCAEntitles *pTemp = SKYCAS_NULL;
    pTemp = (SCDCAEntitles*)SKYCAS_Mem_Alloc(sizeof(SCDCAEntitles));
    u16Ret = CDCASTB_GetServiceEntitles(&u32SMCHandle,u16TVSID,pTemp);
    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles success\n");
         pServiceEntitles->u16ProductCount = pTemp->m_wProductCount;
         for(SKYCAS_U32 i = 0;i<SKYCAS_MAXNUM_ENTITLE;i++)
         {
             SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles [%d,%d]\n",pServiceEntitles->stEntitles[i].u32ProductID);
             pServiceEntitles->stEntitles[i].u32ProductID = pTemp->m_Entitles[i].m_dwProductID;

             CDDATATODATA((SKYCAS_U16)(pTemp->m_Entitles[i].m_tBeginDate),&pServiceEntitles->stEntitles[i].stBeginTime);
             CDDATATODATA((SKYCAS_U16)(pTemp->m_Entitles[i].m_tExpireDate),&pServiceEntitles->stEntitles[i].stEndTime);
             //pServiceEntitles->stEntitles[i].stBeginTime = pTemp->m_Entitles[i].m_tBeginDate;
             //pServiceEntitles->stEntitles[i].stEndTime = pTemp->m_Entitles[i].m_tExpireDate;
             
             pServiceEntitles->stEntitles[i].u8Video = pTemp->m_Entitles[i].m_bCanTape;
         }
         SKYCAS_Mem_Free(pTemp);
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles pionit null,u16Ret = 0x%x\n",u16Ret);
         SKYCAS_Mem_Free(pTemp);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles card invalid,u16Ret = 0x%x\n",u16Ret);
         SKYCAS_Mem_Free(pTemp);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles tsid not find,u16Ret = 0x%x\n",u16Ret);
         SKYCAS_Mem_Free(pTemp);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetServiceEntitles error unknown,u16Ret = 0x%x\n",u16Ret);
         SKYCAS_Mem_Free(pTemp);
         return SKYCAS_CA_UNKNOWN;
     }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,SKYCAS_U32* pEntitleId )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pEntitleId))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetEntitleIDs(&u32SMCHandle,u16TVSID,(CDCA_U32*)pEntitleId);

    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetEntitleIDs success\n");
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetEntitleIDs pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetEntitleIDs card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetEntitleIDs tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetEntitleIDs error unknown,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,SKYCAS_BOOL *pReadFlag,SKYCAS_U32 *pDetitleChkNums)
{
    SKYCAS_Debugger_Printf("[CDCA][%s,%d] in u16TVSID = %d\n",__FUNCTION__,__LINE__,u16TVSID);
    if ((SKYCAS_NULL == pReadFlag)||(SKYCAS_NULL == pDetitleChkNums))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetDetitleChkNums(&u32SMCHandle,u16TVSID,(CDCA_BOOL*)pReadFlag,(CDCA_U32*)pDetitleChkNums);
    if(CDCA_RC_OK == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleChkNums success,pReadFlag = %s,pDetitleChkNums = %s\n",pReadFlag,pDetitleChkNums);
         return SKYCAS_CA_OK;    
     }
     else if(CDCA_RC_POINTER_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleChkNums pionit null,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_POINTER_INVALID;
     }
     else if(CDCA_RC_CARD_INVALID == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleChkNums card invalid,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleChkNums tsid not find,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_CARD_INVALID;
     }
     else
     {
         SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleChkNums error unknown,u16Ret = 0x%x\n",u16Ret);
         return SKYCAS_CA_UNKNOWN;
     }

}

SKYCAS_BOOL CSkyworth_Cdca::SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    CDCA_BOOL s32Ret = CDCA_FALSE;
    s32Ret = CDCASTB_GetDetitleReaded(&u32SMCHandle,u16TVSID);

    if(CDCA_TRUE == s32Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleReaded already read\n");
        return SKYCAS_TRUE;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetDetitleReaded not read\n");
        return SKYCAS_FALSE;
    }

}

SKYCAS_BOOL CSkyworth_Cdca::SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,SKYCAS_U32 u32DetitleChkNum )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    //SKYCAS_U32 u32SEHandle = SKYCAS_U32_HANDLE_INVALID;

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    CDCA_BOOL s32Ret = CDCA_FALSE;
    s32Ret = CDCASTB_DelDetitleChkNum(&u32SMCHandle,u16TVSID,u32DetitleChkNum);

    if(CDCA_TRUE== s32Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_DelDetitleChkNum success,u16TVSID = %d,u32DetitleChkNum = %d\n",u16TVSID,u32DetitleChkNum);
        return SKYCAS_TRUE;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_DelDetitleChkNum,ret = %d\n",s32Ret);
        return SKYCAS_FALSE;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_IsPaired( SKYCAS_U8* pNum, SKYCAS_U8* pSTBSerial )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pSTBSerial)||(SKYCAS_NULL == pNum))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    
    u16Ret = CDCASTB_IsPaired(&u32SMCHandle,pNum,pSTBSerial);
    
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_IsPaired success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_CARD_PAIROTHER== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_IsPaired PAIR OTHER,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_CARD_PAIROTHER;	
    }
    else if(CDCA_RC_CARD_NOPAIR== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_IsPaired PAIR NONE,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_CARD_NOPAIR;	
    }
    else if(CDCA_RC_CARD_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_IsPaired CARD INVALID,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_IsPaired UNKNOWN ERROR,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram, SKYCAS_U16 u16EcmPid, const SKYCAS_U8 *pbyPinCode,const SKYCAS_IPPV_PRICE_S *pPrice )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pbyPinCode)||(SKYCAS_NULL == pPrice))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }

    CDCA_U16 u16Ret = SKYCAS_CA_OK;
    u16Ret = CDCASTB_StopIPPVBuyDlg(&u32SMCHandle,bBuyProgram,u16EcmPid,pbyPinCode,(SCDCAIPPVPrice*)pPrice);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg POINTER INVALID,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_POINTER_INVALID;	
    }
    else if(CDCA_RC_CARD_NO_ROOM == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg CARD ROOM NOT enough,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_CARD_NO_ROOM;	
    }
    else if(CDCA_RC_CARD_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg CARD INVALID,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_CARD_INVALID;	
    }
    else if(CDCA_RC_PROG_STATUS_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg price status INVALID,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_PROG_STATUS_INVALID;	
    }
    else if(CDCA_RC_PIN_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg pin INVALID,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_PIN_INVALID;	
    }
    else if(CDCA_RC_DATA_NOT_FIND== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg TSID NOT FIND,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_DATA_NOT_FIND;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_StopIPPVBuyDlg UNKNOWN ERROR,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetIPPVProgram(SKYCAS_U16 u16TVSID,SKYCAS_IPPVINFO_S *pIppv,SKYCAS_U16 *u16Number)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pIppv)||(SKYCAS_NULL == u16Number))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetIPPVProgram [%d,%d]\n",u16TVSID,*u16Number);

    CDCA_U16 u16Ret = CDCA_RC_OK;
    SCDCAIppvInfo *pTemp = SKYCAS_NULL;
    pTemp = (SCDCAIppvInfo*)SKYCAS_Mem_Alloc(sizeof(SCDCAIppvInfo));
    u16Ret = CDCASTB_GetIPPVProgram(&u32SMCHandle,u16TVSID,pTemp,u16Number);

    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetIPPVProgram success\n");

        pIppv->u32ProductID = pTemp->m_dwProductID;
        pIppv->u32Price = pTemp->m_wPrice;

        CDDATATODATA(pTemp->m_wExpiredDate,&pIppv->stEndDate);
        
        pIppv->u8Flag = pTemp->m_byBookEdFlag;
        pIppv->u8Video = pTemp->m_bCanTape;
        pIppv->u8SlotID = pTemp->m_bySlotID;

        SKYCAS_Mem_Free(pTemp);
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_CARD_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetIPPVProgram CARD INVALID,u16Ret = %d\n",u16Ret);
        SKYCAS_Mem_Free(pTemp);
        return SKYCAS_CA_CARD_INVALID;	
    }
    else if(CDCA_RC_DATA_NOT_FIND== u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetIPPVProgram TSID NOT FIND,u16Ret = %d\n",u16Ret);
        SKYCAS_Mem_Free(pTemp);
        return SKYCAS_CA_DATA_NOT_FIND;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetIPPVProgram UNKNOWN ERROR,u16Ret = %d\n",u16Ret);
        SKYCAS_Mem_Free(pTemp);
        return SKYCAS_CA_UNKNOWN;
    }
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead, SKYCAS_U8* pCount, SKYCAS_U8* pIndex)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pEmailHead)||(SKYCAS_NULL == pCount)||(SKYCAS_NULL == pIndex))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    
    CDCA_U16 u16Ret = SKYCAS_CA_OK;
    SKYCAS_Debugger_Printf("[%s,%d] pCount = %d,pIndex = %d!\n",__FUNCTION__,__LINE__,*pCount,*pIndex);
    SKYCAS_U8 u8TmpEmailCount = CDCA_MAXNUM_EMAIL;
    SKYCAS_U8 u8TmpEmailEmpty = 0;
    u16Ret = CDCASTB_GetEmailSpaceInfo(&u8TmpEmailCount,&u8TmpEmailEmpty);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d]EmailCount = %d,EmailEmpty = %d!\n",__FUNCTION__,__LINE__,u8TmpEmailCount,u8TmpEmailEmpty);
    }

   SCDCAEmailHead* ptmpEmailHead = new SCDCAEmailHead[100];
   SKYCAS_Mem_set(ptmpEmailHead,0,sizeof(SCDCAEmailHead)*100);
   
    u16Ret = CDCASTB_GetEmailHeads(ptmpEmailHead,pCount,pIndex);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] pCount = %d,pIndex = %d!\n",__FUNCTION__,__LINE__,*pCount,*pIndex);
        for(SKYCAS_U32 i = 0; i < *pCount; i ++)
        {
        SKYCAS_Debugger_Printf("[%s,%d][%d,%c,%s,%c,%d]!\n",__FUNCTION__,__LINE__,
            ptmpEmailHead[i].m_dwActionID,ptmpEmailHead[i].m_wImportance,ptmpEmailHead[i].m_szEmailHead,
            ptmpEmailHead[i].m_bNewEmail,ptmpEmailHead[i].m_tCreateTime);
        
        pEmailHead[i].u32EmailID = ptmpEmailHead[i].m_dwActionID;
        
        //pEmailHead[i].u32SendTime = ptmpEmailHead[i].m_tCreateTime;

        struct tm *p;
        time_t timer;
        timer = (time_t)(ptmpEmailHead[i].m_tCreateTime+8*3600);
        p=localtime(&timer);
        pEmailHead[i].u32SendTime.u16Year = (1970+p->tm_year);
        pEmailHead[i].u32SendTime.u8Month = (1+p->tm_mon);
        pEmailHead[i].u32SendTime.u8Day = p->tm_mday;
        pEmailHead[i].u32SendTime.u8Hour = p->tm_hour;
        pEmailHead[i].u32SendTime.u8Min = p->tm_min;
        pEmailHead[i].u32SendTime.u8Sec= p->tm_sec;
        SKYCAS_Debugger_Printf("[%s,%d]sendtime:[%d,%d,%d,%d,%d,%d]!\n",__FUNCTION__,__LINE__,
            pEmailHead[i].u32SendTime.u16Year,pEmailHead[i].u32SendTime.u8Month,pEmailHead[i].u32SendTime.u8Day,
            pEmailHead[i].u32SendTime.u8Hour,pEmailHead[i].u32SendTime.u8Min,pEmailHead[i].u32SendTime.u8Sec);

        
        pEmailHead[i].u8Importance = ptmpEmailHead[i].m_wImportance;
        pEmailHead[i].u8NewEmail = ptmpEmailHead[i].m_bNewEmail;

        memcpy(pEmailHead[i].u8EmailHead,ptmpEmailHead[i].m_szEmailHead,30+1);
        }

        return SKYCAS_CA_OK;
    }
    else if(CDCA_RC_POINTER_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] CDCASTB_GetEmailHeads pointe invalid!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] CDCASTB_GetEmailHeads unknown error!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pEmailHead)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetEmailHead(u32EmailId,(SCDCAEmailHead*)pEmailHead);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] u32EmailId = %d\n",__FUNCTION__,__LINE__,u32EmailId);
        return SKYCAS_CA_OK;
    }
    else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailHead tsid not find!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_DATA_NOT_FIND;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailHead unknown error,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }
    
}
SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if (SKYCAS_NULL == pEmailCont)
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    
    CDCA_U16 u16Ret = CDCA_RC_OK;

    SCDCAEmailContent pTmpEmailCont; //= (SCDCAEmailContent*)SKYCAS_Mem_Alloc(sizeof(SCDCAEmailContent));
    SKYCAS_Mem_set(&pTmpEmailCont,0,sizeof(SCDCAEmailContent));
    u16Ret = CDCASTB_GetEmailContent(u32EmailId,&pTmpEmailCont);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] u32EmailId = %d\n",__FUNCTION__,__LINE__,u32EmailId);
        pEmailCont->u32EmailLen = strlen(pTmpEmailCont.m_szEmail);
        SKYCAS_Mem_copy(pEmailCont->u8MailContent,pTmpEmailCont.m_szEmail,pEmailCont->u32EmailLen);
        return SKYCAS_CA_OK;
    }
    else if(CDCA_RC_POINTER_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailContents POINTER INVALID!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailContents tsid not find!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_DATA_NOT_FIND;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailContents unknown error,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }
    
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_DelEmail(SKYCAS_U32 u32EmailID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    
    SKYCAS_Debugger_Printf("[%s,%d] u32EmailID = %d\n",__FUNCTION__,__LINE__,u32EmailID);
    CDCASTB_DelEmail(u32EmailID);
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetEmailSpaceInfo( SKYCAS_U8 *pEmailNum,SKYCAS_U8 *pEmptyNum )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pEmailNum)||(SKYCAS_NULL == pEmptyNum))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetEmailSpaceInfo(pEmailNum,pEmptyNum);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] pEmailNum = %d,pEmptyNum = %d!\n",__FUNCTION__,__LINE__,*pEmailNum,*pEmptyNum);
        return SKYCAS_CA_OK;
    }
    else if(CDCA_RC_POINTER_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailSpaceInfo POINTER INVALID!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetEmailSpaceInfo unknown error,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
													SKYCAS_U8 *pIsChild,
													SKYCAS_U16 *pDelayTime,
													SKYCAS_U32 *pLastFeedTime,
													SKYCAS_U8 *pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed )
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pIsCanFeed)||(SKYCAS_NULL == pDelayTime)||(SKYCAS_NULL == pLastFeedTime)
        ||(SKYCAS_NULL == pParentCardSN)||(SKYCAS_NULL == pIsCanFeed))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }

    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    u16Ret = CDCASTB_GetOperatorChildStatus(&u32SMCHandle,u16TVSID,pIsChild,pDelayTime,(CDCA_TIME*)pLastFeedTime,(char*)pParentCardSN,(CDCA_BOOL*)pIsCanFeed);
    
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorChildStatus success\n");
        return SKYCAS_CA_OK;	
    }
    else if(CDCA_RC_POINTER_INVALID== u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetOperatorChildStatus POINTER INVALID!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_POINTER_INVALID;
    }
    else if(CDCA_RC_CARD_INVALID == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetOperatorChildStatus POINTER INVALID!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_CARD_INVALID;
    }
    else if(CDCA_RC_DATA_NOT_FIND == u16Ret)
    {
        SKYCAS_Debugger_Printf("[%s,%d] SKYCAS_GetOperatorChildStatus tsid not find!,u16Ret = 0x%x\n",__FUNCTION__,__LINE__,u16Ret);
        return SKYCAS_CA_DATA_NOT_FIND;
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_GetOperatorChildStatus FAIL,u16Ret = %d\n",u16Ret);
        return SKYCAS_CA_UNKNOWN;
    }

}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID, SKYCAS_U8 *pFeedData, SKYCAS_U8 *pLen)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pFeedData)||(SKYCAS_NULL == pLen))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    
    u16Ret = CDCASTB_ReadFeedDataFromParent(&u32SMCHandle,u16TVSID,pFeedData,pLen);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ReadFeedDataFromParent success\n");
        return SKYCAS_RET_SUCCESS;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_ReadFeedDataFromParent FAIL,u16Ret = %d\n",u16Ret);
        return SKYCAS_RET_FAILURE;
    }
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, const SKYCAS_U8 *pFeedData, SKYCAS_U8 u8Len)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pFeedData))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_POINTER_INVALID;
    }
    if(!bInitflg)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] you need first init ca lib\n",__FUNCTION__,__LINE__);
        return SKYCAS_CA_UNKNOWN;
    }
    
    CDCA_U16 u16Ret = CDCA_RC_OK;
    
    u16Ret = CDCASTB_WriteFeedDataToChild(&u32SMCHandle,u16TVSID,pFeedData,u8Len);
    if(CDCA_RC_OK == u16Ret)
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_WriteFeedDataToChild success\n");
        return SKYCAS_RET_SUCCESS;	
    }
    else
    {
        SKYCAS_Debugger_Printf("[CDCA]SKYCAS_WriteFeedDataToChild FAIL,u16Ret = %d\n",u16Ret);
        return SKYCAS_RET_FAILURE;
    }
}

SKYCAS_VOID CSkyworth_Cdca::SKYCAS_RefreshInterface(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    CDCASTB_RefreshInterface();
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pServ))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    mpTVService = pServ;
    SKYCAS_Debugger_Printf("[%s,%d] mpTVService is %p\n",__FUNCTION__,__LINE__,mpTVService);
    
    return SKYCAS_RET_SUCCESS;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetPinLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return CDCA_MAXLEN_PINCODE;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetCardNumLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return CDCA_MAXLEN_SN;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetSendNameMaxLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_SENDER_NAME;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetTitleMaxLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_TITLE;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetContentMaxLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return SKYCAS_MAXLEN_EMAIL_CONTENT;
}

SKYCAS_U16 CSkyworth_Cdca::SKYCAS_GetOSDMaxLen(SKYCAS_VOID)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    return CDCA_MAXLEN_OSD;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_PortingVendor(SKYCAS_U8 *pUser)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate)
{
	return SKYCAS_CA_UNKNOWN;
}

#if 0
SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum)
{
	return SKYCAS_CA_UNKNOWN;
}
#endif

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index)
{
	return SKYCAS_CA_UNKNOWN;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_PerformPairing(SKYCAS_U8 *pData)
{
	return SKYCAS_CA_UNKNOWN;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetCardType(SKYCAS_S32 *pType)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetPCCardType(SKYCAS_U8 *pCardID)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate)
{
	return SKYCAS_CA_UNKNOWN;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData)
{
	return SKYCAS_CA_UNKNOWN;
}

SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData)
{
	return SKYCAS_CA_UNKNOWN;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_GetFilePath(SKYCAS_CHAR *pFilePath)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if ((SKYCAS_NULL == pFilePath))
    {
        SKYCAS_Debugger_Printf("[%s,%d] para error!\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    if(strcmp((const char *)pCDcaFilePath,"") == 0)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d]file path is SKYCAS_NULL\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }

    strcpy(pFilePath, pCDcaFilePath);
    return SKYCAS_RET_SUCCESS;
}


SKYCAS_S32 CSkyworth_Cdca::SKYCAS_SetPrivateData(SKYCAS_U8 u8ReqID, const SKYCAS_U8* pU8Match, 
	const SKYCAS_U8* pU8Mask, SKYCAS_U8 u8FilterDeep, SKYCAS_U16 u16DataPid, SKYCAS_U8 byWaitSeconds)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    
    if(pU8Match == SKYCAS_NULL)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] pU8Match is null\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    if(pU8Mask == SKYCAS_NULL)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d]pU8Mask is null \n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    if(u8FilterDeep>=SKYCAS_FILTER_DEEP)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] u8FilterDeep > 16\n",__FUNCTION__,__LINE__);
        return SKYCAS_RET_FAILURE;
    }
    
    SKYCAS_U32 i = 0;
#if 1
{
    SKYCAS_Debugger_Printf("+++ %s  u8ReqID:%d pid:0x%X dep:%d TO:%d+++\n", __FUNCTION__,u8ReqID,u16DataPid,u8FilterDeep,byWaitSeconds);
    SKYCAS_Debugger_Printf("Match[%d]:",u8FilterDeep);
    for (i=0;i<u8FilterDeep;i++)
    SKYCAS_Debugger_Printf("%02x ", *(pU8Match+i));
    SKYCAS_Debugger_Printf("\n");
    SKYCAS_Debugger_Printf("Mask[%d]:",u8FilterDeep);
    for (i=0;i<u8FilterDeep;i++)
    SKYCAS_Debugger_Printf("%02x ", *(pU8Mask+i));
    SKYCAS_Debugger_Printf("\n");
}
#endif


    if (u16DataPid==0x10)
    {
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] NIT!!!\n",__FUNCTION__,__LINE__);
        //1 ToDo;
        //return SKYCAS_RET_SUCCESS;
    }

    SKYCAS_U32 logicFilterIndex = SKYCAS_U32_HANDLE_INVALID;
    
    SKYCAS_Mutex_lock(m_U32PriDataMutexId);
    
    for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)  //如果有过滤器正在使用并且过滤条件相同则不处理
    {
        if(SKYCAS_TRUE == IsSame(i,u8ReqID,u16DataPid,pU8Match,pU8Mask,u8FilterDeep))
        {
            m_U32FilterIdArray[i].m_bUsed = SKYCAS_TRUE;
            m_U32FilterIdArray[i].m_WaitSecond = byWaitSeconds;
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
        SKYCAS_Debugger_Printf("[CDCA][%s,%d] all filter is in use\n",__FUNCTION__,__LINE__);
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
        SKYCAS_Debugger_Printf("[%s,%d]Alloc filter fail!reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, u8ReqID, u16DataPid);
        return SKYCAS_RET_FAILURE;		
    }
    //SKYCAS_Debugger_Printf("\n ljc the new filter id is %d \n",U32FilterId);

    FilterPara.SKY_CAS_FILTER_EVENT_CB = &SKYCAS_FilterEvent_CB;
    FilterPara.u16FilterDataPid = u16DataPid;
    FilterPara.u32FilterTimeout = byWaitSeconds*1000;
    FilterPara.u8FilterDeep = u8FilterDeep;
    if(SKYCAS_CDCAS_FILTER_ONESHOT_JUDGE(u8ReqID))
    {
        FilterPara.eFilterMode = SKYCAS_FILTER_MODE_ONESHOT;
        SKYCAS_Mem_copy(FilterPara.u8ArrFilterMacth,pU8Match,u8FilterDeep);
        SKYCAS_Mem_copy(FilterPara.u8ArrFilterMask,pU8Mask,u8FilterDeep);
        //SKYCAS_Mem_set(FilterPara.u8ArrFilterNeg,0,u8FilterDeep);
    }
    else
    {
        FilterPara.eFilterMode = SKYCAS_FILTER_MODE_CONTINUE;
        FilterPara.u8ArrFilterMacth[0] = pU8Match[0];
        FilterPara.u8ArrFilterMask[0] = pU8Mask[0];
        SKYCAS_Mem_copy(&(FilterPara.u8ArrFilterMacth[3]),&(pU8Match[1]),u8FilterDeep-1);
        SKYCAS_Mem_copy(&(FilterPara.u8ArrFilterMask[3]),&(pU8Mask[1]),u8FilterDeep-1);
        //SKYCAS_Mem_set(FilterPara.u8ArrFilterNeg,0,u8FilterDeep);
        FilterPara.u8FilterDeep = u8FilterDeep+2;
    }

    
    s32Ret = SKYCAS_Filter_Set(U32FilterId,&FilterPara);
    if(SKYCAS_RET_SUCCESS != s32Ret)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Filter_Free(U32FilterId);
        SKYCAS_Debugger_Printf("[%s,%d]Set filter fail!reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__, u8ReqID, u16DataPid,U32FilterId);
        return SKYCAS_RET_FAILURE;		
    }
    s32Ret = SKYCAS_Filter_Start(U32FilterId);
    if(SKYCAS_RET_SUCCESS != s32Ret)
    {
        SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
        SKYCAS_Filter_Free(U32FilterId);
        SKYCAS_Debugger_Printf("[%s,%d]Start filter fail!reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__, u8ReqID, u16DataPid,U32FilterId);
        return SKYCAS_RET_FAILURE;		
    }

    m_U32FilterIdArray[logicFilterIndex].m_RequestId = u8ReqID;
    m_U32FilterIdArray[logicFilterIndex].m_U32FilterId = U32FilterId;
    m_U32FilterIdArray[logicFilterIndex].m_StartTimeTicks = SKYCAS_Time_Get_Times();
    m_U32FilterIdArray[logicFilterIndex].m_Pid = u16DataPid;
    m_U32FilterIdArray[logicFilterIndex].m_WaitSecond = byWaitSeconds;
    m_U32FilterIdArray[logicFilterIndex].m_Deep = FilterPara.u8FilterDeep;
    m_U32FilterIdArray[logicFilterIndex].m_bUsed = SKYCAS_TRUE;
    SKYCAS_Mem_copy(m_U32FilterIdArray[logicFilterIndex].match,pU8Match,FilterPara.u8FilterDeep);
    SKYCAS_Mem_copy(m_U32FilterIdArray[logicFilterIndex].mask,pU8Mask,FilterPara.u8FilterDeep);
    SKYCAS_Mem_set(m_U32FilterIdArray[logicFilterIndex].negate,0,FilterPara.u8FilterDeep);

    SKYCAS_Mutex_unlock(m_U32PriDataMutexId);
    SKYCAS_Debugger_Printf("[%s,%d]Start filter data... index:%d reqId:%d, pid:0x%X, filterId:0x%X\n",__FUNCTION__,__LINE__,logicFilterIndex, u8ReqID, u16DataPid,U32FilterId);

#if 1
    SKYCAS_Debugger_Printf("[%s,%d]TIMECOUNT:%d, second:%d!!!\n",__FUNCTION__,__LINE__, SKYCAS_Time_Get_Times(), byWaitSeconds);
    for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
    {
        SKYCAS_Debugger_Printf("[%d]:m_RequestId[%d] U32FilterId[0x%X] pid[0x%X]\n",i,m_U32FilterIdArray[i].m_RequestId,m_U32FilterIdArray[i].m_U32FilterId,m_U32FilterIdArray[i].m_Pid);
    }
#endif

    return SKYCAS_RET_SUCCESS;
}


/*当(SKYCAS_U8_HANDLE_INVALID==u8ReqID)时表示只根据PID来释放过滤器，否则必须判断u8ReqID和PID*/
SKYCAS_S32 CSkyworth_Cdca::SKYCAS_ReleasePrivateDataFilter(SKYCAS_U8 u8ReqID,SKYCAS_U16 wPid)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_Debugger_Printf("[%s,%d]reqId:%d,pid:0x%X\n",__FUNCTION__,__LINE__, u8ReqID,wPid);

    return SKYCAS_ReleaseFilter(u8ReqID,wPid);
}


SKYCAS_VOID CSkyworth_Cdca::CallbackSmartcardProcess(SKYCAS_VOID)
{
	SKYCAS_Debugger_Printf("[CDCA]CallbackSmartcardProcess in\n");
	SKYCAS_Semp_Release(m_ScSemaphore);
}

SKYCAS_VOID CSkyworth_Cdca::CAPrivateDataTask(SKYCAS_VOID* param)
{
    SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    SKYCAS_U32 i = 0;
    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;
    SKYCAS_U32 U32FilterId = SKYCAS_U32_HANDLE_INVALID;
    SKYCAS_U8 u8RequestId = SKYCAS_U8_HANDLE_INVALID;
    SKYCAS_U16 u16Pid = SKYCAS_PID_INVALID;
    SKYCAS_U32 sectionlength = sizeof(SKYCAS_CdcaSectionMessage_S);
    CSkyworth_Cdca* pCDCA = CSkyworth_Cdca::GetCaInstance();

    SKYCAS_CdcaSectionMessage_S Msg;
    Msg.m_FilterEvent = SKYCAS_FILTER_EVENT_INVALID;
    Msg.m_FilterId = SKYCAS_U32_HANDLE_INVALID;
    Msg.m_data = SKYCAS_NULL;
    Msg.m_length = 0;
    
    while (SKYCAS_FALSE == pCDCA->m_PriDataTaskStoped)
    {
        s32Ret = SKYCAS_Queue_RecvMsg(pCDCA->m_u32SectionMsgQueueId,(SKYCAS_U8 *)&Msg,&sectionlength,SKYCAS_U32_HANDLE_INVALID);
        if (SKYCAS_RET_SUCCESS != s32Ret)
        {
            //SKYCAS_Debugger_Printf("[CDCA][%s,%d]SKYCAS_Queue_RecvMsg fail\n",__FUNCTION__,__LINE__);
            continue;
        }
        SKYCAS_Debugger_Printf("\nMsg.m_length %d Msg.m_FilterId %d Msg.m_FilterEvent %d  \n",Msg.m_length,Msg.m_FilterId,Msg.m_FilterEvent);

        U32FilterId = Msg.m_FilterId;
        SKYCAS_Mutex_lock(pCDCA->m_U32PriDataMutexId);
        for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
        {
            //SKYCAS_Debugger_Printf("ljc pCDCA->m_U32FilterIdArray[i].m_bUsed %d U32FilterId %d i %d\n",pCDCA->m_U32FilterIdArray[i].m_bUsed,U32FilterId,i);
            if((SKYCAS_TRUE == pCDCA->m_U32FilterIdArray[i].m_bUsed)&&(U32FilterId == pCDCA->m_U32FilterIdArray[i].m_U32FilterId)
                && (SKYCAS_TRUE == pCDCA->IsSame(i,pCDCA->m_U32FilterIdArray[i].m_RequestId,pCDCA->m_U32FilterIdArray[i].m_Pid,pCDCA->m_U32FilterIdArray[i].match,pCDCA->m_U32FilterIdArray[i].mask,8)))
            {
                u8RequestId = pCDCA->m_U32FilterIdArray[i].m_RequestId;
                u16Pid = pCDCA->m_U32FilterIdArray[i].m_Pid;
                break;
            }
        }
        SKYCAS_Mutex_unlock(pCDCA->m_U32PriDataMutexId);

        if(i>=SKYCAS_MAX_FILTER_COUNT)
        {
            SKYCAS_Debugger_Printf("[%s,%d]Got msg, but can't find filter[%d]:reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, U32FilterId, u8RequestId, u16Pid);
            for(i=0;i<SKYCAS_MAX_FILTER_COUNT;i++)
            {
                SKYCAS_Debugger_Printf("[%d]:m_bUsed[%d] U32FilterId[0x%X] pid[0x%X]\n",i,pCDCA->m_U32FilterIdArray[i].m_bUsed,pCDCA->m_U32FilterIdArray[i].m_U32FilterId,pCDCA->m_U32FilterIdArray[i].m_Pid);
            }
            continue;
        }
        
        if(SKYCAS_FILTER_EVENT_READY == Msg.m_FilterEvent)
        {
        #if 1
            if (0x10 == u16Pid)
            {
                SKYCAS_Debugger_Printf("[%s,%d]Got NIT! filter[%d]:reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, U32FilterId, u8RequestId, u16Pid);
            }
        #endif
            if(SKYCAS_CDCAS_FILTER_ONESHOT_JUDGE(u8RequestId))
            {
                pCDCA->SKYCAS_ReleasePrivateDataFilter(u8RequestId,u16Pid);
            }
            if ((SKYCAS_NULL != Msg.m_data)&&(0 != Msg.m_length))
            {
                CDCASTB_PrivateDataGot(u8RequestId,CDCA_FALSE,u16Pid,(CDCA_U8 *)Msg.m_data,(CDCA_U16)Msg.m_length);
            }
            else
            {
                SKYCAS_Debugger_Printf("[%s,%d]Got dat, but ther buffer len[%d]! filter[%d]:reqId:%d, pid:0x%X\n",__FUNCTION__,__LINE__, Msg.m_length,U32FilterId, u8RequestId, u16Pid);
                continue;
            }
        }
        else
        {
            if(SKYCAS_CDCAS_FILTER_ONESHOT_JUDGE(u8RequestId))
            {
                pCDCA->SKYCAS_ReleasePrivateDataFilter(u8RequestId,u16Pid);
            }
            CDCASTB_PrivateDataGot(u8RequestId,CDCA_TRUE,u16Pid,SKYCAS_NULL,0);
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
    }
}

SKYCAS_BOOL CSkyworth_Cdca::IsSame(SKYCAS_U8 index,SKYCAS_U8 reqId,SKYCAS_U16 pid,const SKYCAS_U8 *filterData,const SKYCAS_U8 *filterMask,SKYCAS_U8 filterLen)
{
    //SKYWORTH_TFCA_FUNC_ENTRY(FLAG_SKYWORTH_TFCA_FUNC_ENTRY);
    if(filterData == SKYCAS_NULL)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filterData is null\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }
    
    if(filterMask == SKYCAS_NULL)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filterMask is null\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }
    
    if(filterLen>=SKYCAS_FILTER_DEEP)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filterLen > 16\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }
    
    if(index >= SKYCAS_MAX_FILTER_COUNT)
    {
        SKYCAS_Debugger_Printf("[DVNCA][%s,%d] filter count > 12\n",__FUNCTION__,__LINE__);
        return SKYCAS_FALSE;
    }

    if(m_U32FilterIdArray[index].m_RequestId!=reqId) return SKYCAS_FALSE;	
    if(m_U32FilterIdArray[index].m_Pid!=pid) return SKYCAS_FALSE;	
    if(memcmp(filterData,m_U32FilterIdArray[index].match,filterLen)!=0) return SKYCAS_FALSE;	
    if(memcmp(filterMask,m_U32FilterIdArray[index].mask,filterLen)!=0) return SKYCAS_FALSE;

    return SKYCAS_TRUE;
}


