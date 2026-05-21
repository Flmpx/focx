#include <stdio.h>
#include "../../../../Stack/ArrStack/Single_Data/arrstack_sdata.h"
#include "../../../../Stack/ArrStack/Single_Data/arrstack_sdata_private.h"
#include "../../../../base.h"
#include "../../../../Oper/Int_Info/int_info.h"



int main () {
    ArrStack_S stack;
    initSArrStack(&stack, &Info_Int, false, 24);
    for (int i = 0; i < 30; i++) {
        if (pushSValInSArrStack(&stack, Data_S_OWN(&i, NULL), Data_Copy) == Warning) {
            printf("栈满!\n");
        }
    }
    printSArrStack(&stack);
    freeSArrStack(&stack);
}