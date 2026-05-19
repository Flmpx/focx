#ifndef ARRSTACK_MDATA_H
#define ARRSTACK_MDATA_H
#include "../../../base.h"


/// @brief ArrStack_M类型, 指可以存储任意类型的通过数组来实现的栈
typedef struct {
    Data_M* arr;
    int top;
    int size;
    int capacity;
    bool isUnlimited;
} ArrStack_M;

/// @brief 初始化多类型数组栈
/// @param pStack 栈指针
/// @note 不会为其分配空间
extern void initMArrStack(ArrStack_M* pStack);

/// @brief 为多类型数组栈设置属性, 分配数组空间
/// @param pStack 栈指针
/// @param isUnlimited 是否无限制容量
/// @param capacity 初始容量(>0)
/// @return 操作状态码
extern InfoOfReturn allocMArrStack(ArrStack_M* pStack, bool isUnlimited, int capacity);

/// @brief 判断多类型数组栈是否已满
/// @param pStack 栈指针
/// @return 若满返回true, 否则false
extern bool isFullMArrStack(ArrStack_M* pStack);

/// @brief 判断多类型数组栈是否为空
/// @param pStack 栈指针
/// @return 若空返回true, 否则false
extern bool isEmptyMArrStack(ArrStack_M* pStack);


/// @brief 判断多类型数组栈中是否有当前值
/// @param pStack 栈指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则返回false
extern bool hasMValInMArrStack(ArrStack_M* pStack, Data_M val);


/// @brief 压入值到多类型数组栈
/// @param pStack 栈指针
/// @param val 待压入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作状态码
extern InfoOfReturn pushMValInMArrStack(ArrStack_M* pStack, Data_M val, selectOfCopy isCopyVal);

/// @brief 弹出多类型数组栈中的值
/// @param pStack 栈指针
/// @return 弹出的值, 若栈空, 返回空值(用字段isEmpty检查)
/// @note 弹出的值记得使用freeMValInMArrStack函数释放
extern Data_M popMValInMArrStack(ArrStack_M* pStack);


/// @brief 释放多类型数组栈中的值
/// @param val 带释放的值
void freeMValInMArrStack(Data_M* val);


/// @brief 释放多类型数组栈
/// @param pStack 栈指针
/// @note 数据以及栈数组都会清除也会删除
void freeMArrStack(ArrStack_M* pStack);


/// @brief 清空多类型数组栈
/// @param pStack 栈指针
/// @note 只清除数据, 不清除栈数组
void clearMArrStack(ArrStack_M* pStack);



#endif