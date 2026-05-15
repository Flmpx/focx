#include <stdio.h>
#include "string_info.h"
#include <string.h>
#include <stdlib.h>

static ll mod = 1000000007;
static ll base = 131;

static void freedata_String(void* data, void* content) {
    free(data);
}


static ll hashdata_String(void* data, void* content) {
    const char* string = (char*)data;
    ll result = 0;
    int len = strlen(string);
    for (int i = 0; i < len; i++) {
        result = (result * base % mod + string[i] + 199)%mod;
    }
    return result;
}

static CmpResult cmpdata_String(void* data_a, void* content_a, void* data_b, void* content_b) {
    const char* string_a = (char*)data_a;
    const char* string_b = (char*)data_b;
    return strcmp(string_a, string_b);
}

static void* copydata_String(void* data, void* content) {
    const char* string = (char*)data;
    int len = strlen(string);
    char* newString = (char*)malloc((len+1)*sizeof(char));
    strcpy(newString, string);
    return newString;
}

static void printdata_String(void* data, void* content) {
    printf("\"%s\"", (char*)data);
}

static Operation oper_String = {
    freedata_String,
    hashdata_String,
    cmpdata_String,
    copydata_String,
    printdata_String,
    NULL,
    NULL
};


InfoOfData Info_String = {
    &oper_String,
    false
};


