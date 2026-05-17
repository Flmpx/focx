#define NODE_S_INCHAINMAP
#define LIST_S_INCHAINMAP


#define GET_LARGESTPRIME

#define DATA_S_OPER
#include "chainmap_sdata_private.h"
#include "chainmap_sdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool isEmptySList(List_S_inChainMap* plist);
static void initSList(List_S_inChainMap* plist);

//只要是需要两个InfoOfData的函数,直接传入Map类型指针!!!

/////////////////////////////////////////////////////////////////////////////////


static Entry_S_inChainMap getEmptySEntry() {
    Entry_S_inChainMap entry;
    entry.key = getEmptySData();
    entry.val = getEmptySData();
    entry.isEmpty = true;
    return entry;
}



/************** */
void initSChainMap(ChainMap_S* pMap, InfoOfData* keyInfo, InfoOfData* valInfo) {
    pMap->arr = NULL;
    pMap->len = pMap->size = 0;
    pMap->mod = 2;
    pMap->keyInfo = keyInfo;
    pMap->valInfo = valInfo;
}











///////////////////////////////////////////////////////////////////////////////////
//比较类



//////////////////////////////////////////////////////////////////////////////////////
//释放类


/**************** */
//释放Data数据

void freeSValInSChainMap(ChainMap_S* pMap, Data_S* val) {
    freeSData(val, pMap->valInfo);
}

void freeSKeyInSChainMap(ChainMap_S* pMap, Data_S* key) {
    freeSData(key, pMap->keyInfo);
}

/************ */
//这个仅仅只会把Entry中的key和value的data和others(不会释放oper,因为同种类型数据是要共用同一个opertion类型的指针)
void freeSEntryInSChainMap(ChainMap_S* pMap, Entry_S_inChainMap* entry) {
    if (entry->isEmpty) return;
    freeSData(&(entry->key), pMap->keyInfo);
    freeSData(&(entry->val), pMap->valInfo);
    entry->isEmpty = true;
}


/************** */
//释放一个链表,包括它里面Data类型数据所指向的内容
static void freeSList(List_S_inChainMap* plist, ChainMap_S* pMap) {
    if (plist->size == 0) {
        return;
    }
    Node_S_inChainMap* p = plist->head;
    Node_S_inChainMap* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        freeSEntryInSChainMap(pMap, &(p->entry));
        free(q);
    }
    initSList(plist);
}




/**************** */
void freeSChainMap(ChainMap_S* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        freeSList(&(pMap->arr[i]), pMap);
    }
    free(pMap->arr);
    initSChainMap(pMap, pMap->keyInfo, pMap->valInfo);
}




///////////////////////////////////////////////////////////////////////////////////////
//链表的构建


static bool isEmptySList(List_S_inChainMap* plist) {
    return plist->size == 0;
}


/// @brief 初始化链表
/// @param plist 链表指针
static void initSList(List_S_inChainMap* plist) {
    plist->head = plist->tail = NULL;
    plist->size = 0;
}


/************* */
static Node_S_inChainMap* getNodeBySKey(List_S_inChainMap* plist, Data_S key, InfoOfData* keyInfo) {
    if (isEmptySList(plist)) return NULL;
    Node_S_inChainMap* p = plist->head;
    for (int i = 0; i < plist->size; i++, p = p->next) {
        if (compareSData(p->entry.key, keyInfo, key, keyInfo) == SAME) {
            return p;
        }
    } 
    return NULL;
}

//这个是直接在后面插入的,不会判断key是否已经存在
static int insertSEntryInSList(List_S_inChainMap* plist, Entry_S_inChainMap entry) {
    Node_S_inChainMap* newNode = (Node_S_inChainMap*)malloc(sizeof(Node_S_inChainMap));
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



static int delStartNode(List_S_inChainMap* plist, ChainMap_S* pMap) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainMap* p = plist->head;
    
    if (plist->size > 1) {
        plist->head = plist->head->next;
        plist->head->prev = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSEntryInSChainMap(pMap, &(p->entry));
    free(p);
    plist->size--;
    return Success;

}



static int delEndNode(List_S_inChainMap* plist, ChainMap_S* pMap) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainMap* p = plist->tail;
    
    if (plist->size > 1) {
        plist->tail = plist->tail->prev;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeSEntryInSChainMap(pMap, &(p->entry));
    free(p);
    plist->size--;
    return Success;
}



static int delNodeBySKey(List_S_inChainMap* plist, Data_S key, ChainMap_S* pMap) {
    if (isEmptySList(plist)) {
        //没找到
        return None;
    }
    Node_S_inChainMap* p = getNodeBySKey(plist, key, pMap->keyInfo);
    if (p == NULL) {
        //没找到
        return None;
    }
    if (p == plist->head) return delStartNode(plist, pMap);
    if (p == plist->tail) return delEndNode(plist, pMap);


    p->prev->next = p->next;
    p->next->prev = p->prev;

    freeSEntryInSChainMap(pMap, &(p->entry));
    free(p);
    plist->size--;
    return Success;
}







///////////////////////////////////////////////////////////////////////////////////////////////////
//复制类

//复制一个Entry
static Entry_S_inChainMap createSEntryBySKeyAndMVal(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    if (key.isEmpty || val.isEmpty) {
        //不可以传入空数据
        return getEmptySEntry();
    }
    


    Entry_S_inChainMap newEntry;

    if (isCopyKey == Data_Copy) {
        newEntry.key = copySData(key, pMap->keyInfo);
        if (newEntry.key.isEmpty) {
            return getEmptySEntry();
        }
        
    } else {
        newEntry.key = key;
    }

    if (isCopyVal == Data_Copy) {
        newEntry.val = copySData(val, pMap->valInfo);
        if (newEntry.val.isEmpty) {
            freeSData(&(newEntry.key), pMap->keyInfo);
            return getEmptySEntry();
        }
    } else {
        newEntry.val = val;
    }

    //设置是否有权限
    newEntry.key.isOwner = key.isOwner;
    newEntry.val.isOwner = val.isOwner;

    newEntry.isEmpty = false;

    return newEntry;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//添加keyandval类




//这个函数保证可以添加
static int addSEntryFunction(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    //hash
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    Node_S_inChainMap* p = getNodeBySKey(&(pMap->arr[index]), key, pMap->keyInfo);
    if (p) {
        //完全按照使用者的意思
        Entry_S_inChainMap newEntry = createSEntryBySKeyAndMVal(pMap, key, isCopyKey, val, isCopyVal);
        if (newEntry.isEmpty) {
            //内存分配失败
            return Warning;
        }
        freeSEntryInSChainMap(pMap, &(p->entry));

        p->entry = newEntry;
        return Success;
    }
    
    
    Entry_S_inChainMap newEntry = createSEntryBySKeyAndMVal(pMap, key, isCopyKey, val, isCopyVal);
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }
    if (insertSEntryInSList(&(pMap->arr[index]), newEntry) == Warning) {
        //如果插入失败, 为防止调用者把isOwner设置无权且要求复制, 那么在释放的时候就要强行把她设置为有权
        if (isCopyKey == Data_Copy) {
            newEntry.key.isOwner = true;
        }
        if (isCopyVal == Data_Copy) {
            newEntry.val.isOwner = true;
        }
        freeSEntryInSChainMap(pMap, &newEntry);
        //内存分配失败
        return Warning;
    }
    pMap->size++;
    return Success;
}




//专门为重哈希做的软拷贝方式添加的Entry
static int addSEntryForFreshSChainMap(ChainMap_S* pMap, Data_S key, Data_S val) {
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    Entry_S_inChainMap entry;
    entry.isEmpty = false;
    entry.key = key;
    entry.val = val;
    if (insertSEntryInSList(&(pMap->arr[index]), entry) == Warning) {
        //如果失败的话, 调用这个函数的freshMap函数会进行清理操作, 这里直接返回错误码就行
        //内存分配失败
        return Warning;
    }
    pMap->size++;
    return Success;
}


//专门做的一个软删除的freeList
static void shallowFreeSList(List_S_inChainMap* plist) {
    if (isEmptySList(plist)) {
        return;
    }
    Node_S_inChainMap* p = plist->head;
    Node_S_inChainMap* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
    initSList(plist);
}

static void shallowFreeSChainMap(ChainMap_S* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        shallowFreeSList(&(pMap->arr[i]));
    }
    free(pMap->arr);
    initSChainMap(pMap, pMap->keyInfo, pMap->valInfo);
}

//重hash
static int freshSChainMap(ChainMap_S* pMap) {
    int newLen = 0;
    //无论如何都要保证len至少为16
    if (pMap->len == 0) {
        newLen = 16;
    } else if (4*(pMap->size) >= 3*(pMap->len)) {
        newLen = pMap->len*2;
    } else if (4*(pMap->size) <= pMap->len && pMap->len >= 32) {
        //至少要保证缩容之后len至少为16
        newLen = pMap->len/2;
    } else {
        return None;
    }

    int newSize = pMap->size;
    int newMod = getLargestPrime(newLen);
    
    //新创建一个newMap;
    ChainMap_S newMap;
    List_S_inChainMap* newArray = (List_S_inChainMap*)malloc(newLen*sizeof(List_S_inChainMap));
    if (newArray == NULL) {
        //内存分配失败
        return Warning;
    }
    for (int i = 0; i < newLen; i++) {
        //初始化
        initSList(newArray+i);
    }

    newMap.keyInfo = pMap->keyInfo;
    newMap.valInfo = pMap->valInfo;
    newMap.arr = newArray;
    newMap.len = newLen;
    newMap.mod = newMod;
    newMap.size = 0;    //再添加函数中会自动加,这里设置为0

    for (int i = 0; i < pMap->len; i++) {
        Node_S_inChainMap* p = pMap->arr[i].head;
        while (p) {
            if (addSEntryForFreshSChainMap(&newMap, p->entry.key, p->entry.val) == Warning) {
                shallowFreeSChainMap(&newMap);
                //内存分配失败
                return Warning;
            }
            p = p->next;
        }
        // for (int j = 0; j < pMap->arr[i].size; j++, p = p->next) {
        //     if (addSEntryForFreshSChainMap(&newMap, p->entry.key, p->entry.val) == Warning) {
        //         //内存分配失败
        //         return Warning;
        //     }
        // }
    }

    shallowFreeSChainMap(pMap);

    *pMap = newMap;

    return Success;
}


int insertSKeyAndSValInSChainMap(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    //当填充因子大于75%时或者Map为空时自动扩容

    if (freshSChainMap(pMap) == Warning) {
        //如果重hash失败要提示插入失败, 防止继续插入导致Map出错
        return Warning;
    }
    //如果插入失败, 添加函数会进行处理后事
    return addSEntryFunction(pMap, key, isCopyKey, val, isCopyVal);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//查找类


Data_S getSKeyBySKeyInSChainMap(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyKey) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptySData();
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_S_inChainMap* p = getNodeBySKey(&(pMap->arr[index]), key, pMap->keyInfo);
    if (p == NULL) {
        return getEmptySData();
    }
    
    if (isCopyKey == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_S类型,
            所有这里直接返回就行
        */
        return copySData(p->entry.key, pMap->keyInfo);
    } else {
        return p->entry.key;
    }
}



Data_S getSValBySKeyInSChainMap(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyVal) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptySData();
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_S_inChainMap* p = getNodeBySKey(&(pMap->arr[index]), key, pMap->keyInfo);
    if (p == NULL) {
        return getEmptySData();
    }
    if (isCopyVal == Data_Copy) {
        return copySData(p->entry.val, pMap->valInfo);
    } else {
        return p->entry.val;
    }
}

Entry_S_inChainMap getSEntryBySKeyInSChainMap(ChainMap_S* pMap, Data_S key, selectOfCopy isCopyEntry) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptySEntry();
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_S_inChainMap* p = getNodeBySKey(&(pMap->arr[index]), key, pMap->keyInfo);
    if (p == NULL) {
        return getEmptySEntry();
    }

    if (isCopyEntry == Data_Copy) {
        Entry_S_inChainMap newEntry;
        
        newEntry = createSEntryBySKeyAndMVal(pMap, p->entry.key, Data_Copy, p->entry.val, Data_Copy);
        
        //函数已经说明是会复制的了, 返回的应当具有权限
        newEntry.key.isOwner = true;
        newEntry.val.isOwner = true;
        
        
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Entry_S_inChainMap类型,
            所有这里直接返回就行
        */
        return newEntry;
    } else {
        return p->entry;
    }
}

bool hasSKeyInSChainMap(ChainMap_S* pMap, Data_S key) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return false;
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_S_inChainMap* p = getNodeBySKey(&(pMap->arr[index]), key, pMap->keyInfo);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//删除类


InfoOfReturn delSEntryBySKeyInSChainMap(ChainMap_S* pMap, Data_S key) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return Warning;
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    if (delNodeBySKey(&(pMap->arr[index]), key, pMap) ==  None) {
        //没找到
        return None;
    }
    pMap->size--;

    //删除后进行重hash
    freshSChainMap(pMap);
    return Success;

}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打印类

void printSKeyInSChainMap(ChainMap_S* pMap, Data_S key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    pMap->keyInfo->oper->printdata(key.data, key.content);
    printf("]");

}

void printSValInSChainMap(ChainMap_S* pMap, Data_S val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    pMap->keyInfo->oper->printdata(val.data, val.content);
    printf("]");

}

void printSEntryInSChainMap(ChainMap_S* pMap, Entry_S_inChainMap entry) {
    if (entry.isEmpty) {
        printf("\nEntry is empty, cannot print\n");
        return;
    }
    //key
    printf("[key:");
    pMap->keyInfo->oper->printdata(entry.key.data, entry.key.content);
    
    //val
    printf(", val:");
    pMap->valInfo->oper->printdata(entry.val.data, entry.val.content);
    printf("]");
}

void printSChainMap(ChainMap_S* pMap) {
    if (pMap->size == 0) {
        printf("\nMap is empty, cannot print\n");
        return;
    }
    int cnt = 0;
    printf("[");
    for (int i = 0; i < pMap->len; i++) {
        Node_S_inChainMap* p = pMap->arr[i].head;
        for (int j = 0; j < pMap->arr[i].size; j++, p = p->next) {
            if (cnt != 0) {
                printf(", ");
            }
            printSEntryInSChainMap(pMap, p->entry);
            cnt++;
        }
    }
    printf("]");
}


//////////////////////////////////////////////////////////////////////////////////////////////////








