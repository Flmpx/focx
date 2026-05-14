#ifndef OAMAP_MDATA_PRIVATE_H
#define OAMAP_MDATA_PRIVATE_H
#include "oamap_mdata_private.h"

/// @brief 打印多类型开放寻址哈希表的键
/// @param key 待打印的键
extern void printMKeyInMOAMap(Data_M key);


/// @brief 打印多类型开放寻址哈希表的值
/// @param val 待打印的值
extern void printMValInMOAMap(Data_M val);

/// @brief 打印多类型开放寻址哈希表的条目
/// @param entry 待打印的条目
extern void printMEntryInMOAMap(Entry_M_inOAMap entry);

/// @brief 打印多类型开放寻址哈希表
/// @param pMap 哈希表指针
extern void printMOAMap(OAMap_M* pMap);
#endif