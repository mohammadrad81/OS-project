#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define PROCESS_COUNT 32
#define ARRAY_SIZE 10000

int calculate_sum(int *arr, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += arr[i];
    }
    return sum;
}

void job(){
    int arr_1[ARRAY_SIZE] = {0};
    int arr_2[ARRAY_SIZE] = {0};
    calculate_sum(arr_1, ARRAY_SIZE);
    calculate_sum(arr_2, ARRAY_SIZE);

}

void runProcesses(char* algorithm_name){
    int fds[PROCESS_COUNT][2];
    int turnaround_times[PROCESS_COUNT];
    int waiting_times[PROCESS_COUNT];
    int ready_times[PROCESS_COUNT];
    int running_times[PROCESS_COUNT];

    for(int i = 0; i < PROCESS_COUNT; i++){
        pipe(fds[i]);
        int child_pid = fork();
        if(child_pid == 0){ //child
            close(fds[i][0]); //close read for child
            int my_pid = getpid();
            int pid = getpid();
            int turn_around_time = getProcTick(pid);
            int waiting_time = getProcWaitingTicks(pid);
            int ready_time = getProcReadyTicks(pid);
            int running_time = getProcRunningTicks(pid);
            printf("process with pid: %d, turnaround time: %d\n", my_pid, turn_around_time);
            printf("process with pid: %d, waiting time: %d\n", my_pid, waiting_time);
            printf("process with pid: %d, ready time: %d\n", my_pid, ready_time);
            printf("process with pid: %d, running time: %d\n", my_pid, running_time);
            write(fds[i][1], &turn_around_time, sizeof(turn_around_time));
            write(fds[i][1], &waiting_time, sizeof(waiting_time));
            write(fds[i][1], &ready_time, sizeof(ready_time));
            write(fds[i][1], &running_time, sizeof(running_time));
            close(fds[i][1]);
            exit(0);
        }
        if(child_pid != 0){//parent
            close(fds[i][1]);//parent does not write messages to children
        }
    }
    int status = 0;
    for(int i = 0; i < PROCESS_COUNT; i++){
        int turnaround_t;
        int wait_t;
        int ready_t;
        int running_t;
        read(fds[i][0], &turnaround_t, sizeof(turnaround_t));
        read(fds[i][0], &wait_t, sizeof(wait_t));
        read(fds[i][0], &ready_t, sizeof(ready_t));
        read(fds[i][0], &running_t, sizeof(running_t));
        close(fds[i][0]);
        turnaround_times[i] = turnaround_t;
        waiting_times[i] = wait_t;
        ready_times[i] = ready_t;
        running_times[i] = running_t;
        wait(&status);
    }
    double ave_turnaround_time = ((double)calculate_sum(turnaround_times, PROCESS_COUNT)) / PROCESS_COUNT;
    double ave_waiting_time = ((double)calculate_sum(waiting_times, PROCESS_COUNT)) / PROCESS_COUNT;
    double ave_ready_time = ((double)calculate_sum(ready_times, PROCESS_COUNT)) / PROCESS_COUNT;
    double ave_running_time = ((double)calculate_sum(running_times, PROCESS_COUNT)) / PROCESS_COUNT;
    
    printf("TEST RESULT FOR ALGORITHM: %s:\n", algorithm_name);
    printf("average turnaround time: %f\n", ave_turnaround_time);
    printf("average waiting time: %f\n", ave_waiting_time);
    printf("average ready time: %f\n", ave_ready_time);
    printf("average running time: %f\n", ave_running_time);

}

int main() {

    sched_RR();
    runProcesses("RR");
    sched_FCFS();
    runProcesses("FCFS");
    exit(0);
}

