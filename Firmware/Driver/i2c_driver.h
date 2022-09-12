#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"

typedef struct I2cDrv_t
{
    S32 (*I2cOpen)(S8 *Path, S32 Flag);
    S32 (*I2cClose)(S32 Fd);
    S32 (*I2cSetAddr)(S32 Fd, U8 Addr);
    S32 (*I2cRead)(S32 Fd, U8 *Buff, S32 Len);
    S32 (*I2cWrite)(S32 Fd, U8 *Buff, S32 Len);
} I2cDrv_t;

S32 I2cOpen(S8 *Path, S32 Flag);
S32 I2cClose(S32 Fd);
S32 I2cSetAddr(S32 Fd, U8 Addr);
S32 I2cRead(S32 Fd, U8 *Buff, S32 Len);
S32 I2cWrite(S32 Fd, U8 *Buff, S32 Len);
S32 I2cDrvRegister(I2cDrv_t *Drv);

#ifdef __cplusplus
}
#endif
#endif  //ifndef _I2C_DRIVER_H_
