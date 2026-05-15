#ifndef CHAINSET_SDATA_H
#define CHAINSET_SDATA_H
#include <stdbool.h>
#include "../../../base.h"


typedef struct Node_S_inChainSet Node_S_inChainSet;
typedef struct List_S_inChainSet List_S_inChainSet;



#ifdef ENTRY_S_INCHAINSET

/// @brief 在ChainSet_S中的Entry类型(Entry_S_inChainSet)
typedef struct Entry_S_inChainSet {
    Data_S key;
    bool isEmpty;
} Entry_S_inChainSet;

#endif

#ifdef NODE_S_INCHAINSET

/// @brief 在ChainSet_S中的链表节点(Node_S_inChainSet类型)
struct Node_S_inChainSet {
    struct Node_S_inChainSet* next;
    struct Node_S_inChainSet* prev;
    Entry_S_inChainSet entry;
};

#endif

#ifdef LIST_S_INCHAINSET

/// @brief 在ChainSet_S中的链表(Node_S_inChainSet类型)
struct List_S_inChainSet {
    Node_S_inChainSet* head;
    Node_S_inChainSet* tail;
    int size;
};
#endif



/// @brief ChainSet_S->采用链表法的Set, 可存储单一类型数据
typedef struct ChainSet_S {
    List_S_inChainSet* arr;
    InfoOfData* keyInfo;
    int mod;
    int len;
    int size;
} ChainSet_S;

/// @brief 初始化单类型链式集合
/// @param pSet 集合指针
/// @param keyInfo 键的数据信息的指针
extern void initSChainSet(ChainSet_S* pSet, InfoOfData* keyInfo);






/// @brief 释放单类型链式集合中的键
/// @param pSet 集合指针
/// @param key 待释放的键
extern void freeSKeyInSChainSet(ChainSet_S* pSet, Data_S* key);


/// @brief 释放单类型链式集合
/// @param pSet 集合指针
extern void freeSChainSet(ChainSet_S* pSet);


/// @brief 在单类型链式集合中插入键
/// @param pSet 集合指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @return 操作结果状态码
extern InfoOfReturn insertSKeyInSChainSet(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey);

/// @brief 通过键找到单类型链式集合中的键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSKeyInSChainSet函数释放
extern Data_S getSKeyBySKeyInSChainSet(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey);



/// @brief 判断单类型链式集合中是否存在当前键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasSKeyInSChainSet(ChainSet_S* pSet, Data_S key);

/// @brief 删除单类型链式集合中的键
/// @param pSet 集合指针
/// @param key 待删除的键
/// @return 操作结果状态码
extern InfoOfReturn delSKeyBySKeyInSChainSet(ChainSet_S* pSet, Data_S key);


#endif