#define NODE_M_IN_DLIST
#define DATA_M_OPER
#include "../../../base.h"
#include "dlist_mdata.h"
#include "dlist_mdata_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>







static bool isEmptyMDList(DList_M* plist) {
    return plist->size == 0;
}

void initMDList(DList_M* plist) {
    plist->head = plist->tail = NULL;
    plist->size = 0;    
}


//////////////////////////////////////////////////////////////////////////////////






/// @brief 通过val来返回链表节点
/// @param plist 链表指针
/// @param val Data_M类型数据
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_M_inDList* getNodeByMVal(DList_M* plist, Data_M val) {
    if (isEmptyMDList(plist)) return NULL;
    Node_M_inDList* p = plist->head;
    for (; p; p = p->next) {
        if (compareMData(p->val, val) == SAME) {
            return p;
        }
    }
    return NULL;
}


/// @brief 通过位置返回链表节点
/// @param plist 链表指针
/// @param pos 位置(范围应在[0, list.size-1])
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_M_inDList* getNodeByPos(DList_M* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return NULL;
    Node_M_inDList* p = NULL;
    if (pos > plist->size/2) {
        p = plist->tail;
        int diff = plist->size - pos - 1;
        for (int i = 0; i < diff; i++) {
            p = p->prev;
        }
    } else {
        p = plist->head;
        for (int i = 0; i < pos; i++) {
            p = p->next;
        }
    }
    return p;
}

Data_M getMValByMValInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal) {
    Node_M_inDList* p = getNodeByMVal(plist, val);
    if (p == NULL) {
        return getEmptyMData();
    }
    if (isCopyVal == Data_Copy) {
        return copyMData(p->val);
    } else {
        return p->val;
    }
}

Data_M getMValByPosInMDList(DList_M* plist, int pos, selectOfCopy isCopyVal) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptyMData();
    //此时保证不会返回空指针的p
    Node_M_inDList* p = getNodeByPos(plist, pos);
    if (isCopyVal == Data_Copy) {
        return copyMData(p->val);
    } else {
        return p->val;
    }
}

bool hasMValInMDList(DList_M* plist, Data_M val) {
    Node_M_inDList* p = getNodeByMVal(plist, val);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



void freeMValInMDList(Data_M* val) {
    //freeMData只能内部使用, 使用freeMDataInMList作为外部接口
    freeMData(val);
}

void freeMDList(DList_M* plist) {
    Node_M_inDList* p = plist->head;
    Node_M_inDList* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        freeMData(&(q->val));
        free(q);
    }
    initMDList(plist);
}

//这个创建创建一个Node,并整合数据, 根据isCopyVal判断是否要复制数据
static Node_M_inDList* createNodeAndVal(Data_M val, selectOfCopy isCopyVal) {
    Node_M_inDList* newNode = (Node_M_inDList*)malloc(sizeof(Node_M_inDList));
    if (newNode == NULL) return NULL; 
    if (isCopyVal == Data_Copy) {
        Data_M newVal = copyMData(val);
        if (newVal.isEmpty) {
            //内存分配失败
            free(newNode);
            return NULL;
        }
        //是否有权限由外部决定
        newVal.isOwner = val.isOwner;
        newNode->val = newVal;
    } else {
        newNode->val = val;
    }
    return newNode;
}



InfoOfReturn insertMValAtEndInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_M_inDList* newNode = createNodeAndVal(val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    //creatNode函函数已经处理了data, content的复制了
    newNode->next = NULL;
    newNode->prev = plist->tail;
   
    
    if (plist->size) {
        plist->tail->next = newNode;
        plist->tail = newNode;
    } else {
        plist->head = plist->tail = newNode;
    }
    
    plist->size++;
    return Success;
}


InfoOfReturn insertMValAtStartInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_M_inDList* newNode = createNodeAndVal(val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    //creatNode函函数已经处理了data, content的复制了
    newNode->next = plist->head;
    newNode->prev = NULL;

    if (plist->size) {
        plist->head->prev = newNode;
        plist->head = newNode;
    } else {
        plist->head = plist->tail = newNode;
    }
    
    plist->size++;
    return Success;
}

/************ */
InfoOfReturn insertMValAtPosInMDList(DList_M* plist, Data_M val, selectOfCopy isCopyVal, int pos) {
    if ((pos < 0) || (pos > plist->size)) return Warning;
    if (pos == 0) return insertMValAtStartInMDList(plist, val, isCopyVal);
    if (pos == plist->size) return insertMValAtEndInMDList(plist, val, isCopyVal);

    
    
    //创建节点
    Node_M_inDList* newNode = createNodeAndVal(val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    Node_M_inDList* p = getNodeByPos(plist, pos);
    
    //creatNode函函数已经处理了data, content的复制
    newNode->next = p;
    newNode->prev = p->prev;

    p->prev->next = newNode;
    p->prev = newNode;

    plist->size++;
    return Success;
}

InfoOfReturn delEndNodeInMDList(DList_M* plist) {
    if (isEmptyMDList(plist)) return Warning;

    Node_M_inDList* p = plist->tail;

    
    //释放掉Data_M内存
    freeMData(&(p->val));

    if (plist->size > 1) {
        plist->tail = plist->tail->prev;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }
    free(p);
    plist->size--;
    return Success;
}


InfoOfReturn delStartNodeInMDList(DList_M* plist) {
    if (isEmptyMDList(plist)) return Warning;

    Node_M_inDList* p = plist->head;
    
    freeMData(&(p->val));

    if (plist->size > 1) {
        plist->head = plist->head->next;
        plist->head->prev = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }
    free(p);
    plist->size--;
    return Success;
}

InfoOfReturn delNodeByMValInMDList(DList_M* plist, Data_M val) {
    if (isEmptyMDList(plist)) return Warning;

    Node_M_inDList* p = getNodeByMVal(plist, val);
    if (p == NULL) return None;
    if (p == plist->head) return delStartNodeInMDList(plist);
    if (p == plist->tail) return delEndNodeInMDList(plist);
    
    p->prev->next = p->next;
    p->next->prev = p->prev;


    freeMData(&(p->val));

    free(p);
    plist->size--;
    return Success;
}
InfoOfReturn delNodeByPosInMDList(DList_M* plist, int pos) {
    if (isEmptyMDList(plist)) return Warning;
    if ((pos < 0) || (pos >= plist->size)) return Warning;
    
    if (pos == 0) return delStartNodeInMDList(plist);
    if (pos == plist->size-1) return delEndNodeInMDList(plist);


    Node_M_inDList* p = getNodeByPos(plist, pos);
    
    p->prev->next = p->next;
    p->next->prev = p->prev;


    freeMData(&(p->val));
    free(p);
    plist->size--;
    return Success;
}

void reverseMDList(DList_M* plist) {
    if (plist->size < 2) return;
    Node_M_inDList* temp = NULL;
    Node_M_inDList* p = plist->head;
    while (p) {
        temp = p->next;
        p->next = p->prev;
        p->prev = temp;
        
        p = p->prev;
    }
    temp = plist->head;
    plist->head = plist->tail;
    plist->tail = temp;
}



void printMValInMDList(Data_M val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printMDList(DList_M* plist) {
    Node_M_inDList* p = plist->head;
    printf("[");
    int cnt = 0;
    for (; p; p = p->next) {
        if (cnt != 0) {
            printf("-->");
        }
        p->val.dataInfo->oper->printdata(p->val.data, p->val.content);
        cnt++;
    }
    printf("]");
    
}






//////////////////////////////////////////////////////////////////////////////
//高级函数功能, 函数声明位于private头文件中

Data_M* getPtrMValInByPosInMDList(DList_M* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return NULL;
    //这里保证不会出现p为空的情况
    Node_M_inDList* p = getNodeByPos(plist, pos);

    return &(p->val);
}


