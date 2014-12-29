#ifndef __YK_H
#define __YK_H
#define TxBufferSize2   64
#define RxBufferSize2   64

#define YK_SUCCESS 0x89
#define YK_FAIL 0XE0

extern u8  TxBuffer2[TxBufferSize2];
extern u8  RxBuffer2[RxBufferSize2];
extern __IO u8 TxCounter2;
extern __IO u8 RxCounter2;
extern u8 NumofTX;
extern u8 NumofRX;

#endif