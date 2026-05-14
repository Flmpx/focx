#ifndef OASET_SDATA_PRIVATE_H
#define OASET_SDATA_PRIVATE_H
#include "oaset_sdata.h"


/// @brief 打印单类型开放寻址集合的键
/// @param pSet 集合指针
/// @param key 待打印的键
extern void printSKeyInSOASet(OASet_S* pSet, Data_S key);


/// @brief 打印单类型开放寻址集合
/// @param pSet 集合指针
extern void printSOASet(OASet_S* pSet);

#endif