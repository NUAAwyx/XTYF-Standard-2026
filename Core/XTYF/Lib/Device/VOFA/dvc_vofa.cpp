#include "dvc_vofa.h"

/**
 * @brief 遥控器VOFA初始化
 *
 * @param huart 指定的UART
 * @param data  数据
 * @param num   数据个数
 *
 */
void Class_VOFA::Init(UART_HandleTypeDef *huart,float *data, uint8_t num)
{
    if (huart->Instance == USART1)
    {
        UART_Manage_Object = &USART1_Manage_Object;
    }
    else if (huart->Instance == USART2)
    {
        UART_Manage_Object = &USART2_Manage_Object;
    }
    else if (huart->Instance == USART3)
    {
        UART_Manage_Object = &USART3_Manage_Object;
    }
    else if (huart->Instance == UART4)
    {
        UART_Manage_Object = &UART4_Manage_Object;
    }
    else if (huart->Instance == UART5)
    {
        UART_Manage_Object = &UART5_Manage_Object;
    }
    else if (huart->Instance == USART6)
    {
        UART_Manage_Object = &USART6_Manage_Object;
    }
    else if (huart->Instance == UART7)
    {
        UART_Manage_Object = &UART7_Manage_Object;
    }
    else if (huart->Instance == UART8)
    {
        UART_Manage_Object = &UART8_Manage_Object;
    }
    else if (huart->Instance == UART9)
    {
        UART_Manage_Object = &UART9_Manage_Object;
    }
    else if (huart->Instance == USART10)
    {
        UART_Manage_Object = &USART10_Manage_Object;
    }

    // 绑定数据
    memcpy(Data, data, sizeof(float) * num);
    // 绑定数据个数
    Num = num;
}

/**
 * @brief 数据输出到发送缓冲区
 *
 */
void Class_VOFA::Output()
{
    uint8_t *tmp_buffer = UART_Manage_Object->Tx_Buffer;

    for (int i = 0; i < Num; i++)
    {
        memcpy(tmp_buffer + i * sizeof(float), &Data[i], sizeof(float));
    }

    tmp_buffer[4 * Num + 0] = 0x00;
    tmp_buffer[4 * Num + 1] = 0x00;
    tmp_buffer[4 * Num + 2] = 0x80;
    tmp_buffer[4 * Num + 3] = 0x7F;
}

/**
 * @brief 发送绘图数据
 *
 */
void Class_VOFA::Send()
{
    Output();

    UART_Send_Data(UART_Manage_Object->UART_Handler, UART_Manage_Object->Tx_Buffer, 4 * Num + 4);
}

/**
 * @brief 获取命令数据
 *
 * @param Rx_Data 接收的数据
 */
void Class_VOFA::Cmd_Data_Process(uint8_t *Rx_Data)
{
    // 假设 data 是一个以空字符结尾的字符串
    char *str = (char*)Rx_Data;
    char *endptr;

    // 跳过非数字字符，找到 '=' 后的部分
    while (*str && *str != '=') {
        str++;
    }

    // 如果找到了 '='，则跳过它
    if (*str == '=') {
        str++;
    }

    // 将字符串转换为浮点数
    Cmd_Data = strtof(str, &endptr);
}

/**
 * @brief 获取命令类型
 *
 * @param Rx_Data 接收的数据
 */
void Class_VOFA::Cmd_Type_Process(uint8_t *Rx_Data)
{
    int tmp = 0;

    tmp = Rx_Data[0] + Rx_Data[1];

    Cmd_Data = tmp;
}

/**
 * @brief 命令数据处理
 *
 */
void Class_VOFA::Data_Process(uint8_t *Rx_Data)
{
    Cmd_Data_Process(Rx_Data);
    Cmd_Type_Process(Rx_Data);
}

/**
 * @brief UART接收中断回调函数
 *
 */
void Class_VOFA::UART_RxCpltCallback(uint8_t *Rx_Data, uint16_t Length)
{
    // 数据处理
    Data_Process(Rx_Data);
}

