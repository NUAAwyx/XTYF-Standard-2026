#ifndef USART_H
#define USART_H

#include "usart.h"
#include <cstring>

// 缓冲区字节长度
#define UART_BUFFER_SIZE 512


/**
 * @brief UART通信接收回调函数数据类型
 *
 */
typedef void (*UART_Call_Back)(uint8_t *Buffer, uint16_t Length);

/**
 * @brief UART通信处理结构体
 */
struct Struct_UART_Manage_Object
{
    UART_HandleTypeDef *UART_Handler;
    uint8_t Tx_Buffer[UART_BUFFER_SIZE];
    uint8_t Rx_Buffer[UART_BUFFER_SIZE];
    uint16_t Rx_Buffer_Length;
    UART_Call_Back Callback_Function;
};

extern bool init_finished;

extern Struct_UART_Manage_Object USART1_Manage_Object;
extern Struct_UART_Manage_Object USART2_Manage_Object;
extern Struct_UART_Manage_Object USART3_Manage_Object;
extern Struct_UART_Manage_Object UART4_Manage_Object;
extern Struct_UART_Manage_Object UART5_Manage_Object;
extern Struct_UART_Manage_Object USART6_Manage_Object;
extern Struct_UART_Manage_Object UART7_Manage_Object;
extern Struct_UART_Manage_Object UART8_Manage_Object;
extern Struct_UART_Manage_Object UART9_Manage_Object;
extern Struct_UART_Manage_Object USART10_Manage_Object;



void UART_Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);

void UART_Reinit(UART_HandleTypeDef *huart);

uint8_t UART_Send_Data(UART_HandleTypeDef *huart, uint8_t *Data, uint16_t Length);







#endif
