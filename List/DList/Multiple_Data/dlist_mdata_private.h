#ifndef DLIST_MDATA_PRIVATE_H
#define DLIST_MDATA_PRIVATE_H
#include "dlist_mdata.h"


/// @brief 打印多类型双向链表中的值
/// @param val 待打印的值
extern void printMValInMDList(Data_M val);

/// @brief 打印多类型双向链表
/// @param plist 链表指针
extern void printMDList(DList_M* plist);

/// @brief 得到在多类型双向链表中指定位置的Val指针
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 指定位置的值的指针
extern Data_M* getPtrMValByPosInMDList(DList_M* plist, int pos);

#endif