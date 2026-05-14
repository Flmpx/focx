#ifndef CHAINSET_MDATA_PRIVATE_H
#define CHAINSET_MDATA_PRIVATE_H
#include "chainset_mdata.h"


/// @brief 打印多类型链式集合的键
/// @param key 待打印的键
extern void printMKeyInMChainSet(Data_M key);


/// @brief 打印多类型链式集合
/// @param pSet 集合指针
extern void printMChainSet(ChainSet_M* pSet);
#endif