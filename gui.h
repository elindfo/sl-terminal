#include <ncurses.h>
#include "SLResponse.h"

#ifndef GUI_H
#define GUI_H

#define WIN1_LINES 20
#define WIN1_COLS 80

struct filter_data{
    char line_number[10];
    int vehicle;
};
typedef struct filter_data FilterData;

struct thread_data{
    SLResponse *queryResults;
    WINDOW *w1;
    WINDOW *w2;
    WINDOW *w3;
    WINDOW *w4;
    WINDOW *w5;
};
typedef struct thread_data ThreadData;

extern const char * program_title;
extern const char * title_information_text;
extern const char * title_author;

static void welcome_screen();
void draw_GUI(long site_id, int filter, int vehicle, char *data);
void *gui_thread_function(void *p);
void filter_query_results(SLResponse *queryResults, int filter, FilterData data);

#endif