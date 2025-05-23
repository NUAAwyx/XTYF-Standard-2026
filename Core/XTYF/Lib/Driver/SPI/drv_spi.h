#ifndef DRV_SPI_H
#define DRV_SPI_H

#include "spi.h"
#include <cstring>

// 缓冲区字节长度
#define SPI_BUFFER_SIZE 256

/**
 * @brief SPI通信接收回调函数数据类型
 *
 */
typedef void (*SPI_Call_Back)(uint8_t *Tx_Buffer, uint8_t *Rx_Buffer, uint16_t Length);

/**
 * @brief SPI通信处理结构体
 *
 */
struct Struct_SPI_Manage_Object
{
    SPI_HandleTypeDef *SPI_Handler;
    GPIO_TypeDef *Now_GPIOx;
    uint16_t Now_GPIO_Pin;
    uint8_t Tx_Buffer[SPI_BUFFER_SIZE];
    uint8_t Rx_Buffer[SPI_BUFFER_SIZE];
    uint16_t Now_Tx_Length;
    uint16_t Now_Rx_Length;
    SPI_Call_Back Callback_Function;
};

extern bool init_finished;

extern Struct_SPI_Manage_Object SPI1_Manage_Object;
extern Struct_SPI_Manage_Object SPI2_Manage_Object;
extern Struct_SPI_Manage_Object SPI3_Manage_Object;
extern Struct_SPI_Manage_Object SPI4_Manage_Object;
extern Struct_SPI_Manage_Object SPI5_Manage_Object;
extern Struct_SPI_Manage_Object SPI6_Manage_Object;

void SPI_Init(SPI_HandleTypeDef *hspi, SPI_Call_Back Callback_Function);

uint8_t SPI_Send_Receive_Data(SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t Tx_Length, uint16_t Rx_Length);


#endif
