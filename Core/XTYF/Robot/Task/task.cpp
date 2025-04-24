#include "task.h"

#include "Lib/Device/DR16/dvc_dr16.h"
#include "Lib/Device/VOFA/dvc_vofa.h"

Class_DR16 DR16;
Class_VOFA VOFA;

bool init_finished;

float VOFA_Data[10] = {0};

/**
 * @brief 遥控器串口回调函数
 *
 */
void DR16_UART_Callback(uint8_t *Rx_Data, uint16_t Length)
{
    DR16.UART_RxCpltCallback(Rx_Data, Length);
}


void Task_Init()
{
    init_finished = false;
    // Initialize devices

    DR16.Init(&huart5,DR16_UART_Callback, 512);
    VOFA.Init(&huart1,NULL, 512);

    // Set the initialization flag to true
    init_finished = true;
}

void Task()
{
    VOFA_Data[0] = DR16.Get_Left_X();
    VOFA_Data[1] = DR16.Get_Left_Y();
    VOFA_Data[2] = DR16.Get_Right_X();
    VOFA_Data[3] = DR16.Get_Right_Y();

    VOFA.Send(VOFA_Data, 4);
}