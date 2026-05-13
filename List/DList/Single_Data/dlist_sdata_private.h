#ifndef DLIST_SDATA_PRIVATE_H
#define DLIST_SDATA_PRIVATE_H
#include "dlist_sdata.h"

/// @brief 打印单类型双向链表中的值
/// @param plist 链表指针
/// @param val 待打印的值
extern void printSValInSDList(DList_S* plist, Data_S val);


/// @brief 打印单类型双向链表
/// @param plist 链表指针
extern void printSDList(DList_S* plist);

#endif