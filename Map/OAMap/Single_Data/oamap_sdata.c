#define ENTRY_STATE_IN_OAMAP
#define GET_LARGESTPRIME
#define DATA_S_OPER
#include "oamap_sdata.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void initSOAMap(OAMap_S* pMap, InfoOfData* keyInfo, InfoOfData* valInfo)  {
    pMap->arr = NULL;
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
        if (pMap->arr[i].state == EXIST_IN_MAP) {
            freeSEntryInSOAMap(pMap, &(pMap->arr[i]));
        }
    }
    free(pMap->arr);
    initSOAMap(pMap, pMap->keyInfo, pMap->valInfo);
}






//////////////////////////////////////////////////////

//复制类

//复制一个Entry,注意:entry.state不是自动赋值,必须要自己赋值
static Entry_S_inOAMap deepCopySEntry(OAMap_S* pMap, Entry_S_inOAMap inputEntry) {
    Entry_S_inOAMap newEntry;

    newEntry.key = deepCopySData(inputEntry.key, pMap->keyInfo);

    if (newEntry.key.isEmpty) {
        printf("\nMemory allocation failed\n");
        return getEmptySEntry();
    }

    newEntry.val = deepCopySData(inputEntry.val, pMap->valInfo);
    if (newEntry.val.isEmpty) {
        freeSData(&(newEntry.key), pMap->keyInfo);
        printf("\nMemory allocation failed\n");
        return getEmptySEntry();
    }
    newEntry.isEmpty = false;
    return newEntry;
}
static Entry_S_inOAMap smartCopySEntry(OAMap_S* pMap, Entry_S_inOAMap inputEntry) {
    Entry_S_inOAMap newEntry;

    newEntry.key = deepCopySData(inputEntry.key, pMap->keyInfo);

    if (newEntry.key.isEmpty) {
        printf("\nMemory allocation failed\n");
        return getEmptySEntry();
    }

    newEntry.val = smartCopySData(inputEntry.val, pMap->valInfo);
    if (newEntry.val.isEmpty) {
        freeSData(&(newEntry.key), pMap->keyInfo);
        printf("\nMemory allocation failed\n");
        return getEmptySEntry();
    }
    newEntry.isEmpty = false;
    return newEntry;
}



/////////////////////////////////////////////////////////////////////////////////
//添加类



static InfoOfReturn addSEntryFunction(OAMap_S* pMap, Data_S key, Data_S val) {
    //对key进行hash
    ull index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%(pMap->mod);
    int flagFindDel = 0;
    ull firstDelIndex = pMap->len+10;

    while (pMap->arr[index].state != NONE_IN_MAP) {
        if (pMap->arr[index].state == DEL_IN_MAP && flagFindDel == 0) {
            firstDelIndex = index;
            flagFindDel = 1;
        }
        if (compareSData(pMap->arr[index].key, pMap->keyInfo, key, pMap->keyInfo) == SAME)  {
            Data_S newVal = smartCopySData(val, pMap->valInfo);
            if (newVal.isEmpty) {
                printf("\nMemory allocation failed\n");
                return Warning;
            }
            freeSData(&(pMap->arr[index].val), pMap->valInfo);
            pMap->arr[index].val = newVal;
            return Success;
        }
        index++;
        index %= pMap->len;
    }

    if (flagFindDel) {
        index = firstDelIndex;
    }

    Entry_S_inOAMap oldEntry;
    oldEntry.isEmpty = false;
    oldEntry.key = key;
    oldEntry.val = val;
    oldEntry.state = EXIST_IN_MAP;

    
    pMap->arr[index] = smartCopySEntry(pMap, oldEntry);
    if (pMap->arr[index].isEmpty) {
        printf("\nMemory allocation failed\n");
        return Warning;
    }
    pMap->arr[index].state = EXIST_IN_MAP;
    pMap->size++;
    return Success;

}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addSEntryForFreshSOAMap(OAMap_S* pMap, Data_S key, Data_S val) {
    ull index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    while (pMap->arr[index].state != NONE_IN_MAP) {
        index++;
        index %= pMap->len;
    }

    pMap->arr[index].key = key;
    pMap->arr[index].val = val;
    pMap->arr[index].state = EXIST_IN_MAP;
    pMap->arr[index].isEmpty = false;
    pMap->size++;
    return Success;

}

static InfoOfReturn freshSOAMap(OAMap_S* pMap) {
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
    
    OAMap_S newMap;
    
    newMap.len = newLen;
    newMap.mod = getLargestPrime(newLen);
    newMap.keyInfo = pMap->keyInfo;
    newMap.valInfo = pMap->valInfo;

    
    newMap.size = 0;    //再添加函数中会自动加,这里设置为0

    newMap.arr = (Entry_S_inOAMap*)malloc(newLen*sizeof(Entry_S_inOAMap));
    if (newMap.arr == NULL) {
        printf("\nMemory allocation failed\n");
        return Warning;
    }

    for (int i = 0; i < newMap.len; i++) {
        newMap.arr[i] = getEmptySEntry();
    }

    
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->arr[i].state == EXIST_IN_MAP) {
            addSEntryForFreshSOAMap(&newMap, pMap->arr[i].key, pMap->arr[i].val);

        }
    }
    free(pMap->arr);

    *pMap = newMap;

    return Success;

}

InfoOfReturn insertSKeyAndSValInSOAMap(OAMap_S* pMap, Data_S key, Data_S val) {
    // if (4*(pMap->size) >= 3*(pMap->len)) {
    //     if (freshSOAMap(pMap) == Warning) {
    //         printf("\nMemory allocation failed\n");
    //         return Warning;
    //     }
    // }
    //先进行重hash
    freshSOAMap(pMap);
    return addSEntryFunction(pMap, key, val);
}



///////////////////////////////////////////////////////
//查找类

static Position getIndexBySKey(OAMap_S* pMap, Data_S key) {
    if (pMap->arr == NULL || pMap->size == 0 || pMap->len == 0) return NOT_FOUND;
    ull index = (pMap->keyInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    for (int i = 0; i < pMap->len; i++) {
        if (pMap->arr[index].state == NONE_IN_MAP) {
            return NOT_FOUND;
        }

        if (compareSData(pMap->arr[index].key, pMap->keyInfo, key, pMap->keyInfo) == SAME) {
            return index;
        }
        index++;
        index %= pMap->len;
    }
    return NOT_FOUND;
}


Data_S getCopySValBySKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    int index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        printf("\nNot Found\n");
        return getEmptySData();
    } else {
        Data_S newData;
        newData = deepCopySData(pMap->arr[index].val, pMap->valInfo);
        if (newData.isEmpty) {
            printf("\nMemory allocation failed\n");
        } 
        return newData;
    }
}



Data_S getPtrSValBySKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    int index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptySData();
    } else {
        return pMap->arr[index].val;
    }
}


Entry_S_inOAMap getCopySEntryBySKeyInSOAMap(OAMap_S* pMap, Data_S key) {
    int index = getIndexBySKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptySEntry();
    } else {
        Entry_S_inOAMap newEntry;
        newEntry = deepCopySEntry(pMap, pMap->arr[index]);
        if (newEntry.isEmpty) {
            printf("\nMemory allocation failed\n");
        }
        return newEntry;
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
        freeSEntryInSOAMap(pMap, &(pMap->arr[index]));
        pMap->arr[index].state = DEL_IN_MAP;
        pMap->size--;
        //删除后进行重hash
        freshSOAMap(pMap);
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
    printf("[key:");
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
        if (pMap->arr[i].isEmpty == false) {
            if (cnt != 0) {
                printf(", ");
            }
            printSEntryInSOAMap(pMap, pMap->arr[i]);
            cnt++;
        }
    }
    printf("]");
}






