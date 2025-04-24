#ifndef DVC_VOFA_H
#define DVC_VOFA_H

#include "Lib/Driver/USART/drv_usart.h"
#include <cstdlib>

//ASCII码， P：80，I：73，D：68，F：70，T：84，O：79，A：65
enum Enum_Cmd_Type
{
    OP = 159,
    OI = 152,
    OD = 147,
    OF = 149,
    OT = 163,
    AP = 145,
    AI = 138,
    AD = 133,
    AF = 135,
    AT = 149,
};


class Class_VOFA
{
public:
    void Init(UART_HandleTypeDef *huart, UART_Call_Back Callback_Function, uint16_t Rx_Buffer_Length);

    void Send(float *data ,uint8_t num);

    void UART_RxCpltCallback(uint8_t* Rx_Data, uint16_t Length);

    inline Enum_Cmd_Type Get_Cmd_Type();

    inline float Get_Cmd_Data();

protected:
    // 绑定的UART
    Struct_UART_Manage_Object* UART_Manage_Object;
    // 要绘图的数据
    float Data[10];
    // 要绘图的数据个数
    uint8_t Num;
    // 命令数据
    float Cmd_Data;
    // 命令类型
    Enum_Cmd_Type Cmd_Type;

    void Output();

    void Data_Process(uint8_t* Rx_Data);

    void Cmd_Data_Process(uint8_t* Rx_Data);

    void Cmd_Type_Process(uint8_t* Rx_Data);
};

/**
 * @brief 获取命令类型
 *
 * @return Enum_Cmd_Type 命令类型
*/
inline Enum_Cmd_Type Class_VOFA::Get_Cmd_Type()
{
    return Cmd_Type;
}

/**
 * @brief 获取命令数据
 *
 * @return float 命令数据
 */
inline float Class_VOFA::Get_Cmd_Data()
{
    return Cmd_Data;
}


#endif
