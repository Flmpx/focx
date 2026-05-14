#ifndef CHAINMAP_SDATA_PRIVATE_H
#define CHAINMAP_SDATA_PRIVATE_H
#include "chainmap_sdata.h"

/// @brief 打印单类型链式哈希表的键
/// @param pMap 哈希表指针
/// @param key 待打印的键
extern void printSKeyInSChainMap(ChainMap_S* pMap, Data_S key);

/// @brief 打印单类型链式哈希表的值
/// @param pMap 哈希表指针
/// @param val 待打印的值
extern void printSValInSChainMap(ChainMap_S* pMap, Data_S val);

/// @brief 打印单类型链式哈希表的条目
/// @param pMap 哈希表指针
/// @param entry 待打印的条目
extern void printSEntryInSChainMap(ChainMap_S* pMap, Entry_S_inChainMap entry);




/// @brief 打印单类型链式哈希表
/// @param pMap 哈希表指针
extern void printSChainMap(ChainMap_S* pMap);

#endif