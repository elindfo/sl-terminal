#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "platsuppslag.h"
#include "realtidsinfo.h"
#include "SLResponse.h"
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

const char * program_title = "---WELCOME---";
const char * title_information_text = "Latest public transport information from SL";
const char * title_author = "Creator: Elindfo";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void draw_GUI(long site_id, int filter, int vehicle, char *data){

    pthread_t gui_thread;

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    welcome_screen();

    int term_row, term_col;
    getmaxyx(stdscr, term_row, term_col);

    WINDOW *buses_win = newwin(term_row, term_col / 5, 0, 0);
    box(buses_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(buses_win, 0, 4, "Buses");

    WINDOW *metros_win = newwin(term_row, term_col / 5, 0, (1.0/5)*term_col);
    box(metros_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(metros_win, 0, 4, "Metros");

    WINDOW *trains_win = newwin(term_row, term_col / 5, 0, (2.0/5)*term_col);
    box(trains_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(trains_win, 0, 4, "Trains");

    WINDOW *trams_win = newwin(term_row, term_col / 5, 0, (3.0/5)*term_col);
    box(trams_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(trams_win, 0, 4, "Trams");

    WINDOW *ships_win = newwin(term_row, term_col / 5, 0, (4.0/5)*term_col);
    box(ships_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(ships_win, 0, 4, "Ships");

    refresh();

    json_t ri_root;
    SLResponse *queryResults = malloc(sizeof(SLResponse));
    queryResults = query(site_id, &ri_root);
    ThreadData gui_thread_data;

    if(filter != -1){ //If filter
        FilterData filter_d;

        switch(filter){
            case 0:{
                strcpy(filter_d.line_number, data);
                filter_d.vehicle = vehicle;
                break;
            }
        }
        filter_query_results(queryResults, filter, filter_d);
    }
    

    gui_thread_data.w1 = malloc(sizeof(WINDOW));
    gui_thread_data.w2 = malloc(sizeof(WINDOW));
    gui_thread_data.w3 = malloc(sizeof(WINDOW));
    gui_thread_data.w4 = malloc(sizeof(WINDOW));
    gui_thread_data.w5 = malloc(sizeof(WINDOW));
    gui_thread_data.queryResults = malloc(sizeof(SLResponse));
    gui_thread_data.w1 = buses_win;
    gui_thread_data.w2 = metros_win;
    gui_thread_data.w3 = trains_win;
    gui_thread_data.w4 = trams_win;
    gui_thread_data.w5 = ships_win;
    gui_thread_data.queryResults = queryResults;

    if(pthread_create(&gui_thread, NULL, gui_thread_function, &gui_thread_data) != 0){
        perror("Unable to create gui_thread.");
        exit(1);
    }
    sleep(10);
    while(1){
        pthread_mutex_lock(&mutex);
        free(queryResults);
        queryResults = malloc(sizeof(SLResponse));
        queryResults = query(site_id, &ri_root);

        if(filter != -1){
        FilterData filter_d;

        switch(filter){
            case 0:{
                strcpy(filter_d.line_number, data);
                filter_d.vehicle = vehicle;
                break;
            }
        }
        filter_query_results(queryResults, filter, filter_d);
    }

        pthread_mutex_unlock(&mutex);
        sleep(10);
    }
    pthread_join(gui_thread, NULL);

    endwin();
}

static void welcome_screen(){
    int term_row, term_col;

    getmaxyx(stdscr, term_row, term_col);
    mvprintw(term_row / 2 - 1, (term_col - strlen(program_title)) / 2 ,"%s", program_title);
    mvprintw(term_row / 2, (term_col - strlen(title_information_text)) / 2 ,"%s", title_information_text);
    mvprintw(term_row / 2 + 1, (term_col - strlen(title_author)) / 2 ,"%s", title_author);

    mvprintw(term_row - 2, 0, "Press any key to continue...");
    refresh();
    getch();
    clear();
}


void *gui_thread_function(void *p){
    ThreadData *data = (ThreadData *)p;
    WINDOW *w1 = (data -> w1);
    WINDOW *w2 = (data -> w2);
    WINDOW *w3 = (data -> w3);
    WINDOW *w4 = (data -> w4);
    WINDOW *w5 = (data -> w5);
    SLResponse *queryResults = (data -> queryResults);

    int counter = 0;
    int no_of_results_to_show = 6;


    while(1){
        pthread_mutex_lock(&mutex);

        werase(w1);
        werase(w2);
        werase(w3);
        werase(w4);
        werase(w5);

        for(int i = 0, added = 0; i < queryResults -> responseData.noOfBuses; i++){
            if(added > no_of_results_to_show){
                break;
            }
            mvwprintw(w1, added * 6 + 2, 2, "From  : %s", queryResults -> responseData.buses[i].stopAreaName);
            mvwprintw(w1, added * 6 + 3, 2, "To    : %s", queryResults -> responseData.buses[i].destination);
            mvwprintw(w1, added * 6 + 4, 2, "Dep in: %s", queryResults -> responseData.buses[i].displayTime);
            mvwprintw(w1, added * 6 + 5, 2, "Nr    : %s", queryResults -> responseData.buses[i].lineNumber);
            mvwprintw(w1, added * 6 + 6, 2, "");
            added++;
        }
        for(int i = 0, added = 0; i < queryResults -> responseData.noOfMetros; i++){
            if(added > no_of_results_to_show){
                break;
            }
            mvwprintw(w2, added * 6 + 2, 2, "From  : %s", queryResults -> responseData.metros[i].stopAreaName);
            mvwprintw(w2, added * 6 + 3, 2, "To    : %s", queryResults -> responseData.metros[i].destination);
            mvwprintw(w2, added * 6 + 4, 2, "Dep in: %s", queryResults -> responseData.metros[i].displayTime);
            mvwprintw(w2, added * 6 + 5, 2, "Nr    : %s", queryResults -> responseData.metros[i].lineNumber);
            mvwprintw(w2, added * 6 + 6, 2, "");
            added++;
        }
        for(int i = 0, added = 0; i < queryResults -> responseData.noOfTrains; i++){
            if(added > no_of_results_to_show){
                break;
            }
            mvwprintw(w3, added * 6 + 2, 2, "From  : %s", queryResults -> responseData.trains[i].stopAreaName);
            mvwprintw(w3, added * 6 + 3, 2, "To    : %s", queryResults -> responseData.trains[i].destination);
            mvwprintw(w3, added * 6 + 4, 2, "Dep in: %s", queryResults -> responseData.trains[i].displayTime);
            mvwprintw(w3, added * 6 + 5, 2, "Nr    : %s", queryResults -> responseData.trains[i].lineNumber);
            mvwprintw(w3, added * 6 + 6, 2, "");
            added++;
        }
        for(int i = 0, added = 0; i < queryResults -> responseData.noOfTrams; i++){
            if(added > no_of_results_to_show){
                break;
            }
            mvwprintw(w4, added * 6 + 2, 2, "From  : %s", queryResults -> responseData.trams[i].stopAreaName);
            mvwprintw(w4, added * 6 + 3, 2, "To    : %s", queryResults -> responseData.trams[i].destination);
            mvwprintw(w4, added * 6 + 4, 2, "Dep in: %s", queryResults -> responseData.trams[i].displayTime);
            mvwprintw(w4, added * 6 + 5, 2, "Nr    : %s", queryResults -> responseData.trams[i].lineNumber);
            mvwprintw(w4, added * 6 + 6, 2, "");
            added++;
        }
        for(int i = 0, added = 0; i < queryResults -> responseData.noOfShips; i++){
            if(added > no_of_results_to_show){
                break;
            }
            mvwprintw(w5, added * 6 + 2, 2, "From  : %s", queryResults -> responseData.ships[i].stopAreaName);
            mvwprintw(w5, added * 6 + 3, 2, "To    : %s", queryResults -> responseData.ships[i].destination);
            mvwprintw(w5, added * 6 + 4, 2, "Dep in: %s", queryResults -> responseData.ships[i].displayTime);
            mvwprintw(w5, added * 6 + 5, 2, "Nr    : %s", queryResults -> responseData.ships[i].lineNumber);
            mvwprintw(w5, added * 6 + 6, 2, "");
            added++;
        }

        box(w1, ACS_VLINE, ACS_HLINE);
        box(w2, ACS_VLINE, ACS_HLINE);
        box(w3, ACS_VLINE, ACS_HLINE);
        box(w4, ACS_VLINE, ACS_HLINE);
        box(w5, ACS_VLINE, ACS_HLINE);

        mvwprintw(w1, 0, 4, "Buses");
        mvwprintw(w2, 0, 4, "Metros");
        mvwprintw(w3, 0, 4, "Trains");
        mvwprintw(w4, 0, 4, "Trams");
        mvwprintw(w5, 0, 4, "Ships");

        wrefresh(w1);
        wrefresh(w2);
        wrefresh(w3);
        wrefresh(w4);
        wrefresh(w5);

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void filter_query_results(SLResponse *queryResults, int filter, FilterData data){

    switch(filter){
        case 0:{ //Filter by line number
            switch(data.vehicle){
                case 0:{ //Bus
                    for(int i = 0; i < queryResults->responseData.noOfBuses; i++){
                        if(strcmp(queryResults -> responseData.buses[i].lineNumber, data.line_number) != 0){ //If the bus has the wrong line number, remove it
                            for(int j = i; j < queryResults->responseData.noOfBuses - 1; j++){
                                queryResults -> responseData.buses[j] = queryResults -> responseData.buses[j + 1];
                            }
                            (queryResults->responseData.noOfBuses)--;
                            i--;
                        }
                    }
                    break;
                }
                case 1:{ //Metro
                    for(int i = 0; i < queryResults->responseData.noOfMetros; i++){
                        if(strcmp(queryResults -> responseData.metros[i].lineNumber, data.line_number) != 0){ //If the bus has the wrong line number, remove it
                            for(int j = i; j < queryResults->responseData.noOfMetros - 1; j++){
                                queryResults -> responseData.metros[j] = queryResults -> responseData.metros[j + 1];
                            }
                            (queryResults->responseData.noOfMetros)--;
                            i--;
                        }
                    }
                    break;
                }
                case 2:{ //Train
                    for(int i = 0; i < queryResults->responseData.noOfTrains; i++){
                        if(strcmp(queryResults -> responseData.trains[i].lineNumber, data.line_number) != 0){ //If the bus has the wrong line number, remove it
                            for(int j = i; j < queryResults->responseData.noOfTrains - 1; j++){
                                queryResults -> responseData.trains[j] = queryResults -> responseData.trains[j + 1];
                            }
                            (queryResults->responseData.noOfTrains)--;
                            i--;
                        }
                    }
                    break;
                }
                case 3:{ //Tram
                    for(int i = 0; i < queryResults->responseData.noOfTrams; i++){
                        if(strcmp(queryResults -> responseData.trams[i].lineNumber, data.line_number) != 0){ //If the bus has the wrong line number, remove it
                            for(int j = i; j < queryResults->responseData.noOfTrams - 1; j++){
                                queryResults -> responseData.trams[j] = queryResults -> responseData.trams[j + 1];
                            }
                            (queryResults->responseData.noOfTrams)--;
                            i--;
                        }
                    }
                    break;
                }
                case 4:{ //Ship
                    for(int i = 0; i < queryResults->responseData.noOfShips; i++){
                        if(strcmp(queryResults -> responseData.ships[i].lineNumber, data.line_number) != 0){ //If the bus has the wrong line number, remove it
                            for(int j = i; j < queryResults->responseData.noOfShips - 1; j++){
                                queryResults -> responseData.ships[j] = queryResults -> responseData.ships[j + 1];
                            }
                            (queryResults->responseData.noOfShips)--;
                            i--;
                        }
                    }
                    break;
                }
            }
        }
    }
}