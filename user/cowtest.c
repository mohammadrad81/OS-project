#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int ALLOCATION = (int) ((2/3) * 128 * 1024 * 1024);
    char *ptr = sbrk(ALLOCATION);
    if((uint64)ptr != -1) {
        printf("2/3 of main memory is allocated to current process.\n");
    } else {
        printf("memory allocation failed\n");
        return -1;
    }
    for(int i = 0; i < ALLOCATION; i++){
        ptr[i] = '1';
    }
    int pid;
    pid = fork();
    if (pid > 0) {
        printf("forked successfuly, this is parent process, pid: %d.\n", getpid());
    } else if (pid == 0) {
        printf("forked successfuly, this is child process, pid: %d.\n", getpid());
    } else {
        printf("error occured, couldn't fork!\n");
        return -1;
    }
    return 0;
}
