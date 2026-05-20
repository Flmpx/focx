#define ENTRY_STATE_IN_OAMAP
#define GET_LARGESTPRIME
#define DATA_S_OPER


#include "oamap_sdata_private.h"
#include "oamap_sdata.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void initSOAMap(OAMap_S* pMap, InfoOfData* keyInfo, InfoOfData* valInfo)  {
    pMap->buckets = NULL;
    pMap->keyInfo = keyInfo;
    pMap->valInfo = valInfo;
    pMap->len = pMap->size = 0;
    pMap->mod = 2;
}

/////////////////////////////////////////////
//空

//得到空Entry_S_inOAMap
static Entry_S_inOAMap getEmptySEntry() {
    Entry_S_inOAMap empty;
    empty.isEmpty = true;
    empty.key = empty.val = getEmptySData();
    empty.state = NONE_IN_MAP;
    return empty;
}


/////////////////////////////////////////////////////////////////////////////////////
//释放类




void freeSValInSOAMap(OAMap_S* pMap, Data_S* val) {
    freeSData(val, pMap->valInfo);
}

void freeSKeyInSOAMap(OAMap_S* pMap, Data_S* key) {
    freeSData(key, pMap->keyInfo);
}

//不会自动给entry的state进行赋值, 自己根据情况进行赋值
void freeSEntryInSOAMap(OAMap_S* pMap, Entry_S_inOAMap* entry) {
    if (entry->isEmpty) return;
    freeSData(&(entry->key), pMap->keyInfo);
    freeSData(&(entry->val), pMap->valInfo);
    entry->isEmpty = true;
    entry->state = NONE_IN_MAP;
}


void freeSOAMap(OAMap_S* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            freeSEntryInSOAMap(pMap, &(pMap->buckets[i]));
        }
    }
    free(pMap->buckets);
    initSOAMap(pMap, pMap->keyInfo, pMap->valInfo);
}

void clearSOAMap(OAMap_S* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            freeSEntryInSOAMap(pMap, &(pMap->buckets[i]));
        }
        //全部设置为空, 无论del或者exist
        pMap->buckets[i].state = NONE_IN_MAP;
    }
    pMap->size = 0;
}





//////////////////////////////////////////////////////

//复制类

/// @note 复制一个Entry,注意:entry.state不是自动赋值,必须要自己赋值
static Entry_S_inOAMap createSEntryBySKeyAndMVal(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    if (key.isEmpty || val.isEmpty) {
        //不可以传入空数据
        return getEmptySEntry();
    }
    


    Entry_S_inOAMap newEntry;

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



/////////////////////////////////////////////////////////////////////////////////
//添加类



static InfoOfReturn addSEntryFunction(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    //对key进行hash
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%(pMap->mod);
    int flagFindDel = 0;
    ll firstDelIndex = pMap->len+10;

    while (pMap->buckets[index].state != NONE_IN_MAP) {
        if (pMap->buckets[index].state == DEL_IN_MAP && flagFindDel == 0) {
            firstDelIndex = index;
            flagFindDel = 1;
        }
        if (compareSData(pMap->buckets[index].key, pMap->keyInfo, key, pMap->keyInfo) == SAME)  {
            //完全按照使用者的意思
            Entry_S_inOAMap newEntry = createSEntryBySKeyAndMVal(pMap, key, isCopyKey, val, isCopyVal);
            if (newEntry.isEmpty) {
                //内存分配失败
                return Warning;
            }
            //由于createSEntryBySKeyAndMVal函数不会自动给state赋值
            newEntry.state = EXIST_IN_MAP;
            freeSEntryInSOAMap(pMap, &(pMap->buckets[index]));

            pMap->buckets[index] = newEntry;
            return Success;
        }
        index++;
        index %= pMap->len;
    }

    if (flagFindDel) {
        index = firstDelIndex;
    }

    
    Entry_S_inOAMap newEntry = createSEntryBySKeyAndMVal(pMap, key, isCopyKey, val, isCopyVal);
    
    if (newEntry.isEmpty) {
        //内存分配失败;
        return Warning;
    }

    pMap->buckets[index] = newEntry;
    pMap->buckets[index].state = EXIST_IN_MAP;
    pMap->size++;
    return Success;

}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addSEntryForFreshSOAMap(OAMap_S* pMap, Data_S key, Data_S val) {
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    while (pMap->buckets[index].state != NONE_IN_MAP) {
        index++;
        index %= pMap->len;
    }

    pMap->buckets[index].key = key;
    pMap->buckets[index].val = val;
    pMap->buckets[index].state = EXIST_IN_MAP;
    pMap->buckets[index].isEmpty = false;
    pMap->size++;
    return Success;

}

static InfoOfReturn freshSOAMap(OAMap_S* pMap, int newLen) {


    //无论如何都要保证len至少为16
    if (newLen < 16) {
        newLen = 16;
    }
    
    if (newLen < pMap->size) {
        return Warning;
    }
    
    OAMap_S newMap;
    
    newMap.len = newLen;
    newMap.mod = getLargestPrime(newLen);
    newMap.keyInfo = pMap->keyInfo;
    newMap.valInfo = pMap->valInfo;

    
    newMap.size = 0;    //再添加函数中会自动加,这里设置为0

    newMap.buckets = (Entry_S_inOAMap*)malloc(newLen*sizeof(Entry_S_inOAMap));
    if (newMap.buckets == NULL) {
        //内存分配失败;
        return Warning;
    }

    for (int i = 0; i < newMap.len; i++) {
        newMap.buckets[i] = getEmptySEntry();
    }

    
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            addSEntryForFreshSOAMap(&newMap, pMap->buckets[i].key, pMap->buckets[i].val);

        }
    }
    free(pMap->buckets);

    *pMap = newMap;

    return Success;

}


InfoOfReturn shrinkSOAMap(OAMap_S* pMap) {
    if (pMap == NULL) {
        return Warning;
    }

    //容量必须保证有16个以上, 只有填充因子小于25%才进行缩容
    if (pMap->len < 32 || 4*(pMap->size) > pMap->len) {
        return None;
    }
    //缩容为当前元素总量的2倍
    int newLen = pMap->size*2;
    if (newLen < 16) {
        newLen = 16;
    }
    return freshSOAMap(pMap, newLen);

}

InfoOfReturn insertSKeyAndSValInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey, Data_S val, selectOfCopy isCopyVal) {
    //当填充因子大于75%时自动扩容
    bool flagOfExpend = false;
    int newLen = 0;
    if (pMap->len == 0) {
        flagOfExpend = true;
        newLen = 16;
    } else if (4*(pMap->size) >= 3*(pMap->len)) {
        flagOfExpend = true;
        newLen = pMap->len*2;
    }

    if (flagOfExpend) {
        if (freshSOAMap(pMap, newLen) == Warning) {
            //如果重hash失败要提示插入失败, 防止继续插入导致Map出错
            return Warning;
        }

    }

    //如果插入失败, 添加函数会进行处理后事
    return addSEntryFunction(pMap, key, isCopyKey, val, isCopyVal);
}



///////////////////////////////////////////////////////
//查找类

static Position getIndexBySKey(OAMap_S* pMap, Data_S key) {
    if (pMap->buckets == NULL || pMap->size == 0 || pMap->len == 0) return NOT_FOUND;
    ll index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[index].state == NONE_IN_MAP) {
            return NOT_FOUND;
        }

        if (compareSData(pMap->buckets[index].key, pMap->keyInfo, key, pMap->keyInfo) == SAME) {
            return index;
        }
        index++;
        index %= pMap->len;
    }
    return NOT_FOUND;
}


Data_S getSKeyBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyKey) {
    Position index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        //没找到
        return getEmptySData();
    }
    if (isCopyKey == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_S类型,
            所有这里直接返回就行
        */
        return copySData(pMap->buckets[index].key, pMap->keyInfo);
    } else {
        return pMap->buckets[index].key;
    }
}


Data_S getSValBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyVal) {
    Position index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptySData();
    }
    if (isCopyVal == Data_Copy) {
        return copySData(pMap->buckets[index].val, pMap->valInfo);
    } else {
        return pMap->buckets[index].val;
    }
    
}

Entry_S_inOAMap getSEntryBySKeyInSOAMap(OAMap_S* pMap, Data_S key, selectOfCopy isCopyEntry) {
    Position index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptySEntry();
    }
    if (isCopyEntry == Data_Copy) {
        Entry_S_inOAMap newEntry;
        
        newEntry = createSEntryBySKeyAndMVal(pMap, pMap->buckets[index].key, Data_Copy, pMap->buckets[index].val, Data_Copy);
        
        //函数已经说明是会复制的了, 返回的应当具有权限
        newEntry.key.isOwner = true;
        newEntry.val.isOwner = true;
        
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Entry_S_inOAMap类型,
            所有这里直接返回就行
        */
        return newEntry;
    } else {
        return pMap->buckets[index];
    }
}


bool hasSKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    if (getIndexBySKey(pMap, key) == NOT_FOUND) {
        return false;
    } else {
        return true;
    }
}


///////////////////////////////////////////////////
//删除类

InfoOfReturn delSEntryBySKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    int index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        return None;
    } else {
        freeSEntryInSOAMap(pMap, &(pMap->buckets[index]));
        pMap->buckets[index].state = DEL_IN_MAP;
        pMap->size--;
        return Success;
    }
}


///////////////////////////////////////////////



void printSKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    pMap->keyInfo->oper->printdata(key.data, key.content);
    printf("]");
}

void printSValInSOAMap(OAMap_S* pMap, Data_S val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    pMap->keyInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printSEntryInSOAMap(OAMap_S* pMap, Entry_S_inOAMap entry) {
    if (entry.isEmpty) {
        printf("\nEntry is empty, cannot print\n");
        return;
    }
    //key
    printf("[key:");
    pMap->keyInfo->oper->printdata(entry.key.data, entry.key.content);

    //val;
    printf(", value:");
    pMap->valInfo->oper->printdata(entry.val.data, entry.val.content);
    printf("]");
}

void printSOAMap(OAMap_S* pMap) {
    if (pMap->size == 0) {
        printf("\nMap is empty, cannot print\n");
        return;
    }

    int cnt = 0;
    printf("[");

    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].isEmpty == false) {
            if (cnt != 0) {
                printf(", ");
            }
            printSEntryInSOAMap(pMap, pMap->buckets[i]);
            cnt++;
        }
    }
    printf("]");
}






