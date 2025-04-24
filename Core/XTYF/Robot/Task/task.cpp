#include "task.h"

#include "Lib/Driver/CAN/drv_can.h"

bool init_finished;

void Task_Init()
{
    FDCAN_Init(&hfdcan1, nullptr);

    init_finished = true;
}

void Task()
{
    FDCAN1_0x200_Tx_Data[0] = 10;
    FDCAN1_0x200_Tx_Data[1] = 10;
    FDCAN_Send_Data(&hfdcan1, 0x200, FDCAN1_0x200_Tx_Data, 8);
}