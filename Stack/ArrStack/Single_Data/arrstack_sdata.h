#ifndef ARRSTACK_SDATA_H
#define ARRSTACK_SDATA_H
#include "../../../base.h"

/// @brief ArrStack_S类型, 指可以存储单一类型的通过数组来实现的栈
typedef struct {
    Data_S* vals;
    int top;
    int size;
    int capacity;
    bool isUnlimited;
    InfoOfData* valInfo;
} ArrStack_S;


/// @brief 初始化单类型数组栈, 同时分配数组空间
/// @param pStack 栈指针
/// @param isUnlimited 是否无限制容量
/// @param valInfo 值的数据信息的指针
/// @param capacity 初始容量(>0)
/// @return 操作状态码
extern InfoOfReturn initSArrStack(ArrStack_S* pStack, InfoOfData* valInfo, bool isUnlimited, int capacity);



/// @brief 判断单类型数组栈是否已满
/// @param pStack 栈指针
/// @return 若满返回true, 否则false
extern bool isFullSArrStack(ArrStack_S* pStack);


/// @brief 判断单类型数组栈是否为空
/// @param pStack 栈指针
/// @return 若空返回true, 否则false
extern bool isEmptySArrStack(ArrStack_S* pStack);



/// @brief 判断单类型数组栈中是否有当前值
/// @param pStack 栈指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则返回false
extern bool hasSValInSArrStack(ArrStack_S* pStack, Data_S val);




/// @brief 压入值到单类型数组栈
/// @param pStack 栈指针
/// @param val 待压入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作状态码
extern InfoOfReturn pushSValInSArrStack(ArrStack_S* pStack, Data_S val, selectOfCopy isCopyVal);


/// @brief 弹出单类型数组栈中的值
/// @param pStack 栈指针
/// @return 弹出的值, 若栈空, 返回空值(用字段isEmpty检查)
/// @note 弹出的值记得使用freeSValInSArrStack函数释放
extern Data_S popSValInSArrStack(ArrStack_S* pStack);



/// @brief 释放单类型数组栈中的值
/// @param pStack 栈指针
/// @param val 待释放的值的指针
extern void freeSValInSArrStack(ArrStack_S* pStack, Data_S* val);


/// @brief 释放单类型数组栈
/// @param pStack 栈指针
/// @note 数据以及栈数组都会清除也会删除
extern void freeSArrStack(ArrStack_S* pStack);

/// @brief 清空单类型数组栈
/// @param pStack 栈指针
/// @note 只清除数据, 不清除栈数组
extern void clearSArrStack(ArrStack_S* pStack);




#endif