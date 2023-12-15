#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define MAX_PROCESSES 10

typedef struct {
    char name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int start_time; 
    GdkRGBA color;
} Process;

typedef struct {
    GtkWidget *window;
    GtkWidget *drawing_area;
    Process *processes;
    int num_processes;
    int time_quantum;
    int selected_algorithm; 
} AppData;

void generate_random_color(GdkRGBA *color) {
    color->red = g_random_double();
    color->green = g_random_double();
    color->blue = g_random_double();
    color->alpha = 1.0;
}

void draw_fifo_gantt_chart(AppData *data, cairo_t *cr) {
    int current_time = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;

    for (int i = 0; i < data->num_processes; i++) {
        
        if (current_time < data->processes[i].arrival_time) {
            current_time = data->processes[i].arrival_time;
        }

        
        int rect_width = data->processes[i].burst_time * 20;  

        generate_random_color(&(data->processes[i].color));

        cairo_set_source_rgba(cr, data->processes[i].color.red, data->processes[i].color.green, data->processes[i].color.blue, data->processes[i].color.alpha);
        cairo_rectangle(cr, x, y, rect_width, rect_height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 12.0);
        char process_name[2] = {data->processes[i].name, '\0'};
        cairo_move_to(cr, x + 5, y + rect_height / 2);
        cairo_show_text(cr, process_name);

        
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 10.0);
        char start_time[5];
        snprintf(start_time, sizeof(start_time), "%d", current_time);
        cairo_move_to(cr, x, y + rect_height + 12);
        cairo_show_text(cr, start_time);

        
        current_time = current_time + data->processes[i].burst_time;
        x += rect_width;

        
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 10.0);
        char end_time[5];
        snprintf(end_time, sizeof(end_time), "%d", current_time);
        cairo_move_to(cr, x - 10, y + rect_height + 12);
        cairo_show_text(cr, end_time);
    }

}
void priorityScheduling(AppData *data, cairo_t *cr) {
    int current_time = 0;
    int executed_processes = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;

    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (executed_processes < data->num_processes) {
        int highest_priority = -1;
        int selected_process = -1;

        
        for (int i = 0; i < data->num_processes; i++) {
            if (data->processes[i].remaining_time > 0 && data->processes[i].arrival_time <= current_time) {
                if (highest_priority == -1 || data->processes[i].priority > highest_priority) {
                    highest_priority = data->processes[i].priority;
                    selected_process = i;
                }
            }
        }

        if (selected_process == -1) {
            
            printf("-");
            current_time++;
        } else {
            
            int execute_time = (data->processes[selected_process].remaining_time < 1) ? data->processes[selected_process].remaining_time : 1;
            data->processes[selected_process].remaining_time -= execute_time;
            current_time += execute_time;

            
            for (int j = 0; j < execute_time; j++) {
                printf("%c", data->processes[selected_process].name);
            }
            printf(" ");

            
            int rect_width = execute_time * 20;  
            generate_random_color(&(data->processes[selected_process].color));
            cairo_set_source_rgba(cr, data->processes[selected_process].color.red,
                                  data->processes[selected_process].color.green,
                                  data->processes[selected_process].color.blue,
                                  data->processes[selected_process].color.alpha);
            cairo_rectangle(cr, x, y, rect_width, rect_height);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 12.0);
            char process_name[2] = {data->processes[selected_process].name, '\0'};
            cairo_move_to(cr, x + 5, y + rect_height / 2);
            cairo_show_text(cr, process_name);

            
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 10.0);
            char start_time[5];
            snprintf(start_time, sizeof(start_time), "%d", current_time - execute_time);
            cairo_move_to(cr, x, y + rect_height + 12);
            cairo_show_text(cr, start_time);

            
            x += rect_width;

            
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 10.0);
            char end_time[5];
            snprintf(end_time, sizeof(end_time), "%d", current_time);
            cairo_move_to(cr, x - 10, y + rect_height + 12);
            cairo_show_text(cr, end_time);

            if (data->processes[selected_process].remaining_time == 0) {
                executed_processes++;
            }
        }
    }

    printf("\n-------------\n");
}
void draw_robin_gantt_chart(AppData *data, cairo_t *cr) {
    int remaining_processes = data->num_processes;
    int current_time = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;
    int rect_width = 20;

    while (remaining_processes > 0) {
        for (int i = 0; i < data->num_processes; i++) {
            if (data->processes[i].remaining_time > 0) {
                int execute_time = (data->processes[i].remaining_time < data->time_quantum)
                                       ? data->processes[i].remaining_time
                                       : data->time_quantum;

                data->processes[i].remaining_time -= execute_time;
                current_time += execute_time;

                generate_random_color(&(data->processes[i].color));

                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                cairo_set_font_size(cr, 10.0);
                char start_time[5];
                snprintf(start_time, sizeof(start_time), "%d", current_time - execute_time);
                cairo_move_to(cr, x, y + rect_height + 12);
                cairo_show_text(cr, start_time);

                for (int j = 0; j < execute_time; j++) {
                    generate_random_color(&(data->processes[i].color));

                    cairo_set_source_rgba(cr, data->processes[i].color.red, data->processes[i].color.green, data->processes[i].color.blue, data->processes[i].color.alpha);

                    cairo_rectangle(cr, x, y, rect_width, rect_height);
                    cairo_fill(cr);

                    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                    cairo_set_font_size(cr, 12.0);
                    char process_name[2] = {data->processes[i].name, '\0'};
                    cairo_move_to(cr, x + 5, y + rect_height / 2);
                    cairo_show_text(cr, process_name);

                    x += rect_width;
                }

                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                cairo_set_font_size(cr, 10.0);
                char end_time[5];
                snprintf(end_time, sizeof(end_time), "%d", current_time);
                cairo_move_to(cr, x - 10, y + rect_height + 12);
                cairo_show_text(cr, end_time);

                if (data->processes[i].remaining_time == 0) {
                    remaining_processes--;
                }
            }
        }
    }
}
void draw_srt_gantt_chart(AppData *data, cairo_t *cr) {
    int remaining_processes = data->num_processes;
    int current_time = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;
    int rect_width;

    while (remaining_processes > 0) {
        int shortest_index = -1;
        int shortest_time = INT_MAX;

        for (int i = 0; i < data->num_processes; i++) {
            if (data->processes[i].arrival_time <= current_time && data->processes[i].remaining_time > 0) {
                if (data->processes[i].remaining_time < shortest_time) {
                    shortest_time = data->processes[i].remaining_time;
                    shortest_index = i;
                }
            }
        }

        if (shortest_index != -1) {
            int execute_time = 1;
            data->processes[shortest_index].remaining_time -= execute_time;

            
            generate_random_color(&(data->processes[shortest_index].color));
            cairo_set_source_rgba(cr, data->processes[shortest_index].color.red, data->processes[shortest_index].color.green, data->processes[shortest_index].color.blue, data->processes[shortest_index].color.alpha);

            rect_width = execute_time * 20;  
            for (int j = 0; j < execute_time; j++) {
                cairo_rectangle(cr, x, y, rect_width, rect_height);
                cairo_fill(cr);

                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                cairo_set_font_size(cr, 12.0);
                char process_name[2] = {data->processes[shortest_index].name, '\0'};
                cairo_move_to(cr, x + 5, y + rect_height / 2);
                cairo_show_text(cr, process_name);

                
                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                cairo_set_font_size(cr, 10.0);
                char start_time[5];
                snprintf(start_time, sizeof(start_time), "%d", current_time);
                cairo_move_to(cr, x + 5, y + rect_height + 12);
                cairo_show_text(cr, start_time);

                x += rect_width;
            }

            
            if (data->processes[shortest_index].remaining_time == 0) {
                remaining_processes--;

                cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
                cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
                cairo_set_font_size(cr, 10.0);
                char end_time[5];
                snprintf(end_time, sizeof(end_time), "%d", current_time);
                cairo_move_to(cr, x - 10, y + rect_height + 12);
                cairo_show_text(cr, end_time);
            }
        } else {

            rect_width = 20;
            cairo_set_source_rgb(cr, 0.8, 0.8, 0.8); 
            cairo_rectangle(cr, x, y, rect_width, rect_height);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 10.0);
            char start_time[5];
            snprintf(start_time, sizeof(start_time), "%d", current_time);
            cairo_move_to(cr, x + 5, y + rect_height + 12);
            cairo_show_text(cr, start_time);

            x += rect_width;
        }

        current_time++;
    }
}
void draw_multi_gantt_chart(AppData *data, cairo_t *cr) {
    int completed = 0;
    int time = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;
    int rect_width;

    printf("\nGantt Chart:\n");
    printf("-------------\n");

    while (completed < data->num_processes) {
        Process *selected_process = NULL;

        for (int i = 0; i < data->num_processes; ++i) {
            if (data->processes[i].remaining_time > 0 && data->processes[i].arrival_time <= time) {
                if (selected_process == NULL || data->processes[i].priority > selected_process->priority) {
                    selected_process = &(data->processes[i]);
                } else if (data->processes[i].priority == selected_process->priority &&
                           data->processes[i].arrival_time < selected_process->arrival_time) {
                    selected_process = &(data->processes[i]);
                }
            }
        }

        if (selected_process != NULL) {
            int execution_time = (selected_process->remaining_time < data->time_quantum)
                                     ? selected_process->remaining_time
                                     : data->time_quantum;

            selected_process->remaining_time -= execution_time;
            selected_process->start_time = time;

            generate_random_color(&(selected_process->color));

            rect_width = execution_time * 20;  

            cairo_set_source_rgba(cr, selected_process->color.red, selected_process->color.green, selected_process->color.blue, selected_process->color.alpha);
            cairo_rectangle(cr, x, y, rect_width, rect_height);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 12.0);
            char process_name[2] = {selected_process->name, '\0'};
            cairo_move_to(cr, x + 5, y + rect_height / 2);
            cairo_show_text(cr, process_name);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 10.0);
            char start_time[5];
            snprintf(start_time, sizeof(start_time), "%d", selected_process->start_time);
            cairo_move_to(cr, x + 5, y + rect_height + 12);
            cairo_show_text(cr, start_time);

            x += rect_width;

            if (selected_process->remaining_time == 0) {
                completed++;
            }

            time += execution_time;

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(cr, 10.0);
            char end_time[5];
            snprintf(end_time, sizeof(end_time), "%d", time);
            cairo_move_to(cr, x - 10, y + rect_height + 12);
            cairo_show_text(cr, end_time);
        } else {

            rect_width = data->time_quantum * 20;  
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0); 
            cairo_rectangle(cr, x, y, rect_width, rect_height);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
            cairo_set_font_size(cr, 12.0);
            cairo_move_to(cr, x + 5, y + rect_height / 2);
            cairo_show_text(cr, "-");

            x += rect_width;
            time += data->time_quantum;
        }
    }

    printf("\n-------------\n");
}


void draw_gantt_chart(AppData *data, cairo_t *cr) {
    switch (data->selected_algorithm) {
        case 0:
            
            draw_fifo_gantt_chart(data, cr);
            break;
        case 1:
            draw_srt_gantt_chart(data, cr);
            break;
        case 2:
            draw_multi_gantt_chart(data, cr);
            break;
        case 3:
            draw_robin_gantt_chart(data, cr);
            break;
        case 4:
            priorityScheduling(data, cr);
            break;
        default:
            break;
    }
}


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_rectangle(cr, 0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
    cairo_fill(cr);
    draw_gantt_chart(data, cr);

    return FALSE;
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
        processes[i].color = (GdkRGBA){0.0, 0.0, 0.0, 1.0};
    }

    fclose(file);
}
void initialize_app_data_from_file(AppData *data, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &data->num_processes) != 1) {
        fprintf(stderr, "Error reading the number of processes from file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (data->num_processes <= 0 || data->num_processes > MAX_PROCESSES) {
        fprintf(stderr, "Invalid number of processes in file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < data->num_processes; i++) {
        if (fscanf(file, " %c %d %d %d %d",
                   &data->processes[i].name,
                   &data->processes[i].arrival_time,
                   &data->processes[i].burst_time,
                   &data->processes[i].remaining_time,
                   &data->processes[i].priority) != 5) {
            fprintf(stderr, "Error reading process information from file %s\n", filename);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        if (data->processes[i].arrival_time < 0 ||
            data->processes[i].burst_time < 0 ||
            data->processes[i].priority < 0) {
            fprintf(stderr, "Invalid process parameters in file %s\n", filename);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

static void on_algorithm_button_clicked(GtkWidget *button, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    free(data->processes);

    data->processes = (Process *)malloc(MAX_PROCESSES * sizeof(Process));
    if (data->processes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    loadProcessesFromFile(data->processes, &(data->num_processes), "file.txt");

    const char *label = gtk_button_get_label(GTK_BUTTON(button));

    if (strcmp(label, "FIFO") == 0) {
        data->selected_algorithm = 0;
    } else if (strcmp(label, "SRTF") == 0) {
        data->selected_algorithm = 1;
    } else if (strcmp(label, "Round Robin") == 0) {
        data->selected_algorithm = 2;
    } else if (strcmp(label, "Priority") == 0) {
        data->selected_algorithm = 3;
    } else if (strcmp(label, "Multilevel") == 0) {
        data->selected_algorithm = 4;
    }

    gtk_widget_queue_draw(data->drawing_area);
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    gtk_init(&argc, &argv);

    AppData data;
    data.processes = (Process *)malloc(MAX_PROCESSES * sizeof(Process));
    if (data.processes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    data.num_processes = 0;
    data.selected_algorithm = 0; 
    data.time_quantum = 2;  
    loadProcessesFromFile(data.processes, &(data.num_processes), argv[1]);
    initialize_app_data_from_file(&data, "file.txt");
    
   

    data.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(data.window), "Gantt Chart");
    gtk_container_set_border_width(GTK_CONTAINER(data.window), 10);
    gtk_widget_set_size_request(data.window, 800, 400);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(data.window), grid);

    const char *algorithm_labels[] = {"FIFO", "SRTF", "Round Robin", "Priority", "Multilevel"};
    GtkWidget *algorithm_buttons[5];

    for (int i = 0; i < 5; i++) {
        algorithm_buttons[i] = gtk_button_new_with_label(algorithm_labels[i]);
        g_signal_connect(G_OBJECT(algorithm_buttons[i]), "clicked", G_CALLBACK(on_algorithm_button_clicked), &data);
        gtk_grid_attach(GTK_GRID(grid), algorithm_buttons[i], i, 0, 1, 1);
    }

    GtkWidget *draw_button = gtk_button_new_with_label("Draw Gantt Chart");
    g_signal_connect(G_OBJECT(draw_button), "clicked", G_CALLBACK(on_draw_event), &data);
    gtk_grid_attach(GTK_GRID(grid), draw_button, 5, 0, 1, 1);

    data.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(data.drawing_area, 600, 300);
    g_signal_connect(G_OBJECT(data.drawing_area), "draw", G_CALLBACK(on_draw_event), &data);
    gtk_grid_attach(GTK_GRID(grid), data.drawing_area, 0, 1, 6, 1);

    gtk_widget_show_all(data.window);

    gtk_main();

    free(data.processes);

    return 0;
}
