#define DATA_M_OPER

#include "chainstack_mdata.h"
#include "chainstack_mdata_private.h"

#include "../../../List/SList/Multiple_Data/slist_mdata.h"
#include "../../../List/SList/Multiple_Data/slist_mdata_private.h"


/*

    复用链表的栈使用头节点作为栈顶

    使用链表的栈没有擦除函数
    The stack using a linked list does not have a erase function

*/




InfoOfReturn initMChainStack(ChainStack_M* pStack, bool isUnlimited, int capacity) {
    if (capacity <= 0) {
        //数量异常
        return Warning;
    }
    pStack->isUnlimited = isUnlimited;
    pStack->capacity = capacity;
    pStack->size = 0;
    initMSList(&pStack->list);
    return Success;
}

bool isFullMChainStack(ChainStack_M* pStack) {
    return pStack->capacity == pStack->size;
}

bool isEmptyMChainStack(ChainStack_M* pStack) {
    return pStack->size == 0;
}

bool hasMValInMChainStack(ChainStack_M* pStack, Data_M val) {
    return hasMValInMSList(&(pStack->list), val);
}

InfoOfReturn pushMValInMChainStack(ChainStack_M* pStack, Data_M val, selectOfCopy isCopyVal) {
    if (!pStack->isUnlimited && isFullMChainStack(pStack)) {
        //不是无限大小且满了
        return Warning;
    }
    if (insertMValAtStartInMSList(&(pStack->list), val, isCopyVal) == Warning) {
        //头插失败
        return Warning;
    }
    
    pStack->size++;
    if (pStack->isUnlimited && pStack->size > pStack->capacity) {
        //无限容量模式下，同步更新容量上限
        pStack->capacity = pStack->size;
    }
    
    return Success;
}

Data_M popMValInMChainStack(ChainStack_M* pStack) {
    if (isEmptyMChainStack(pStack)) {
        return getEmptyMData();
    }
    //此时一定不会是空指针
    Data_M* pVal = getPtrMValByPosInMSList(&(pStack->list), 0);

    Data_M val = *pVal;

    
    //强行改为无权释放, 防止数据丢失
    setMDataToNoOwner(pVal);

    delStartNodeInMSList(&(pStack->list));

    pStack->size--;
    
    return val;

}

void freeMValInMChainStack(Data_M* val) {

    freeMData(val);
}


void freeMChainStack(ChainStack_M* pStack) {
    freeMSList(&(pStack->list));
    pStack->capacity = 0;
    pStack->isUnlimited = false;
    pStack->size = 0;
    initMSList(&(pStack->list));
}


////////////////////////////////////////////////////////////
//以下是不公开函数, 用于调试, 存于private头文件中


void printMValInMChainStack(Data_M val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    val.dataInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printMChainStack(ChainStack_M* pStack) {
    printf("bottom");
    printMSList(&(pStack->list));
    printf("top");
}


