//
// Created by mohammad on 3/29/23.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]){
    int pid = atoi(argv[1]);
    printf("xv6 process with id = %d ticks: %d\n", pid, getProcTick(pid));
    exit(0);
}