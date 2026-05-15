#define ENTRY_S_INOASET

#define ENTRY_STATE_IN_OASET

#define GET_LARGESTPRIME

#define DATA_S_OPER

#include "oaset_sdata_private.h"
#include "oaset_sdata.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void initSOASet(OASet_S* pSet, InfoOfData* keyInfo)  {
    pSet->arr = NULL;
    pSet->keyInfo = keyInfo;
    pSet->len = pSet->size = 0;
    pSet->mod = 2;
}


static Entry_S_inOASet getEmptySEntry() {
    Entry_S_inOASet entry;
    entry.isEmpty = true;
    entry.key = getEmptySData();
    entry.state = NONE_IN_SET;
    return entry;
}

/////////////////////////////////////////////////////////////////////////////////////
//释放类




void freeSKeyInSOASet(OASet_S* pSet, Data_S* key) {
    freeSData(key, pSet->keyInfo);
}

//不会自动给entry的state进行赋值, 自己根据情况进行赋值
static void freeSEntry(OASet_S* pSet, Entry_S_inOASet* entry) {
    if (entry->isEmpty) return;
    freeSData(&(entry->key), pSet->keyInfo);
    entry->isEmpty = true;
    entry->state = NONE_IN_SET;
}


void freeSOASet(OASet_S* pSet) {
    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[i].state == EXIST_IN_SET) {
            freeSEntry(pSet, &(pSet->arr[i]));
        }
    }
    free(pSet->arr);
    initSOASet(pSet, pSet->keyInfo);
}





/////////////////////////////////////////////////////////////////////////////////
//添加类


/// @note 复制一个Entry,注意:entry.state不是自动赋值,必须要自己赋值
Entry_S_inOASet createSEntryBySkey(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    if (key.isEmpty) {
        return getEmptySEntry();
    }
    Entry_S_inOASet newEntry;
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



static InfoOfReturn addSEntryFunction(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    //对key进行hash
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%(pSet->mod);
    int flagFindDel = 0;
    ll firstDelIndex = pSet->len+10;

    while (pSet->arr[index].state != NONE_IN_SET) {
        if (pSet->arr[index].state == DEL_IN_SET && flagFindDel == 0) {
            firstDelIndex = index;
            flagFindDel = 1;
        }
        if (compareSData(pSet->arr[index].key, pSet->keyInfo, key, pSet->keyInfo) == SAME)  {
            Entry_S_inOASet newEntry = createSEntryBySkey(pSet, key, isCopyKey);
            if (newEntry.isEmpty) {
                //内存分配失败
                return Warning;
            }
            newEntry.state = EXIST_IN_SET;
            freeSEntry(pSet, &(pSet->arr[index]));
            pSet->arr[index] = newEntry;
            return None;
        }
        index++;
        index %= pSet->len;
    }

    if (flagFindDel) {
        index = firstDelIndex;
    }

    Entry_S_inOASet newEntry = createSEntryBySkey(pSet, key, isCopyKey);
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }

    pSet->arr[index] = newEntry;
    pSet->arr[index].state = EXIST_IN_SET;
    pSet->size++;
    return Success;

}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addSEntryForFreshSOASet(OASet_S* pSet, Data_S key) {
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;
    while (pSet->arr[index].state != NONE_IN_SET) {
        index++;
        index %= pSet->len;
    }

    pSet->arr[index].key = key;
    pSet->arr[index].state = EXIST_IN_SET;
    pSet->arr[index].isEmpty = false;
    pSet->size++;
    return Success;

}

static InfoOfReturn freshSOASet(OASet_S* pSet) {
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
    
    OASet_S newSet;
    
    newSet.len = newLen;
    newSet.mod = getLargestPrime(newLen);
    newSet.keyInfo = pSet->keyInfo;
    newSet.size = 0;    //再添加函数中会自动加,这里设置为0
    newSet.arr = (Entry_S_inOASet*)malloc(newLen*sizeof(Entry_S_inOASet));
    if (newSet.arr == NULL) {
        //内存分配失败
        return Warning;
    }

    for (int i = 0; i < newSet.len; i++) {
        newSet.arr[i].isEmpty = true;
        newSet.arr[i].state = NONE_IN_SET;
    }

    
    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[i].state == EXIST_IN_SET) {
            addSEntryForFreshSOASet(&newSet, pSet->arr[i].key);

        }
    }
    free(pSet->arr);

    *pSet = newSet;

    return Success;

}

InfoOfReturn insertSKeyInSOASet(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    // if (4*(pSet->size) >= 3*(pSet->len)) {
    //     if (freshSOASet(pSet) == Warning) {
    //         //内存分配失败
    //         return Warning;
    //     }
    // }
    //先进行重hash
    freshSOASet(pSet);
    return addSEntryFunction(pSet, key, isCopyKey);
}



///////////////////////////////////////////////////////
//查找类

static Position getIndexBySKey(OASet_S* pSet, Data_S key) {
    if (pSet->arr == NULL || pSet->size == 0 || pSet->len == 0) return NOT_FOUND;
    ll index = (pSet->keyInfo->oper->hashdata(key.data, key.content))%pSet->mod;

    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[index].state == NONE_IN_SET) {
            return NOT_FOUND;
        }

        if (compareSData(pSet->arr[index].key, pSet->keyInfo, key, pSet->keyInfo) == SAME) {
            return index;
        }
        index++;
        index %= pSet->len;
    }
    return NOT_FOUND;
}


Data_S getSKeyBySKeyInSOASet(OASet_S* pSet, Data_S key, selectOfCopy isCopyKey) {
    Position index = getIndexBySKey(pSet, key);\
    if (index == NOT_FOUND) {
        return getEmptySData();
    } 
    
    if (isCopyKey == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_M类型,
            所有这里直接返回就行
        */
        return copySData(pSet->arr[index].key, pSet->keyInfo);
    } else {
        return pSet->arr[index].key;
    }
    
}


bool hasSKeyInSOASet(OASet_S* pSet, Data_S key) {
    if (getIndexBySKey(pSet, key) == NOT_FOUND) {
        return false;
    } else {
        return true;
    }
    
    
}


///////////////////////////////////////////////////
//删除类

InfoOfReturn delSKeyBySKeyInSOASet(OASet_S* pSet, Data_S key) {
    int index = getIndexBySKey(pSet, key);
    if (index == NOT_FOUND) {
        return None;
    } else {
        freeSEntry(pSet, &(pSet->arr[index]));
        pSet->arr[index].state = DEL_IN_SET;
        pSet->size--;
        //删除后进行重hash
        freshSOASet(pSet);
        return Success;
    }
}


///////////////////////////////////////////////



void printSKeyInSOASet(OASet_S* pSet, Data_S key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    pSet->keyInfo->oper->printdata(key.data, key.content);
    printf("]");
}


void printSOASet(OASet_S* pSet) {
    if (pSet->size == 0) {
        printf("\nSet is empty, cannot print\n");
        return;
    }

    int cnt = 0;
    printf("[");

    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[i].isEmpty == false) {
            if (cnt != 0) {
                printf(", ");
            }
            printSKeyInSOASet(pSet, pSet->arr[i].key);
            cnt++;
        }
    }
    printf("]");
}






