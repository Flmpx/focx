#ifndef CHAINSTACK_MDATA_H
#define CHAINSTACK_MDATA_H
#include "../../../base.h"
//复用链表
#include "../../../List/SList/Multiple_Data/slist_mdata.h"


/// @brief ChainStack_M类型, 指可以存储任意类型的通过链表来实现的栈
typedef struct {
    SList_M list;
    int size;
    int capacity;
    bool isUnlimited;
} ChainStack_M;



/// @brief 初始化多类型链表栈
/// @param pStack 栈指针
/// @param isUnlimited 是否无限制容量
/// @param capacity 初始容量(>0)
/// @return 操作状态码
extern InfoOfReturn initMChainStack(ChainStack_M* pStack, bool isUnlimited, int capacity);



/// @brief 判断多类型链表栈是否已满
/// @param pStack 栈指针
/// @return 若满返回true, 否则false
extern bool isFullMChainStack(ChainStack_M* pStack);


/// @brief 判断多类型链表栈是否为空
/// @param pStack 栈指针
/// @return 若空返回true, 否则false
extern bool isEmptyMChainStack(ChainStack_M* pStack);



/// @brief 判断多类型链表栈中是否有当前值
/// @param pStack 栈指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则返回false
extern bool hasMValInMChainStack(ChainStack_M* pStack, Data_M val);




/// @brief 压入值到多类型链表栈
/// @param pStack 栈指针
/// @param val 待压入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作状态码
extern InfoOfReturn pushMValInMChainStack(ChainStack_M* pStack, Data_M val, selectOfCopy isCopyVal);




/// @brief 弹出多类型链表栈中的值
/// @param pStack 栈指针
/// @return 弹出的值, 若栈空, 返回空值(用字段isEmpty检查)
/// @note 弹出的值记得使用freeMValInMChainStack函数释放
extern Data_M popMValInMChainStack(ChainStack_M* pStack);


/// @brief 释放多类型链表栈中的值
/// @param val 带释放的值的指针
extern void freeMValInMChainStack(Data_M* val);


/// @brief 释放多类型链表栈
/// @param pStack 栈指针
extern void freeMChainStack(ChainStack_M* pStack);




#endif