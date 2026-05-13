#ifndef DLIST_MDATA_H
#define DLIST_MDATA_H
#include "../../../base.h"
#include <stdbool.h>

typedef struct Node_M_inDList Node_M_inDList;



#ifdef NODE_M_IN_DLIST

/// @brief 在DList_M的中的Node_M_inDList节点, 节点的数据任意, 是Data_M类型
struct Node_M_inDList {
    struct Node_M_inDList* prev;
    struct Node_M_inDList* next;
    Data_M val;
};
#endif

/// @brief DList_M类型, 可以存储任意类型
typedef struct {
    Node_M_inDList* head;
    Node_M_inDList* tail;
    int size;
} DList_M;


/*** */
/// @brief 初始化多类型双向链表
/// @param plist 链表指针
extern void initMDList(DList_M* plist);


/// @brief 释放多类型双向链表中的值
/// @param val 待释放的值
extern void freeMValInMDList(Data_M* val);


/// @brief 通过值找到多类型双向链表中值, 可以直接修改内部数据
/// @param plist 链表指针
/// @param val 待查找的值
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_M getPtrMValByMValInMDList(DList_M* plist, Data_M val);

/// @brief 查找在多类型双向链表的指定位置的值, 使用完后用freeMValInMDList函数进行释放
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_M getCopyMValByPosInMDList(DList_M* plist, int pos);


/// @brief 查找在多类型双向链表的指定位置的值, 可以直接修改内部数据
/// @param plist 链表指针
/// @param pos 位置(从0开始)
/// @return 返回找到的值, 若没找到, 返回空值(用字段isEmpty检查)
extern Data_M getPtrMValByPosInMDList(DList_M* plist, int pos);

/// @brief 判断多类型双向链表中是否有当前值
/// @param plist 链表指针
/// @param val 待查找的值
/// @return 如果存在返回true, 否则false
extern bool hasMValInMDList(DList_M* plist, Data_M val);




/// @brief 在多类型双向链表尾部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val, 使用selectOfCopy枚举类型
/// @return 操作结果状态码
extern InfoOfReturn insertMValAtEndInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal);


/// @brief 在多类型双向链表头部插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val, 使用selectOfCopy枚举类型
/// @return 操作结果状态码
extern InfoOfReturn insertMValAtStartInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal);

/// @brief 在多类型双向链表指定位置插入值
/// @param plist 链表指针
/// @param val 待插入的值
/// @param isCopyVal 是否要复制传入的val, 使用selectOfCopy枚举类型
/// @param pos 位置的范围在[0, 链表大小],范围的两端分别代表头插和尾插
/// @return 操作结果状态码
extern InfoOfReturn insertMValAtPosInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal, int pos);

/// @brief 删除多类型双向链表的尾节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delEndNodeInMDList(DList_M* plist);


/// @brief 删除多类型双向链表的头节点
/// @param plist 链表指针
/// @return 操作结果状态码
extern InfoOfReturn delStartNodeInMDList(DList_M* plist);

/// @brief 删除多类型双向链表中指定值(只能删除第一次出现的)
/// @param plist 链表指针
/// @param val 待删除的值
/// @return 操作结果状态码
extern InfoOfReturn delNodeByMValInMDList(DList_M* plist, Data_M val);

/// @brief 删除多类型双向链表中指定位置的值
/// @param plist 链表指针
/// @param pos 待删除的位置(从0开始)
/// @return 操作结果状态码
extern InfoOfReturn delNodeByPosInMDList(DList_M* plist, int pos);



/// @brief 反转多类型双向链表
/// @param plist 链表指针
extern void reverseMDList(DList_M* plist);





/// @brief 释放多类型双向链表
/// @param plist 链表指针
extern void freeMDList(DList_M* plist);


#endif


