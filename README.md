# FocX

- 注: 
    1. M(Multiple)为一种数据结构中可以插入任意类型, S(Single)为一种数据结构中只能插入一种类型, 但这种类型任意  
    2. 再所有的数据结构中Key为键, Val为值, 如果不可以重复即为键, 否则值
## 🚀快速开始

- 代码
```c
//下载库, 引入(按需引入, 这里方便直接复制)
#include "List\DList\Multiple_Data\dlist_mdata.h"
#include "List\DList\Single_Data\dlist_sdata.h"
#include "Map\ChainMap\Multiple_Data\chainmap_mdata.h"
#include "Map\ChainMap\Single_Data\chainmap_sdata.h"
#include "Map\OAMap\Multiple_Data\oamap_mdata.h"
#include "Map\OAMap\Single_Data\oamap_sdata.h"  //此代码使用
#include "Oper\Bool_Info\bool_info.h"
#include "Oper\Double_Info\double_info.h"
#include "Oper\Int_Info\int_info.h" //此代码使用
#include "Oper\String_Info\string_info.h"   //此代码使用
#include "base.h"
int main()
{
    OAMap_S map;    //创建一个使用开放寻址法(OA)的存储单一类型数据(S)的Map
    initSOAMap(&map, &Info_Int, &Info_String);  //初始化Map, 使用库提供的Int和String信息
    char string[101] = "abcd";
    for (int i = 0; i < 10; i++) {
        //插入Key和Val, 使用宏进行将数据的指针转为不同的Data_S类型, REF为数据借用, OWN为数据独有
        insertSkeyAndSValInSOAMap(&map, Data_S_OWN(&i, NULL), Data_S_REF(string, NULL));
    }

    //打印Map
    printSOAMap(&map);
    
    //所有的val都是借用的, 如果改了借用的, 会发生什么呢?
    string[2] = 'z';
    printf("\n");
    
    //再次打印
    printSOAMap(&map);

    //用完后记得释放
    freeSOAMap(&map);
}


```

- 运行结果
```txt
[[key:0, value:"abcd"], [key:1, value:"abcd"], [key:2, value:"abcd"], [key:3, value:"abcd"], [key:4, value:"abcd"], [key:5, value:"abcd"], [key:6, value:"abcd"], [key:7, value:"abcd"], [key:8, value:"abcd"], [key:9, value:"abcd"]]                                 
[[key:0, value:"abzd"], [key:1, value:"abzd"], [key:2, value:"abzd"], [key:3, value:"abzd"], [key:4, value:"abzd"], [key:5, value:"abzd"], [key:6, value:"abzd"], [key:7, value:"abzd"], [key:8, value:"abzd"], [key:9, value:"abzd"]]
```


## 1 设计理念
1.基于Data_S和Data_M进行泛型设计, 结构体内容如下  
```c

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


/// @brief 按自己的方式释放content
typedef void (*_freecontent)(void* content);


/// @brief 通过按自己的方式解析content内容,然后创建一个完全一样的
typedef void* (*_copycontent)(void* content);


/// @brief 创建的这种类型的变量是不允许删除的(把他设置为全局变量),它代表的是某一种类型的相关操作函数
typedef struct Operation {
    //分别为上面的函数类型, 自定义数据的时候需要用到
    _freedata freedata;
    _hashdata hashdata;
    _cmpdata cmpdata;
    _copydata copydata;
    _printdata printdata;
    _copycontent copycontent;
    _freecontent freecontent;
} Operation;


typedef struct InfoOfData {
    Operation* oper;    //操作函数集合指针
    bool hasContent;    //当前数据是否有content内容
} InfoOfData;


typedef struct Data_S {
    void* data; //存储数据的void* 指针
    void* content;  //存储描述性信息的void* 指针(比如描述二维数组的col和row)
    bool isEmpty;   //Data_S是否为空
    bool isOwner;   //数据是否为Data独有, 如果true, 代表数据的生死由Data管理(会复制一份, 以保证是Data中的内容是独有的), 如果false, 代表数据由外部管理, Data只管理指针
} Data_S;



typedef struct Data_M {
    void* data; //存储数据的void* 指针
    void* content;  //存储描述性信息的void* 指针(比如描述二维数组的col和row)
    InfoOfData* dataInfo;   //由于要存储多种类型数据, Data_M自带InfoOfData类型数据指针(Data_S由于存储的是单一类型(比如一个DList_S中只存int), 那完全可以让DList_S结构体来存储InfoOfData, 以达到减少内存的目的)
    int type;   //数据标签(用于区分, 但不是主要区分标志)
    bool isEmpty;   //Data_M是否为空

    bool isOwner;   //数据是否为Data独有, 如果true, 代表数据的生死由Data管理, 如果false, 代表数据由外部管理, Data只管理指针, 没有释放的权力
} Data_M;

```
- 如何使用数据的所有权(宏)(还有一点, 在Map中Key一定是独有的, 即便传入借用的, 也会自动转为独有)
- 如何使用见上面"快速开始"中的代码

```c


//OWN, 数据是自己的, 可以在释放的时候进行释放 data: 数据指针, content: 描述性信息指针, dataInfo: InfoOfData类型指针, type: 数据标签
#define Data_M_OWN(data, content, dataInfo, type) ((Data_M){(data), (content), (dataInfo), (type), false, true})


//REF, 数据不是自己的, 只是传个指针, 释放的时候不会释放数据 data: 数据指针, content: 描述性信息指针, dataInfo: InfoOfData类型指针, type: 数据标签
#define Data_M_REF(data, content, dataInfo, type) ((Data_M){(data), (content), (dataInfo), (type), false, false})


//OWN, 数据是自己的, 可以在释放的时候进行释放, data: 数据指针, content: 描述性信息指针
#define Data_S_OWN(data, content) ((Data_S){(data), (content), false, true})
//REF, 数据不是自己的, 只是传个指针, 释放的时候不会释放数据, data: 数据指针, content: 描述性信息指针
#define Data_S_REF(data, content) ((Data_S){(data), (content), false, false})

```


<!-- - [详细内容以及函数的使用](https://flmpx.github.io/focx/) -->



<!-- ## 注意事项


- 在List中的数据叫做MData或者SData
- 在Map中的数据叫做MVal, MKey, SVal, SKey, MEntry, SEntry
- 具体什么类型, 在括号中解释 -->
## 2 文件夹结构
    ├── base.c
    ├── base.h
    ├── List
    │   ├── DList
    │   │   ├── Multiple_Data
    │   │   │   ├── dlist_mdata.c
    │   │   │   └── dlist_mdata.h
    │   │   └── Single_Data
    │   │       ├── dlist_sdata.c
    │   │       └── dlist_sdata.h
    │   └── SList
    │       ├── Multiple_Data
    │       │   ├── slist_mdata.c
    │       │   └── slist_mdata.h
    │       └── Single_Data
    │           ├── slist_sdata.c
    │           └── slist_sdata.h
    ├── Map
    │   ├── ChainMap
    │   │   ├── Multiple_Data
    │   │   │   ├── chainmap_mdata.c
    │   │   │   └── chainmap_mdata.h
    │   │   └── Single_Data
    │   │       ├── chainmap_sdata.c
    │   │       └── chainmap_sdata.h
    │   └── OAMap
    │       ├── Multiple_Data
    │       │   ├── oamap_mdata.c
    │       │   └── oamap_mdata.h
    │       └── Single_Data
    │           ├── oamap_sdata.c
    │           └── oamap_sdata.h
    ├── Oper
    │   ├── Bool_Info
    │   │   ├── bool_info.c
    │   │   └── bool_info.h
    │   ├── Double_Info
    │   │   ├── double_info.c
    │   │   └── double_info.h
    │   ├── Int_Info
    │   │   ├── int_info.c
    │   │   └── int_info.h
    │   ├── README.md
    │   └── String_Info
    │       ├── string_info.c
    │       └── string_info.h
    └── Set
        ├── ChainSet
        │   ├── Multiple_Data
        │   │   ├── chainset_mdata.c
        │   │   └── chainset_mdata.h
        │   └── Single_Data
        │       ├── chainset_sdata.c
        │       └── chainset_sdata.h
        └── OASet
            ├── Multiple_Data
            │   ├── oaset_mdata.c
            │   └── oaset_mdata.h
            └── Single_Data
                ├── oaset_sdata.c
                └── oaset_sdata.h

            
## 3 变量命名规则
<strong style = "color: red">首先注意一下, 函数中的变量命名(如SSList)的第一个字母(S)一定代表数据类型是单一的函数任意的</strong>

    1. 变量名开头一定是本质(如:ChainMap代表使用链表来处理冲突的Map, List代表链表)
    
    2. 变量名后下划线后代表当前数据结构内部使用的是S(任意类型)还是M(单一类型)
    
    3. 数据类型下划线后代表的是其他属性, 比如位置inList(在链表里面), inChainMap(在用链表处理冲突的Map里面)
- 一个函数中的变量不需要严格遵循这个规则, 但要可阅读性强
## 变量内容
- 注: 不可见变量的命名规则一致, 不介绍  
### 基础
    
    1. InfoOfData-->用于存储的变量的操作函数和内容(全局静态变量)
    
    2. Data_S-->存储的单一类型的数据(不自带InfoOfData, 由它的上一级存储它的InfoOfData以节省内存, 比如List_S中的Data_S的InfoOfData就是存在List_S结构体中的)
    
    3. Data_M-->存储任意类型的数据(自带InfoOfData)
    4. Operation-->当前数据的操作函数, 具体见自定义数据的使用方法

### List
#### 1 DList
    
    1. DList_M-->存储任意类型数据的DList
    
    2. DList_S-->存储单一类型数据的DList
### 2 SList

    1. SList_M-->存储任意类型数据的SList

    2. SList_S-->存储单一类型数据的DList

### Map
#### 1 OAMap
    
    1. OAMap_M-->存储任意类型数据的OAMap
    
    2. OAMap_S-->存储单一类型数据的OAMap  
    
    3. Entry_M_inOAMap-->在OAMap中存储任意类型数据的Entry(即包含key和val)  
    
    4. Entry_S_inOAMap-->在OAMap中存储单一类型数据的Entry(即包含key和val)

#### 2 ChainMap
    
    1. ChainMap_M-->存储任意类型数据的ChainMap
    
    2. ChainMap_S-->存储单一类型数据的ChainMap
    
    3. Entry_M_inChainMap-->在ChainMap中的存储任意类型数据的Entry(即包含key和val)
    
    4. Entry_S_inChainMap-->在ChainMap中的存储单一类型数据的Entry(即包含key和val)
    
### Set
#### 1 OASet
    
    1. OASet_M-->存储任意类型数据的OASet
    
    2. OASet_S-->存储单一类型数据的OASet  
    

#### 2 ChainSet
    
    1. ChainSet_M-->存储任意类型数据的ChainSet
    
    2. ChainSet_S-->存储单一类型数据的ChainSet

- 再Set内部有Entry这个概念, 但在外部没有
    
    
### 其他
    1. CmpResult-->数据比较结果枚举类型, 里面右两个值  
        1. SAME-->相同 
        2. DIFFERENT-->不同
    2. InfoOfReturn-->返回信息, 有三个值 
        1. Warning-->操作失败(比如插入数据时内存分配失败, 要删除空链表中的数据) 
        2. None-->操作无效(比如上传链表中的元素, 但没找, 那么删除就无效) 
        3. Success-->操作成功操作(比如链表插入元素成功操作)
    3. Position-->位置index(比如在开放寻址法处理冲突的Map中, 通过返回位置下标来找到数据)
    
    



## 函数命名规则
    1. 函数名字内的变量遵循"变量属性_变量本质"的命名方式, 比如getSValBySKeyInSChainMap表示在存储单一类型数据的ChianMap中通过单一数据类型的Key来返回SVal
    
    2. get-->得到
    
    3. Copy/Ptr-->这个数据是复制一份还是传一个指针
    
    4. in-->在哪种数据结构中
    
    5. free-->释放
    
    6. print-->打印
    
    7. has-->有没有
    
    8. insert-->插入
    
    9. del-->删除
    
    10. reverse-->反转(一般是链表)

## 🤔如何创建自己的Info_Self呢?
