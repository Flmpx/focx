#define GET_LARGESTPRIME
#define ENTRY_STATE_IN_OAMAP
#define DATA_M_OPER

#include "oamap_mdata_private.h"
#include "oamap_mdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


static Entry_M_inOAMap getEmptyMEntry() {
    Entry_M_inOAMap entry;
    entry.key = getEmptyMData();
    entry.val = getEmptyMData();
    entry.state = NONE_IN_MAP;
    entry.isEmpty = true;
    return entry;
}



void initMOAMap(OAMap_M* pMap) {
    pMap->buckets = NULL;
    pMap->len = pMap->size = 0;
    pMap->mod = 2;
}


///////////////////////////////////////////////////////////////////////////////////
//比较类



//////////////////////////////////////////////////////////////////////////////////////
//释放类


void freeMValInMOAMap(Data_M* val) {
    freeMData(val);
}


void freeMKeyInMOAMap(Data_M* key) {
    freeMData(key);
}

/// @note 不会自动给entry的state进行赋值, 自己根据情况进行赋值,这个仅仅只会把Entry中的key和value的data和others(不会释放oper,因为同种类型数据是要共用同一个opertion类型的指针)
void freeMEntryInMOAMap(Entry_M_inOAMap* entry) {
    if (entry->isEmpty) return;
    freeMData(&(entry->key));
    freeMData(&(entry->val));
    entry->isEmpty = true;
}

void freeMOAMap(OAMap_M* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            freeMEntryInMOAMap(&(pMap->buckets[i]));
        }
    }
    free(pMap->buckets);
    initMOAMap(pMap);
}


void clearMOAMap(OAMap_M* pMap) {
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            freeMEntryInMOAMap(&(pMap->buckets[i]));
        }
        //全部设置为空, 无论del或者exist
        pMap->buckets[i].state = NONE_IN_MAP;
    }
    pMap->size = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//复制类

/// @note 复制一个Entry,注意:entry.state不是自动赋值,必须要自己赋值
static Entry_M_inOAMap creatMEntryByMKeyAndMVal(Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
    if (key.isEmpty || val.isEmpty) {
        //不可以传入空数据
        return getEmptyMEntry();
    }
    


    Entry_M_inOAMap newEntry;

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
static InfoOfReturn addMEntryFunction(OAMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
    //hash
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;

    //用于记录第一次出现的的Del位置, 这么做的原因是即便碰到Del标记也不能直接插入
    int flagFindDel = 0;
    ll firstDelIndex = pMap->len+10;
    //找到一个NONE或者DEl标记的位置
    while (pMap->buckets[index].state != NONE_IN_MAP) {
        if (pMap->buckets[index].state == DEL_IN_MAP && flagFindDel == 0) {
            firstDelIndex = index;
            flagFindDel = 1;
        }
        //如果发现是同一个key,则更新数据
        if (compareMData(pMap->buckets[index].key, key) == SAME) {
            //完全按照使用者的意思
            Entry_M_inOAMap newEntry = creatMEntryByMKeyAndMVal(key, isCopyKey, val, isCopyVal);
            if (newEntry.isEmpty) {
                //内存分配失败
                return Warning;
            }
            //由于creatMEntryByMKeyAndMVal函数不会给state赋值, 所有这里进行赋值
            newEntry.state = EXIST_IN_MAP;
            freeMEntryInMOAMap(&(pMap->buckets[index]));

            pMap->buckets[index] = newEntry;

            return Success;
        }
        index++;
        index %= pMap->len;
    }
    //如果之间找到了Del标记, 就在Del位置拆即可
    if (flagFindDel) {
        index = firstDelIndex;
    }


    Entry_M_inOAMap newEntry = creatMEntryByMKeyAndMVal(key, isCopyKey, val, isCopyVal);
    
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }
    pMap->buckets[index] = newEntry;
    pMap->buckets[index].state = EXIST_IN_MAP;
    pMap->size++;
    return Success;
}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addMEntryForFreshMOAMap(OAMap_M* pMap, Data_M key, Data_M val) {
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
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


//重hash
static InfoOfReturn freshMOAMap(OAMap_M* pMap, int newLen) {
    //无论如何都要保证len至少为16
    if (newLen < 16) {
        newLen = 16;
    }
    
    if (newLen < pMap->size) {
        return Warning;
    }

    int newSize = pMap->size;
    int newMod = getLargestPrime(newLen);
    
    //新创建一个newMap;
    OAMap_M newMap;
    Entry_M_inOAMap* newArray = (Entry_M_inOAMap*)malloc(newLen*sizeof(Entry_M_inOAMap));
    if (newArray == NULL) {
        //内存分配失败
        return Warning;
    }
    for (int i = 0; i < newLen; i++) {
        //初始化
        newArray[i].state = NONE_IN_MAP;
        newArray[i].isEmpty = true;
    }

    newMap.buckets = newArray;
    newMap.len = newLen;
    newMap.mod = newMod;
    newMap.size = 0;    //再添加函数中会自动加,这里设置为0
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[i].state == EXIST_IN_MAP) {
            addMEntryForFreshMOAMap(&newMap, pMap->buckets[i].key, pMap->buckets[i].val);
        }
    }

    //释放掉原有Entry数组
    free(pMap->buckets);

    //给新址
    *pMap = newMap;
    return Success;
}



InfoOfReturn shrinkMOAMap(OAMap_M* pMap) {
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
    return freshMOAMap(pMap, newLen);
}


InfoOfReturn insertMKeyAndMValInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyKey, Data_M val, selectOfCopy isCopyVal) {
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
        if (freshMOAMap(pMap, newLen) == Warning) {
            //如果重hash失败要提示插入失败, 防止继续插入导致Map出错
            return Warning;
        }

    }
    //如果插入失败, 添加函数会进行处理后事
    return addMEntryFunction(pMap, key, isCopyKey, val, isCopyVal);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//查找类


//通过key返回key在Map中的位置
static Position getIndexByMKey(OAMap_M* pMap, Data_M key) {
    if (pMap->len == 0 || pMap->size == 0 || pMap->buckets == NULL) return NOT_FOUND;
    ll index = (key.dataInfo->oper->hashdata(key.data, key.content))%pMap->mod;
    for (int i = 0; i < pMap->len; i++) {
        if (pMap->buckets[index].state == NONE_IN_MAP) {
            return NOT_FOUND;
        }
        
        //这个也可以比较为空的情况
        if (compareMData(pMap->buckets[index].key, key) == SAME) {
            return index;
        }
        index++;
        index %= (pMap->len);
    }
    return NOT_FOUND;
}

Data_M getMKeyByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyKey) {
    Position index = getIndexByMKey(pMap, key);
    if (index == NOT_FOUND) {
        //没找到
        return getEmptyMData();
    } 
    if (isCopyKey == Data_Copy) {
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_M类型,
            所有这里直接返回就行
        */
        return copyMData(pMap->buckets[index].key);
    } else {
        return pMap->buckets[index].key;
    }
}

Data_M getMValByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyVal) {
    Position index = getIndexByMKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptyMData();
    } 
    if (isCopyVal == Data_Copy) {
        return copyMData(pMap->buckets[index].val);
    } else {
        return pMap->buckets[index].val;
    }
}

Entry_M_inOAMap getMEntryByMKeyInMOAMap(OAMap_M* pMap, Data_M key, selectOfCopy isCopyEntry) {
    Position index = getIndexByMKey(pMap, key);
    if (index == NOT_FOUND) {
        return getEmptyMEntry();
    }
    if (isCopyEntry == Data_Copy) {
        Entry_M_inOAMap newEntry;
        newEntry = creatMEntryByMKeyAndMVal(pMap->buckets[index].key, Data_Copy, pMap->buckets[index].val, Data_Copy);
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
        return pMap->buckets[index];
    }
}


bool hasMKeyInMOAMap(OAMap_M* pMap, Data_M key) {
    if (getIndexByMKey(pMap, key) == NOT_FOUND) {
        return false;
    } else {
        return true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//删除类


InfoOfReturn delMEntryByMKeyInMOAMap(OAMap_M* pMap, Data_M key) {
    int index = getIndexByMKey(pMap, key);
    if (index == NOT_FOUND) {
        return None;
    } else {
        freeMEntryInMOAMap(&(pMap->buckets[index]));
        pMap->buckets[index].state = DEL_IN_MAP;
        pMap->size--;
        return Success;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打印类

void printMKeyInMOAMap(Data_M key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    key.dataInfo->oper->printdata(key.data, key.content);
    printf("]");
    
}

void printMValInMOAMap(Data_M val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");

}

void printMEntryInMOAMap(Entry_M_inOAMap entry) {
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

void printMOAMap(OAMap_M* pMap) {
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
            printMEntryInMOAMap(pMap->buckets[i]);
            cnt++;
        }
        
    }
    printf("]");
}


//////////////////////////////////////////////////////////////////////////////////////////////////

