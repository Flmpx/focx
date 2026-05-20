#ifndef OAMAP_SDATA_H
#define OAMAP_SDATA_H
#include <stdbool.h>
#include "../../../base.h"




/// @brief 在OAMap_S中的Entry类型(Entry_S_inOAMap)
typedef struct Entry_S_inOAMap {
    Data_S key;
    Data_S val;
    int state;
    bool isEmpty;
} Entry_S_inOAMap;

/// @brief OAMap_S->采用开放寻址法的Map, 可存储单一类型数据
typedef struct OAMap_S {
    Entry_S_inOAMap* buckets;
    int mod;
    int len;
    int size;
    InfoOfData* keyInfo;
    InfoOfData* valInfo;
} OAMap_S;



/// @brief 初始化单类型开放寻址哈希表
/// @param pMap 哈希表指针
/// @param keyInfo 键的数据信息的指针
/// @param valInfo 值的数据信息的指针
extern void initSOAMap(OAMap_S* pMap, InfoOfData* keyInfo, InfoOfData* valInfo);




/// @brief 释放单类型开放寻址哈希表中的键
/// @param pMap 哈希表指针
/// @param val 待释放的键
extern void freeSKeyInSOAMap(OAMap_S* pMap, Data_S* key);


/// @brief 释放单类型开放寻址哈希表中的值
/// @param pMap 哈希表指针
/// @param val 待释放的值
extern void freeSValInSOAMap(OAMap_S* pMap, Data_S* val);

/// @brief 释放单类型开放寻址哈希表中的条目
/// @param pMap 哈希表指针
/// @param entry 待释放的条目
extern void freeSEntryInSOAMap(OAMap_S* pMap, Entry_S_inOAMap* entry);


/// @brief 释放单类型开放寻址哈希表
/// @param pMap 哈希表指针
/// @note 数据以及哈希表的桶也会删除
extern void freeSOAMap(OAMap_S* pMap);

/// @brief 清空单类型开放寻址哈希表
/// @param pMap 哈希表指针
/// @note 只清除数据, 不清除哈希表的桶
extern void clearSOAMap(OAMap_S* pMap);

/// @brief 在单类型开放寻址哈希表中插入键值对
/// @param pMap 哈希表指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSKeyAndSValInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal);


/// @brief 对单类型开放寻址哈希表进行缩容
/// @param pMap 哈希表指针
/// @return 操作状态码
extern InfoOfReturn shrinkSOAMap(OAMap_S* pMap);


/// @brief 通过键找到单类型开放寻址哈希表中的键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @param isCopyKey 返回的键是否要复制
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSKeyInSOAMap函数释放
extern Data_S getSKeyBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey);



/// @brief 通过键找到单类型开放寻址哈希表中的值
/// @param pMap 哈希表指针
/// @param key 待查找的值的键
/// @param isCopyKey 返回的值是否要复制
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSValInSOAMap函数释放
extern Data_S getSValBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyVal);



/// @brief 通过键找到单类型开放寻址哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待查找的条目键
/// @param isCopyEntry 返回的条目是否要复制
/// @return 返回找到的条目, 若没找到, 返回空条目(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSEntryInSOAMap函数释放
extern Entry_S_inOAMap getSEntryBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyEntry);



/// @brief 判断单类型开放寻址哈希表中是否存在当前键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasSKeyInSOAMap(OAMap_S* pMap, Data_S key);


/// @brief 删除单类型开放寻址哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待删除的条目的键
/// @return 操作结果状态码
extern InfoOfReturn delSEntryBySKeyInSOAMap(OAMap_S* pMap, Data_S key);





#endif