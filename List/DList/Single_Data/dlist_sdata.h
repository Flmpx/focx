#ifndef DLIST_SDATA_H
#define DLIST_SDATA_H
#include <stdbool.h>
#include "../../../base.h"

typedef struct Node_S_inDList Node_S_inDList;

#ifdef NODE_S_IN_DLIST

/// @brief 在DList_S的中的Node_S_inDList节点, 节点的数据类型单一, 是Data_S类型
struct Node_S_inDList {
    struct Node_S_inDList* prev;
    struct Node_S_inDList* next;
    Data_S val;
};

#endif

/// @brief DList_S类型, 存储单一类型数据
typedef struct {
    Node_S_inDList* head;
    Node_S_inDList* tail;
    InfoOfData* valInfo;
    int size;
} DList_S;



/// @brief 初始化单类型双向链表
/// @param plist 链表指针
/// @param valInfo 值的数据信息的指针
extern void initSDList(DList_S* plist, InfoOfData* valInfo);


/// @brief 释放单类型双向链表中的值
/// @param plist 链表指针
/// @param val 待释放的值
extern void freeSValInSDList(DList_S* plist, Data_S* val);



/// @brief 通过值找到单类型双向链表中值
/// @param plist 链表指针
/// @param val 待查找的值
/// @param isCopyVal 返回的值是否要复制
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSValInSDList函数释放
extern Data_S getSValBySValInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal);




/// @brief 查找在单类型双向链表的指定位置的值
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @param isCopyVal 返回的值是否要复制
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
/// @note 如果不复制, 可用于修改内部数据, 如果复制, 会创建副本, 副本默认拥有数据所有权, 使用完后通过freeSValInSDList函数释放
extern Data_S getSValByPosInSDList(DList_S* plist, int pos, selectOfCopy isCopyVal);


/// @brief 判断单类型双向链表中是否有当前值
/// @param plist 链表指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则false
extern bool hasSValInSDList(DList_S* plist, Data_S val);



/// @brief 在单类型双向链表尾部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtEndInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal);


/// @brief 在单类型双向链表头部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtStartInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal);

/// @brief 在单类型双向链表指定位置插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val
/// @param pos 位置的范围在[0, 链表大小],范围的两端分别代表头插和尾插
/// @return 操作结果状态码
extern InfoOfReturn insertSValAtPosInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal, int pos);

/// @brief 删除单类型双向链表的尾节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delEndNodeInSDList(DList_S* plist);


/// @brief 删除单类型双向链表的头节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delStartNodeInSDList(DList_S* plist);

/// @brief 删除单类型双向链表中指定值(只能删除第一次出现的)
/// @param plist 链表指针
/// @param val 待删除的值
/// @return 操作结果状态码
extern InfoOfReturn delNodeBySValInSDList(DList_S* plist, Data_S val);

/// @brief 删除单类型双向链表中指定位置的值
/// @param plist 链表指针
/// @param pos 待删除的位置(从0开始)
/// @return 操作结果状态码
extern InfoOfReturn delNodeByPosInSDList(DList_S* plist, int pos);

/// @brief 反转单类型双向链表
/// @param plist 链表指针
extern void reverseSDList(DList_S* plist);





/// @brief 释放单类型双向链表
/// @param plist 链表指针
extern void freeSDList(DList_S* plist);
#endif

