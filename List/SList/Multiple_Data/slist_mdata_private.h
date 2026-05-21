#ifndef SLIST_MDATA_PRIVATE_H
#define SLIST_MDATA_PRIVATE_H
#include "slist_mdata.h"




/// @brief 打印多类型单向链表中的值
/// @param val 待打印的值
extern void printMValInMSList(Data_M val);

/// @brief 打印多类型单向链表
/// @param plist 链表指针
extern void printMSList(SList_M* plist);


/// @brief 得到在多类型单向链表中指定位置的值的指针
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 指定位置的值的指针
extern Data_M* getPtrMValByPosInMSList(SList_M* plist, int pos);


#endif