#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define PROCESS_COUNT 32
#define ARRAY_SIZE 1000

int calculate_sum(int *arr, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += arr[i];
    }
    return sum;
}

int job(){
    int counter = 0;
    for(int i = 0; i < ARRAY_SIZE; i++){
        for(int j = 0; j < ARRAY_SIZE; j++){
            for(int k = 0; k < ARRAY_SIZE; k++){
                // for(int q = 0; q < ARRAY_SIZE; q++){
                    counter++;
                // }
            }
        }
    }
    // printf("counter: %d\n", counter);
    return counter;
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
            int my_pid = getpid();
            printf("process with pid created: %d\n", my_pid);
            int sum = job();
            sum = sum;
            printf("%d\n", sum);
            printf("job for pid: %d done!\n", my_pid);
            close(fds[i][0]); //close read for child
            
            int turn_around_time = getProcTick(my_pid);
            int waiting_time = getProcWaitingTicks(my_pid);
            int ready_time = getProcReadyTicks(my_pid);
            int running_time = getProcRunningTicks(my_pid);
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
    int ave_turnaround_time = (calculate_sum(turnaround_times, PROCESS_COUNT)) / PROCESS_COUNT;
    int ave_waiting_time = (calculate_sum(waiting_times, PROCESS_COUNT)) / PROCESS_COUNT;
    int ave_ready_time = (calculate_sum(ready_times, PROCESS_COUNT)) / PROCESS_COUNT;
    int ave_running_time = (calculate_sum(running_times, PROCESS_COUNT)) / PROCESS_COUNT;
    
    printf("TEST RESULT FOR ALGORITHM: %s:\n", algorithm_name);
    printf("average turnaround time: %d\n", ave_turnaround_time);
    printf("average waiting time: %d\n", ave_waiting_time);
    printf("average ready time: %d\n", ave_ready_time);
    printf("average running time: %d\n", ave_running_time);

}

int main() {

    sched_RR();
    runProcesses("RR");
    sched_FCFS();
    runProcesses("FCFS");
    exit(0);
}

