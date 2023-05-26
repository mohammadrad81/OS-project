#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user.h"


#define NUM_PROCESSES 32
#define ARRAY_LENGTH 10000


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
        array1[i] = 90184;
        array1[i] = 57128;
    }

    // Calculate sum of arrays
    int sum = calculateSum(array1, ARRAY_LENGTH) + calculateSum(array2, ARRAY_LENGTH);
    printf("%d ", sum);
}

double calculateAverage(unsigned int *times, int length) {
    unsigned int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += times[i];
    }
    return (double) sum / length;
}

int main() {
//    unsigned int waiting_times[NUM_PROCESSES];
//    unsigned int turnaround_times[NUM_PROCESSES];
    unsigned int child_ids[NUM_PROCESSES];

    for (int i = 0; i < NUM_PROCESSES; i++) {

        int pid = fork();

        if (pid == 0) {
            // Child process
            printf("starting %dth...\n", i);
            childProc();

        } else {
            child_ids[i] = pid;
            } else {
                fprintf(stderr, "Child process %d terminated abnormally.\n", pid);
                exit(1);
            }
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("%d) pid: %d\n", i, child_ids[i]);
    }
//     Calculate average waiting time and turnaround time
    double avg_waiting_time = calculateAverage(waiting_times, NUM_PROCESSES);
    double avg_turnaround_time = calculateAverage(turnaround_times, NUM_PROCESSES);

    printf("Average Waiting Time: %.2f microseconds\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f microseconds\n", avg_turnaround_time);

    return 0;
}