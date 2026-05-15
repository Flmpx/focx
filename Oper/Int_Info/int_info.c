#include <stdio.h>
#include <stdlib.h>
#include "int_info.h"

static void freedata_Int(void* data, void* content) {
    free(data);
}

static ll hashdata_Int(void* data, void* content) {
    int n = *(int*)data;
    return (ll)n;
}

static CmpResult cmpdata_Int(void* data_a, void* content_a, void* data_b, void* content_b) {
    return *(int*)data_a == *(int*)data_b ? SAME : DIFFERENT;
}


static void* copydata_Int(void* data, void* content) {
    int* newdata = (int*)malloc(sizeof(int));
    *newdata = *(int*)data;
    return newdata;
}

static void printdata_Int(void* data, void* content) {
    printf("%d", *(int*)data);
}

static Operation oper_Int = {
    freedata_Int,
    hashdata_Int,
    cmpdata_Int,
    copydata_Int,
    printdata_Int,
    NULL,
    NULL,
};


InfoOfData Info_Int = {
    &oper_Int,
    false
};
