#ifndef OASET_MDATA_PRIVATE_H
#define OASET_MDATA_PRIVATE_H
#include "oaset_mdata.h"

/// @brief 打印多类型开放寻址集合的键
/// @param key 待打印的键
extern void printMKeyInMOASet(Data_M key);


/// @brief 打印多类型开放寻址集合
/// @param pSet 集合指针
extern void printMOASet(OASet_M* pSet);

#endif