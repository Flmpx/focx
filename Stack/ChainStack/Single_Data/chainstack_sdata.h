#ifndef CHAINSTACK_SDATA_H
#define CHAINSTACK_SDATA_H
#include "../../../base.h"
#include "../../../List/SList/Single_Data/slist_sdata.h"

/// @brief ChainStack_S类型, 指可以存储单一类型的通过链表来实现的栈
typedef struct {
    SList_S list;
    int size;
    int capacity;
    bool isUnlimited;
    InfoOfData* valInfo;
} ChainStack_S;


/// @brief 初始化单类型链表栈
/// @param pStack 栈指针
/// @param valInfo 值的数据信息的指针
/// @param isUnlimited 是否无限制容量
/// @param capacity 初始容量(>0)
/// @return 操作状态码
extern InfoOfReturn initSChainStack(ChainStack_S* pStack, InfoOfData* valInfo, bool isUnlimited, int capacity);



/// @brief 判断单类型链表栈是否已满
/// @param pStack 栈指针
/// @return 若满返回true, 否则false
extern bool isFullSChainStack(ChainStack_S* pStack);



/// @brief 判断单类型链表栈是否为空
/// @param pStack 栈指针
/// @return 若空返回true, 否则false
extern bool isEmptySChainStack(ChainStack_S* pStack);


/// @brief 判断单类型链表栈中是否有当前值
/// @param pStack 栈指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则返回false
extern bool hasSValInSChainStack(ChainStack_S* pStack, Data_S val);



/// @brief 压入值到单类型链表栈
/// @param pStack 栈指针
/// @param val 待压入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作状态码
extern InfoOfReturn pushSValInSChainStack(ChainStack_S* pStack, Data_S val, selectOfCopy isCopyVal);


/// @brief 弹出单类型链表栈中的值
/// @param pStack 栈指针
/// @return 弹出的值, 若栈空, 返回空值(用字段isEmpty检查)
/// @note 弹出的值记得使用freeSValInSChainStack函数释放
extern Data_S popSValInSChainStack(ChainStack_S* pStack);



/// @brief 释放单类型链表栈中的值
/// @param pStack 栈指针
/// @param val 带释放的值的指针
extern void freeSValInSChainStack(ChainStack_S* pStack, Data_S* val);

/// @brief 释放单类型链表栈
/// @param pStack 栈指针
extern void freeSChainStack(ChainStack_S* pStack);






#endif