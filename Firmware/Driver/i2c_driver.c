#include "i2c_driver.h"

#define DRV_CNT 1024

static S32 I2cFdCount = 0;
static I2cDrv_t *I2cDrv[DRV_CNT];


S32 I2cOpen(S8 *Path, S32 Flag)
{
    (Void)Path;
    (Void)Flag;
    S32 Status = -1;
    
    if(I2cDrv[I2cFdCount - 1] && I2cDrv[I2cFdCount - 1]->I2cOpen)
    {
        Status = I2cDrv[I2cFdCount - 1]->I2cOpen(Path, Flag);

        if(0 != Status)
            goto ErrorHandler;
        
        return I2cFdCount;
    }

ErrorHandler:
    return -1;
}

S32 I2cClose(S32 Fd)
{
    S32 Status = -1;

    if(Fd <= 0 || 0 == I2cFdCount)
        goto ErrorHandler;

    if(I2cDrv[I2cFdCount - 1] && I2cDrv[I2cFdCount - 1]->I2cClose)
    {
        Status = I2cDrv[I2cFdCount - 1]->I2cClose(Fd);

        if(0 != Status)
            goto ErrorHandler;

        return I2cFdCount;
    }

ErrorHandler:
    return -1;
}

S32 I2cSetAddr(S32 Fd, U8 Addr)
{
    S32 Status = -1;

    if(Fd <= 0 || 0 == I2cFdCount)
        goto ErrorHandler;

    if(I2cDrv[I2cFdCount - 1] && I2cDrv[I2cFdCount - 1]->I2cSetAddr)
    {
        Status = I2cDrv[I2cFdCount - 1]->I2cSetAddr(Fd, Addr);

        if(0 != Status)
            goto ErrorHandler;

        return 0;
    }

ErrorHandler:
    return -1;
}

S32 I2cRead(S32 Fd, U8 *Buff, S32 Len)
{
    S32 Status = -1;

    if(Fd <= 0 || 0 == I2cFdCount)
        goto ErrorHandler;

    if(!Buff)
        goto ErrorHandler;

    if(Len <= 0)
        goto ErrorHandler;

    if(I2cDrv[I2cFdCount - 1] && I2cDrv[I2cFdCount - 1]->I2cRead)
    {
        Status = I2cDrv[I2cFdCount - 1]->I2cRead(Fd, Buff, Len);

        if(0 != Status)
            goto ErrorHandler;

        return Len;
    }

ErrorHandler:
    return -1;
}

S32 I2cWrite(S32 Fd, U8 *Buff, S32 Len)
{
    S32 Status = -1;

    if(Fd <= 0 || 0 == I2cFdCount)
        goto ErrorHandler;

    if(!Buff)
        goto ErrorHandler;

    if(Len <= 0)
        goto ErrorHandler;

    if(I2cDrv[I2cFdCount - 1] && I2cDrv[I2cFdCount - 1]->I2cWrite)
    {
        Status = I2cDrv[I2cFdCount - 1]->I2cWrite(Fd, Buff, Len);

        if(0 != Status)
            goto ErrorHandler;

        return Len;
    }

ErrorHandler:
    return -1;
}


S32 I2cDrvRegister(I2cDrv_t *Drv)
{
    if(!I2cDrv[I2cFdCount])
    {
        I2cDrv[I2cFdCount] = Drv;
        ++I2cFdCount;
        return I2cFdCount;
    }
    
    I2cFdCount = 0;
    
    return I2cFdCount;
}
