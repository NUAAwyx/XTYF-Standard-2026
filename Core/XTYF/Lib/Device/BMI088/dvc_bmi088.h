#ifndef DVC_BMI088_H
#define DVC_BMI088_H

#include "Lib/Driver/SPI/drv_spi.h"
#include "BMI088reg.h"

#define IMU_Calibration_ENABLE  0U

#define BMI088_TEMP_FACTOR 0.125f
#define BMI088_TEMP_OFFSET 23.0f

#define BMI088_WRITE_ACCEL_REG_NUM 6
#define BMI088_WRITE_GYRO_REG_NUM 6

#define BMI088_GYRO_DATA_READY_BIT 0
#define BMI088_ACCEL_DATA_READY_BIT 1
#define BMI088_ACCEL_TEMP_DATA_READY_BIT 2

#define BMI088_LONG_DELAY_TIME 80
#define BMI088_COM_WAIT_SENSOR_TIME 150

#define BMI088_ACCEL_IIC_ADDRESSE (0x18 << 1)
#define BMI088_GYRO_IIC_ADDRESSE (0x68 << 1)

#define BMI088_ACCEL_3G_SEN 0.0008974358974f
#define BMI088_ACCEL_6G_SEN 0.00179443359375f
#define BMI088_ACCEL_12G_SEN 0.0035888671875f
#define BMI088_ACCEL_24G_SEN 0.007177734375f

#define BMI088_GYRO_2000_SEN 0.00106526443603169529841533860381f
#define BMI088_GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define BMI088_GYRO_500_SEN 0.00026631610900792382460383465095346f
#define BMI088_GYRO_250_SEN 0.00013315805450396191230191732547673f
#define BMI088_GYRO_125_SEN 0.000066579027251980956150958662738366f

// 需手动修改
#define GxOFFSET 0.00247530174f
#define GyOFFSET 0.000393082853f
#define GzOFFSET 0.000393082853f
#define gNORM 9.69293118f


typedef enum
{
    BMI088_NO_ERROR                     = 0x00,
    BMI088_ACCEL_PWR_CTRL_ERROR         = 0x01,
    BMI088_ACCEL_PWR_CONF_ERROR         = 0x02,
    BMI088_ACCEL_CONF_ERROR             = 0x03,
    BMI088_ACCEL_SELF_TEST_ERROR        = 0x04,
    BMI088_ACCEL_RANGE_ERROR            = 0x05,
    BMI088_INT1_IO_CTRL_ERROR           = 0x06,
    BMI088_INT_MAP_DATA_ERROR           = 0x07,
    BMI088_GYRO_RANGE_ERROR             = 0x08,
    BMI088_GYRO_BANDWIDTH_ERROR         = 0x09,
    BMI088_GYRO_LPM1_ERROR              = 0x0A,
    BMI088_GYRO_CTRL_ERROR              = 0x0B,
    BMI088_GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C,
    BMI088_GYRO_INT3_INT4_IO_MAP_ERROR  = 0x0D,

    BMI088_SELF_TEST_ACCEL_ERROR        = 0x80,
    BMI088_SELF_TEST_GYRO_ERROR         = 0x40,
    BMI088_NO_SENSOR                    = 0xFF,
}BMI088_Status_e;

class Class_BMI088
{
public:

    void BMI088_Init();

    void BMI088_Data_Update();

protected:

    Struct_SPI_Manage_Object SPI_Manage_Object;

    // 加速度计数据
    int16_t Accel_X;
    int16_t Accel_Y;
    int16_t Accel_Z;

    // 陀螺仪数据
    int16_t Gyro_X;
    int16_t Gyro_Y;
    int16_t Gyro_Z;

    // 温度数据
    int16_t Temperature;

    // 零偏初始化
    bool Offsets_Init;

    // 陀螺仪偏移量
    float Offsets_Gyro_X;
    float Offsets_Gyro_Y;
    float Offsets_Gyro_Z;
    float Offsets_g;


    BMI088_Status_e BMI088_Gyro_Init();
    BMI088_Status_e BMI088_Accel_Init();

    void BMI088_Offset_Update();

    void BMI088_delay_ms(uint16_t ms);
    void BMI088_delay_us(uint16_t us);

    void BMI088_Accel_Write_Single_Reg(uint8_t reg, uint8_t data);
    void BMI088_Accel_Read_Single_Reg(uint8_t reg, uint8_t data);
    void BMI088_Accel_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len);

    void BMI088_Gyro_Write_Single_Reg(uint8_t reg, uint8_t data);
    void BMI088_Gyro_Read_Single_Reg(uint8_t reg, uint8_t data);
    void BMI088_Gyro_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len);

    void BMI088_ACCEL_NS_L();
    void BMI088_ACCEL_NS_H();
    void BMI088_GYRO_NS_L();
    void BMI088_GYRO_NS_H();

    uint8_t BMI088_Read_Write_Byte(uint8_t Tx_Data);

    void BMI088_Write_Single_Reg(uint8_t reg, uint8_t data);
    void BMI088_Read_Single_Reg(uint8_t reg, uint8_t *return_data);
    void BMI088_Read_Multi_Reg(uint8_t reg, uint8_t *buf, uint8_t len);
};



#endif
