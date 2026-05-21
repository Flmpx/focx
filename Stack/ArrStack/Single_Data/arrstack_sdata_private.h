#ifndef ARRSTACK_SDATA_PRIVATE_H
#define ARRSTACK_SDATA_PRIVATE_H
#include "arrstack_sdata.h"


/// @brief 打印单类型数组栈中的值
/// @param val 待打印的值
extern void printSValInSArrStack(ArrStack_S* pStack, Data_S val);



/// @brief 打印单类型数组栈
/// @param pStack 栈指针
extern void printSArrStack(ArrStack_S* pStack);

#endif