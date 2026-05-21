#define DATA_S_OPER

#include "chainstack_sdata.h"
#include "chainstack_sdata_private.h"


#include "../../../List/SList/Single_Data/slist_sdata.h"
#include "../../../List/SList/Single_Data/slist_sdata_private.h"


/*

    复用链表的栈使用头节点作为栈顶

    使用链表的栈没有擦除函数
    The stack using a linked list does not have a erase function

*/

InfoOfReturn initSChainStack(ChainStack_S* pStack, InfoOfData* valInfo, bool isUnlimited, int capacity) {
    if (capacity <= 0) {
        //数量异常
        return Warning;
    }
    pStack->isUnlimited = isUnlimited;
    pStack->capacity = capacity;
    pStack->size = 0;
    pStack->valInfo = valInfo;
    initSSList(&(pStack->list), pStack->valInfo);
    return Success;
}

bool isFullSChainStack(ChainStack_S* pStack) {
    return pStack->capacity == pStack->size;
}

bool isEmptySChainStack(ChainStack_S* pStack) {
    return pStack->size == 0;
}

bool hasSValInSChainStack(ChainStack_S* pStack, Data_S val) {
    return hasSValInSSList(&(pStack->list), val);
}


InfoOfReturn pushSValInSChainStack(ChainStack_S* pStack, Data_S val, selectOfCopy isCopyVal) {
    if (!pStack->isUnlimited && isFullSChainStack(pStack)) {
        //不是无限大小且满了
        return Warning;
    }
    if (insertSValAtStartInSSList(&(pStack->list), val, isCopyVal) == Warning) {
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


Data_S popSValInSChainStack(ChainStack_S* pStack) {
    if (isEmptySChainStack(pStack)) {
        return getEmptySData();
    }
    //此时一定不会是空指针
    Data_S* pVal = getPtrSValByPosInSSList(&(pStack->list), 0);

    Data_S val = *pVal;

    
    //强行改为无权释放, 防止数据丢失
    setSDataToNoOwner(pVal);

    delStartNodeInSSList(&(pStack->list));

    pStack->size--;
    
    return val;
}

void freeSValInSChainStack(ChainStack_S* pStack, Data_S* val) {

    freeSData(val, pStack->valInfo);
}

void freeSChainStack(ChainStack_S* pStack) {
    freeSSList(&(pStack->list));
    pStack->capacity = 0;
    pStack->isUnlimited = false;
    pStack->size = 0;
    //InfoData无需改动
    initSSList(&(pStack->list), pStack->valInfo);

}

////////////////////////////////////////////////////////////
//以下是不公开函数, 用于调试, 存于private头文件中

void printSValInSChainStack(ChainStack_S* pStack, Data_S val) {
    if (val.isEmpty) {
        printf("\nval is empty, cannot print\n");
        return;
    }
    printf("[val:");
    pStack->valInfo->oper->printdata(val.data, val.content);
    printf("]");
}

void printSChainStack(ChainStack_S* pStack) {
    printf("bottom");
    printSSList(&(pStack->list));
    printf("top");
}

