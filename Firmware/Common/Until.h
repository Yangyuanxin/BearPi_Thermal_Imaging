#ifndef _UNTIL_H_
#define _UNTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"


S32 Min(S32 A, S32 B);
U16 GetColors(U8 Index);
Void TurnRight(Float *Arr);
Void Reverse(Float *p, S32 Size);
Float SignedMag12ToFloat(U16 Val);
U8 ConStrain(U8 Amt, U8 Low, U8 High);
Void FindMaxTemp(Float *Min, Float *Max, Float *Array);
U8 Map(Float Val, U8 InMin, U8 InMax, U8 OutMin, U8 OutMax);

#ifdef __cplusplus
}
#endif
#endif  //ifndef _UNTIL_H_
