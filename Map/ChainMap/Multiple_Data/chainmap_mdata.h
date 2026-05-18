#ifndef CHAINMAP_MDATA_H
#define CHAINMAP_MDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Node_M_inChainMap Node_M_inChainMap;
typedef struct List_M_inChainMap List_M_inChainMap;



/// @brief 在ChainMap_M中的Entry类型(Entry_M_inChainMap)
typedef struct Entry_M_inChainMap {
    Data_M key;
    Data_M val;
    bool isEmpty;
} Entry_M_inChainMap;



#ifdef NODE_M_INCHAINMAP

/// @brief 在ChainMap中的链表节点, Node_M_inChainMap类型
struct Node_M_inChainMap {
    struct Node_M_inChainMap* next;
    struct Node_M_inChainMap* prev;
    Entry_M_inChainMap entry;
};

#endif


#ifdef LIST_M_INCHAINMAP

/// @brief 在ChainMap中的链表, List_M_inChainMap
struct List_M_inChainMap {
    Node_M_inChainMap* head;
    Node_M_inChainMap* tail;
    int size;
};

#endif

/// @brief ChainMap_M->采用链表法的Map, 可存储任意类型数据
typedef struct ChainMap_M {
    List_M_inChainMap* arr;
    int mod;
    int len;
    int size;
} ChainMap_M;


/// @brief 初始化多类型链式哈希表
/// @param pMap 哈希表指针
extern void initMChainMap(ChainMap_M* pMap);




/// @brief 释放多类型链式哈希表中的值
/// @param val 待释放的值
extern void freeMValInMChainMap(Data_M* val);


/// @brief 释放多类型链式哈希表中的键
/// @param key 待释放的键
extern void freeMKeyInMChainMap(Data_M* key);


/// @brief 释放多类型链式哈希表中的条目
/// @param entry 待释放的条目
extern void freeMEntryInMChainMap(Entry_M_inChainMap* entry);

/// @brief 释放多类型链式哈希表
/// @param pMap 哈希表指针
extern void freeMChainMap(ChainMap_M* pMap);

/// @brief 在多类型链式哈希表中插入键值对
/// @param pMap 哈希表指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertMKeyAndMValInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal);


/// @brief 对多类型链式哈希表进行缩容
/// @param pMap 哈希表指针
/// @return 操作状态码
extern InfoOfReturn shrinkMChainMap(ChainMap_M* pMap);



/// @brief 通过键找到多类型链式哈希表中的键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @param isCopyKey 返回的键是否要复制
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMKeyInMChainMap函数释放
extern Data_M getMKeyByMKeyInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyKey);




/// @brief 通过键找到多类型链式哈希表中的值
/// @param pMap 哈希表指针
/// @param key 待查找的值的键
/// @param isCopyKey 返回的值是否要复制
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMValInMChainMap函数释放
extern Data_M getMValByMKeyInMchainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyVal);


/// @brief 通过键找到多类型链式哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待查找的条目的键
/// @param isCopyEntry 返回的条目是否要复制
/// @return 返回找到的条目, 若没找到, 返回空条目(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMEntryInMChainMap函数释放
extern Entry_M_inChainMap getMEntryByMKeyInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyEntry);



/// @brief 判断多类型链式哈希表中是否存在当前键
/// @param pMap 哈希表指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasMKeyInMChainMap(ChainMap_M* pMap, Data_M key);


/// @brief 删除多类型链式哈希表中的条目
/// @param pMap 哈希表指针
/// @param key 待删除的条目的键
/// @return 操作结果状态码
extern InfoOfReturn delMEntryByMKeyInMChainMap(ChainMap_M* pMap, Data_M key);





#endif