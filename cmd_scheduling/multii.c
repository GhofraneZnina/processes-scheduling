#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name;
    int start_time;
    int burst_time;
    int remaining_time;
    int priority;
} Process;

void multiLevelScheduler(Process processes[], int n, int quantum) {
    int completed = 0; 
    int time = 0;      
    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (completed < n) {
        int flag = 0; 
    
        for (int priority = 4; priority >= 1; --priority) {
            for (int i = 0; i < n; ++i) {
                if (processes[i].remaining_time > 0 && processes[i].start_time <= time &&
                    processes[i].priority == priority) {

                    int executionTime = (processes[i].remaining_time < quantum) ? processes[i].remaining_time : quantum;

                    for (int j = 0; j < executionTime; ++j) {
                        printf("%c ", processes[i].name);
                    }
                    printf(" ");

                    processes[i].remaining_time -= executionTime;

                    time += executionTime;

                    if (processes[i].remaining_time == 0) {
                        completed++;
                    }

                    flag = 1; 
                    break;
                }
            }
            if (flag) {
                break; 
            }
        }

        if (flag == 0) {
            printf("- ");
            time++;
        }
    }

    printf("\n-------------\n");
}

int main() {
    FILE *file = fopen("file.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    Process processes[n];

    for (int i = 0; i < n; ++i) {
        fscanf(file, " %c %d %d %d %d",
               &processes[i].name,
               &processes[i].start_time,
               &processes[i].burst_time,
               &processes[i].remaining_time,
               &processes[i].priority);
    }

    fclose(file);

    int quantum = 2;
    multiLevelScheduler(processes, n, quantum);

    return 0;
}
