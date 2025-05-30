#include "dvc_bmi088.h"

static float BMI088_ACCEL_SEN = BMI088_ACCEL_6G_SEN;
static float BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;


static uint8_t Accel_Register_ConfigurationData_ErrorStatus[BMI088_WRITE_ACCEL_REG_NUM][3] =
{
    /* Turn on accelerometer */
    {BMI088_ACCEL_PWR_CTRL, BMI088_ACCEL_ENABLE_ACCEL_ON, BMI088_ACCEL_PWR_CTRL_ERROR},

    /* Pause mode */
    {BMI088_ACCEL_PWR_CONF, BMI088_ACCEL_PWR_ACTIVE_MODE, BMI088_ACCEL_PWR_CONF_ERROR},

    /* ACCELeleration Configuration */
    {
        BMI088_ACCEL_CONF, (BMI088_ACCEL_NORMAL | BMI088_ACCEL_1600_HZ | BMI088_ACCEL_CONF_MUST_Set),
        BMI088_ACCEL_CONF_ERROR
    },

    /* ACCELelerometer setting range */
    {BMI088_ACCEL_RANGE, BMI088_ACCEL_RANGE_6G, BMI088_ACCEL_RANGE_ERROR},

    /* INT1 Configuration input and output pin */
    {
        BMI088_INT1_IO_CTRL, (BMI088_ACCEL_INT1_IO_ENABLE | BMI088_ACCEL_INT1_GPIO_PP | BMI088_ACCEL_INT1_GPIO_LOW),
        BMI088_INT1_IO_CTRL_ERROR
    },

    /* interrupt map pin */
    {BMI088_INT_MAP_DATA, BMI088_ACCEL_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}
};

static uint8_t Gyro_Register_ConfigurationData_ErrorStatus[BMI088_WRITE_GYRO_REG_NUM][3] =
{
    /* Angular rate and resolution */
    {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},

    /* Data Transfer Rate and Bandwidth Settings */
    {BMI088_GYRO_BANDWIDTH, (BMI088_GYRO_2000_230_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set), BMI088_GYRO_BANDWIDTH_ERROR},

    /* Power Mode Selection Register */
    {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},

    /* Data Interrupt Trigger Register */
    {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},

    /* Interrupt Pin Trigger Register */
    {
        BMI088_GYRO_INT3_INT4_IO_CONF, (BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW),
        BMI088_GYRO_INT3_INT4_IO_CONF_ERROR
    },

    /* interrupt map register */
    {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}
};

/**
 * @brief BMI088初始化
 *
 */
void Class_BMI088::BMI088_Init(void)
{
    BMI088_Status_e status = BMI088_NO_ERROR;

    /* Initializes the BMI088 */

    do
    {
        status = static_cast<BMI088_Status_e>(static_cast<int>(status) | static_cast<int>(BMI088_Accel_Init()));
        status = static_cast<BMI088_Status_e>(static_cast<int>(status) | static_cast<int>(BMI088_Gyro_Init()));

        BMI088_delay_ms(2);
    }
    while (status);

    BMI088_Offset_Update();
}

/**
 * @brief BMI088数据更新
 *
 */
void Class_BMI088::BMI088_Data_Update(void)
{
    // uint8_t buf[8] = {0, 0, 0, 0, 0, 0};
    //
    // /* read the accelerator multi data */
    // BMI088_Accel_Read_Multi_Reg(BMI088_ACCEL_XOUT_L, buf, 6);
    // BMI088_Info->MPU_Info.Accel_X = (int16_t)((buf[1] << 8) | buf[0]);
    // BMI088_Info->MPU_Info.Accel_Y = (int16_t)((buf[3] << 8) | buf[2]);
    // BMI088_Info->MPU_Info.Accel_Z = (int16_t)((buf[5] << 8) | buf[4]);
    //
    // /* converts the accelerator data */
    // BMI088_Info->Accel[0] =  BMI088_ACCEL_SEN * BMI088_Info->MPU_Info.Accel_X;
    // BMI088_Info->Accel[1] =  BMI088_ACCEL_SEN * BMI088_Info->MPU_Info.Accel_Y;
    // BMI088_Info->Accel[2] =  BMI088_ACCEL_SEN * BMI088_Info->MPU_Info.Accel_Z;
    //
    // /* read the temperature */
    // BMI088_Accel_Read_Multi_Reg(BMI088_TEMP_M, buf, 2);
    // BMI088_Info->MPU_Info.Temperature = (int16_t)((buf[0] << 3) | (buf[1] >> 5));
    // if (BMI088_Info->MPU_Info.Temperature > 1023) BMI088_Info->MPU_Info.Temperature -= 2048;
    //
    // /* converts the temperature data */
    // BMI088_Info->Temperature = BMI088_Info->MPU_Info.Temperature * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
    //
    // /* read the gyro multi data */
    // BMI088_Gyro_Read_Multi_Reg(BMI088_GYRO_CHIP_ID, buf, 8);
    // /* check the ID */
    // if(buf[0] == BMI088_GYRO_CHIP_ID_VALUE)
    // {
    //     BMI088_Info->MPU_Info.Gyro_X = (int16_t)((buf[3] << 8) | buf[2]);
    //     BMI088_Info->MPU_Info.Gyro_Y = (int16_t)((buf[5] << 8) | buf[4]);
    //     BMI088_Info->MPU_Info.Gyro_Z = (int16_t)((buf[7] << 8) | buf[6]);
    // }
    //
    // /* converts the gyro data */
    // Gyro[0] =   BMI088_GYRO_SEN * Gyro_X - Offsets_Gyro_X;
    // Gyro[1] =   BMI088_GYRO_SEN * Gyro_Y - Offsets_Gyro_Y;
    // Gyro[2] =   BMI088_GYRO_SEN * Gyro_Z - Offsets_Gyro_Z;

}


/**
 * @brief BMI088加速度计初始化
 *
 * @return uint8_t 执行状态
 */
BMI088_Status_e Class_BMI088::BMI088_Accel_Init()
{
    uint8_t res = 0;

    /* check the communication ------------------------------------------------*/
    /* read the accelerator ID address */
    BMI088_Accel_Read_Single_Reg(BMI088_ACCEL_CHIP_ID, res);
    /* waiting 150us */
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    /* read again */
    BMI088_Accel_Read_Single_Reg(BMI088_ACCEL_CHIP_ID, res);
    /* waiting 150us */
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    /* ACCELelerator software Reset ------------------------------------------------*/
    /* write 0xB6 to the register ACCEL_SOFTResET that addRess is 0x7E to reset software */
    BMI088_Accel_Write_Single_Reg(BMI088_ACCEL_SOFTRESET, BMI088_ACCEL_SOFTRESET_VALUE);
    /* software reset waiting time, there is 80ms */
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);

    /* check the communication again ------------------------------------------------*/
    BMI088_Accel_Read_Single_Reg(BMI088_ACCEL_CHIP_ID,res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    /* read again */
    BMI088_Accel_Read_Single_Reg(BMI088_ACCEL_CHIP_ID,res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    /* if the value of the 0x00 register is not 0x1E,reset failed */
    if (res != BMI088_ACCEL_CHIP_ID_VALUE)
    {
        return BMI088_NO_SENSOR;
    }

    /* config the accelerator sensor */
    for (uint8_t write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++)
    {
        BMI088_Accel_Write_Single_Reg(Accel_Register_ConfigurationData_ErrorStatus[write_reg_num][0],
                                      Accel_Register_ConfigurationData_ErrorStatus[write_reg_num][1]);
        /* waiting 150us */
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        /* read the configuration */
        BMI088_Accel_Read_Single_Reg(Accel_Register_ConfigurationData_ErrorStatus[write_reg_num][0], res);
        /* waiting 150us */
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        /* check the configuration and return the specified error */
        if (res != Accel_Register_ConfigurationData_ErrorStatus[write_reg_num][1])
        {
            return (BMI088_Status_e)Accel_Register_ConfigurationData_ErrorStatus[write_reg_num][2];
        }
    }

    /* no error */
    return BMI088_NO_ERROR;
}

/**
 * @brief BMI088加速度计初始化
 *
 * @return uint8_t 执行状态
 */
BMI088_Status_e Class_BMI088::BMI088_Gyro_Init()
{
    uint8_t res = 0;

    /* check the communication ------------------------------------------------*/
    /* read the gyro ID address */
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID, res);
    /* waiting 150us */
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    /* read again */
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID, res);
    /* waiting 150us */
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    /* gyro software reset ------------------------------------------------*/
    /* write 0xB6 to the register GYRO_SOFTRESET that address is 0x14 to reset software */
    BMI088_Gyro_Write_Single_Reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
    /* software reset waiting time, there is 80ms */
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);

    /* check the communication again ------------------------------------------------*/
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    /* read again */
    BMI088_Gyro_Read_Single_Reg(BMI088_GYRO_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    /* if the value of the 0x00 register is not 0x0F,reset failed */
    if (res != BMI088_GYRO_CHIP_ID_VALUE)
    {
        return BMI088_NO_SENSOR;
    }

    /* config the gyro sensor */
    for (uint8_t write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++)
    {
        BMI088_Gyro_Write_Single_Reg(Gyro_Register_ConfigurationData_ErrorStatus[write_reg_num][0],
                                     Gyro_Register_ConfigurationData_ErrorStatus[write_reg_num][1]);
        /* waiting 150us */
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        /* read the configuration */
        BMI088_Gyro_Read_Single_Reg(Gyro_Register_ConfigurationData_ErrorStatus[write_reg_num][0],res);
        /* waiting 150us */
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        /* check the configuration and return the specified error */
        if (res != Gyro_Register_ConfigurationData_ErrorStatus[write_reg_num][1])
        {
            return (BMI088_Status_e)Gyro_Register_ConfigurationData_ErrorStatus[write_reg_num][2];
        }
    }

    /* no error */
    return BMI088_NO_ERROR;
}

/**
 * @brief BMI088零偏校准
 *
 */
void Class_BMI088::BMI088_Offset_Update()
{
#if IMU_Calibration_ENABLE /* ENABLE the BMI088 Calibration */

    uint8_t buf[8] = {0,};

    for(uint16_t i = 0; i < 5000; i++)
    {
        /* read the accelerator multi data */
        BMI088_Accel_Read_Multi_Reg(BMI088_ACCEL_XOUT_L, buf, 6);
        Accel_X = (int16_t)((buf[1]) << 8) | buf[0];
        Accel_Y = (int16_t)((buf[3]) << 8) | buf[2];
        Accel_Z = (int16_t)((buf[5]) << 8) | buf[4];

        /* read the gyro multi data */
        BMI088_Gyro_Read_Multi_Reg(BMI088_GYRO_CHIP_ID, buf, 8);
        /* check the ID */
        if(buf[0] == BMI088_GYRO_CHIP_ID_VALUE)
        {
            Gyro_X = (int16_t)((buf[3]) << 8) | buf[2];
            Gyro_Y = (int16_t)((buf[5]) << 8) | buf[4];
            Gyro_Z = (int16_t)((buf[7]) << 8) | buf[6];

            /* update the gyro offsets */
            Offsets_Gyro_X += BMI088_GYRO_SEN * Gyro_X;
            Offsets_Gyro_Y += BMI088_GYRO_SEN * Gyro_Y;
            Offsets_Gyro_Z += BMI088_GYRO_SEN * Gyro_Z;
        }
        /* waiting 1ms */
        Delay_ms(1);
    }

    Offsets_Gyro_X = Offsets_Gyro_X / 5000.f;
    Offsets_Gyro_Y = Offsets_Gyro_Y / 5000.f;
    Offsets_Gyro_Z = Offsets_Gyro_Z / 5000.f;

#else /* DISABLE the BMI088 Calibration */
    /* store the previous offsets */
    Offsets_Gyro_X =  0.00379787781f;
    Offsets_Gyro_Y =  -0.00349405757f;
    Offsets_Gyro_Z =  9.18259611e-05f;
#endif

    /* set the offset init flag */
    Offsets_Init = true;
}

/**
 * @brief BMI088毫秒延时函数
 *
 * @param ms 延时的毫秒数
 */
void Class_BMI088::BMI088_delay_ms(uint16_t ms)
{
    while (ms--)
    {
        BMI088_delay_us(1000);
    }
}

/**
 * @brief BMI088微秒延时函数
 *
 * @param us 延时的微秒数
 */
void Class_BMI088::BMI088_delay_us(uint16_t us)
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = us * 480;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

/**
 * @brief BMI088写入单个加速度计寄存器
 *
 * @param reg 寄存器地址
 * @param data 写入的数据
 */
void Class_BMI088::BMI088_Accel_Write_Single_Reg(uint8_t reg, uint8_t data)
{
    BMI088_ACCEL_NS_L();
    BMI088_Write_Single_Reg(reg, data);
    BMI088_ACCEL_NS_H();
}

/**
 * @brief BMI088读取单个加速度计寄存器
 *
 * @param reg 寄存器地址
 */
void Class_BMI088::BMI088_Accel_Read_Single_Reg(uint8_t reg, uint8_t data)
{
    BMI088_ACCEL_NS_L();
    BMI088_Read_Write_Byte((reg) | 0x80);
    BMI088_Read_Write_Byte(0x55);
    data = BMI088_Read_Write_Byte(0x55);
    BMI088_ACCEL_NS_H();
}

/**
 * @brief BMI088读取多个加速度计寄存器
 *
 * @param reg 寄存器地址
 * @param buf 读取的数据
 * @param len 读取的长度
 */
void Class_BMI088::BMI088_Accel_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
    BMI088_ACCEL_NS_L();
    BMI088_Read_Write_Byte((reg) | 0x80);
    BMI088_Read_Multi_Reg(reg, buf, len);
    BMI088_ACCEL_NS_H();
}

/**
 * @brief BMI088写入单个陀螺仪寄存器
 *
 * @param reg 寄存器地址
 * @param data 写入的数据
 */
void Class_BMI088::BMI088_Gyro_Write_Single_Reg(uint8_t reg, uint8_t data)
{
    BMI088_GYRO_NS_L();
    BMI088_Write_Single_Reg(reg, data);
    BMI088_GYRO_NS_H();
}

/**
 * @brief BMI088读取单个陀螺仪寄存器
 *
 * @param reg 寄存器地址
 */
void Class_BMI088::BMI088_Gyro_Read_Single_Reg(uint8_t reg, uint8_t data)
{
    BMI088_GYRO_NS_L();
    BMI088_Read_Write_Byte((reg) | 0x80);
    BMI088_Read_Write_Byte(0x55);
    data = BMI088_Read_Write_Byte(0x55);
    BMI088_GYRO_NS_H();
}

/**
 * @brief BMI088读取多个陀螺仪寄存器
 *
 * @param reg 寄存器地址
 * @param buf 读取的数据
 * @param len 读取的长度
 */
void Class_BMI088::BMI088_Gyro_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
    BMI088_GYRO_NS_L();
    BMI088_Read_Multi_Reg(reg, buf, len);
    BMI088_GYRO_NS_H();
}

/**
 * @brief BMI088操作单个寄存器
 *
 * @param Tx_Data 写入的变量
 */
uint8_t Class_BMI088::BMI088_Read_Write_Byte(uint8_t Tx_Data)
{
    uint8_t Rx_Data = 0;

    HAL_SPI_TransmitReceive(&hspi2,&Tx_Data,&Rx_Data,1,100);

    return Rx_Data;
}

/**
 * @brief BMI088写入单个寄存器
 *
 * @param reg 寄存器地址
 * @param data 写入的数据
 */
void Class_BMI088::BMI088_Write_Single_Reg(uint8_t reg, uint8_t data)
{
    BMI088_Read_Write_Byte(reg);

    BMI088_Read_Write_Byte(data);
}

/**
 * @brief BMI088读取单个寄存器
 *
 * @param reg 寄存器地址
 * @param return_data 读取的数据
 */
void Class_BMI088::BMI088_Read_Single_Reg(uint8_t reg, uint8_t *return_data)
{
    BMI088_Read_Write_Byte(reg | 0x80);

    *return_data = BMI088_Read_Write_Byte(0x55);
}

/**
 * @brief BMI088读取多个寄存器
 *
 * @param reg 寄存器地址
 * @param buf 读取的数据
 * @param len 读取的长度
 */
void Class_BMI088::BMI088_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
    BMI088_Read_Write_Byte(reg | 0x80);

    while (len != 0)
    {
        *buf = BMI088_Read_Write_Byte(0x55);
        buf++;
        len--;
    }
}

/**
 * @brief BMI088加速度计片选脚拉低
 *
 */
void Class_BMI088::BMI088_ACCEL_NS_L()
{
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_RESET);
}

/**
 * @brief BMI088加速度计片选脚拉高
 *
 */
void Class_BMI088::BMI088_ACCEL_NS_H()
{
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_SET);
}

/**
 * @brief BMI088陀螺仪片选脚拉低
 *
 */
void Class_BMI088::BMI088_GYRO_NS_L()
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
}

/**
 * @brief BMI088陀螺仪片选脚拉高
 *
 */
void Class_BMI088::BMI088_GYRO_NS_H()
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
}