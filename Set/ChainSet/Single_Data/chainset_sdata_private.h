#ifndef CHAINSET_SDATA_PRIVATE_H
#define CHAINSET_SDATA_PRIVATE_H
#include "chainset_sdata.h"


/// @brief 打印单类型链式集合的键
/// @param pSet 集合指针
/// @param key 待打印的键
extern void printSKeyInSChainSet(ChainSet_S* pSet, Data_S key);



/// @brief 打印单类型链式集合
/// @param pSet 集合指针
extern void printSChainSet(ChainSet_S* pSet);

#endif