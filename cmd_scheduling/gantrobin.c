#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

typedef struct {
    char name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;  
} Process;

void roundRobin(Process processes[], int num_processes, int time_quantum) {
    int remaining_processes = num_processes;
    int current_time = 0;

    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (remaining_processes > 0) {
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                int execute_time = (processes[i].remaining_time < time_quantum) ? processes[i].remaining_time : time_quantum;

                processes[i].remaining_time -= execute_time;
                current_time += execute_time;

                for (int j = 0; j < execute_time; j++) {
                    printf("%c", processes[i].name);
                }
                printf(" ");

                if (processes[i].remaining_time == 0) {
                    remaining_processes--;
                }
            }
        }
    }

    printf("\n-------------\n");
}

void loadProcessesFromFile(Process processes[], int *num_processes, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", num_processes);

    for (int i = 0; i < *num_processes; i++) {
        fscanf(file, " %c %d %d %d %d", &processes[i].name, &processes[i].arrival_time, &processes[i].burst_time, &processes[i].remaining_time, &processes[i].priority);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Process processes[MAX_PROCESSES];
    int num_processes;

    loadProcessesFromFile(processes, &num_processes, argv[1]);

    int time_quantum = 2;

    printf("Round Robin Scheduling:\n");

    roundRobin(processes, num_processes, time_quantum);

    return 0;
}
