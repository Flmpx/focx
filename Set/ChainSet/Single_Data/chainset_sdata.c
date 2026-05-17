#define ENTRY_S_INCHAINSET
#define NODE_S_INCHAINSET
#define LIST_S_INCHAINSET

#define GET_LARGESTPRIME


#define DATA_S_OPER

#include "chainset_sdata_private.h"
#include "chainset_sdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool isEmptySList(List_S_inChainSet* plist);
static void initSList(List_S_inChainSet* plist);


/////////////////////////////////////////////////////////////////////////////////


static Entry_S_inChainSet getEmptySEntry() {
    Entry_S_inChainSet entry;
    entry.isEmpty = true;
    entry.key = getEmptySData();
    return entry;
}




void initSChainSet(ChainSet_S* pSet, InfoOfData* keyInfo) {
    pSet->arr = NULL;
    pSet->len = pSet->size = 0;
    pSet->mod = 2;
    pSet->keyInfo = keyInfo;
}











///////////////////////////////////////////////////////////////////////////////////
//比较类



//////////////////////////////////////////////////////////////////////////////////////
//释放类


/**************** */
//释放Data数据

void freeSKeyInSChainSet(ChainSet_S* pSet, Data_S* key) {
    freeSData(key, pSet->keyInfo);
}



/************ */
//这个仅仅只会把Entry中的key的data和others(不会释放oper,因为同种类型数据是要共用同一个opertion类型的指针)
static void freeSEntry(ChainSet_S* pSet, Entry_S_inChainSet* entry) {
    if (entry->isEmpty) return;
    freeSData(&(entry->key), pSet->keyInfo);
    entry->isEmpty = true;
}


/************** */
//释放一个链表,包括它里面Data类型数据所指向的内容
static void freeSList(List_S_inChainSet* plist, ChainSet_S* pSet) {
    if (plist->size == 0) {
        return;
    }
    Node_S_inChainSet* p = plist->head;
    Node_S_inChainSet* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        freeSEntry(pSet, &(p->entry));
        free(q);
    }
    initSList(plist);
}




void freeSChainSet(ChainSet_S* pSet) {
    for (int i = 0; i < pSet->len; i++) {
        freeSList(&(pSet->arr[i]), pSet);
    }
    free(pSet->arr);
    initSChainSet(pSet, pSet->keyInfo);
}




///////////////////////////////////////////////////////////////////////////////////////
//链表的构建


static bool isEmptySList(List_S_inChainSet* plist) {
    return plist->size == 0;
}


/// @brief 初始化链表
/// @param plist 链表指针
static void initSList(List_S_inChainSet* plist) {
    plist->head = plist->tail = NULL;
    plist->size = 0;
}



static Node_S_inChainSet* getNodeBySKey(List_S_inChainSet* plist, Data_S key, InfoOfData* keyInfo) {
    if (isEmptySList(plist)) return NULL;
    Node_S_inChainSet* p = plist->head;
    for (int i = 0; i < plist->size; i++, p = p->next) {
        if (compareSData(p->entry.key, keyInfo, key, keyInfo) == SAME) {
            return p;
        }
    } 
    return NULL;
}

//这个是直接在后面插入的,不会判断key是否已经存在
static int insertSEntryInSList(List_S_inChainSet* plist, Entry_S_inChainSet entry) {
    Node_S_inChainSet* newNode = (Node_S_inChainSet*)malloc(sizeof(Node_S_inChainSet));
    if (newNode == NULL) {
        //内存分配失败
        return Warning;
    }
    newNode->entry = entry;
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



static int delStartNode(List_S_inChainSet* plist, ChainSet_S* pSet) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainSet* p = plist->head;
    
    if (plist->size > 1) {
        plist->head = plist->head->next;
        plist->head->prev = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSEntry(pSet, &(p->entry));
    free(p);
    plist->size--;
    return Success;

}



static int delEndNode(List_S_inChainSet* plist, ChainSet_S* pSet) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainSet* p = plist->tail;
    
    if (plist->size > 1) {
        plist->tail = plist->tail->prev;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSEntry(pSet, &(p->entry));
    free(p);
    plist->size--;
    return Success;
}



static int delNodeBySKey(List_S_inChainSet* plist, Data_S key, ChainSet_S* pSet) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainSet* p = getNodeBySKey(plist, key, pSet->keyInfo);
    if (p == NULL) {
        //没找到
        return None;
    }
    if (p == plist->head) return delStartNode(plist, pSet);
    if (p == plist->tail) return delEndNode(plist, pSet);


    p->prev->next = p->next;
    p->next->prev = p->prev;

    freeSEntry(pSet, &(p->entry));
    free(p);
    plist->size--;
    return Success;
}







///////////////////////////////////////////////////////////////////////////////////////////////////
//添加key类


Entry_S_inChainSet createSEntryBySKey(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    if (key.isEmpty) {
        return getEmptySEntry();
    }

    Entry_S_inChainSet newEntry;
    if (isCopyKey == Data_Copy) {
        newEntry.key = copySData(key, pSet->keyInfo);
        if (newEntry.key.isEmpty) {
            return getEmptySEntry();
        }
    } else {
        newEntry.key = key;
    }
    newEntry.key.isOwner = key.isOwner;

    newEntry.isEmpty = false;

    return newEntry;
}



//这个函数保证可以添加
static int addSEntryFunction(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    //hash
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;

    /*
        TODO: 可以先进行复制Key, 复制都失败了, 还有必要查找吗?
    */

    Node_S_inChainSet* p = getNodeBySKey(&(pSet->arr[index]), key, pSet->keyInfo);
    if (p) {
        //完全按照使用者的意思
        Entry_S_inChainSet newEntry = createSEntryBySKey(pSet, key, isCopyKey);
        if (newEntry.isEmpty) {
            //内存分配失败
            return Warning;
        }
        freeSEntry(pSet, &(p->entry));

        p->entry = newEntry;
        return None;
    }


    Entry_S_inChainSet newEntry = createSEntryBySKey(pSet, key, isCopyKey);
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }
    

    if (insertSEntryInSList(&(pSet->arr[index]), newEntry) == Warning) {
        //如果插入失败, 为防止调用者把isOwner设置无权且要求复制, 那么在释放的时候就要强行把她设置为有权
        if (isCopyKey == Data_Copy) {
            newEntry.key.isOwner = true;
        }
        freeSEntry(pSet, &newEntry);
        //内存分配失败
        return Warning;
    }
    pSet->size++;
    return Success;
}




//专门为重哈希做的软拷贝方式添加的Entry
static int addSEntryForFreshSChainSet(ChainSet_S* pSet, Data_S key) {
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;
    Entry_S_inChainSet entry;
    entry.isEmpty = false;
    entry.key = key;
    if (insertSEntryInSList(&(pSet->arr[index]), entry) == Warning) {
        //如果失败的话, 调用这个函数的freshMap函数会进行清理操作, 这里直接返回错误码就行
        //内存分配失败
        return Warning;
    }
    pSet->size++;
    return Success;
}


//专门做的一个软删除的freeList
static void shallowFreeSList(List_S_inChainSet* plist) {
    if (isEmptySList(plist)) {
        return;
    }
    Node_S_inChainSet* p = plist->head;
    Node_S_inChainSet* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
    initSList(plist);
}

static void shallowFreeSChainSet(ChainSet_S* pSet) {
    for (int i = 0; i < pSet->len; i++) {
        shallowFreeSList(&(pSet->arr[i]));
    }
    free(pSet->arr);
    initSChainSet(pSet, pSet->keyInfo);
}


//重hash
static int freshSChainSet(ChainSet_S* pSet) {
    int newLen = 0;
    //无论如何都要保证len至少为16
    if (pSet->len == 0) {
        newLen = 16;
    } else if (4*(pSet->size) >= 3*(pSet->len)) {
        newLen = pSet->len*2;
    } else if (4*(pSet->size) <= pSet->len && pSet->len >= 32) {
        //至少要保证缩容之后len至少为16
        newLen = pSet->len/2;
    } else {
        return None;
    }

    int newSize = pSet->size;
    int newMod = getLargestPrime(newLen);
    
    //新创建一个newSet;
    ChainSet_S newSet;
    List_S_inChainSet* newArray = (List_S_inChainSet*)malloc(newLen*sizeof(List_S_inChainSet));
    if (newArray == NULL) {
        //内存分配失败
        return Warning;
    }
    for (int i = 0; i < newLen; i++) {
        //初始化
        initSList(newArray+i);
    }

    newSet.keyInfo = pSet->keyInfo;
    newSet.arr = newArray;
    newSet.len = newLen;
    newSet.mod = newMod;
    newSet.size = 0;    //再添加函数中会自动加,这里设置为0

    for (int i = 0; i < pSet->len; i++) {
        Node_S_inChainSet* p = pSet->arr[i].head;

        while (p) {
            if (addSEntryForFreshSChainSet(&newSet, p->entry.key) == Warning) {
                shallowFreeSChainSet(&newSet);
                //内存分配失败
                return Warning;
            }
            p = p->next;
        }
        // for (int j = 0; j < pSet->arr[i].size; j++, p = p->next) {
        //     if (addSEntryForFreshSChainSet(&newSet, p->entry.key) == Warning) {
        //         //内存分配失败
        //         return Warning;
        //     }
        // }
    }

    for (int i = 0; i < pSet->len; i++) {
        shallowFreeSList(&(pSet->arr[i]));
    }
    free(pSet->arr);
    *pSet = newSet;

    return Success;
}


int insertSKeyInSChainSet(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    //当填充因子大于75%时或者Set为空时自动扩容
    
    if (freshSChainSet(pSet) == Warning) {
        //如果重hash失败要提示插入失败, 防止继续插入导致Ser出错
        return Warning;
    }
    
    //如果插入失败, 添加函数会进行处理后事
    return addSEntryFunction(pSet, key, isCopyKey);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//查找类


Data_S getSKeyBySKeyInSChainSet(ChainSet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    if (pSet->len == 0 || pSet->size == 0 || pSet->arr == NULL) return getEmptySData();
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;
    
    Node_S_inChainSet* p = getNodeBySKey(&(pSet->arr[index]), key, pSet->keyInfo);
    if (p == NULL) {
        return getEmptySData();
    }
    if (isCopyKey == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_M类型,
            所有这里直接返回就行
        */
        return copySData(p->entry.key, pSet->keyInfo);
    } else {
        return p->entry.key;
    }
}



bool hasSKeyInSChainSet(ChainSet_S* pSet, Data_S key) {
    if (pSet->len == 0 || pSet->size == 0 || pSet->arr == NULL) return false;
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;
    
    Node_S_inChainSet* p = getNodeBySKey(&(pSet->arr[index]), key, pSet->keyInfo);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//删除类


InfoOfReturn delSKeyBySKeyInSChainSet(ChainSet_S* pSet, Data_S key) {
    if (pSet->len == 0 || pSet->size == 0 || pSet->arr == NULL) return Warning;
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;

    if (delNodeBySKey(&(pSet->arr[index]), key, pSet) ==  None) {
        //没找到
        return None;
    }
    pSet->size--;

    //删除后进行重hash
    freshSChainSet(pSet);
    return Success;

}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打印类

void printSKeyInSChainSet(ChainSet_S* pSet, Data_S key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    pSet->keyInfo->oper->printdata(key.data, key.content);
    printf("]");

}


void printSChainSet(ChainSet_S* pSet) {
    if (pSet->size == 0) {
        printf("\nSet is empty, cannot print\n");
        return;
    }
    int cnt = 0;
    printf("[");
    for (int i = 0; i < pSet->len; i++) {
        Node_S_inChainSet* p = pSet->arr[i].head;
        for (int j = 0; j < pSet->arr[i].size; j++, p = p->next) {
            if (cnt != 0) {
                printf(", ");
            }
            printSKeyInSChainSet(pSet, p->entry.key);
            cnt++;
        }
    }
    printf("]");
}


//////////////////////////////////////////////////////////////////////////////////////////////////








