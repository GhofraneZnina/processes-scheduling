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

void priorityScheduling(Process processes[], int num_processes) {
    int current_time = 0;
    int executed_processes = 0;

    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (executed_processes < num_processes) {
        int highest_priority = -1;
        int selected_process = -1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
                if (highest_priority == -1 || processes[i].priority > highest_priority) {
                    highest_priority = processes[i].priority;
                    selected_process = i;
                }
            }
        }

        if (selected_process == -1) {
           
            printf("-");
            current_time++;
        } else {
           
            int execute_time = (processes[selected_process].remaining_time < 1) ? processes[selected_process].remaining_time : 1;
            processes[selected_process].remaining_time -= execute_time;
            current_time += execute_time;

            for (int j = 0; j < execute_time; j++) {
                printf("%c", processes[selected_process].name);
            }
            printf(" ");

            if (processes[selected_process].remaining_time == 0) {
                executed_processes++;
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

    printf("Priority Scheduling:\n");

    priorityScheduling(processes, num_processes);

    return 0;
}
