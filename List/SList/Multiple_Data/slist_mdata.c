#define NODE_M_IN_SLIST
#define DATA_M_OPER
#include "slist_mdata.h"
#include "slist_mdata_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>







static bool isEmptyMSList(SList_M* plist) {
    return plist->size == 0;
}

void initMSList(SList_M* plist) {
    plist->head = plist->tail = NULL;
    plist->size = 0;    
}


//////////////////////////////////////////////////////////////////////////////////






/// @brief 通过val来返回链表节点
/// @param plist 链表指针
/// @param val Data_M类型数据
/// @return 若存在节点,返回节点, 如果位置无效,返回NULL
static Node_M_inSList* getNodeByMVal(SList_M* plist, Data_M val) {
    if (isEmptyMSList(plist)) return NULL;
    Node_M_inSList* p = plist->head;
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
static Node_M_inSList* getNodeByPos(SList_M* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return NULL;
    Node_M_inSList* p = plist->head;
    for (int i = 0; i < pos; i++) {
        p = p->next;
    }
    return p;
}

Data_M getPtrMValByMValInMSList(SList_M* plist, Data_M val) {
    Node_M_inSList* p = getNodeByMVal(plist, val);
    if (p == NULL) {
        return getEmptyMData();
    } else {
        return p->val;
    }
}


Data_M getCopyMValByPosInMSList(SList_M* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptyMData();
    Node_M_inSList* p = plist->head;
    //由于是单链表, 不能优化位置查找函数了
    
    for (int i = 0; i < pos; i++) {
        p = p->next;
    }
    //返回的一定是深拷贝的Data
    Data_M newVal = copyMData(p->val);
    if (newVal.isEmpty) {
        //内存分配失败
        return getEmptyMData();
    }
    return newVal;
}

Data_M getPtrMValByPosInMSList(SList_M* plist, int pos) {
    if ((pos < 0) || (pos >= plist->size)) return getEmptyMData();
    Node_M_inSList* p = plist->head;
    for (int i = 0; i < pos; i++) {
        p = p->next;
    }
    return p->val;
}

bool hasMValInMSList(SList_M* plist, Data_M val) {
    Node_M_inSList* p = getNodeByMVal(plist, val);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



void freeMValInMSList(Data_M* val) {
    //freeMData只能内部使用, 使用freeMDataInMList作为外部接口
    freeMData(val);
}



//这个创建创建一个Node,并整合数据, 根据isCopyVal判断是否要复制数据
static Node_M_inSList* createNodeAndVal(Data_M val, selectOfCopy isCopyVal) {
    Node_M_inSList* newNode = (Node_M_inSList*)malloc(sizeof(Node_M_inSList));

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

/*********** */
InfoOfReturn insertMValAtEndInMSList(SList_M* plist, Data_M val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_M_inSList* newNode = createNodeAndVal(val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    //creatNode函函数已经处理了data, content的复制了
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


InfoOfReturn insertMValAtStartInMSList(SList_M* plist, Data_M val, selectOfCopy isCopyVal) {
    
    //创建节点
    Node_M_inSList* newNode = createNodeAndVal(val, isCopyVal);
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
InfoOfReturn insertMValAtPosInMSList(SList_M* plist, Data_M val, selectOfCopy isCopyVal, int pos) {
    if ((pos < 0) || (pos > plist->size)) return Warning;
    if (pos == 0) return insertMValAtStartInMSList(plist, val, isCopyVal);
    if (pos == plist->size) return insertMValAtEndInMSList(plist, val, isCopyVal);

    
    
    //创建节点
    Node_M_inSList* newNode = createNodeAndVal(val, isCopyVal);
    if (newNode == NULL) {
        //内存分配失败
        return Warning; 
    }
    
    //由于单链表的插入必须要得到前驱节点才行

    
    Node_M_inSList* p = plist->head;
    Node_M_inSList* q = NULL;


    //找到插入位置
    for (int i = 0; i < pos; i++) {
        q = p;
        p = p->next;
    }

    newNode->next = p;
    q->next = newNode;

    plist->size++;
    return Success;
}

InfoOfReturn delEndNodeInMSList(SList_M* plist) {
    if (isEmptyMSList(plist)) return Warning;

    Node_M_inSList* p = plist->head;
    Node_M_inSList* q = NULL;

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


    //释放掉Data_M内存
    freeMData(&(p->val));
    free(p);
    plist->size--;
    return Success;
}


InfoOfReturn delStartNodeInMSList(SList_M* plist) {
    if (isEmptyMSList(plist)) return Warning;

    Node_M_inSList* p = plist->head;
    
    
    if (plist->size > 1) {
        plist->head = plist->head->next;
    } else {
        plist->head = plist->tail = NULL;
    }
    freeMData(&(p->val));
    free(p);
    plist->size--;
    return Success;
}

InfoOfReturn delNodeByMValInMSList(SList_M* plist, Data_M val) {
    if (isEmptyMSList(plist)) return Warning;

    Node_M_inSList* p = plist->head;
    Node_M_inSList* q = NULL;

    while (p != NULL) {
        if (compareMData(val, p->val) == SAME) {
            if (p == plist->head) return delStartNodeInMSList(plist);

            //TODO:这个可以优化, 找到最后一个经历了一次循环, delEnd函数也会经历一次循环, 这可以优化
            if (p == plist->tail) return delEndNodeInMSList(plist);
            
            freeMData(&(p->val));
            q->next = p->next;
            free(p);
            plist->size--;
            return Success;
        }
        q = p;
        p = p->next;
    }


    return None;

}
InfoOfReturn delNodeByPosInMSList(SList_M* plist, int pos) {
    if (isEmptyMSList(plist)) return Warning;
    if ((pos < 0) || (pos >= plist->size)) return Warning;
    Node_M_inSList* p  = plist->head;
    Node_M_inSList* q = NULL;

    for (int i = 0; i < pos; i++) {
        q = p;
        p = p->next;
    }



    // TODO: 如果找到的是最后一个节点, 那明明可以不跑
    if (p == plist->head) return delStartNodeInMSList(plist);
    if (p == plist->tail) return delEndNodeInMSList(plist);
    
    q->next = p->next;
    

    freeMData(&(p->val));
    free(p);
    plist->size--;
    return Success;
}




void printMValInMSList(Data_M val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printMSList(SList_M* plist) {
    Node_M_inSList* p = plist->head;
    printf("[");
    int cnt = 0;
    for (; p; p = p->next) {
        if (cnt != 0) {
            printf("-->");
        }
        printMValInMSList(p->val);
        cnt++;
    }
    printf("]");
    
}



void freeMSList(SList_M* plist) {
    Node_M_inSList* p = plist->head;
    Node_M_inSList* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        freeMData(&(q->val));
        free(q);
    }
    initMSList(plist);
}

