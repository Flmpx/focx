#ifndef OASET_SDATA_H
#define OASET_SDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Entry_S_inOASet Entry_S_inOASet;

#ifdef ENTRY_S_INOASET

/// @brief 在OASet_S中的Entry类型(Entry_S_inOASet)
typedef struct Entry_S_inOASet {
    Data_S key;
    int state;
    //如果要把内存压缩到极致, 这个属性可以删除, 这里是为了保持与Map有相似之处
    bool isEmpty;
} Entry_S_inOASet;

#endif

/// @brief OASet_S->采用开放寻址法的Set, 可存储单一类型数据
typedef struct OASet_S {
    Entry_S_inOASet* arr;
    int mod;
    int len;
    int size;
    InfoOfData* keyInfo;
} OASet_S;



/// @brief 初始化单类型开放寻址集合
/// @param pSet 集合指针
/// @param keyInfo 键的数据信息的指针
extern void initSOASet(OASet_S* pSet, InfoOfData* keyInfo);



/// @brief 释放单类型开放寻址集合中的键
/// @param pSet 集合指针
/// @param key 待释放的键
extern void freeSKeyInSOASet(OASet_S* pSet, Data_S* key);


/// @brief 释放单类型开放寻址集合
/// @param pSet 集合指针
/// @note 数据以及集合的桶也会删除
extern void freeSOASet(OASet_S* pSet);



/// @brief 清空单类型开放寻址集合
/// @param pMap 集合指针
/// @note 只清除数据, 不清除集合的桶
extern void freeSOASet(OASet_S* pSet);

/// @brief 在单类型开放寻址集合中插入键
/// @param pSet 集合指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @return 操作结果状态码
extern InfoOfReturn insertSKeyInSOASet(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey);



/// @brief 对单类型开放寻址集合进行缩容
/// @param pSet 集合指针
/// @return 操作状态码
extern InfoOfReturn shrinkSOASet(OASet_S* pSet);


/// @brief 通过键找到单类型开放寻址集合中的键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSKeyInSOASet函数释放
extern Data_S getSKeyBySKeyInSOASet(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey);




/// @brief 判断单类型开放寻址集合中是否存在当前键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasSKeyInSOASet(OASet_S* pSet, Data_S key);


/// @brief 删除单类型开放寻址集合中的键
/// @param pSet 集合指针
/// @param key 待删除的键
/// @return 操作结果状态码
extern InfoOfReturn delSKeyBySKeyInSOASet(OASet_S* pSet, Data_S key);




#endif