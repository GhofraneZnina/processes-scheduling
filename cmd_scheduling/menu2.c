#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_PROCESSES 30

typedef struct {
    char name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority; 
    int turnaround_time;
    int waiting_time;
} Process;


typedef struct {
    float avg_turnaround_time_R;
    float avg_waiting_time_R;
    float avg_turnaround_time_F;
    float avg_waiting_time_F;
    float avg_turnaround_time_S;
    float avg_waiting_time_S;
    float avg_turnaround_time_pr;
    float avg_waiting_time_Pr;
    float avg_turnaround_time_M;
    float avg_waiting_time_M;
} AlgorithmMetrics;

const char *algorithm_names[] = {"Round Robin","FIFO","SRT","Priority","Multi"};

void resetProcesses(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
    }
}

void roundRobin(Process processes[], int num_processes, int time_quantum, AlgorithmMetrics *metrics) {
    int remaining_processes = num_processes;
    resetProcesses(processes, num_processes);
    int current_time = 0;
    int total_waiting_time_RR =0;
    int total_turnaround_time_RR=0;

    printf("\nGantt Chart RR:\n");
    printf("-------------\n");

    while (true) {
        int remaining_processes = 0;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0 && processes[i].arrival_time <= current_time) {
                remaining_processes++;
                int execute_time = (processes[i].remaining_time < time_quantum) ? processes[i].remaining_time : time_quantum;

                processes[i].remaining_time -= execute_time;
                current_time += execute_time;

                for (int j = 0; j < execute_time; j++) {
                    printf("%c", processes[i].name);
                }
                printf(" ");

                if (processes[i].remaining_time == 0) {
                    remaining_processes--;

                    processes[i].turnaround_time = current_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

                    total_waiting_time_RR += processes[i].waiting_time;
                    total_turnaround_time_RR +=processes[i].turnaround_time;
                }
            
             
            }
            
        }

        if (remaining_processes == 0){
            break;
        }
        
    }
    printf("\n-------------\n");
    printf("\nMetrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c - Turnaround Time: %d, Waiting Time: %d\n", processes[i].name, processes[i].turnaround_time, processes[i].waiting_time);
    }

    float avg_waiting_time_RR = (float)total_waiting_time_RR / num_processes;
    float avg_turnaround_time_RR = (float)total_turnaround_time_RR / num_processes;

    metrics->avg_waiting_time_R = avg_waiting_time_RR;
    metrics->avg_turnaround_time_R = avg_turnaround_time_RR;
     printf("Average Waiting Time: %.2f\n",avg_waiting_time_RR);
     printf("Average Turnaround Time: %.2f\n",avg_turnaround_time_RR);
}

void multiLevelScheduler(Process processes[], int num_processes, int quantum, AlgorithmMetrics *metrics) {
    resetProcesses(processes, num_processes);
    int completed = 0; 
    int time = 0;      
    int total_waiting_time_Multi = 0;
    int total_turnaround_time_Multi = 0;

    printf("\nGantt Chart Multi:\n");
    printf("-------------\n");

    while (completed < num_processes) {
        int flag = 0; 

        for (int priority = 4; priority >= 1; --priority) {
            for (int i = 0; i < num_processes; ++i) {
                if (processes[i].remaining_time > 0 && processes[i].arrival_time <= time &&
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

                    processes[i].turnaround_time = time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

                    total_waiting_time_Multi= processes[i].waiting_time;
                    total_turnaround_time_Multi=processes[i].turnaround_time;
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
    
    printf("\nMetrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c - Turnaround Time: %d, Waiting Time: %d\n", processes[i].name, processes[i].turnaround_time, processes[i].waiting_time);
    }

    printf("\n-------------\n");

    float avg_waiting_time_Multi = (float)total_waiting_time_Multi / num_processes;
    float avg_turnaround_time_Multi = (float)total_turnaround_time_Multi / num_processes;

    metrics->avg_waiting_time_M = avg_waiting_time_Multi;
    metrics->avg_turnaround_time_M = avg_turnaround_time_Multi;

    printf("Average Waiting Time: %.2f\n",avg_waiting_time_Multi);
    printf("Average Turnaround Time: %.2f\n",avg_turnaround_time_Multi);
     
}

void fifo(Process processes[], int num_processes, AlgorithmMetrics *metrics) {
    resetProcesses(processes, num_processes);
    int current_time = 0;
    int total_waiting_time_FIFO = 0;
    int total_turnaround_time_FIFO =0;

    printf("\nGantt Chart FIFO:\n");
    printf("-------------\n");

    for (int i = 0; i < num_processes; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }

        for (int j = 0; j < processes[i].burst_time; j++) {
            printf("%c", processes[i].name);
        }
        printf(" ");

        current_time += processes[i].burst_time;
        processes[i].remaining_time = 0;

        processes[i].turnaround_time = current_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        total_waiting_time_FIFO += processes[i].waiting_time;
        total_turnaround_time_FIFO +=processes[i].turnaround_time;

    }

    printf("\n-------------\n");
    printf("\nMetrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c - Turnaround Time: %d, Waiting Time: %d\n", processes[i].name, processes[i].turnaround_time, processes[i].waiting_time);
    }
    
    float avg_waiting_time_FIFO = (float)total_waiting_time_FIFO / num_processes;
    float avg_turnaround_time_FIFO = (float)total_turnaround_time_FIFO / num_processes;

    metrics->avg_waiting_time_F = avg_waiting_time_FIFO;
    metrics->avg_turnaround_time_F = avg_turnaround_time_FIFO;


     printf("Average Waiting Time: %.2f\n",avg_waiting_time_FIFO);
     printf("Average Turnaround Time: %.2f\n",avg_turnaround_time_FIFO);

     
}

void srtScheduling(Process processes[], int num_processes, AlgorithmMetrics *metrics) {
    resetProcesses(processes, num_processes);
    int current_time = 0;
    int total_waiting_time_SRT = 0;
    int total_turnaround_time_SRT = 0;
    int remaining_processes = num_processes;

    printf("\nGantt Chart SRT:\n");
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

                processes[shortest_index].turnaround_time = current_time - processes[shortest_index].arrival_time + 1;
                processes[shortest_index].waiting_time = processes[shortest_index].turnaround_time - processes[shortest_index].burst_time;

            total_waiting_time_SRT += processes[shortest_index].waiting_time;
            total_turnaround_time_SRT +=processes[shortest_index].turnaround_time;
            }
        }

        current_time++;
    }

    printf("\n-------------\n");
    printf("\nMetrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c - Turnaround Time: %d, Waiting Time: %d\n", processes[i].name, processes[i].turnaround_time, processes[i].waiting_time);
    }

    float avg_waiting_time_SRT = (float)total_waiting_time_SRT / num_processes;
    float avg_turnaround_time_SRT = (float)total_turnaround_time_SRT/ num_processes;

    metrics->avg_waiting_time_S = avg_waiting_time_SRT;
    metrics->avg_turnaround_time_S = avg_turnaround_time_SRT;
     printf("Average Waiting Time: %.2f\n",avg_waiting_time_SRT);
     printf("Average Turnaround Time: %.2f\n",avg_turnaround_time_SRT);
}

void priorityScheduling(Process processes[], int num_processes, AlgorithmMetrics *metrics) {
    resetProcesses(processes, num_processes);
    int current_time = 0;
    int executed_processes = 0;
    int total_waiting_time_P = 0;
    int total_turnaround_time_P = 0;

    printf("\nGantt Chart Priority:\n");
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

                processes[selected_process].turnaround_time = current_time - processes[selected_process].arrival_time;
                processes[selected_process].waiting_time = processes[selected_process].turnaround_time - processes[selected_process].burst_time;
                total_waiting_time_P += processes[selected_process].waiting_time;
                total_turnaround_time_P +=processes[selected_process].turnaround_time;
            }
        }
    }

    printf("\n-------------\n");
    printf("\nMetrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("Process %c - Turnaround Time: %d, Waiting Time: %d\n", processes[i].name, processes[i].turnaround_time, processes[i].waiting_time);
    }

    float avg_waiting_time_P = (float)total_waiting_time_P / num_processes;
    float avg_turnaround_time_P = (float)total_turnaround_time_P/ num_processes;
    metrics->avg_waiting_time_Pr = avg_waiting_time_P;
    metrics->avg_turnaround_time_pr = avg_turnaround_time_P;
     printf("Average Waiting Time: %.2f\n",avg_waiting_time_P);
     printf("Average Turnaround Time: %.2f\n",avg_turnaround_time_P);
}

void compareMetrics(AlgorithmMetrics metrics[]) {
    printf("\nComparison Table:\n");
    printf("---------------------------------------------------------\n");
    printf("| Algorithm      | Average Turnaround Time | Average Waiting Time |\n");
    printf("---------------------------------------------------------\n");
    printf("| %-15s | %-23.2f | %-20.2f |\n", "Round Robin", metrics->avg_turnaround_time_R, metrics->avg_waiting_time_R);
    printf("| %-15s | %-23.2f | %-20.2f |\n", "FIFO", metrics->avg_turnaround_time_F,metrics->avg_waiting_time_F);
    printf("| %-15s | %-23.2f | %-20.2f |\n", "SRT", metrics->avg_turnaround_time_S, metrics->avg_waiting_time_S);
    printf("| %-15s | %-23.2f | %-20.2f |\n", "Priority", metrics->avg_turnaround_time_pr, metrics->avg_waiting_time_Pr);
    printf("| %-15s | %-23.2f | %-20.2f |\n", "Multi", metrics->avg_turnaround_time_M, metrics->avg_waiting_time_M);
    

    printf("---------------------------------------------------------\n");
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
    int current_time = 0;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Process processes[MAX_PROCESSES];
    int num_processes;

    loadProcessesFromFile(processes, &num_processes, argv[1]);

    int choice;
    int time_quantum = 0;
    
    AlgorithmMetrics algorithmMetrics[5] ={0};

    do {
        printf("\nMenu:\n");
        printf("1. Round Robin\n");
        printf("2. FiFo\n");
        printf("3. SRT\n");
        printf("4. Priority\n");
        printf("5. Multi Level \n");
        printf("6. Comparer les métriques\n");  
        printf("0. Quitter\n");

        printf("Choisissez un algorithme (0 pour quitter): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Entrez le quantum pour Round Robin : ");
                scanf("%d", &time_quantum);
                roundRobin(processes, num_processes, time_quantum, algorithmMetrics);
                break;

            case 2:
                fifo(processes, num_processes, algorithmMetrics);
                break;

            case 3:
                
                srtScheduling(processes, num_processes, algorithmMetrics);
                break;

            case 4:
                priorityScheduling(processes, num_processes, algorithmMetrics);
                break;

            case 5:
                multiLevelScheduler(processes, num_processes, 2, algorithmMetrics);
                break;

            case 6:
                compareMetrics(algorithmMetrics);
                break;

            case 0:
                printf("Programme terminé.\n");
                break;

            default:
                printf("Choix non valide. Veuillez réessayer.\n");
        }

    } while (choice != 0);

    return 0;
}