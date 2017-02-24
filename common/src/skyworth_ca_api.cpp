//<Skyworth Software>
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////////////


#include "skycas_datatype.h"
#include "skyworth_ca_api.h"
#include "skycas_driver.h"
#if (CASCD)
#include "skyworth_tfca.h"
#endif
#if (CASDVN)
#include "skyworth_dvnca.h"
#endif


skyworth_ca_api * skyworth_ca_api::pSkyworthCAApi = SKYCAS_NULL;
skyworth_ca_api::skyworth_ca_api()
{
	return ;
}

skyworth_ca_api::~skyworth_ca_api()
{
	return ;
}

skyworth_ca_api* skyworth_ca_api::GetCaInstance(SKYCAS_VOID)
{
#if (CASCD)
    CSkyworth_Cdca * pCdcas = SKYCAS_NULL;
    pCdcas = CSkyworth_Cdca::GetCaInstance();	
    if(SKYCAS_NULL != pCdcas)
    {
        pSkyworthCAApi = dynamic_cast<skyworth_ca_api *>(pCdcas);		
        return pSkyworthCAApi;
    }
#endif
#if (CASDVN)
        CSkyworth_Dvnca * pDvncas = SKYCAS_NULL;
        pDvncas = CSkyworth_Dvnca::GetCaInstance();   
        if(SKYCAS_NULL != pDvncas)
        {
            pSkyworthCAApi = dynamic_cast<skyworth_ca_api *>(pDvncas);       
            return pSkyworthCAApi;
        }
#endif
    return SKYCAS_NULL;
}
SKYCAS_VOID skyworth_ca_api::DestroyInstance(SKYCAS_VOID)
{
#if (CASCD)
    CSkyworth_Cdca::DestroyInstance();
#endif
#if (CASDVN)
    CSkyworth_Dvnca::DestroyInstance();   
#endif
}

SKYCAS_U32 skyworth_ca_api::SKYCAS_GetCAType(SKYCAS_VOID)
{
    return u32CAType;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_CaInit(SKYCAS_INIT_PARA_S *pInitPara)
{
	return SKYCAS_RET_SUCCESS;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_CaDeInit(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;
}

SKYCAS_BOOL skyworth_ca_api::SKYCAS_CheckSystem(SKYCAS_U16 u16SystemID)
{
	return SKYCAS_FALSE;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_SetEmmPid(SKYCAS_U16 u16EmmPid)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_DoUpdateEmm(SKYCAS_U8 *pbuffer)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_SetEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
	return SKYCAS_RET_SUCCESS;

}
SKYCAS_S32 skyworth_ca_api::SKYCAS_StopEcmPid(SKYCAS_SERVICEINFO_S* pServiceInfo)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_SetNITData(SKYCAS_U8 *DataBuf)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetCardSN(SKYCAS_U8* pCardSN)
{
	return SKYCAS_CA_OK;

}


SKYCAS_S32 skyworth_ca_api::SKYCAS_ChangePin( const SKYCAS_U8* pOldPin, const SKYCAS_U8* pNewPin)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_VerifyPin(const SKYCAS_PININFO_S *pPIN)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_IsPinLocked(SKYCAS_BOOL * bLocked)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_SetRating( const SKYCAS_U8* pPin,SKYCAS_U8 u8Rating )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetRating( SKYCAS_U8* pRating )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_SetWorkTime( const SKYCAS_U8* pPin,SKYCAS_DATETIME_S *StartTime,SKYCAS_DATETIME_S *EndTime)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetWorkTime(SKYCAS_DATETIME_S *stStartTime,SKYCAS_DATETIME_S *stEndTime)
{
	return SKYCAS_CA_OK;

}

SKYCAS_U32 skyworth_ca_api::SKYCAS_GetVer(SKYCAS_U8 *pData)
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetPlatformID(SKYCAS_VOID)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetOperatorIds( SKYCAS_U16* pTVSID )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetOperatorInfo( SKYCAS_U16 u16TVSID, SKYCAS_U8 * pCount, SKYCAS_OPERATORINFO_S* stOperatorInfo )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetACList( SKYCAS_U16 u16TVSID, SKYCAS_U32* pACArray )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetSlotIDs( SKYCAS_U16 u16TVSID, SKYCAS_U8* pSlotID)//, SKYCAS_U32 *pSlotNum)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetSlotInfo( SKYCAS_U16  u16TVSID,SKYCAS_U8 u8SlotID,SKYCAS_TVSLOTINFO_S* pSlotInfo )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetServiceEntitles( SKYCAS_U16 u16TVSID, SKYCAS_Entitles* pServiceEntitles )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetEntitleIDs( SKYCAS_U16  u16TVSID,SKYCAS_U32* pEntitleId )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetDetitleChkNums( SKYCAS_U16 u16TVSID,SKYCAS_BOOL *pReadFlag,SKYCAS_U32 *pDetitleChkNums)
{
	return SKYCAS_CA_OK;

}

SKYCAS_BOOL skyworth_ca_api::SKYCAS_GetDetitleReaded( SKYCAS_U16 u16TVSID)
{
	return SKYCAS_TRUE;

}

SKYCAS_BOOL skyworth_ca_api::SKYCAS_DelDetitleChkNum( SKYCAS_U16 u16TVSID,SKYCAS_U32 u32DetitleChkNum )
{
	return SKYCAS_TRUE;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_IsPaired( SKYCAS_U8* pNum, SKYCAS_U8* pSTBSerial )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_StopIPPVBuyDlg(SKYCAS_BOOL bBuyProgram, SKYCAS_U16 u16EcmPid, const SKYCAS_U8 *pbyPinCode,const SKYCAS_IPPV_PRICE_S *pPrice )
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetIPPVProgram(SKYCAS_U16 u16TVSID,SKYCAS_IPPVINFO_S *pIppv,SKYCAS_U16 *u16Number)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetEmailHeads(SKYCAS_EMAILHEADINFO_S* pEmailHead, SKYCAS_U8* pCount, SKYCAS_U8* pIndex)
{
	return SKYCAS_CA_OK;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetEmailHead(SKYCAS_U32 u32EmailId,SKYCAS_EMAILHEADINFO_S* pEmailHead)
{
    return SKYCAS_CA_OK;
    
}
SKYCAS_S32 skyworth_ca_api::SKYCAS_GetEmailContents(SKYCAS_U32 u32EmailId,SKYCAS_EMAILCONTENT_S* pEmailCont)
{
    return SKYCAS_CA_OK;
    
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_DelEmail(SKYCAS_U32 u32EmailID)
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetEmailSpaceInfo( SKYCAS_U8 *pEmailNum,SKYCAS_U8 *pEmptyNum )
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetOperatorChildStatus( SKYCAS_U16 u16TVSID,
													SKYCAS_U8 *pIsChild,
													SKYCAS_U16 *pDelayTime,
													SKYCAS_U32*pLastFeedTime,
													SKYCAS_U8 *pParentCardSN,
													SKYCAS_BOOL *pIsCanFeed )
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_ReadFeedDataFromParent( SKYCAS_U16 u16TVSID, SKYCAS_U8 *pFeedData, SKYCAS_U8 *pLen)
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_WriteFeedDataToChild( SKYCAS_U16 u16TVSID, const SKYCAS_U8 *pFeedData, SKYCAS_U8 u8Len)
{
	return SKYCAS_CA_OK;

}

SKYCAS_VOID skyworth_ca_api::SKYCAS_RefreshInterface(SKYCAS_VOID)
{
	return ;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_RegisterService(SKYCAS_PostEvent_CB pServ)
{
	return SKYCAS_CA_OK;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetPinLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetCardNumLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetSendNameMaxLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetTitleMaxLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetContentMaxLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_U16 skyworth_ca_api::SKYCAS_GetOSDMaxLen(SKYCAS_VOID)
{
	return SKYCAS_RET_SUCCESS;

}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetSTBAttribute(SKYCAS_U8 *pSTBAtr)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetAccountNo(SKYCAS_U8 *pAcNo)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetSysDescriptor (SKYCAS_U8 u8index,SKYCAS_U8 *pSysDes)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetSmartCardVersion(SKYCAS_U8 *pSCVer)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_PortingVendor(SKYCAS_U8 *pUser)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_CALIBBuildDate(SKYCAS_U8 *pDate)
{
	return SKYCAS_CA_OK;
}

#if 0
SKYCAS_S32 skyworth_ca_api::SKYCAS_GetEntitleNum(SKYCAS_S32 s32TVSID,SKYCAS_S32 *pNum)
{
	return SKYCAS_CA_OK;
}
#endif 

SKYCAS_S32 skyworth_ca_api::SKYCAS_ChangeSlot(SKYCAS_U8 u8Flag,SKYCAS_U8 u8Index)
{
	return SKYCAS_CA_OK;
}


SKYCAS_S32 skyworth_ca_api::SKYCAS_DeductSlot(SKYCAS_FLOAT fPrice,SKYCAS_U8 u8Type,SKYCAS_U32 u32ProgID)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetProgramStatus(SKYCAS_S32 s32ProductID)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_PerformPairing(SKYCAS_U8 *pData)
{
	return SKYCAS_CA_OK;
}


SKYCAS_S32 skyworth_ca_api::SKYCAS_GetPCCardType(SKYCAS_S32 *pType)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetPCardID(SKYCAS_U8 *pCardID)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetCardExpiryDate(SKYCAS_U8 u8Flag,SKYCAS_DATETIME_S *stExpiryDate)
{
	return SKYCAS_CA_OK;
}


SKYCAS_S32 skyworth_ca_api::SKYCAS_AddIPPV(SKYCAS_S32 s32ProuctID,SKYCAS_FLOAT fPrice,SKYCAS_DATETIME_S *stStartTime,
											SKYCAS_DATETIME_S *stEndTime,SKYCAS_DATETIME_S *stTrasTime,SKYCAS_U8 *pPin)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetTransRecordNum(SKYCAS_U8 *pData)
{
	return SKYCAS_CA_OK;
}

SKYCAS_S32 skyworth_ca_api::SKYCAS_GetTransRecord(SKYCAS_U16 u16Num,SKYCAS_U8 *pData)
{
	return SKYCAS_CA_OK;
}


SKYCAS_U32 skyworth_ca_api::SKYCAS_ReadCAType(SKYCAS_VOID)
{
    //从flash中读取CA类型
    u32CAType = SKYCAS_TYPE_DVN;
    return u32CAType;
}


#if FLAG_SKYCAS_RESOURCE_RECYCLING

SKYCAS_RESOURCE_T s_u32ArrSkycasResource[SKYCAS_RESOURCE_NUM_MAX];

SKYCAS_VOID SKYCAS_Lib_Resource_Init(SKYCAS_VOID)
{
    SKYCAS_U16 i=0;
    for (i=0;i<SKYCAS_RESOURCE_NUM_MAX;++i)
    {
        s_u32ArrSkycasResource[i].eType = SKYCAS_RESOURCE_INVALID;
        s_u32ArrSkycasResource[i].u32Id = SKYCAS_U32_HANDLE_INVALID;
    }
}
SKYCAS_VOID SKYCAS_Lib_Resource_Set(SKYCAS_RESOURCE_TYPE_E eType ,SKYCAS_U32 u32Id)
{
    SKYCAS_U16 i=0;
    for (i=0;i<SKYCAS_RESOURCE_NUM_MAX;++i)
    {
        if ((SKYCAS_RESOURCE_INVALID == s_u32ArrSkycasResource[i].eType)
            && (SKYCAS_U32_HANDLE_INVALID == s_u32ArrSkycasResource[i].u32Id))
        {
            s_u32ArrSkycasResource[i].eType = eType;
            s_u32ArrSkycasResource[i].u32Id = u32Id;
            break;
        }
    }
}

SKYCAS_VOID SKYCAS_Lib_Resource_Clear(SKYCAS_RESOURCE_TYPE_E eType ,SKYCAS_U32 u32Id)
{
    SKYCAS_U16 i=0;
    for (i=0;i<SKYCAS_RESOURCE_NUM_MAX;++i)
    {
        if ((eType == s_u32ArrSkycasResource[i].eType)
            && (u32Id == s_u32ArrSkycasResource[i].u32Id))
        {
            s_u32ArrSkycasResource[i].eType = SKYCAS_RESOURCE_INVALID;
            s_u32ArrSkycasResource[i].u32Id = SKYCAS_U32_HANDLE_INVALID;
            break;
        }
    }
}

SKYCAS_VOID SKYCAS_Lib_Resource_Release_All(SKYCAS_VOID)
{
    SKYCAS_U16 i=0;
    SKYCAS_S32 s32Ret = SKYCAS_RET_SUCCESS;

    for (i=0;i<SKYCAS_RESOURCE_NUM_MAX;++i)
    {
        if ((SKYCAS_RESOURCE_INVALID != s_u32ArrSkycasResource[i].eType)
            && (SKYCAS_U32_HANDLE_INVALID != s_u32ArrSkycasResource[i].u32Id))
        {
            SKYCAS_Debugger_Printf("[%s,%d]Destory:%d---> Type:%d, Id:%d\n",__FUNCTION__,__LINE__, i,s_u32ArrSkycasResource[i].eType,s_u32ArrSkycasResource[i].u32Id);
            switch(s_u32ArrSkycasResource[i].eType)
            {
                case SKYCAS_RESOURCE_TASK:
                    s32Ret = SKYCAS_Task_Destory(s_u32ArrSkycasResource[i].u32Id);
                    break;
                case SKYCAS_RESOURCE_SEMAP:
                    s32Ret = SKYCAS_Semp_Destory(s_u32ArrSkycasResource[i].u32Id);
                    break;
                case SKYCAS_RESOURCE_MUTEX:
                    s32Ret = SKYCAS_Mutex_Destory(s_u32ArrSkycasResource[i].u32Id);
                    break;
                case SKYCAS_RESOURCE_QUEUE:
                    s32Ret = SKYCAS_Queue_Destory(s_u32ArrSkycasResource[i].u32Id);
                    break;
                default:
                    s32Ret = SKYCAS_RET_SUCCESS;
                    break;
            }
            
            if (SKYCAS_RET_SUCCESS != s32Ret)
            {
                SKYCAS_Debugger_Printf("[%s,%d]Error! Destory fail! Type:%d, Id:%d\n",__FUNCTION__,__LINE__, s_u32ArrSkycasResource[i].eType,s_u32ArrSkycasResource[i].u32Id);
            }

            s_u32ArrSkycasResource[i].eType = SKYCAS_RESOURCE_INVALID;
            s_u32ArrSkycasResource[i].u32Id = SKYCAS_U32_HANDLE_INVALID;
        }
    }
}
#endif

