#include "AMG883x.h"

S32 Fd = -1;
Float HDTemp[LINE_NUM][LINE_NUM];
extern I2C_HandleTypeDef hi2c1;

S32 AMG883xInit(Void)
{
    S32 Ret = -1;
    S32 Len = -1;
    U8 Buff[2] = {0};

    printf("I2cDevAmg8833Init start!\n");

    Ret = I2cDevAmg8833Init();

    if(Ret < 0)
    {
        printf("I2cDevAmg8833Init failed!\n");
        goto ErrorHandler;
    }

    Fd = I2cOpen(Null, 0);

    if(Fd < 0)
    {
        printf("I2cOpen failed!\n");
        goto ErrorHandler;
    }

    Ret = I2cSetAddr(Fd, AMG88xx_ADDRESS);

    if(Ret < 0)
    {
        printf("I2cSetAddr failed!\n");
        goto ErrorHandler;
    }

    Buff[0] = AMG88xx_PCTL;
    Buff[1] = AMG88xx_NORMAL_MODE;
    Len = I2cWrite(Fd, Buff, 2);

    if(Len < 0)
    {
        printf("I2cWrite failed!\n");
        goto ErrorHandler;
    }

    Buff[0] = AMG88xx_RST;
    Buff[1] = AMG88xx_INITIAL_RESET;
    Len = I2cWrite(Fd, Buff, 2);

    if(Len < 0)
    {
        printf("I2cWrite failed!\n");
        return -1;
    }

    Buff[0] = AMG88xx_FPSC;
    Buff[1] = AMG88xx_FPS_10;
    Len = I2cWrite(Fd, Buff, 2);

    if(Len < 0)
    {
        printf("I2cWrite failed!\n");
        goto ErrorHandler;
    }

    printf("I2cDevAmg8833Init success!\n");

    return 0;

ErrorHandler:
    return -1;
}

Float AMG883xReadMatrix(Float *Buf, U8 Size)
{
    S32 Len = -1;
    U16 Recast;
    U8 DevRegAddr;
    Float Converted = 0.0;
    U8 RawDataLen = Min(Size << 1, AMG88xx_PIXEL_ARRAY_SIZE << 1);
    U8 RawData[RawDataLen];

    DevRegAddr = AMG88xx_PIXEL_OFFSET;
    Len = I2cWrite(Fd, &DevRegAddr, 1);

    if(Len < 0)
    {
        printf("I2cWrite failed!\n");
        return 0;
    }

    Len = I2cRead(Fd, RawData, RawDataLen);

    if(Len < 0)
    {
        printf("I2cRead failed!\n");
        return 0;
    }

    for(int i = 0; i < Size; i++)
    {
        U8 Pos = i << 1;
        Recast = ((U16)RawData[Pos + 1] << 8) | ((U16)RawData[Pos]);
        Converted = SignedMag12ToFloat(Recast) * (Double)AMG88xx_PIXEL_TEMP_CONVERSION;
        Buf[i] = Converted;
    }

    return Converted;
}

Void ZoomArr(Float *SiganlArr, Float DoubleArr[][LINE_NUM])
{
    Float intPoint, val;
    U8 Row, Col, Alow, AHigh, InCr;

    for(Row = 0; Row < 8; Row++)
    {
        for(Col = 0; Col < 4; Col++)
        {
            val = SiganlArr[Row * 8 + Col];
            SiganlArr[Row * 8 + Col] = SiganlArr[Row * 8 + 8 - Col - 1];
            SiganlArr[Row * 8 + 8 - Col - 1] = val;
        }
    }

   TurnRight(SiganlArr);

    for (Row = 0; Row < 8; Row ++)
    {
        for (Col = 0; Col < LINE_NUM; Col ++)
        {
            Alow =  Col / 10 + (Row * 8);
            AHigh = (Col / 10) + 1 + (Row * 8);
            intPoint = ((SiganlArr[AHigh] - SiganlArr[Alow] ) / (Double)10.0);
            InCr = Col % 10;
            val = (intPoint * InCr ) +  SiganlArr[Alow];
            DoubleArr[ (7 - Row) * 10][Col] = val;
        }
    }

    for (Col = 0; Col < LINE_NUM; Col ++)
    {
        for (Row = 0; Row < 70; Row ++)
        {
            Alow =  (Row / 10 ) * 10;
            AHigh = Alow + 10;
            intPoint =  ((DoubleArr[AHigh][Col] - DoubleArr[Alow][Col] ) / (Double)10.0);
            InCr = Row % 10;
            val = (intPoint * InCr ) +  DoubleArr[Alow][Col];
            DoubleArr[ Row ][Col] = val;
        }
    }
}

Void AMG883xMatrixDisplay(Void)
{
    U8 j = 0;
    Float Inc = 0.0;
    S8 Buff[25] = {0};
    Float Min = 0, Max = 0;
    static U8 DisplayOn = 0;
		Float RawMatrix[AMG88xx_PIXEL_ARRAY_SIZE];
    
    AMG883xReadMatrix(RawMatrix, AMG88xx_PIXEL_ARRAY_SIZE);
    FindMaxTemp(&Min, &Max, RawMatrix);

    ZoomArr(RawMatrix, HDTemp);

    for (int row = 0; row < 70; row ++)
    {
        for (int col = 0; col < 70; col++)
        {
            uint8_t Index = Map(HDTemp[row][col], MINTEMP, MAXTEMP, 0, 255);
            Index = ConStrain(Index, 0, 255);
            fillRect((row * 3) + 0, (col * 3) + 0, 3, 3, GetColors(Index));
        }
    }
    
    Inc = (Max - Min) / (Double)80.0;
    for(Float i = Min; i < Max; i+=Inc)
    {
        uint8_t Index = Map(i, MINTEMP, MAXTEMP, 0, 255);
        Index = ConStrain(Index, 0, 255);
        drawFastHLine(215, 205 - (j++) * 2.6, 20, GetColors(Index));
    }

    memset(Buff, 0, sizeof(Buff));
    snprintf(Buff, sizeof(Buff), "Min:%.1f Max:%.1f*C", Min, Max);
    Buff[strlen(Buff)] = '\0';
    lcd_draw_text(0, 210, Buff, 24);
    
    if(0 == DisplayOn)
    {
        lcd_display_on();
        DisplayOn = 1;
    }
}
