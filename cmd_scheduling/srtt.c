#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PROCESSES 10

typedef struct {
    char name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
} Process;

void srtScheduling(Process processes[], int num_processes) {
    int current_time = 0;
    int remaining_processes = num_processes;

    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (remaining_processes > 0) {
        int shortest_index = -1;
        int shortest_time = INT_MAX;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < shortest_time) {
                    shortest_time = processes[i].remaining_time;
                    shortest_index = i;
                }
            }
        }

        if (shortest_index != -1) {
            int execute_time = 1;
            processes[shortest_index].remaining_time -= execute_time;

            printf("%c ", processes[shortest_index].name);

            if (processes[shortest_index].remaining_time == 0) {
                remaining_processes--;
            }
        } else {
            printf("- ");
        }

        current_time++;
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

    printf("Shortest Remaining Time (SRT) Scheduling:\n");

    srtScheduling(processes, num_processes);

    return 0;
}
