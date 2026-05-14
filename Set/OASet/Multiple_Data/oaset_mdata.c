#define ENTRY_M_INOASET
#define GET_LARGESTPRIME
#define ENTRY_STATE_IN_OASET
#define DATA_M_OPER
#include "oaset_mdata_private.h"
#include "oaset_mdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


Entry_M_inOASet getEmptyMEntry() {
    Entry_M_inOASet entry;
    entry.key = getEmptyMData();
    entry.isEmpty = true;
    entry.state = NONE_IN_SET;
    return entry;
}


void initMOASet(OASet_M* pSet) {
    pSet->arr = NULL;
    pSet->len = pSet->size = 0;
    pSet->mod = 2;
}


///////////////////////////////////////////////////////////////////////////////////
//比较类



//////////////////////////////////////////////////////////////////////////////////////
//释放类


void freeMKeyInMOASet(Data_M* key) {
    freeMData(key);
}


//不会自动给entry的state进行赋值, 自己根据情况进行赋值,这个仅仅只会把Entry中的key的data和content(不会释放oper,因为同种类型数据是要共用同一个opertion类型的指针)
static void freeMEntry(Entry_M_inOASet* entry) {
    if (entry->isEmpty) return;
    freeMData(&(entry->key));
    entry->isEmpty = true;
}

void freeMOASet(OASet_M* pSet) {
    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[i].state == EXIST_IN_SET) {
            freeMEntry(&(pSet->arr[i]));
        }
    }
    free(pSet->arr);
    initMOASet(pSet);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//添加key类


/// @note 复制一个Entry,注意:entry.state不是自动赋值,必须要自己赋值
static Entry_M_inOASet createMEntryByMKey(Data_M key, selectOfCopy isCopyKey) {
    if (key.isEmpty) {
        return getEmptyMEntry();
    }

    Entry_M_inOASet newEntry;
    if (isCopyKey == Data_Copy) {
        newEntry.key = copyMData(key);
        if (newEntry.key.isEmpty) {
            return getEmptyMEntry();
        }
    } else {
        newEntry.key = key;
    }
    newEntry.isEmpty = false;
    newEntry.key.isOwner = key.isOwner;
    return newEntry;
} 



//这个函数保证可以添加
static InfoOfReturn addMEntryFunction(OASet_M* pSet, Data_M key, selectOfCopy isCopyKey) {
    //hash
    ull index = (key.dataInfo->oper->hashdata(key.data, key.content))%pSet->mod;

    //用于记录第一次出现的的Del位置, 这么做的原因是即便碰到Del标记也不能直接插入
    int flagFindDel = 0;
    ull firstDelIndex = pSet->len+10;
    //找到一个NONE或者DEl标记的位置
    while (pSet->arr[index].state != NONE_IN_SET) {
        if (pSet->arr[index].state == DEL_IN_SET && flagFindDel == 0) {
            firstDelIndex = index;
            flagFindDel = 1;
        }
        //如果发现是同一个key, 直接退出就行
        if (compareMData(pSet->arr[index].key, key) == SAME) {
            Entry_M_inOASet newEntry = createMEntryByMKey(key, isCopyKey);
            if (newEntry.isEmpty) {
                //内存分配失败
                return Warning;
            }
            newEntry.state = EXIST_IN_SET;
            freeMEntry(&(pSet->arr[index]));
            
            pSet->arr[index] = newEntry;

            return None;
        }
        index++;
        index %= pSet->len;
    }
    //如果之间找到了Del标记, 就在Del位置拆即可
    if (flagFindDel) {
        index = firstDelIndex;
    }

    Entry_M_inOASet newEntry = createMEntryByMKey(key, isCopyKey);
    if (newEntry.isEmpty) {
        //内存分配失败
        return Warning;
    }
    pSet->arr[index].state = EXIST_IN_SET;
    pSet->size++;
    return Success;
}




//专门为重哈希做的软拷贝方式添加的Entry
static InfoOfReturn addMEntryForFreshMOASet(OASet_M* pSet, Data_M key) {
    ull index = (key.dataInfo->oper->hashdata(key.data, key.content))%pSet->mod;
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


//重hash
static InfoOfReturn freshMOASet(OASet_M* pSet) {
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
    OASet_M newSet;
    Entry_M_inOASet* newArray = (Entry_M_inOASet*)malloc(newLen*sizeof(Entry_M_inOASet));
    if (newArray == NULL) {
        //内存分配失败
        return Warning;
    }
    for (int i = 0; i < newLen; i++) {
        //初始化
        newArray[i].state = NONE_IN_SET;
        newArray[i].isEmpty = true;
    }

    newSet.arr = newArray;
    newSet.len = newLen;
    newSet.mod = newMod;
    newSet.size = 0;    //再添加函数中会自动加,这里设置为0
    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[i].state == EXIST_IN_SET) {
            addMEntryForFreshMOASet(&newSet, pSet->arr[i].key);
        }
    }

    //释放掉原有Entry数组
    free(pSet->arr);

    //给新址
    *pSet = newSet;
    return Success;
}



InfoOfReturn insertMKeyInMOASet(OASet_M* pSet, Data_M key, selectOfCopy isCopyKey) {
    //当填充因子大于75%时自动扩容
    freshMOASet(pSet);
    return addMEntryFunction(pSet, key, isCopyKey);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//查找类


//通过key返回key在Set中的位置
static Position getIndexByMKey(OASet_M* pSet, Data_M key) {
    if (pSet->len == 0 || pSet->size == 0 || pSet->arr == NULL) return NOT_FOUND;
    ull index = (key.dataInfo->oper->hashdata(key.data, key.content))%pSet->mod;
    for (int i = 0; i < pSet->len; i++) {
        if (pSet->arr[index].state == NONE_IN_SET) {
            return NOT_FOUND;
        }
        
        //这个也可以比较为空的情况
        if (compareMData(pSet->arr[index].key, key) == SAME) {
            return index;
        }
        index++;
        index %= (pSet->len);
    }
    return NOT_FOUND;
}


//返回的Data数据为新建,用完后记得释放
Data_M getCopyMKeyByMKeyInMOASet(OASet_M* pSet, Data_M key) {
    int index = getIndexByMKey(pSet, key);
    if (index == NOT_FOUND) {
        printf("\nNot Found\n");
        return getEmptyMData();
    } else {
        Data_M newData;
        newData = copyMData(pSet->arr[index].key);
        /*
            由于复制类函数如果复制不成功, 
            那会自动返回空的Data_M类型,
            所有这里直接返回就行
        */
        return newData;
    }
}



bool hasMKeyInMOASet(OASet_M* pSet, Data_M key) {
    if (getIndexByMKey(pSet, key) == NOT_FOUND) {
        return false;
    } else {
        return true;
    }
}



/////////////////////////////////////////////////////////////////////////////////////////
//删除类


InfoOfReturn delMKeyByMKeyInMOASet(OASet_M* pSet, Data_M key) {
    int index = getIndexByMKey(pSet, key);
    if (index == NOT_FOUND) {
        return None;
    } else {
        freeMEntry(&(pSet->arr[index]));
        pSet->arr[index].state = DEL_IN_SET;
        pSet->size--;
        //删除之后进行重hash
        freshMOASet(pSet);
        return Success;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//打印类

void printMKeyInMOASet(Data_M key) {
    if (key.isEmpty) {
        printf("\nkey is empty, cannot print\n");
        return;
    }
    printf("[key:");
    key.dataInfo->oper->printdata(key.data, key.content);
    printf("]");
    
}


void printMOASet(OASet_M* pSet) {
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
            printMKeyInMOASet(pSet->arr[i].key);
            cnt++;
        }
        
    }
    printf("]");
}


//////////////////////////////////////////////////////////////////////////////////////////////////

