#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define MAX_PROCESSES 10

typedef struct {
    char name1;
    int arrival_time1;
    int burst_time1;
    int remaining_time1;
    int priority1;
    GdkRGBA color;
} Process;

typedef struct {
    GtkWidget *window;
    GtkWidget *drawing_area;
    Process *processes;
    int num_processes;
} AppData;

void generate_random_color(GdkRGBA *color) {
    color->red = g_random_double();
    color->green = g_random_double();
    color->blue = g_random_double();
    color->alpha = 1.0;
}

void draw_gantt_chart(AppData *data, cairo_t *cr) {
    int current_time = 0;
    int x = 50;
    int y = 50;
    int rect_height = 30;

    for (int i = 0; i < data->num_processes; i++) {
        if (current_time < data->processes[i].arrival_time1) {
            current_time = data->processes[i].arrival_time1;
        }

        int rect_width = data->processes[i].burst_time1 * 20; 

        generate_random_color(&(data->processes[i].color));

        cairo_set_source_rgba(cr, data->processes[i].color.red, data->processes[i].color.green, data->processes[i].color.blue, data->processes[i].color.alpha);
        cairo_rectangle(cr, x, y, rect_width, rect_height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 12.0);
        char process_name[2] = {data->processes[i].name1, '\0'};
        cairo_move_to(cr, x + 5, y + rect_height / 2);
        cairo_show_text(cr, process_name);

        cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 10.0);
        char start_time[5];
        snprintf(start_time, sizeof(start_time), "%d", current_time);
        cairo_move_to(cr, x, y + rect_height + 12);
        cairo_show_text(cr, start_time);

        current_time = current_time + data->processes[i].burst_time1;
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

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    AppData *data = (AppData *)user_data;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_rectangle(cr, 0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 16.0);
    cairo_move_to(cr, 10, 30);
    cairo_show_text(cr, "FIFO Gantt Chart");

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
        fscanf(file, " %c %d %d %d %d", &processes[i].name1, &processes[i].arrival_time1, &processes[i].burst_time1, &processes[i].remaining_time1, &processes[i].priority1);
        processes[i].color = (GdkRGBA){0.0, 0.0, 0.0, 1.0};
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    gtk_init(&argc, &argv);

    AppData data;
    data.processes = (Process *)malloc(MAX_PROCESSES * sizeof(Process));
    data.num_processes = 0;

    loadProcessesFromFile(data.processes, &(data.num_processes), argv[1]);

    data.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(data.window), "FIFO Gantt Chart");
    gtk_container_set_border_width(GTK_CONTAINER(data.window), 10);
    gtk_widget_set_size_request(data.window, 800, 400);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(data.window), grid);

    data.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(data.drawing_area, 600, 300);
    gtk_grid_attach(GTK_GRID(grid), data.drawing_area, 1, 0, 1, 1);
    g_signal_connect(G_OBJECT(data.drawing_area), "draw", G_CALLBACK(on_draw_event), &data);

    gtk_widget_show_all(data.window);

    gtk_main();

    free(data.processes);

    return 0;
}
