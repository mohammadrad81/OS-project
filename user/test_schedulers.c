#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"


#define NUM_PROCESSES 32
#define ARRAY_LENGTH 10000


void runProcesses();

int main() {

    runProcesses();
    sched_FCFS();
    runProcesses();

    exit(0);
}

int calculateSum(int *arr, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += arr[i];
    }
    return sum;
}

void childProc() {

    int array1[ARRAY_LENGTH];
    int array2[ARRAY_LENGTH];

    for (int i = 0; i < ARRAY_LENGTH; i++) {
        array1[i] = 14232;
        array2[i] = 34564;
    }

    // Calculate sum of arrays
    int sum = calculateSum(array1, ARRAY_LENGTH) + calculateSum(array2, ARRAY_LENGTH);
    sum = sum;  //  To prevent error: unused variable ‘sum’
}

double calculateAverage(unsigned int *times, int length) {
    unsigned int sum = 0;

    for (int i = 0; i < length; i++) {
        sum += times[i];
    }
    printf("5. calculateAverage...\n");
    printf("sum: %d\n", sum);
    printf("length: %d\n", length);
    double avg = (double) sum / length;
    printf("6. calculateAverage...\n");
    return avg;
}

void runProcesses() {
    unsigned int waiting_times[NUM_PROCESSES];
    unsigned int turnaround_times[NUM_PROCESSES];
    unsigned int child_ids[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {

        int pid = fork();

        if (pid == 0) {
            // Child process
            childProc();
            exit(0);

        } else {
            // Parent process
            child_ids[i] = pid;
        }
    }
//
//    sleep(5);
    for (int i = 0; i < NUM_PROCESSES; i++) {
//        printf("%d) pid: %d\n", i, child_ids[i]);

        int waitingTicks = getProcWaitingTicks(child_ids[i]);
        int turnaroundTicks = getProcTurnaroundTicks(child_ids[i]);

        printf("%d) turnaroundTicks: %d\n", child_ids[i], turnaroundTicks);
        printf("%d) waitingTicks: %d\n", child_ids[i], waitingTicks);

        waiting_times[i] = waitingTicks;
        turnaround_times[i] = turnaroundTicks;



    }

//     Calculate average waiting time and turnaround time
    printf("avg_waiting_time...\n");
    double avg_waiting_time = calculateAverage(waiting_times, NUM_PROCESSES);
    printf("avg_turnaround_time...\n");
    double avg_turnaround_time = calculateAverage(turnaround_times, NUM_PROCESSES);

    sleep(5);
    printf("Average Waiting Time: %.2f microseconds\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f microseconds\n", avg_turnaround_time);
}
