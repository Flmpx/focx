#ifndef CHAINSET_MDATA_H
#define CHAINSET_MDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Node_M_inChainSet Node_M_inChainSet;
typedef struct List_M_inChainSet List_M_inChainSet;

#ifdef ENTRY_M_INCHAINSET

/// @brief 在ChainSet_M中的Entry类型(Entry_M_inChainSet)
typedef struct Entry_M_inChainSet {
    Data_M key;
    bool isEmpty;
} Entry_M_inChainSet;

#endif

#ifdef NODE_M_INCHAINSET

/// @brief 在ChainSet中的链表节点, Node_M_inChainSet类型
struct Node_M_inChainSet {
    struct Node_M_inChainSet* next;
    struct Node_M_inChainSet* prev;
    Entry_M_inChainSet entry;
};

#endif

#ifdef LIST_M_INCHAINSET

/// @brief 在ChainSet中的链表, List_M_inChainSet
struct List_M_inChainSet {
    Node_M_inChainSet* head;
    Node_M_inChainSet* tail;
    int size;
};

#endif

/// @brief ChainSet_M->采用链表法的Set, 可存储任意类型数据
typedef struct ChainSet_M {
    List_M_inChainSet* arr;
    int mod;
    int len;
    int size;
} ChainSet_M;


/// @brief 初始化多类型链式集合
/// @param pSet 集合指针
extern void initMChainSet(ChainSet_M* pSet);



/// @brief 释放多类型链式集合中的键
/// @param key 待释放的键
extern void freeMKeyInMChainSet(Data_M* key);

/// @brief 释放多类型链式集合
/// @param pSet 集合指针
extern void freeMChainSet(ChainSet_M* pSet);

/// @brief 在多类型链式集合中插入键
/// @param pSet 集合指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @return 操作结果状态码
extern InfoOfReturn insertMKeyInMChainSet(ChainSet_M* pSet, Data_M key, selectOfCopy isCopyKey);

/// @brief 对多类型链式集合进行缩容
/// @param pSet 集合指针
/// @return 操作状态码
extern InfoOfReturn shrinkMChainSet(ChainSet_M* pSet);



/// @brief 通过键找到多类型链式集合中的键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMKeyInMChainSet函数释放
extern Data_M getMKeyByMKeyInMChainSet(ChainSet_M* pSet, Data_M key, selectOfCopy isCopyKey);




/// @brief 判断多类型链式集合中是否存在当前键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasMKeyInMChainSet(ChainSet_M* pSet, Data_M key);


/// @brief 删除多类型链式集合中的键
/// @param pSet 集合指针
/// @param key 待删除的键
/// @return 操作结果状态码
extern InfoOfReturn delMKeyByMKeyInMChainSet(ChainSet_M* pSet, Data_M key);



#endif