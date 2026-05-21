#define DATA_S_OPER

#include "arrstack_sdata.h"
#include "arrstack_sdata_private.h"
#include <stdlib.h>


InfoOfReturn initSArrStack(ArrStack_S* pStack, InfoOfData* valInfo, bool isUnlimited, int capacity) {
    if (capacity <= 0) {
        //数量异常
        return Warning;
    }
    //先跟新容量
    pStack->capacity = capacity;

    //申请当前容量大小的空间
    pStack->vals = (Data_S*)malloc((pStack->capacity)*sizeof(Data_S));

    if (pStack->vals == NULL) {
        //内存分配失败
        return Warning;
    }
    pStack->isUnlimited = isUnlimited;
    //以防万一, 这里还是自动归零
    pStack->size = 0;
    pStack->top = -1;
    pStack->valInfo = valInfo;
    return Success;
}

bool isFullSArrStack(ArrStack_S* pStack) {
    return pStack->capacity == pStack->size;
}

bool isEmptySArrStack(ArrStack_S* pStack) {
    return pStack->size == 0;
}

bool hasSValInSArrStack(ArrStack_S* pStack, Data_S val) {
    for (int i = 0; i < pStack->size; i++) {
        if (compareSData(pStack->vals[i], pStack->valInfo, val, pStack->valInfo) == SAME) {
            return true;
        }
    }
    return false;
}


InfoOfReturn pushSValInSArrStack(ArrStack_S* pStack, Data_S val, selectOfCopy isCopyVal) {
    //如果栈满了, 判断是否可以扩容
    if (isFullSArrStack(pStack)) {

        if (pStack->isUnlimited) {
            //扩容
            
            Data_S* newArr = (Data_S*)realloc(pStack->vals, (pStack->capacity*2)*sizeof(Data_S));
            if (newArr == NULL) {
                return Warning;
            }
            pStack->capacity *= 2;
            
            pStack->vals = newArr;
        } else {
            //栈满
            return Warning;
        }
    }

    Data_S newVal;
    if (isCopyVal == Data_Copy) {
        newVal = copySData(val, pStack->valInfo);
        if (newVal.isEmpty) {
            //这里错误不必释放上面的数组
            return Warning;
        }
        //遵循调用者的意思
        newVal.isOwner = val.isOwner;
    } else {
        newVal = val;
    }

    pStack->top++;
    pStack->vals[pStack->top] = newVal;
    pStack->size++;

    return Success;
}


Data_S popSValInSArrStack(ArrStack_S* pStack) {
    if (isEmptySArrStack(pStack)) {
        return getEmptySData();
    }

    Data_S val = pStack->vals[pStack->top];
    pStack->top--;
    pStack->size--;
    return val;
}

void freeSValInSArrStack(ArrStack_S* pStack, Data_S* val) {
    freeSData(val, pStack->valInfo);
}


void freeSArrStack(ArrStack_S* pStack) {
    for (int i = 0; i < pStack->size; i++) {
        freeSData(&(pStack->vals[i]), pStack->valInfo);
    }
    free(pStack->vals);

    //初始化
    pStack->vals = NULL;
    pStack->capacity = 0;
    pStack->size = 0;
    pStack->top = -1;
    pStack->isUnlimited = false;
    //不处理valInfo, 没必要
}


void clearSArrStack(ArrStack_S* pStack) {
    for (int i = 0; i < pStack->size; i++) {
        freeSData(&(pStack->vals[i]), pStack->valInfo);
    }
    pStack->size = 0;
    pStack->top = -1;
}




////////////////////////////////////////////////////////////
//一下是不公开函数, 用于调试, 存于private头文件中


void printSValInSArrStack(ArrStack_S* pStack, Data_S val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    pStack->valInfo->oper->printdata(val.data, val.content);
    printf("]");
}


void printSArrStack(ArrStack_S* pStack) {
    printf("bottom[");
    int cnt = 0;
    for (int i = 0; i < pStack->size; i++) {
        if (cnt != 0) {
            printf("-->");
        }
        Data_S val = pStack->vals[i];
        pStack->valInfo->oper->printdata(val.data, val.content);
        cnt++;
    }
    printf("]top");
}

