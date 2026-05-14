#ifndef CHAINMAP_SDATA_H
#define CHAINMAP_SDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Node_S_inChainMap Node_S_inChainMap;
typedef struct List_S_inChainMap List_S_inChainMap;

/// @brief 在ChainMap_S中的Entry类型(Entry_S_inChainMap)
typedef struct Entry_S_inChainMap {
    Data_S key;
    Data_S val;
    bool isEmpty;
} Entry_S_inChainMap;

#ifdef NODE_S_INCHAINMAP

/// @brief 在ChainMap_S中的链表节点(Node_S_inChainMap类型)
struct Node_S_inChainMap {
    struct Node_S_inChainMap* next;
    struct Node_S_inChainMap* prev;
    Entry_S_inChainMap entry;
};
#endif

#ifdef LIST_S_INCHAINMAP

/// @brief 在ChainMap_S中的链表(Node_S_inChainMap类型)
struct List_S_inChainMap {
    Node_S_inChainMap* head;
    Node_S_inChainMap* tail;
    int size;
};
#endif



/// @brief ChainMap_S->采用链表法的Map, 可存储单一类型数据
typedef struct ChainMap_S {
    List_S_inChainMap* arr;
    InfoOfData* keyInfo;
    InfoOfData* valInfo;
    int mod;
    int len;
    int size;
} ChainMap_S;

/// @brief 初始化单类型链式哈希表
/// @param pMap 哈希表指针
/// @param keyInfo 键的数据信息的指针
/// @param valInfo 值的数据信息的指针
extern void initSChainMap(ChainMap_S* pMap, InfoOfData* keyInfo, InfoOfData* valInfo);






/// @brief 释放单类型链式哈希表中的值
/// @param pMap 哈希表指针
/// @param val 待释放的值
extern void freeSValInSChainMap(ChainMap_S* pMap, Data_S* val);

/// @brief 释放单类型链式哈希表中的条目
/// @param pMap 哈希表指针
/// @param entry 待释放的条目
extern void freeSEntryInSChainMap(ChainMap_S* pMap, Entry_S_inChainMap* entry);

/// @brief 释放单类型链式哈希表
/// @param pMap 哈希表指针
extern void freeSChainMap(ChainMap_S* pMap);


/// @brief 在单类型链式哈希表中插入键值对
/// @param pMap 哈希表指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSKeyAndSValInSChainMap(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal);

/// @brief 通过键找到单类型链式哈希表中的值, 使用完后用freeSValInSChainMap函数进行释放
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_S getCopySValBySKeyInSChainMap(ChainMap_S* pMap, Data_S key);


/// @brief 通过键找到单类型链式哈希表中的值, 可以直接修改内部数据
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_S getPtrSValBySKeyInSChainMap(ChainMap_S* pMap, Data_S key);

/// @brief 通过键找到单类型链式哈希表中的条目, 使用完后用freeSEntryInSChainMap函数进行释放
/// @param pMap 哈希表指针
/// @param key 待查找的条目的键
/// @return 返回找到的条目, 若没找到, 返回空条目(用字段isEmpty检查)
extern Entry_S_inChainMap getCopySEntryBySKeyInSChainMap(ChainMap_S* pMap, Data_S key);

/// @brief 判断单类型链式哈希表中是否存在当前键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasSKeyInSChainMap(ChainMap_S* pMap, Data_S key);

/// @brief 删除单类型链式哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待删除的条目的键
/// @return 操作结果状态码
extern InfoOfReturn delSEntryBySKeyInSChainMap(ChainMap_S* pMap, Data_S key);


#endif