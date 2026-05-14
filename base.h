#ifndef BASE_H
#define BASE_H
#include <stdbool.h>
#include <stdio.h>
#define NOT_FOUND -1
typedef unsigned long long ull;
typedef int Position;

#ifdef ENTRY_STATE_IN_OAMAP

enum EntryStateInMap {
    DEL_IN_MAP = -1,
    NONE_IN_MAP = 0,
    EXIST_IN_MAP = 1
};

#endif

#ifdef ENTRY_STATE_IN_OASET

enum EntryStateInSet {
    DEL_IN_SET = -1,
    NONE_IN_SET = 0,
    EXIST_IN_SET = 1
};


#endif



/// @brief 数据是否需要复制
typedef enum {
    Data_NoCopy = 0,    //不复制数据
    Data_Copy = 1   //复制数据
} selectOfCopy;









//注:Data和Entry的空不能作为有效的内容,只是为了在出现问题时返回空

/*
使用方法:
    4 content内容一般为结构体,比如二维动态数组content的应该为,注意要是指针
                        typedef struct {
                            int row;
                            int col;
                        } MetriArray;





*/


/// @brief 数据比较结果
typedef enum cmpresult {
    SAME = 0,
    DIFFERENT = 1,
} CmpResult;


/// @brief 操作结果状态码
typedef enum info {
    Warning = -1,
    None = 0,
    Success = 1
} InfoOfReturn;

//以下函数都需要自己提供

/// @brief 释放void* data
typedef void (*_freedata)(void* data, void* content);

/// @brief 对void* data的内容进行hash的函数(hash函数必须返回ull类型的数据)
typedef ull (*_hashdata)(void* data, void* content);

/// @brief 对void* data进行比较的函数
typedef CmpResult (*_cmpdata)(void* data_a, void* content_a, void* data_b, void* content_b);

/// @brief 对void* data进行复制的函数
typedef void* (*_copydata)(void* data, void* content);

/// @brief 对void* data进行输出的函数
typedef void (*_printdata)(void* data, void* content);


/// @brief 释放void* content
typedef void (*_freecontent)(void* content);


/// @brief 复制void* content
typedef void* (*_copycontent)(void* content);


/// @brief 这是一类数据的操作函数
typedef struct Operation {
    _freedata freedata;
    _hashdata hashdata;
    _cmpdata cmpdata;
    _copydata copydata;
    _printdata printdata;
    _copycontent copycontent;
    _freecontent freecontent;
} Operation;


/// @brief 将操作函数和是否右描述信息结合成的当前数据的信息
typedef struct InfoOfData {
    Operation* oper;
    bool hasContent;
} InfoOfData;


/// @brief 只能容纳一种数据的Data类型, 操作函数会由List或者Map提供
typedef struct Data_S {
    void* data;
    void* content;
    bool isEmpty;
    bool isOwner;
} Data_S;


/// @brief 可容纳多种数据的Data类型, 操作函数自带
typedef struct Data_M {
    void* data;
    void* content;
    InfoOfData* dataInfo;
    int type;
    bool isEmpty;
    bool isOwner;   //是否管理权, 一般是指可以释放当前的数据(void* content和void* content)吗
} Data_M;

//OWN, 有权释放数据, 有数据的所有权 data: 数据指针, content: 描述性信息指针, dataInfo: InfoOfData类型指针, type: 数据标签
#define Data_M_OWN(data, content, dataInfo, type) ((Data_M){(data), (content), (dataInfo), (type), false, true})


//REF, 只是接管指针, 引用数据, 无权释放数据 data: 数据指针, content: 描述性信息指针, dataInfo: InfoOfData类型指针, type: 数据标签
#define Data_M_REF(data, content, dataInfo, type) ((Data_M){(data), (content), (dataInfo), (type), false, false})


//OWN, 有权释放数据, 有数据的所有权 data: 数据指针, content: 描述性信息指针
#define Data_S_OWN(data, content) ((Data_S){(data), (content), false, true})


//REF, 只是接管指针, 引用数据, 无权释放数据 data: 数据指针, content: 描述性信息指针
#define Data_S_REF(data, content) ((Data_S){(data), (content), false, false})



#ifdef GET_LARGESTPRIME


/// @brief 返回小于等于n的最大质数
/// @param n n
/// @return 最大质数
extern int getLargestPrime(int n);


#endif

//直接提供Data的复制, 比较, 空数据的获得

#ifdef DATA_S_OPER

/////////////////////////////
//空数据的获得

/// @brief 得到空数据Data_S
/// @return 空数据Data_S
extern Data_S getEmptySData();

//////////////////////////////
//数据的比较

/// @brief 比较两个Data_S是否相同
/// @param Data_a Data_S类型数据1
/// @param info_a InfoOfData类型数据指针1
/// @param Data_b Data_S类型数据2
/// @param info_b InfoofData类型数据指针2
/// @return 比较结果, 使用SAME和DIFFERENT来进行判断
extern CmpResult compareSData(Data_S Data_a, InfoOfData* info_a, Data_S Data_b, InfoOfData* info_b);

///////////////////////////////////////
//数据的复制

/// @brief 复制Data_S的全部内容, 需要提供InfoOfData类型数据指针以便进行复制
/// @param inputData 输入的Data_S类型数据
/// @param info InfoOfData类型数据指针
/// @return 复制好的Data_S类型数据
/// @note 默认对数据具有控制权
extern Data_S copySData(Data_S inputData, InfoOfData* info);



///////////////////////////////////
//数据的释放

/// @brief 释放掉Data_S数据
/// @param inputData 输入的Data_S类型数据
/// @param info InfoOfData类型数据指针
extern void freeSData(Data_S* inputData, InfoOfData* info);

#endif

#ifdef DATA_M_OPER
/////////////////////////////
//空数据的获得

/// @brief 得到空数据Data_M
/// @return 空数据Data_M
extern Data_M getEmptyMData();

//////////////////////////////
//数据的比较

/// @brief 比较两个Data_M是否相同
/// @param Data_a Data_M类型数据1
/// @param Data_b Data_M类型数据2
/// @return 比较结果, 使用SAME和DIFFERENT来进行判断
extern CmpResult compareMData(Data_M Data_a, Data_M Data_b);

///////////////////////////////////////
//数据的复制



/// @brief 复制Data_M的全部内容, 包括void* data和void* content
/// @param inputData 输入的Data_M类型数据
/// @return 复制好的Data_M类型数据
/// @note 默认对数据具有控制权
extern Data_M copyMData(Data_M inputData);



///////////////////////////////////
//数据的释放

/// @brief 释放掉Data_M数据
/// @param inputData 输入的Data_M类型数据
extern void freeMData(Data_M* inputData);

#endif




#endif