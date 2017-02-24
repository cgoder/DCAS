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
	SKYCAS_SMC_EVENT_IN = 0,		/*卡插入事件*/
	SKYCAS_SMC_EVENT_OUT,		/*卡拔出事件*/
	SKYCAS_SMC_EVENT_INVALID	/*无效*/
} SKYCAS_SMC_EVENT_E;


typedef enum skycas_smc_status_e
{
	SKYCAS_SMC_STATUS_NOCARD = 0,	/*未检测到卡*/
	SKYCAS_SMC_STATUS_BUSY,		/*卡正忙/正在初始化/收发数据*/
	SKYCAS_SMC_STATUS_ACTIVE,	/*卡已正确初始化完成*/
	SKYCAS_SMC_STATUS_INACTIVE,	/*卡不能正确初始化/无效卡*/
	SKYCAS_SMC_STATUS_FAIL,		/*获取状态失败/未知状态*/
	SKYCAS_SMC_STATUS_INVALID	/*无效*/
} SKYCAS_SMC_STATUS_E;
typedef enum skycas_smc_mode_e
{
	SKYCAS_SMC_MODE_T0 = 0,	/*T0协议*/
	SKYCAS_SMC_MODE_T1,
	SKYCAS_SMC_MODE_T14,
	SKYCAS_SMC_MODE_INVALID	/*无效*/
}SKYCAS_SMC_MODE_E;

/******************************************************************************************
	智能卡回调函数定义。
******************************************************************************************/
typedef	SKYCAS_VOID (*SKY_CAS_SMC_EVENT_CB)(SKYCAS_U8 cardSlot, SKYCAS_SMC_EVENT_E eSmcEvent);

/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_Init(SKYCAS_U8 cardSlot)
功能:
	智能卡模块初始化。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
	[OUT]
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Init(SKYCAS_U8 cardSlot);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_Deinit(SKYCAS_U8 cardSlot)
功能:
	智能卡模块去初始化。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Deinit(SKYCAS_U8 cardSlot);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_Open(SKYCAS_U8 cardSlot, SKYCAS_SMC_MODE_E smcOpenMode)
功能:
	打开智能卡模块。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
		SKYCAS_SMC_MODE_E smcOpenMode	智能卡初始化协议。T0/T14/T1等。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Open(SKYCAS_U8 cardSlot, SKYCAS_SMC_MODE_E smcOpenMode);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_Close(SKYCAS_U8 cardSlot)
功能:
	关闭智能卡模块。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Close(SKYCAS_U8 cardSlot);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_Reset(SKYCAS_U8 cardSlot)
功能:
	复位智能卡模块。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_Reset(SKYCAS_U8 cardSlot);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_SendRecv (SKYCAS_U8 cardSlot, const SKYCAS_U8* const pU8SendData, SKYCAS_U16* const pU16SendLen, 
								SKYCAS_U8* const pU8RecvData, SKYCAS_U16* const pU16RecvLen);
功能:
	智能卡通讯/收发数据。
参数:
	[IN]	
		SKYCAS_U8 cardSlot					智能卡插槽通道号。单卡槽机器默认为0。
		const SKYCAS_U8* const pU8SendData		数组指针。发送数据缓冲区地址。
		SKYCAS_U16* const pU16SendLen			变量指针。该变量存储需要发送数据的字节长度。
		SKYCAS_U8* const pU8RecvData			数组指针。接收数据缓冲区地址。
		SKYCAS_U16* const pU16RecvLen			变量指针。该变量存储最大可接收的数据字节长度。
	[OUT]	
		SKYCAS_U16* const pU16SendLen			变量指针。该变量存储实际已发送数据的字节长度。
		SKYCAS_U16* const pU16RecvLen			变量指针。该变量存储实际已接收的数据字节长度。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_SendRecv (SKYCAS_U8 cardSlot, const SKYCAS_U8* const pU8SendData, SKYCAS_U16* const pU16SendLen, 
											SKYCAS_U8* const pU8RecvData, SKYCAS_U16* const pU16RecvLen);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_GetATR (SKYCAS_U8 cardSlot, SKYCAS_U8* const pU8AtrData, SKYCAS_U16* const pU16AtrLen);
功能:
	获取智能卡ATR返回值。
参数:
	[IN]	
		SKYCAS_U8 cardSlot			智能卡插槽通道号。单卡槽机器默认为0。
		SKYCAS_U8* pU8AtrData const	数组指针。ATR值缓冲区地址。
		SKYCAS_U16* pU16AtrLen const	变量指针。该变量存储最大可接收的数据字节长度。
	[OUT]	
		SKYCAS_U16* pU16AtrLen const	变量指针。该变量存储实际收到的数据字节长度。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_GetATR (SKYCAS_U8 cardSlot, SKYCAS_U8* const pU8AtrData, SKYCAS_U16* const pU16AtrLen);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_SMC_RegistCB(SKYCAS_U8 cardSlot, SKY_CAS_SMC_EVENT_CB pFnSmcCB)
功能:
	注册智能卡回调函数。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
		SKY_CAS_SMC_EVENT_CB pFnSmcCB	函数指针。指向由上层传递过来的智能卡事件回调函数。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_SMC_RegistCB(SKYCAS_U8 cardSlot, SKY_CAS_SMC_EVENT_CB pFnSmcCB);
/******************************************************************************************
函数名:	
	SKYCAS_SMC_STATUS_E SKYCAS_SMC_GetStatus(SKYCAS_U8 cardSlot)
功能:
	获取智能卡状态。
参数:
	[IN]	
		SKYCAS_U8 cardSlot	智能卡插槽通道号。单卡槽机器默认为0。
	[OUT]	
		无。
	[RET]		
		返回枚举值SKYCAS_SMC_STATUS_E。
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
	SKYCAS_U16 u16FilterDataPid;	/*过滤器过滤的ECMPID数据*/
	SKYCAS_FILTER_MODE_E eFilterMode;
	SKYCAS_U8 u8FilterDeep;		/*过滤器深度*/
	SKYCAS_U32 u32FilterTimeout;	/*过滤器超时时间。单位毫秒。为0时代表一直等待不需要超时*/
	SKYCAS_U8 u8ArrFilterMacth[SKYCAS_FILTER_DEEP];	/*Match*/
	SKYCAS_U8 u8ArrFilterMask[SKYCAS_FILTER_DEEP];	/*Mask*/
	SKYCAS_U8 u8ArrFilterNeg[SKYCAS_FILTER_DEEP];	/*Negate*/
	SKYCAS_S32 (*SKY_CAS_FILTER_EVENT_CB)(SKYCAS_U32 u32FilterId,  const SKYCAS_U8* const pU8Buffer, SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);	/*过滤器回调函数指针*/
}SKYCAS_FILTER_PARA_S;

/******************************************************************************************
	过滤器回调函数定义。返回值预保留，现阶段直接返回SKYCAS_RET_SUCCESS
******************************************************************************************/
typedef	SKYCAS_S32 (*SKY_CAS_FILTER_EVENT_CB)(SKYCAS_U32 u32FilterId, const SKYCAS_U8* const pU8Buffer, 
															SKYCAS_U32 u32BufferLen, SKYCAS_FILTER_EVENT_E eFilterEvent);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Filter_Alloc(SKYCAS_U32* const pU32FilterId, SKYCAS_U32 u32FilterSectionSize)
功能:
	分配过滤器。
参数:
	[IN]	
		SKYCAS_U32* const pU32FilterId		变量指针。存储默认值。
		SKYCAS_FILTER_SECTION_SIZE_E eFilterSectionSize		过滤器属性之一。表示过滤器section大小。
	[OUT]	
		SKYCAS_U32* const pU32FilterId		变量指针。存储分配到的过滤器ID。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Alloc(SKYCAS_U32* const pU32FilterId, SKYCAS_FILTER_SECTION_SIZE_E eFilterSectionSize);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Filter_Free(SKYCAS_U32 u32FilterId)
功能:
	释放过滤器。
参数:
	[IN]	
		SKYCAS_U32 u32FilterId	过滤器ID。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Free(SKYCAS_U32 u32FilterId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Filter_Start(SKYCAS_U32 u32FilterId)
功能:
	启动过滤器。
参数:
	[IN]	
		SKYCAS_U32 u32FilterId	过滤器ID。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Start(SKYCAS_U32 u32FilterId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Filter_Stop(SKYCAS_U32 u32FilterId)
功能:
	停止过滤器。
参数:
	[IN]	
		SKYCAS_U32 u32FilterId	过滤器ID。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Filter_Stop(SKYCAS_U32 u32FilterId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Filter_Set(SKYCAS_U32 u32FilterId, const SKYCAS_FILTER_PARA_S* const pFilterPara);
功能:
	设置过滤器属性。
参数:
	[IN]	
		SKYCAS_U32 u32FilterId	过滤器ID。
		const SKYCAS_FILTER_PARA_S* const pFilterPara	结构体指针。成员属性参见结构体SKYCAS_FILTER_PARA_S。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
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
函数名:	
	SKYCAS_S32 SKYCAS_Des_SetKey(SKYCAS_U16 u16EcmPid, const SKYCAS_U8* const pU8OddKey, const SKYCAS_U8* const pU8EvenKey);
功能:
	设置控制字。
参数:
	[IN]	
		SKYCAS_U16 u16EcmPid				控制字对应的EcmPid。
		const SKYCAS_U8* const pU8OddKey	数组指针。指向奇控制字串。
		const SKYCAS_U8* const pU8EvenKey	数组指针。指向偶控制字串。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
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
函数名:	
	SKYCAS_S32 SKYCAS_Flash_Read(SKYCAS_U32 readAddr, SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
功能:
	读Flash数据(扇区地址型)
参数:
	[IN]	
		SKYCAS_U32 readAddr			Flash地址。
		SKYCAS_U32* const pReadLen		变量指针。该变量存储需要读取的数据字节长度。
		SKYCAS_U8* const pReadBuf		数组指针。该数组存储读取到的数据。
	[OUT]	
		SKYCAS_U8* const pReadLen		变量指针。该变量存储实际读取到的数据字节长度。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Flash_Read(SKYCAS_U32 readAddr, SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Flash_Write(SKYCAS_U32 writeAddr, SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
功能:
	写Flash数据(扇区地址型)
参数:
	[IN]	
		SKYCAS_U32 writeAddr				Flash地址。
		SKYCAS_U32* const pWriteLen		变量指针。该变量存储需要写入的数据字节长度。
		const SKYCAS_U8* const pWriteBuf	数组指针。该数组存储等待写入的数据。
	[OUT]	
		SKYCAS_U32* const pWriteLen		变量指针。该变量存储实际写入的数据字节长度。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Flash_Write(SKYCAS_U32 writeAddr, SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
/******************************************************************************************
函数名:	
    SKYCAS_S32 SKYCAS_File_Init(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 u32FileSize);
功能:
	初始化CAS需要的文件(文件型)。
	判断文件存在与否，若存在，则不做任何事情；
	若不存在，则生成文件并初始化为参数指定大小，且内容为全0xFF。
参数:
	[IN]	
		const SKYCAS_CHAR* const pFileName	字符串。代表需要初始化的文件路径。
		SKYCAS_U32 u32FileSize		文件大小。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Init(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 u32FileSize);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_File_Read(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 readOffsetAddr, 
										SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
功能:
	读Flash数据(文件型)
参数:
	[IN]	
		const SKYCAS_CHAR* const pFileName	字符串。代表需要读取数据的文件路径。
		SKYCAS_U32 readOffsetAddr		文件偏移地址。
		SKYCAS_U32* const pReadLen		变量指针。该变量存储需要读取的数据字节长度。
		SKYCAS_U8* const pReadBuf		数组指针。该数组存储即将读取到的数据。
	[OUT]	
		SKYCAS_U32* const pReadLen		变量指针。该变量存储实际读取到的数据字节长度。
            SKYCAS_U8* const pReadBuf       数组指针。该数组存储即将读取到的数据。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Read(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 readOffsetAddr, 
									SKYCAS_U32* const pReadLen, SKYCAS_U8* const pReadBuf);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_File_Write(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 writeOffsetAddr, 
										SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);
功能:
	写Flash数据(文件型)
参数:
	[IN]	
		const SKYCAS_CHAR* const pFileName		字符串。代表需要写入数据的文件路径。
		SKYCAS_U32 writeOffsetAddr			文件偏移地址。
		SKYCAS_U32* const pWriteLen		变量指针。该变量存储需要写入的数据字节长度。
		const SKYCAS_U8* const pWriteBuf	数组指针。该数组存储等待写入的数据。
	[OUT]	
		SKYCAS_U32* const pWriteLen		变量指针。该变量存储实际写入的数据字节长度。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_File_Write(const SKYCAS_CHAR* const pFileName, SKYCAS_U32 writeOffsetAddr, 
									SKYCAS_U32* const pWriteLen, const SKYCAS_U8* const pWriteBuf);


/**************************************************
	OS/Timer
**************************************************/
/******************************************************************************************
	Task函数定义。
******************************************************************************************/
typedef SKYCAS_VOID* (*SKYCAS_TASK_FUNC)(SKYCAS_VOID* pTaskPara);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Task_Creat(SKYCAS_U32* const pU32TaskId, const SKYCAS_CHAR* const pU8TaskName, SKYCAS_U16 u16Priority, 
											SKYCAS_U32 u32StackSize, SKYCAS_TASK_FUNC const pTaskFunc, const SKYCAS_VOID* pTaskPara);
功能:
	创建任务/线程。
参数:
	[IN]	
		SKYCAS_U32* const pU32TaskId			变量指针。存储默认taskID。
		const SKYCAS_CHAR* const pU8TaskName		字符串。任务名。
		SKYCAS_U16 u16Priority				任务优先级。
		SKYCAS_U32 u32StackSize				任务堆栈大小。
		SKYCAS_TASK_FUNC const pTaskFunc		函数指针。指向任务主函数。
		const SKYCAS_VOID* const pTaskPara		指针。传递任务函数参数。
	[OUT]	
		SKYCAS_U32* const pU32TaskId			变量指针。存储创建成功的taskID。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Creat(SKYCAS_U32* const pU32TaskId, const SKYCAS_CHAR* const pU8TaskName, SKYCAS_U16 u16Priority, 
										SKYCAS_U32 u32StackSize, SKYCAS_TASK_FUNC const pTaskFunc,  const SKYCAS_VOID* const pTaskPara);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Task_Destory(SKYCAS_U32 u32TaskId)
功能:
	销毁任务/线程。
参数:
	[IN]	
		SKYCAS_U32 u32TaskId		任务/线程ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Destory(SKYCAS_U32 u32TaskId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Task_Sleep(SKYCAS_U32 u32TimeofMS)
功能:
	延时任务/线程。
参数:
	[IN]	
		SKYCAS_U32 u32TimeofMS	延时时间。单位毫秒。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Task_Sleep(SKYCAS_U32 u32TimeofMS);

/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Semp_Creat(SKYCAS_U32* const pU32SempId, SKYCAS_U8 u8InitValue);
功能:
	创建信号量
参数:
	[IN]	
		SKYCAS_U32* const pU32SempId	变量指针。存储默认SempID。
		SKYCAS_U8 u8InitValue			信号量初始化值。初值为0时代表信号量现不可用。
	[OUT]	
		SKYCAS_U32* const pU32SempId	变量指针。存储创建成功的SempID。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Creat(SKYCAS_U32* const pU32SempId, SKYCAS_U8 u8InitValue);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Semp_Destory(SKYCAS_U32 u32SempId)
功能:
	销毁信号量
参数:
	[IN]	
		SKYCAS_U32 u32SempId		信号量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Destory(SKYCAS_U32 u32SempId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Semp_Wait(SKYCAS_U32 u32SempId, SKYCAS_U32 u32WaitTime)
功能:
	等待信号量
参数:
	[IN]	
		SKYCAS_U32 u32SempId		信号量ID
		SKYCAS_U32 u32WaitTime	等待超时时间。单位毫秒。若赋0xFFFFFFFF，则为一直等待。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Wait(SKYCAS_U32 u32SempId, SKYCAS_U32 u32WaitTime);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Semp_Release(SKYCAS_U32 u32SempId)
功能:
	释放信号量
参数:
	[IN]	
		SKYCAS_U32 u32SempId		信号量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Semp_Release(SKYCAS_U32 u32SempId);

/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mutex_Creat(SKYCAS_U32* const pU32MutexId);
功能:
	创建互斥量
参数:
	[IN]	
		SKYCAS_U32* const pU32MutexId		变量指针。存储默认MutexId。
	[OUT]	
		SKYCAS_U32* const pU32MutexId		变量指针。存储创建成功的MutexId。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_Creat(SKYCAS_U32* const pU32MutexId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
功能:
	销毁互斥量
参数:
	[IN]	
		SKYCAS_U32 u32MutexId		互斥量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mutex_lock(SKYCAS_U32 u32MutexId);
功能:
	获取互斥量
参数:
	[IN]	
		SKYCAS_U32 u32MutexId		互斥量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_lock(SKYCAS_U32 u32MutexId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
功能:
	释放互斥量
参数:
	[IN]	
		SKYCAS_U32 u32MutexId		互斥量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_unlock(SKYCAS_U32 u32MutexId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mutex_Destory(SKYCAS_U32 u32MutexId)
功能:
	尝试获取互斥量
参数:
	[IN]	
		SKYCAS_U32 u32MutexId		互斥量ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mutex_trylock(SKYCAS_U32 u32MutexId);

/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Queue_Creat(SKYCAS_U32* const pU32QueueId, SKYCAS_U32 u32MsgSize, SKYCAS_U32 u32QueueLen)
功能:
	创建消息队列。
参数:
	[IN]	
		SKYCAS_U32* const pU32QueueId	变量指针。存储创建的QueueId。
		SKYCAS_U32 u32MsgSize		消息大小。
		SKYCAS_U32 u32QueueLen	      消息队列长度。
	[OUT]	
		SKYCAS_U32* const pU32QueueId	变量指针。存储创建成功的QueueId。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_Creat(SKYCAS_U32* const pU32QueueId, SKYCAS_U32 u32MsgSize, SKYCAS_U32 u32QueueLen);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Queue_Destory(SKYCAS_U32 u32QueueId)
功能:
	销毁消息队列
参数:
	[IN]	
		SKYCAS_U32 u32QueueId		消息队列ID
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_Destory(SKYCAS_U32 u32QueueId);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Queue_SendMsg(SKYCAS_U32 u32QueueId, SKYCAS_U8* pU8SendMsg, 
								SKYCAS_U32 u32SendMsgSize, SKYCAS_U32 u32Timeout)
功能:
	发送消息
参数:
	[IN]	
		SKYCAS_U32 u32QueueId				消息队列ID
		const SKYCAS_U8* const pU8SendMsg	指针。指向待发送的消息。
		SKYCAS_U32 u32SendMsgSize			等待发送的消息大小。
		SKYCAS_U32 u32Timeout				超时时间。0xFFFFFFFF代表一直等待。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_SendMsg(SKYCAS_U32 u32QueueId, const SKYCAS_U8* const pU8SendMsg, 
											SKYCAS_U32 u32SendMsgSize, SKYCAS_U32 u32Timeout);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Queue_RecvMsg(SKYCAS_U32 u32QueueId, SKYCAS_U8* const pU8RecvMsg, 
												SKYCAS_U32* const u32RecvMsgSize, SKYCAS_U32 u32Timeout);
功能:
	接收消息
参数:
	[IN]	
		SKYCAS_U32 u32QueueId				消息队列ID
		SKYCAS_U8* const pU8RecvMsg	      指针。指向接收到的消息。
		SKYCAS_U32* const u32RecvMsgSize	期望接收到的消息大小。队列可能不止包含一个消息。
		                                        故此大小表示需要接收的队列内消息个数与消息大小的积.
		SKYCAS_U32 u32Timeout				超时时间。0xFFFFFFFF代表一直等待。
	[OUT]	
		SKYCAS_U32* const u32RecvMsgSize	 实际接收到的消息大小。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Queue_RecvMsg(SKYCAS_U32 u32QueueId,  SKYCAS_U8* const pU8RecvMsg, 
											SKYCAS_U32* const u32RecvMsgSize, SKYCAS_U32 u32Timeout);

/******************************************************************************************
函数名:	
    SKYCAS_VOID* SKYCAS_Mem_Alloc(SKYCAS_U32 u32AllocSize);
功能:
	分配内存
参数:
	[IN]	
		SKYCAS_U32 u32AllocSize			待分配大小。
	[OUT]	
		无。
	[RET]		
		成功返回分配的地址；失败返回SKYCAS_NULL。
******************************************************************************************/
SKYCAS_VOID* SKYCAS_Mem_Alloc(SKYCAS_U32 u32AllocSize);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mem_Free(const SKYCAS_VOID* const pAllocBufAddr);
功能:
	释放内存
参数:
	[IN]	
		const SKYCAS_VOID* const pAllocBufAddr	指针。指向待释放的内存地址。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_Free(const SKYCAS_VOID* const pAllocBufAddr);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mem_set(SKYCAS_VOID* const pBufAddr, SKYCAS_U8 value, SKYCAS_U32 u32MemSetSize)
功能:
	set内存
参数:
	[IN]	
		SKYCAS_VOID* const pBufAddr	指针。指向待set的内存地址。
		SKYCAS_U8 value			需要set的值。
		SKYCAS_U32 u32MemSetSize	需要set的内存大小。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_set(SKYCAS_VOID* const pBufAddr, SKYCAS_U8 value, SKYCAS_U32 u32MemSetSize);
/******************************************************************************************
函数名:	
	SKYCAS_S32 SKYCAS_Mem_copy(SKYCAS_VOID* const pDesBufAddr, const SKYCAS_VOID* const pSrcBufAddr);
功能:
	内存拷贝
参数:
	[IN]	
		SKYCAS_VOID* const pDesBufAddr		指针。指向目标内存地址。
		const SKYCAS_VOID* const pSrcBufAddr		指针。指向源内存地址。
		SKYCAS_U32 u32MemCpySize		拷贝大小。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Mem_copy(SKYCAS_VOID* const pDesBufAddr, const SKYCAS_VOID* const pSrcBufAddr, SKYCAS_U32 u32MemCpySize);

/******************************************************************************************
函数名:	
	SKYCAS_U32 SKYCAS_Time_Get_Times(SKYCAS_VOID)
功能:
	获取当前系统的时间值。以毫秒为单位。
	非Linux系统请将系统时钟节拍值转换为毫秒值返回。
参数:
	[IN]	
		无。
	[OUT]	
		无。
	[RET]		
		返回系统时间值。
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
函数名:	
	SKYCAS_S32 SKYCAS_Debugger_OnOff(SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff)
功能:
	打印开关
参数:
	[IN]	
		SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff	打印开关枚举。
	[OUT]	
		无。
	[RET]		
		成功返回SKYCAS_RET_SUCCESS；失败返回SKYCAS_RET_FAILURE。
******************************************************************************************/
SKYCAS_S32 SKYCAS_Debugger_OnOff(SKYCAS_DEBUGGER_STATUS_E eDebuggerOnOff);
/******************************************************************************************
函数名:	
	SKYCAS_VOID SKYCAS_Debugger_Printf(const SKYCAS_CHAR* const fmt, ...)
功能:
	打印信息
参数:
	[IN]	
		const SKYCAS_CHAR* const fmt	打印信息流。可变参数。
	[OUT]	
		无。
	[RET]		
		无。
******************************************************************************************/
SKYCAS_VOID SKYCAS_Debugger_Printf(const SKYCAS_CHAR* const fmt, ...);


#ifdef __cplusplus
 #if __cplusplus
	}
 #endif     /* __cplusplus */
#endif  /* __cplusplus */
	
#endif /* __SKYCAS_DRIVER_H__ */

