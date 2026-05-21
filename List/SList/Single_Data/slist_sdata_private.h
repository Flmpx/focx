#ifndef SLIST_SDATA_PRIVATE_H
#define SLIST_SDATA_PRIVATE_H
#include "slist_sdata.h"



/// @brief 得到在单类型单向链表中指定位置的值的指针
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 指定位置的值的指针
extern Data_S* getPtrSValByPosInSSList(SList_S* plist, int pos);

/// @brief 打印单类型单向链表中的值
/// @param plist 链表指针
/// @param val 待打印的值
extern void printSValInSSList(SList_S* plist, Data_S val);


/// @brief 打印单类型单向链表
/// @param plist 链表指针
extern void printSSList(SList_S* plist);


#endif