#include "base.h"



//判断是否为质数
static bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n%2 == 0 || n%3 == 0) return false;
    for (int i = 5; i <= n/i; i+=6) {
        if (n%i == 0 || n%(i+2) == 0) return false;
    }
    return true;
}




int getLargestPrime(int n) {
    if (n <= 2) return 2;
    if (n%2 == 0) n--;
    for (int i = n; ; i-=2) {
        if (isPrime(i)) return i;
    }
    return 2;
}


Data_S getEmptySData() {
    Data_S empty;
    empty.content = empty.data = NULL;
    //空Data表示一种错误状态, 除了isEmpty的值要设置为true, 其他的值即便随便
    empty.isEmpty = true;
    empty.isOwner = false;
    return empty;
}

Data_M getEmptyMData() {
    Data_M empty;
    empty.content = empty.data = NULL;
    empty.dataInfo = NULL;
    //空Data表示一种错误状态, 除了isEmpty的值要设置为true, 其他的值即便随便
    empty.isEmpty = true;
    empty.type = NOT_FOUND;
    empty.isOwner = false;
    return empty;
}




CmpResult compareSData(Data_S Data_a, InfoOfData* info_a, Data_S Data_b, InfoOfData* info_b) {
    //只要有一个为空, 直接不同
    if (Data_a.isEmpty || Data_b.isEmpty) {
        return DIFFERENT;
    }

    //也是一样
    if (info_a == NULL || info_b == NULL) {
        return DIFFERENT;
    }
    //信息不同
    if (info_a != info_b) {
        return DIFFERENT;
    }

    //到这一步, 说明是同一种类型
    _cmpdata cmp = info_a->oper->cmpdata;
    return cmp(Data_a.data, Data_a.content, Data_b.data, Data_b.content);


}

CmpResult compareMData(Data_M Data_a, Data_M Data_b) {
    if (Data_a.isEmpty || Data_b.isEmpty) {
        return DIFFERENT;
    }

    if (Data_a.type != Data_b.type) {
        return DIFFERENT;
    }
    if (Data_a.dataInfo != Data_b.dataInfo) {
        //类型相同但数据自带的操作函数及信息不同,说明有问题
        printf("\nType is the same but operation is different! Please check!\n");
        return DIFFERENT;
    }


    _cmpdata cmp;   //能走到这一步,说明二者的比较函数相同
    cmp = Data_a.dataInfo->oper->cmpdata;
    return cmp(Data_a.data, Data_a.content, Data_b.data, Data_b.content);


}

Data_M copyMData(Data_M inputData) {
    if (inputData.isEmpty) {
        return getEmptyMData();
    }
    Data_M newData;

    //copy函数不仅仅只是把指针赋值,还要把整个void* data赋值一遍
    newData.data = inputData.dataInfo->oper->copydata(inputData.data, inputData.content);

    if (newData.data == NULL) {
        printf("\nMemory allocation failed\n");
        return getEmptyMData();
    }
    //有content才复制content
    if (inputData.dataInfo->hasContent) {
        newData.content = inputData.dataInfo->oper->copycontent(inputData.content);
        if (newData.content == NULL) {
            printf("\nMemory allocation failed\n");
            inputData.dataInfo->oper->freedata(newData.data, inputData.content);
            return getEmptyMData();
        }
    } else {
        newData.content = NULL;
    }
    
    /*默认对数据具有权限*/
    newData.isOwner = true; 
    
    //提供的相应操作函数因该是全局的
    newData.dataInfo = inputData.dataInfo;
    newData.type = inputData.type;
    newData.isEmpty = false;
    return newData;
}


Data_S copySData(Data_S inputData, InfoOfData* info) {
    if (inputData.isEmpty) {
        return getEmptySData();
    }

    Data_S newData;
    newData.data = info->oper->copydata(inputData.data, inputData.content);
    if (newData.data == NULL) {
        printf("\nMemory allocation failed\n");
        return getEmptySData();
    }

    //有content才进行复制
    if (info->hasContent) {
        newData.content = info->oper->copycontent(inputData.content);
        if (newData.content == NULL) {
            printf("\nMemory allocation failed\n");
            info->oper->freedata(newData.data, inputData.content);
            return getEmptySData();
        }
    } else {
        newData.content = NULL;
    }
    
    newData.isOwner = true;
    newData.isEmpty = false;
    return newData;
}




void freeSData(Data_S* inputData, InfoOfData* info) {
    if (inputData->isEmpty) return;

    if (inputData->isOwner) {
        //只有数据的所有权是自己的, 才进行释放
        info->oper->freedata(inputData->data, inputData->content);
        if (info->hasContent) {
            info->oper->freecontent(inputData->content);
        }
    }
    //数据释放为空
    *inputData = getEmptySData();
}


void freeMData(Data_M* inputData) {
    //为空不释放
    if (inputData->isEmpty) return;

    if (inputData->isOwner) {
        //data的释放
        inputData->dataInfo->oper->freedata(inputData->data, inputData->content);
    
        //有content时才释放
        if (inputData->dataInfo->hasContent) {
            inputData->dataInfo->oper->freecontent(inputData->content);
        }
    }


    //数据释放为空
    *inputData = getEmptyMData();
}


void setSDataToOwner(Data_S* inputData) {
    inputData->isOwner = true;
}

void setSDataToNoOwner(Data_S* inputData) {
    inputData->isOwner = false;
}

void setMDataToOwner(Data_M* inputData) {
    inputData->isOwner = true;
}
void setMDataToNoOwner(Data_M* inputData) {
    inputData->isOwner = false;
}




