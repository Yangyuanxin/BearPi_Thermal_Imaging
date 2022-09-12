#include "i2c_device_amg8833.h"

static U8 DevAddr = 0;
extern I2C_HandleTypeDef hi2c1;

S32 I2cDevAmg8833Open(S8 *Path, S32 Flag)
{
    (Void)Path;
    (Void)Flag;

    MX_I2C1_Init();

    return 0;
}

S32 I2cDevAmg8833Close(S32 Fd)
{
    (Void)Fd;

    HAL_I2C_DeInit(&hi2c1);

    return 0;
}

S32 I2cDevAmg8833SetAddr(S32 Fd, U8 Addr)
{
    (Void)Fd;

    DevAddr = Addr;

    return 0;
}

S32 I2cDevAmg8833Read(S32 Fd, U8 *Buff, S32 Len)
{
    (Void)Fd;
    U8 Status = 0;
    U16 Addr = (DevAddr << 1) | 0x01;

    Status = HAL_I2C_Master_Receive(&hi2c1, Addr, Buff, Len, 0xff);

    if(Status != HAL_OK)
        return -1;

    return 0;
}

S32 I2cDevAmg8833Write(S32 Fd, U8 *Buff, S32 Len)
{
    (Void)Fd;
    U8 Status = 0;
    U16 Addr = (DevAddr << 1);

    Status = HAL_I2C_Master_Transmit(&hi2c1, Addr, Buff, Len, 0xff);

    if(Status != HAL_OK)
        return -1;

    return 0;
}

I2cDrv_t I2cDev =
{
    .I2cOpen        = I2cDevAmg8833Open,
    .I2cClose       = I2cDevAmg8833Close,
    .I2cSetAddr     = I2cDevAmg8833SetAddr,
    .I2cRead        = I2cDevAmg8833Read,
    .I2cWrite       = I2cDevAmg8833Write,
};

S32 I2cDevAmg8833Init(Void)
{
    return I2cDrvRegister(&I2cDev);
}
