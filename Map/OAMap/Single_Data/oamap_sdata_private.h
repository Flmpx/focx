#ifndef OAMAP_SDATA_PRIVATE_H
#define OAMAP_SDATA_PRIVATE_H
#include "oamap_sdata.h"

/// @brief 打印单类型开放寻址哈希表的键
/// @param pMap 哈希表指针
/// @param key 待打印的键
extern void printSKeyInSOAMap(OAMap_S* pMap, Data_S key);


/// @brief 打印单类型开放寻址哈希表的值
/// @param pMap 哈希表指针
/// @param val 待打印的值
extern void printSValInSOAMap(OAMap_S* pMap, Data_S val);

/// @brief 打印单类型开放寻址哈希表的条目
/// @param pMap 哈希表指针
/// @param entry 待打印的条目
extern void printSEntryInSOAMap(OAMap_S* pMap, Entry_S_inOAMap entry);

/// @brief 打印单类型开放寻址哈希表
/// @param pMap 哈希表指针
extern void printSOAMap(OAMap_S* pMap);

#endif