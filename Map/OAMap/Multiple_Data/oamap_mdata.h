#ifndef OAMAP_MDATA_H
#define OAMAP_MDATA_H
#include <stdbool.h>
#include "../../../base.h"

/// @brief 在OAMap_M中的Entry类型(Entry_M_inOAMap)
typedef struct Entry_M_inOAMap {
    Data_M key;
    Data_M val;
    int state;
    bool isEmpty;
} Entry_M_inOAMap;

/// @brief OAMap_M->采用开放寻址法的Map, 可存储任意类型数据
typedef struct OAMap_M {
    Entry_M_inOAMap* arr;
    int mod;
    int len;
    int size;
} OAMap_M;


/// @brief 初始化多类型开放寻址哈希表
/// @param pMap 哈希表指针
extern void initMOAMap(OAMap_M* pMap);





/// @brief 释放多类型开放寻址哈希表中的值
/// @param val 待释放的值
extern void freeMValInMOAMap(Data_M* val);


/// @brief 释放多类型开放寻址哈希表中的键
/// @param key 待释放的键
extern void freeMKeyInMOAMap(Data_M* key);


/// @brief 释放多类型开放寻址哈希表中的条目
/// @param entry 待释放的条目
extern void freeMEntryInMOAMap(Entry_M_inOAMap* entry);

/// @brief 释放多类型开放寻址哈希表
/// @param pMap 哈希表指针
extern void freeMOAMap(OAMap_M* pMap);

/// @brief 在多类型开放寻址哈希表中插入键值对
/// @param pMap 哈希表指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertMKeyAndMValInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal);



/// @brief 对多类型开放寻址哈希表进行缩容
/// @param pMap 哈希表指针
/// @return 操作状态码
extern InfoOfReturn shrinkMOAMap(OAMap_M* pMap);



/// @brief 通过键找到多类型开放寻址哈希表中的键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @param isCopyKey 返回的键是否要复制
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMKeyInMOAMap函数释放
extern Data_M getMKeyByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyKey);


/// @brief 通过键找到多类型开放寻址哈希表中的值
/// @param pMap 哈希表指针
/// @param key 待查找的值的键
/// @param isCopyKey 返回的值是否要复制
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMValInMOAMap函数释放
extern Data_M getMValByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyVal);

/// @brief 通过键找到多类型开放寻址哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待查找的条目的键
/// @param isCopyEntry 返回的条目是否要复制
/// @return 返回找到的条目, 若没找到, 返回空条目(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMEntryInMOAMap函数释放
extern Entry_M_inOAMap getMEntryByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyEntry);


/// @brief 判断多类型开放寻址哈希表中是否存在当前键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasMKeyInMOAMap(OAMap_M* pMap, Data_M key);


/// @brief 删除多类型开放寻址哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待删除的条目的键
/// @return 操作结果状态码
extern InfoOfReturn delMEntryByMKeyInMOAMap(OAMap_M* pMap, Data_M key);


#endif