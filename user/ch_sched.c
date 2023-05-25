//
// Created by mohammad on 3/29/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int result = 0;
    if(atoi(argv[1]) == 0){
        printf("changing scheduling algorithm to RR\n");
        result = sched_RR();
    }
    else if(atoi(argv[1]) == 1){
        printf("changing scheduling algorithm to FCFS\n");
        result = sched_FCFS();
    }
    if(result == 0){
        printf("changed successfuly!\n");
    }
    else{
        printf("error :(\n");
    }
    exit(0);
}