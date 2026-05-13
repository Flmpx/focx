#define NODE_S_IN_DLIST
#define DATA_S_OPER
#include "dlist_sdata.h"
#include "dlist_sdata_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

//内部函数使用Data类型,外部使用两个void*
//修改为任意数据类型,这个任意是指整个val都是同一种,但这中可以自定义





static bool isEmptySDList(DList_S* plist) {
    return plist->size == 0;
}

void initSDList(DList_S* plist, InfoOfData* valInfo) {
    plist->head = plist->tail = NULL;
    plist->size = 0;
    plist->valInfo = valInfo;
    
}




/// @brief 通过val来返回链表节点
/// @param plist 链表指针
/// @param val Data类型数据
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_S_inDList* getNodeBySVal(DList_S* plist, Data_S val) {
    if (isEmptySDList(plist)) return NULL;
    Node_S_inDList* p = plist->head;
    for (; p; p = p->next) {
        
        //虽然可直接使用plist中的cmpdata函数, 但为了统一性, 使用统一的compareSData函数
        if (compareSData(p->val, plist->valInfo, val, plist->valInfo) == SAME) {
            return p;
        }
    }
    return NULL;
}


/// @brief 通过位置返回链表节点
/// @param plist 链表指针
/// @param pos 位置(范围应在[0, list.size-1])
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_S_inDList* getNodeByPos(DList_S* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return NULL;
    Node_S_inDList* p = plist->head;
    for (int i = 0; i < pos; i++) {
        p = p->next;
    }
    return p;
}


void freeSValInSDList(DList_S* plist, Data_S* val) {

    //统一接口
    freeSData(val, plist->valInfo);
}


Data_S getPtrSValBySValInSDList(DList_S* plist, Data_S val) {
    Node_S_inDList* p = getNodeBySVal(plist, val);
    if (p == NULL) {
        return getEmptySData();
    } else {
        return p->val;
    }
}


Data_S getCopySValByPosInSDList(DList_S* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptySData();
    Node_S_inDList* p = NULL;
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
    Data_S newVal = copySData(p->val, plist->valInfo);
    if (newVal.isEmpty) {
        //内存分配失败
        return getEmptySData();
    }
    return newVal;
}

Data_S getPtrSValByPosInSDList(DList_S* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptySData();
    Node_S_inDList* p = NULL;
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
    return p->val;
}

bool hasSValInSDList(DList_S* plist, Data_S val) {
    Node_S_inDList* p = getNodeBySVal(plist, val);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



//这个创建创建一个Node,并整合数据, 根据isCopyVal判断是否要复制数据
static Node_S_inDList* createNodeAndVal(DList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    Node_S_inDList* newNode = (Node_S_inDList*)malloc(sizeof(Node_S_inDList));
    if (newNode == NULL) return NULL; 
    if (isCopyVal == Data_Copy) {
        Data_S newVal = copySData(val, plist->valInfo);
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

/*********** */
InfoOfReturn insertSValAtEndInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_S_inDList* newNode = createNodeAndVal(plist, val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    //creatNode函函数已经处理了data, content的智能复制了
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


InfoOfReturn insertSValAtStartInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_S_inDList* newNode = createNodeAndVal(plist, val, isCopyVal);
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
InfoOfReturn insertSValAtPosInSDList(DList_S* plist, Data_S val, selectOfCopy isCopyVal, int pos) {
    if ((pos < 0) || (pos > plist->size)) return Warning;
    if (pos == 0) return insertSValAtStartInSDList(plist, val, isCopyVal);
    if (pos == plist->size) return insertSValAtEndInSDList(plist, val, isCopyVal);

    
    //创建节点
    Node_S_inDList* newNode = createNodeAndVal(plist, val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    Node_S_inDList* p = getNodeByPos(plist, pos);
    
    //creatNode函函数已经处理了data, content的复制
    newNode->next = p;
    newNode->prev = p->prev;

    p->prev->next = newNode;
    p->prev = newNode;

    plist->size++;
    return Success;
}

InfoOfReturn delEndNodeInSDList(DList_S* plist) {
    if (isEmptySDList(plist)) return Warning;

    Node_S_inDList* p = plist->tail;

    if (plist->size > 1) {
        plist->tail = plist->tail->prev;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }
    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}
InfoOfReturn delStartNodeInSDList(DList_S* plist) {
    if (isEmptySDList(plist)) return Warning;

    Node_S_inDList* p = plist->head;

    if (plist->size > 1) {
        plist->head = plist->head->next;
        plist->head->prev = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}

InfoOfReturn delNodeBySValInSDList(DList_S* plist, Data_S val) {
    if (isEmptySDList(plist)) return Warning;
    Node_S_inDList* p = getNodeBySVal(plist, val);
    if (p == NULL) return None;
    if (p == plist->head) return delStartNodeInSDList(plist);
    if (p == plist->tail) return delEndNodeInSDList(plist);
    
    p->prev->next = p->next;
    p->next->prev = p->prev;

    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}



InfoOfReturn delNodeByPosInSDList(DList_S* plist, int pos) {
    if (isEmptySDList(plist)) return Warning;
    if ((pos < 0) || (pos >= plist->size)) return Warning;
    Node_S_inDList* p = getNodeByPos(plist, pos);
    if (p == plist->head) return delStartNodeInSDList(plist);
    if (p == plist->tail) return delEndNodeInSDList(plist);
    
    p->prev->next = p->next;
    p->next->prev = p->prev;

    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}

void reverseSDList(DList_S* plist) {
    if (plist->size < 2) return;
    Node_S_inDList* temp = NULL;
    Node_S_inDList* p = plist->head;
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


void printSValInSDList(DList_S* plist, Data_S val) {
    if (val.isEmpty) {
        printf("\ndata is empty, cannot print\n");
        return;
    }
    printf("[val:");
    plist->valInfo->oper->printdata(val.data, val.content);
    printf("]");
}



void printSDList(DList_S* plist) {
    Node_S_inDList* p = plist->head;
    printf("[");
    int cnt = 0;
    for (; p; p = p->next) {
        if (cnt != 0) {
            printf("-->");
        }
        plist->valInfo->oper->printdata(p->val.data, p->val.content);
        cnt++;
    }
    printf("]");
    
}



void freeSDList(DList_S* plist) {
    Node_S_inDList* p = plist->head;
    Node_S_inDList* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        
        freeSData(&(q->val), plist->valInfo);
        free(q);
    }
    initSDList(plist, plist->valInfo);
}

