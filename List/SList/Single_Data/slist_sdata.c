#define NODE_S_IN_SLIST
#define DATA_S_OPER
#include "slist_sdata.h"
#include "slist_sdata_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>






static bool isEmptySSList(SList_S* plist) {
    return plist->size == 0;
}

void initSSList(SList_S* plist, InfoOfData* valInfo) {
    plist->head = plist->tail = NULL;
    plist->size = 0;
    plist->valInfo = valInfo;
    
}




/// @brief 通过val来返回链表节点
/// @param plist 链表指针
/// @param val Data_S类型数据
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_S_inSList* getNodeBySVal(SList_S* plist, Data_S val) {
    if (isEmptySSList(plist)) return NULL;
    Node_S_inSList* p = plist->head;
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
static Node_S_inSList* getNodeByPos(SList_S* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return NULL;
    Node_S_inSList* p = plist->head;
    for (int i = 0; i < pos; i++) {
        p = p->next;
    }
    return p;
}


void freeSValInSSList(SList_S* plist, Data_S* val) {

    //统一接口
    freeSData(val, plist->valInfo);
}

Data_S getSValBySValInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyKey) {
    Node_S_inSList* p = getNodeBySVal(plist, val);
    if (p == NULL) {
        return getEmptySData();
    }
    if (isCopyKey == Data_Copy) {
        return copySData(p->val, plist->valInfo);
    } else {
        return p->val;
    }
}

Data_S getSValByPosINSSList(SList_S* plist, int pos, selectOfCopy isCopyVal) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptySData();
    //此时保证不会返回空指针的p
    Node_S_inSList* p = getNodeByPos(plist, pos);
    if (isCopyVal == Data_Copy) {
        return copySData(p->val, plist->valInfo);
    } else {
        return p->val;
    }
    
}


bool hasSValInSSList(SList_S* plist, Data_S val) {
    Node_S_inSList* p = getNodeBySVal(plist, val);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



//这个创建创建一个Node,并整合数据, 根据isCopyVal判断是否要复制数据
static Node_S_inSList* createNodeAndVal(SList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    Node_S_inSList* newNode = (Node_S_inSList*)malloc(sizeof(Node_S_inSList));
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



InfoOfReturn insertSValAtEndInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_S_inSList* newNode = createNodeAndVal(plist, val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    //creatNode函函数已经处理了data, content的智能复制了
    newNode->next = NULL;
   
    
    if (plist->size) {
        plist->tail->next = newNode;
        plist->tail = newNode;
    } else {
        plist->head = plist->tail = newNode;
    }
    
    plist->size++;
    return Success;
}


InfoOfReturn insertSValAtStartInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_S_inSList* newNode = createNodeAndVal(plist, val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    //creatNode函函数已经处理了data, content的复制了
    newNode->next = plist->head;

    if (plist->size) {
        plist->head = newNode;
    } else {
        plist->head = plist->tail = newNode;
    }
    
    plist->size++;
    return Success;
}

/************ */
InfoOfReturn insertSValAtPosInSSList(SList_S* plist, Data_S val, selectOfCopy isCopyVal, int pos) {
    if ((pos < 0) || (pos > plist->size)) return Warning;
    if (pos == 0) return insertSValAtStartInSSList(plist, val, isCopyVal);
    if (pos == plist->size) return insertSValAtEndInSSList(plist, val, isCopyVal);

    
    //创建节点
    Node_S_inSList* newNode = createNodeAndVal(plist, val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    Node_S_inSList* p = plist->head;
    Node_S_inSList* q = NULL;
    
    //找到插入位置
    for (int i = 0; i < pos; i++) {
        q = p;
        p = p->next;
    }
    
    //creatNode函函数已经处理了data, content的复制
    newNode->next = p;

    q->next = newNode;

    plist->size++;
    return Success;
}

InfoOfReturn delEndNodeInSSList(SList_S* plist) {
    if (isEmptySSList(plist)) return Warning;

    Node_S_inSList* p = plist->head;
    Node_S_inSList* q = NULL;

    for (int i = 0; i < plist->size-1; i++) {
        q = p;
        p = p->next;
    }

    /*
        TODO: 之后修改和优化会参考下面的循环, 这样防御性更好
    */
    // while (p->next != NULL) {
    //     q = p;
    //     p = p->next;
    // }
    

    
    
    if (plist->size > 1) {
        plist->tail = q;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }


    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}
InfoOfReturn delStartNodeInSSList(SList_S* plist) {
    if (isEmptySSList(plist)) return Warning;

    Node_S_inSList* p = plist->head;
    
    
    if (plist->size > 1) {
        plist->head = plist->head->next;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}

InfoOfReturn delNodeBySValInSSList(SList_S* plist, Data_S val) {
    if (isEmptySSList(plist)) return Warning;


    Node_S_inSList* p = plist->head;
    Node_S_inSList* q = NULL;
    while (p != NULL) {
        if (compareSData(p->val, plist->valInfo, val, plist->valInfo) == SAME) {
            if (p == plist->head) return delStartNodeInSSList(plist);
            if (p == plist->tail) return delEndNodeInSSList(plist);
            q->next = p->next;

            freeSData(&(p->val), plist->valInfo);
            free(p);

            plist->size--;
            return Success;

        }
        q = p;
        p = p->next;
    }

    return None;

}



InfoOfReturn delNodeByPosInSSList(SList_S* plist, int pos) {
    if (isEmptySSList(plist)) return Warning;
    if ((pos < 0) || (pos >= plist->size)) return Warning;
    
    Node_S_inSList* p = plist->head;
    Node_S_inSList* q = NULL;

    for (int i = 0; i < pos; i++) {
        q = p;
        p = p->next;
    }

    // TODO: 如果找到的是最后一个节点, 那明明可以不跑
    if (p == plist->head) return delStartNodeInSSList(plist);
    if (p == plist->tail) return delEndNodeInSSList(plist);
    
    q->next = p->next;

    freeSData(&(p->val), plist->valInfo);
    free(p);
    plist->size--;
    return Success;
}

// void reverseSDList(SList_S* plist) {
//     if (plist->size < 2) return;
//     Node_S_inSList* temp = NULL;
//     Node_S_inSList* p = plist->head;
//     while (p) {
//         temp = p->next;
//         p->next = p->prev;
//         p->prev = temp;
        
//         p = p->prev;
//     }
//     temp = plist->head;
//     plist->head = plist->tail;
//     plist->tail = temp;
// }


void printSValInSSList(SList_S* plist, Data_S val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    plist->valInfo->oper->printdata(val.data, val.content);
    printf("]");
}



void printSSList(SList_S* plist) {
    Node_S_inSList* p = plist->head;
    printf("[");
    int cnt = 0;
    for (; p; p = p->next) {
        if (cnt != 0) {
            printf("-->");
        }
        printSValInSSList(plist, p->val);
        cnt++;
    }
    printf("]");
    
}



void freeSSList(SList_S* plist) {
    Node_S_inSList* p = plist->head;
    Node_S_inSList* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        
        freeSData(&(q->val), plist->valInfo);
        free(q);
    }
    initSSList(plist, plist->valInfo);
}

