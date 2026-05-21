cmake_minimum_required(VERSION 4.2.1)

project(test_arrstack_s)

add_executable(test ../../../../Stack/ArrStack/Single_Data/arrstack_sdata.c ../../../../base.c test.c ../../../../Oper/Int_Info/int_info.c)

