#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "double_info.h"

static const char* format_Double = "double%.12edouble"; 


static const ll mod = 1000000007;
static const ll base = 131;

static void freedata_Double(void* data, void* content) {
    free(data);
}


static ll hashdata_Double(void* data, void* content) {
    char string[101];
    sprintf(string, format_Double, *(double*)data);
    int len = strlen(string);
    ll result = 0;
    for (int i = 0; i < len; i++) {
        result = (result * base % mod + string[i] + 199)%mod;
    }
    return result;
}

static CmpResult cmpdata_Double(void* data_a, void* content_a, void* data_b, void* content_b) {
    char string_a[101], string_b[101];
    sprintf(string_a, format_Double, *(double*)data_a);
    sprintf(string_b, format_Double, *(double*)data_b);
    return strcmp(string_a, string_b);
}


static void* copydata_Double(void* data, void* content) {
    double* newDouble = (double*)malloc(sizeof(double));
    memcpy(newDouble, (double*)data, sizeof(double));
    return newDouble;
}

static void printdata_Double(void* data, void* content) {
    double num = *(double*)data;
    printf("%g", num);
}

static Operation oper_Double = {
    freedata_Double,
    hashdata_Double,
    cmpdata_Double,
    copydata_Double,
    printdata_Double,
    NULL,
    NULL
};




InfoOfData Info_Double = {
    &oper_Double,
    false
};