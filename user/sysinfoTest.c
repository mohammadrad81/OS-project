//
// Created by mohammad on 4/7/23.
//
#include "kernel/sysInfo.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    struct sysInfo info;
    struct sysInfo *info_ptr = & info;
    uint64 info_addr = (uint64) info_ptr;
    sysinfo(info_addr);
    printf("sysinfo:\nuptime: %d\nfreeram: %d\ntotalram: %d\nprocs: %d\n", info.uptime, info.freeram, info.totalram, info.procs);
    return 0;
}