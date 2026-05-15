#define NODE_M_INCHAINMAP
#define LIST_M_INCHAINMAP


#define GET_LARGESTPRIME

#define DATA_M_OPER

#include "chainmap_mdata_private.h"
#include "chainmap_mdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool isEmptyMList(List_M_inChainMap* plist);
static void initMList(List_M_inChainMap* plist);



/////////////////////////////////////////////////////////////////////////////////





static Entry_M_inChainMap getEmptyMEntry() {
    Entry_M_inChainMap entry;
    entry.key = getEmptyMData();
    entry.val = getEmptyMData();
    entry.isEmpty = true;
    return entry;
}





void initMChainMap(ChainMap_M* pMap) {
    pMap->arr = NULL;
    pMap->len = pMap->size = 0;
    pMap->mod = 2;
}











///////////////////////////////////////////////////////////////////////////////////
//比较类







//////////////////////////////////////////////////////////////////////////////////////
//释放类

//对外接口


void freeMValInMChainMap(Data_M* val) {
    freeMData(val);
}

void freeMKeyInMChainMap(Data_M* key) {
    freeMData(key);
}


//这个仅仅只会把Entry中的key和value的data和others(不会释放oper,因为同种类型数据是要共用同一个opertion类型的指针)
void freeMEntryInMChainMap(Entry_M_inChainMap* entry) {
    if (entry->isEmpty) return;
    freeMData(&(entry->key));
    freeMData(&(entry->val));
    entry->isEmpty = true;
}


//释放一个链表,包括它里面Data类型数据所指向的内容
static void freeMList(List_M_inChainMap* plist) {
    if (plist->size == 0) {
        return;
    }
    Node_M_inChainMap* p = plist->head;
    Node_M_inChainMap* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        freeMEntryInMChainMap(&(q->entry));
        free(q);
    }
    initMList(plist);
}



void freeMChainMap(ChainMap_M* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        freeMList(&(pMap->arr[i]));
    }
    free(pMap->arr);
    initMChainMap(pMap);
}




///////////////////////////////////////////////////////////////////////////////////////
//链表的构建


static bool isEmptyMList(List_M_inChainMap* plist) {
    return plist->size == 0;
}


/// @brief 初始化链表
/// @param plist 链表指针
static void initMList(List_M_inChainMap* plist) {
    plist->head = plist->tail = NULL;
    plist->size = 0;
}

static Node_M_inChainMap* getNodeByMKey(List_M_inChainMap* plist, Data_M key) {
    if (isEmptyMList(plist)) return NULL;
    Node_M_inChainMap* p = plist->head;
    for (int i = 0; i < plist->size; i++, p = p->next) {
        if (compareMData(p->entry.key, key) == SAME) {
            return p;
        }
    } 
    return NULL;
}

//这个是直接在后面插入的,不会判断key是否已经存在
static InfoOfReturn insertMEntryInMList(List_M_inChainMap* plist, Entry_M_inChainMap entry) {
    Node_M_inChainMap* newNode = (Node_M_inChainMap*)malloc(sizeof(Node_M_inChainMap));
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

static InfoOfReturn delStartNode(List_M_inChainMap* plist) {
    if (isEmptyMList(plist)) {
        //空链表没法删除
        return None;
    }
    Node_M_inChainMap* p = plist->head;
    
    if (plist->size > 1) {
        plist->head = plist->head->next;
        plist->head->prev = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeMEntryInMChainMap(&(p->entry));
    free(p);
    plist->size--;
    return Success;

}



static InfoOfReturn delEndNode(List_M_inChainMap* plist) {
    if (isEmptyMList(plist)) {
        //没找到
        return None;
    }
    Node_M_inChainMap* p = plist->tail;
    
    if (plist->size > 1) {
        plist->tail = plist->tail->prev;
        plist->tail->next = NULL;
    } else {
        plist->head = plist->tail = NULL;
    }

    freeMEntryInMChainMap(&(p->entry));
    free(p);
    plist->size--;
    return Success;
}



static InfoOfReturn delNodeByMKey(List_M_inChainMap* plist, Data_M key) {
    if (isEmptyMList(plist)) {
        //没找到
        return None;
    }
    Node_M_inChainMap* p = getNodeByMKey(plist, key);
    if (p == NULL) {
        //没找到
        return None;
    }
    if (p == plist->head) return delStartNode(plist);
    if (p == plist->tail) return delEndNode(plist);


    p->prev->next = p->next;
    p->next->prev = p->prev;

    freeMEntryInMChainMap(&(p->entry));
    free(p);
    plist->size--;
    return Success;
}







///////////////////////////////////////////////////////////////////////////////////////////////////
//复制类


//复制一个Entry
static Entry_M_inChainMap creatMEntryByMKeyAndMVal(Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
    if (key.isEmpty || val.isEmpty) {
        //不可以传入空数据
        return getEmptyMEntry();
    }
    


    Entry_M_inChainMap newEntry;

    if (isCopyKey == Data_Copy) {
        newEntry.key = copyMData(key);
        if (newEntry.key.isEmpty) {
            return getEmptyMEntry();
        }
        
    } else {
        newEntry.key = key;
    }

    if (isCopyVal == Data_Copy) {
        newEntry.val = copyMData(val);
        if (newEntry.val.isEmpty) {
            freeMData(&(newEntry.key));
            return getEmptyMEntry();
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
static InfoOfReturn addMEntryFunction(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
    //hash
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    //在插入之前先进行一次查找
    Node_M_inChainMap* p = getNodeByMKey(&(pMap->arr[index]), key);
    if (p) {
        //完全按照使用者的意思
        Entry_M_inChainMap newEntry = creatMEntryByMKeyAndMVal(key, isCopyKey, val, isCopyVal);
        if (newEntry.isEmpty) {
            //内存分配失败
            return Warning;
        }
        freeMEntryInMChainMap(&(p->entry));
        p->entry = newEntry;
        return Success;
    }
    
    
    
    Entry_M_inChainMap newEntry = creatMEntryByMKeyAndMVal(key, isCopyKey, val, isCopyVal);
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }
    if (insertMEntryInMList(&(pMap->arr[index]), newEntry) == Warning) {
        //内存分配失败
        return Warning;
    }
    pMap->size++;
    return Success;
}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addMEntryForFreshMChainMap(ChainMap_M* pMap, Data_M key, Data_M val) {
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    Entry_M_inChainMap entry;
    entry.isEmpty = false;
    entry.key = key;
    entry.val = val;
    if (insertMEntryInMList(&(pMap->arr[index]), entry) == Warning) {
        //内存分配失败
        return Warning;
    }
    pMap->size++;
    return Success;
}


static void freeMListForFreshMChainMap(List_M_inChainMap* plist) {
    if (isEmptyMList(plist)) {
        return;
    }
    Node_M_inChainMap* p = plist->head;
    Node_M_inChainMap* q = NULL;
    while (p) {
        q = p;
        p = p->next;
        free(q);
    }
    initMList(plist);
}


//重hash
static InfoOfReturn freshMChainMap(ChainMap_M* pMap) {
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
    ChainMap_M newMap;
    List_M_inChainMap* newArray = (List_M_inChainMap*)malloc(newLen*sizeof(List_M_inChainMap));
    if (newArray == NULL) {
        //内存分配失败
        return Warning;
    }
    for (int i = 0; i < newLen; i++) {
        //初始化
        initMList(newArray+i);
    }


    newMap.arr = newArray;
    newMap.len = newLen;
    newMap.mod = newMod;
    newMap.size = 0;    //再添加函数中会自动加,这里设置为0

    for (int i = 0; i < pMap->len; i++) {
        Node_M_inChainMap* p = pMap->arr[i].head;
        
        /*
            将这个循环改为用p来进行判断
        */
        for (int j = 0; j < pMap->arr[i].size; j++, p = p->next) {
            if (addMEntryForFreshMChainMap(&newMap, p->entry.key, p->entry.val) == Warning) {
                //内存分配失败
                return Warning;
            }
        }
    }

    for (int i = 0; i < pMap->len; i++) {
        freeMListForFreshMChainMap(&(pMap->arr[i]));
    }
    free(pMap->arr);
    *pMap = newMap;
    return Success;
}





InfoOfReturn insertMKeyAndMValInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
    
    //在插入之前进行freshMap
    //当填充因子大于75%时或者Map为空时自动扩容
    freshMChainMap(pMap);
    return addMEntryFunction(pMap, key, isCopyKey, val, isCopyVal);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//查找类


Data_M getMKeyByMKeyInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyKey) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptyMData();
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    Node_M_inChainMap* p = getNodeByMKey(&(pMap->arr[index]), key);
    if (p == NULL) {
        return getEmptyMData();
    }
    if (isCopyKey == Data_Copy) {
        return copyMData(p->entry.key);
    } else {
        return p->entry.key;
    }
}

Data_M getMValByMKeyInMchainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyVal) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptyMData();
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    Node_M_inChainMap* p = getNodeByMKey(&(pMap->arr[index]), key);
    if (p == NULL) {
        return getEmptyMData();
    }
    if (isCopyVal == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_M类型,
            所有这里直接返回就行
        */
        return copyMData(p->entry.val);
    } else {
        return p->entry.val;
    }
}


Entry_M_inChainMap getMEntryByMKeyInMChainMap(ChainMap_M* pMap, Data_M key, selectOfCopy isCopyEntry) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return getEmptyMEntry();
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_M_inChainMap* p = getNodeByMKey(&(pMap->arr[index]), key);

    if (p == NULL) {
        return getEmptyMEntry();
    }

    if (isCopyEntry == Data_Copy) {
        Entry_M_inChainMap newEntry;
        
        newEntry = creatMEntryByMKeyAndMVal(p->entry.key, Data_Copy, p->entry.val, Data_Copy);
        //函数已经说明是会复制的了, 返回的具有权限
        newEntry.key.isOwner = true;
        newEntry.val.isOwner = true;

        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Entry_M_inChainMap类型,
            所有这里直接返回就行
        */
        return newEntry;
    } else {
        return p->entry;
    }
}


bool hasMKeyInMChainMap(ChainMap_M* pMap, Data_M key) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return false;
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    Node_M_inChainMap* p = getNodeByMKey(&(pMap->arr[index]), key);
    if (p == NULL) {
        return false;
    } else {
        return true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//删除类


InfoOfReturn delMEntryByMKeyInMChainMap(ChainMap_M* pMap, Data_M key) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->arr == NULL) return Warning;
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    
    if (delNodeByMKey(&(pMap->arr[index]), key) == None) {
        //没找到
        return None;
    }
    pMap->size--;
    //删除后进行重hash
    freshMChainMap(pMap);
    return Success;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打印类


void printMKeyInMChainMap(Data_M key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    key.dataInfo->oper->printdata(key.data, key.content);
    printf("]");
}

void printMValInMChainMap(Data_M val) {
    if (val.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printMEntryInMChainMap(Entry_M_inChainMap entry) {
    if (entry.isEmpty) {
        printf("\nEntry is empty, cannot print\n");
        return;
    }
    //key
    printf("[key:");
    entry.key.dataInfo->oper->printdata(entry.key.data, entry.key.content);
    
    //val
    printf(", val:");
    entry.val.dataInfo->oper->printdata(entry.val.data, entry.val.content);
    printf("]");
}

void printMChainMap(ChainMap_M* pMap) {
    if (pMap->size == 0) {
        printf("\nMap is empty, cannot print\n");
        return;
    }
    int cnt = 0;
    printf("[");
    for (int i = 0; i < pMap->len; i++) {
        Node_M_inChainMap* p = pMap->arr[i].head;
        for (int j = 0; j < pMap->arr[i].size; j++, p = p->next) {
            if (cnt != 0) {
                printf(", ");
            }
            printMEntryInMChainMap(p->entry);
            cnt++;
        }
    }
    printf("]");
}


//////////////////////////////////////////////////////////////////////////////////////////////////








