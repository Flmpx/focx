#ifndef SLIST_SDATA_PRIVATE_H
#define SLIST_SDATA_PRIVATE_H
#include "slist_sdata.h"



/// @brief 打印单类型单向链表中的值
/// @param plist 链表指针
/// @param val 待打印的值
extern void printSValInSSList(SList_S* plist, Data_S val);


/// @brief 打印单类型单向链表
/// @param plist 链表指针
extern void printSSList(SList_S* plist);


#endif