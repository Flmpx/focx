#ifndef CHAINMAP_MDATA_PRIVATE_H
#define CHAINMAP_MDATA_PRIVATE_H
#include "chainmap_mdata.h"

/// @brief 打印多类型链式哈希表的键
/// @param key 待打印的键
extern void printMKeyInMChainMap(Data_M key);



/// @brief 打印多类型链式哈希表的值
/// @param val 待打印的值
extern void printMValInMChainMap(Data_M val);

/// @brief 打印多类型链式哈希表的条目
/// @param entry 待打印的条目
extern void printMEntryInMChainMap(Entry_M_inChainMap entry);

/// @brief 打印多类型链式哈希表
/// @param pMap 哈希表指针
extern void printMChainMap(ChainMap_M* pMap);

#endif