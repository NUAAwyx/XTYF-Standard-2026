#ifndef DRV_CAN_H
#define DRV_CAN_H

#include "stm32h7xx_hal.h"
#include "fdcan.h"
#include <cstring>

/**
 * @brief CAN接收的信息结构体
 *
 */
struct Struct_FDCAN_Rx_FIFO
{
    FDCAN_RxHeaderTypeDef Header;
    uint8_t Data[8];
};

/**
 * @brief CAN通信接收回调函数数据类型
 *
 */
typedef void (*CAN_Call_Back)(Struct_FDCAN_Rx_FIFO *);

/**
 * @brief CAN通信处理结构体
 *
 */
struct Struct_FDCAN_Manage_Object
{
    FDCAN_HandleTypeDef *FDCAN_Handler;
    Struct_FDCAN_Rx_FIFO Rx_FIFO;
    CAN_Call_Back Callback_Function;
};

extern bool init_finished;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;


extern Struct_FDCAN_Manage_Object FDCAN1_Manage_Object;
extern Struct_FDCAN_Manage_Object FDCAN2_Manage_Object;
extern Struct_FDCAN_Manage_Object FDCAN3_Manage_Object;

extern uint8_t FDCAN1_0x200_Tx_Data[8];

extern uint8_t FDCAN2_0x200_Tx_Data[8];

extern uint8_t FDCAN3_0x200_Tx_Data[8];

void FDCAN_Init(FDCAN_HandleTypeDef *hfdcan, CAN_Call_Back Callback_Function);

uint8_t FDCAN_Send_Data(FDCAN_HandleTypeDef *hfdcan, uint16_t ID, uint8_t *Data, uint16_t Length);


#endif