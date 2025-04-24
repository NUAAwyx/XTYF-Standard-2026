#include "drv_can.h"

// 滤波器编号
#define FDCAN_FILTER(x) ((x) << 3)

// 接收队列
#define FDCAN_FIFO_0 (0 << 2)
#define FDCAN_FIFO_1 (1 << 2)

// 标准帧或扩展帧
#define FDCAN_STDID (0 << 1)
#define FDCAN_EXTID (1 << 1)

// 数据帧或遥控帧
#define FDCAN_DATA_TYPE (0 << 0)
#define FDCAN_REMOTE_TYPE (1 << 0)

// FDCAN1,FDCAN2和FDCAN3的管理对象
Struct_FDCAN_Manage_Object FDCAN1_Manage_Object;
Struct_FDCAN_Manage_Object FDCAN2_Manage_Object;
Struct_FDCAN_Manage_Object FDCAN3_Manage_Object;

uint8_t FDCAN1_0x200_Tx_Data[8];

uint8_t FDCAN2_0x200_Tx_Data[8];

uint8_t FDCAN3_0x200_Tx_Data[8];

/**
 * @brief 配置FDCAN的过滤器
 *
 * @param hfdcan FDCAN编号
 * @param Object_Para 滤波器编号 | FIFOx | ID类型 | 帧类型
 * @param ID ID
 * @param Mask_ID 屏蔽位(0x7FF, 0x1FFFFFFF)
 */
void fdcan_filter_mask_config(FDCAN_HandleTypeDef *hfdcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID)
{
    FDCAN_FilterTypeDef FDCAN_FilterConfig;

    //检测传参是否正确
    assert_param(hfdcan != nullptr);

    // 过滤ID类型选择 标准ID
    FDCAN_FilterConfig.IdType = (Object_Para & 0x02);

    //当前FDCAN过滤器编号，可以设置多个过滤器过滤不同的ID 依次类推0、1、2....
    FDCAN_FilterConfig.FilterIndex = (Object_Para >> 3);

    //过滤器Mask模式 关乎到下面ID1、ID2的配置
    FDCAN_FilterConfig.FilterType = FDCAN_FILTER_MASK;

    //选择哪个FIFO区接收，根据CubeMX的配置来
    FDCAN_FilterConfig.FilterConfig = (Object_Para >> 2) & 0x01;

    // 这个都行，只要ID2配置0x00000000就不会过滤调任何ID
    FDCAN_FilterConfig.FilterID1 = ID;

    //理由如上
    FDCAN_FilterConfig.FilterID2 = Mask_ID;

    //将上述配置到CAN
    HAL_FDCAN_ConfigFilter(hfdcan, &FDCAN_FilterConfig);

    //开启CAN的全局过滤，就是开启过滤器
    HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
}


/**
 * @brief 初始化FDCAN总线
 *
 * @param hfdcan FDCAN编号
 * @param Callback_Function 处理回调函数
 */
void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan, CAN_Call_Back Callback_Function)
{
    HAL_FDCAN_Start(hfdcan);

    //打开FIFO0区的新数据接收中断
    HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    //打开FIFO1区的新数据接收中断
    HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);


    if (hfdcan->Instance == FDCAN1)
    {
        FDCAN1_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN1_Manage_Object.Callback_Function = Callback_Function;

        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(0) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(1) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0, 0);
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        FDCAN2_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN2_Manage_Object.Callback_Function = Callback_Function;

        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(14) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0x00000000, 0x00000000);
        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(15) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0x00000000, 0x00000000);
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        FDCAN3_Manage_Object.FDCAN_Handler = hfdcan;
        FDCAN3_Manage_Object.Callback_Function = Callback_Function;

        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(28) | FDCAN_FIFO_0 | FDCAN_STDID | FDCAN_DATA_TYPE, 0x00000000, 0x00000000);
        fdcan_filter_mask_config(hfdcan, FDCAN_FILTER(29) | FDCAN_FIFO_1 | FDCAN_STDID | FDCAN_DATA_TYPE, 0x00000000, 0x00000000);
    }
}

/**
 * @brief 发送数据帧
 *
 * @param hfdcan FDCAN编号
 * @param ID ID
 * @param Data 被发送的数据指针
 * @param Length 长度
 *
 * @return uint8_t 执行状态
 */
uint8_t FDCAN_Send_Data(FDCAN_HandleTypeDef *hfdcan, uint16_t ID, uint8_t *Data, uint16_t Length)
{
    FDCAN_TxHeaderTypeDef tx_header;

    //检测传参是否正确
    assert_param(hfdcan != nullptr);

    tx_header.Identifier = ID; // ID
    tx_header.IdType = FDCAN_STANDARD_ID; // ID类型
    tx_header.TxFrameType = FDCAN_DATA_FRAME; // 数据帧
    tx_header.DataLength = Length; // 数据长度
    tx_header.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // 错误状态指示器
    tx_header.BitRateSwitch = FDCAN_BRS_OFF; // 速率切换
    tx_header.FDFormat = FDCAN_CLASSIC_CAN; // FDCAN格式
    tx_header.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // 事件FIFO控制
    tx_header.MessageMarker = 0; // 消息标记

    return (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &tx_header, Data));
}

/**
 * @brief HAL库FDCAN接收FIFO0中断
 *
 * @param hfdcan FDCAN编号
 */
void HAL_FDCAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hfdcan)
{
    // 判断程序初始化完成
    if (init_finished == false)
    {
        return;
    }

    // 选择回调函数
    if (hfdcan->Instance == FDCAN1)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_Manage_Object.Rx_FIFO.Header, FDCAN1_Manage_Object.Rx_FIFO.Data);
        if(FDCAN1_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_FIFO);
        }
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_Manage_Object.Rx_FIFO.Header, FDCAN2_Manage_Object.Rx_FIFO.Data);
        if(FDCAN2_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_FIFO);
        }
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN3_Manage_Object.Rx_FIFO.Header, FDCAN3_Manage_Object.Rx_FIFO.Data);
        if(FDCAN3_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN3_Manage_Object.Callback_Function(&FDCAN3_Manage_Object.Rx_FIFO);
        }
    }
}

/**
 * @brief HAL库FDCAN接收FIFO1中断
 *
 * @param hfdcan FDCAN编号
 */
void HAL_FDCAN_RxFifo1MsgPendingCallback(FDCAN_HandleTypeDef *hfdcan)
{
    // 判断程序初始化完成
    if (init_finished == false)
    {
        return;
    }

    // 选择回调函数
    if (hfdcan->Instance == FDCAN1)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN1_Manage_Object.Rx_FIFO.Header, FDCAN1_Manage_Object.Rx_FIFO.Data);
        if(FDCAN1_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN1_Manage_Object.Callback_Function(&FDCAN1_Manage_Object.Rx_FIFO);
        }
    }
    else if (hfdcan->Instance == FDCAN2)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_Manage_Object.Rx_FIFO.Header, FDCAN2_Manage_Object.Rx_FIFO.Data);
        if(FDCAN2_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN2_Manage_Object.Callback_Function(&FDCAN2_Manage_Object.Rx_FIFO);
        }
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN3_Manage_Object.Rx_FIFO.Header, FDCAN3_Manage_Object.Rx_FIFO.Data);
        if(FDCAN3_Manage_Object.Callback_Function != nullptr)
        {
            FDCAN3_Manage_Object.Callback_Function(&FDCAN3_Manage_Object.Rx_FIFO);
        }
    }
}