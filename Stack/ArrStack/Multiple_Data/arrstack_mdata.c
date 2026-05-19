#define DATA_M_OPER

#include "arrstack_mdata.h"
#include "arrstack_mdata_private.h"
#include <stdlib.h>


void initMArrStack(ArrStack_M* pStack) {
    pStack->arr = NULL;
    pStack->capacity = 0;
    pStack->isUnlimited = false;
    pStack->size = 0;
    pStack->top = -1;
}


InfoOfReturn allocMArrStack(ArrStack_M* pStack, bool isUnlimited, int capacity) {
    if (capacity <= 0) {
        //数量异常
        return Warning;
    }
    //先跟新容量
    pStack->capacity = capacity;

    //申请当前容量大小的空间
    pStack->arr = (Data_M*)malloc((pStack->capacity)*sizeof(Data_M));

    if (pStack->arr == NULL) {
        //内存分配失败
        return Warning;
    }
    pStack->isUnlimited = isUnlimited;
    //以防万一, 这里还是自动归零
    pStack->size = 0;
    pStack->top = -1;
    return Success;
}

bool isFullMArrStack(ArrStack_M* pStack) {
    return pStack->capacity == pStack->size;
}

bool isEmptyMArrStack(ArrStack_M* pStack) {
    return pStack->size == 0;
}


bool hasMValInMArrStack(ArrStack_M* pStack, Data_M val) {
    for (int i = 0; i < pStack->size; i++) {
        if (compareMData(pStack->arr[i], val) == SAME) {
            return true;
        }
    }
    return false;
}

InfoOfReturn pushMValInMArrStack(ArrStack_M* pStack, Data_M val, selectOfCopy isCopyVal) {
    //如果栈满了, 判断是否可以扩容
    if (isFullMArrStack(pStack)) {

        if (pStack->isUnlimited) {
            //扩容
            pStack->capacity *= 2;
            
            Data_M* newArr = (Data_M*)realloc(pStack->arr, (pStack->capacity)*sizeof(Data_M));
            if (newArr == NULL) {
                return Warning;
            }

            pStack->arr = newArr;
        } else {
            //栈满
            return Warning;
        }
    }

    Data_M newVal;
    if (isCopyVal == Data_Copy) {
        newVal = copyMData(val);
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
    pStack->arr[pStack->top] = newVal;
    pStack->size++;

    return Success;

}

Data_M popMValInMArrStack(ArrStack_M* pStack) {
    if (isEmptyMArrStack(pStack)) {
        return getEmptyMData();
    }

    Data_M val = pStack->arr[pStack->top];
    pStack->top--;
    pStack->size--;
    return val;
}

void freeMValInMArrStack(Data_M* val) {

    //对外接口
    freeMData(val);
}


void freeMArrStack(ArrStack_M* pStack) {
    for (int i = 0; i < pStack->size; i++) {
        freeMData(&(pStack->arr[i]));
    }
    free(pStack->arr);
    initMArrStack(pStack);
}

void clearMArrStack(ArrStack_M* pStack) {
    for (int i = 0; i < pStack->size; i++) {
        freeMData(&(pStack->arr[i]));
    }
    pStack->size = 0;
    pStack->top = -1;
}


////////////////////////////////////////////////////////////
//一下是不公开函数, 用于调试, 存于private头文件中

void printMValInMArrStack(Data_M val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printMArrStack(ArrStack_M* pStack) {
    printf("bottom[");
    int cnt = 0;
    for (int i = 0; i < pStack->size; i++) {
        if (cnt != 0) {
            printf("---");
        }
        Data_M val = pStack->arr[i];
        val.dataInfo->oper->printdata(val.data, val.content);
    }
    printf("]");
}

