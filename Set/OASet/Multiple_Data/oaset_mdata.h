#ifndef OASET_MDATA_H
#define OASET_MDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Entry_M_inOASet Entry_M_inOASet;

#ifdef ENTRY_M_INOASET


/// @brief 在OASet_M中的Entry类型(Entry_M_inOASet)
typedef struct Entry_M_inOASet {
    Data_M key;
    int state;
    bool isEmpty;
} Entry_M_inOASet;

#endif

/// @brief OASet_M->采用开放寻址法的Set, 可存储任意类型数据
typedef struct OASet_M {
    Entry_M_inOASet* arr;
    int mod;
    int len;
    int size;
} OASet_M;


/// @brief 初始化多类型开放寻址集合
/// @param pSet 集合指针
extern void initMOASet(OASet_M* pSet);





/// @brief 释放多类型开放寻址集合中的键
/// @param key 待释放的键
extern void freeMKeyInMOASet(Data_M* key);



/// @brief 释放多类型开放寻址集合
/// @param pSet 集合指针
extern void freeMOASet(OASet_M* pSet);

/// @brief 在多类型开放寻址集合中插入键
/// @param pSet 集合指针
/// @param key 待插入的键
/// @param isCopyKey 是否要复制传入的key
/// @return 操作结果状态码
extern InfoOfReturn insertMKeyInMOASet(OASet_M* pSet, Data_M key, selectOfCopy isCopyKey);


/// @brief 通过键找到多类型开放寻址集合中的键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 返回找到的键, 若没找到, 返回空键(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeMKeyInMOASet函数释放
extern Data_M getMKeyByMKeyInMOASet(OASet_M* pSet, Data_M key, selectOfCopy isCopyKey);


/// @brief 对多类型开放寻址集合进行缩容
/// @param pSet 集合指针
/// @return 操作状态码
extern InfoOfReturn shrinkMOASet(OASet_M* pSet);

/// @brief 判断多类型开放寻址集合中是否存在当前键
/// @param pSet 集合指针
/// @param key 待查找的键
/// @return 如果存在返回true, 否则false
extern bool hasMKeyInMOASet(OASet_M* pSet, Data_M key);


/// @brief 删除多类型开放寻址集合中的键
/// @param pSet 集合指针
/// @param key 待删除的键
/// @return 操作结果状态码
extern InfoOfReturn delMKeyByMKeyInMOASet(OASet_M* pSet, Data_M key);


#endif