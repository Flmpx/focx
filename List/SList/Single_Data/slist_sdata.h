#ifndef SLIST_SDATA_H
#define SLIST_SDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Node_S_inSList Node_S_inSList;

#ifdef NODE_S_IN_SLIST

/// @brief 在SList_S的中的Node_S_inSList节点, 节点的数据类型单一, 是Data_S类型
struct Node_S_inSList {
    struct Node_S_inSList* next;
    Data_S val;
};

#endif

/// @brief SList_S类型, 存储单一类型数据
typedef struct {
    Node_S_inSList* head;
    Node_S_inSList* tail;
    InfoOfData* valInfo;
    int size;
} SList_S;



/**** */
/// @brief 初始化单类型单向链表
/// @param plist 链表指针
/// @param valInfo 值的数据信息的指针
extern void initSSList(SList_S* plist, InfoOfData* valInfo);


/// @brief 释放单类型单向链表中的值
/// @param plist 链表指针
/// @param val 待释放的值
extern void freeSValInSSList(SList_S* plist, Data_S* val);

/// @brief 通过值找到单类型单向链表中值, 可以直接修改内部数据
/// @param plist 链表指针
/// @param val 待查找的值
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_S getPtrSValBySValInSSList(SList_S* plist, Data_S val);

/// @brief 查找在单类型单向链表的指定位置的值, 使用完后用freeSValInSSList函数进行释放
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_S getCopySValByPosInSSList(SList_S* plist, int pos);


/// @brief 查找在单类型单向链表的指定位置的值, 可以直接修改内部数据
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_S getPtrSValByPosInSSList(SList_S* plist, int pos);

/// @brief 判断单类型单向链表中是否有当前值
/// @param plist 链表指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则false
extern bool hasSValInSSList(SList_S* plist, Data_S val);



/// @brief 在单类型单向链表尾部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtEndInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal);


/// @brief 在单类型单向链表头部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtStartInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal);

/// @brief 在单类型单向链表指定位置插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @param pos 位置的范围在[0, 链表大小],范围的两端分别代表头插和尾插
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtPosInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal, int pos);

/// @brief 删除单类型单向链表的尾节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delEndNodeInSSList(SList_S* plist);


/// @brief 删除单类型单向链表的头节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delStartNodeInSSList(SList_S* plist);

/// @brief 删除单类型单向链表中指定值(只能删除第一次出现的)
/// @param plist 链表指针
/// @param val 待删除的值
/// @return 操作结果状态码
extern InfoOfReturn delNodeBySValInSSList(SList_S* plist, Data_S val);

/// @brief 删除单类型单向链表中指定位置的值
/// @param plist 链表指针
/// @param pos 待删除的位置(从0开始)
/// @return 操作结果状态码
extern InfoOfReturn delNodeByPosInSSList(SList_S* plist, int pos);

// TODO: 单链表的反转





/// @brief 释放单类型单向链表
/// @param plist 链表指针
extern void freeSSList(SList_S* plist);
#endif

