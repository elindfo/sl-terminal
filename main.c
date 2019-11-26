#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SLResponse.h"
#include "platsuppslag.h"
#include "realtidsinfo.h"
#include "gui.h"
#include "curl.h"
#include <jansson.h>
#include <getopt.h>
#include <ctype.h>

static void cleanup(SLResponse *queryResults);

static void arg_usage_error(){
    printf("Usage: SL -s <station> [-f <filter[0]> -m <mode[0-4]> -d <data>]\n");
    printf("station: name\n");
    printf("filter:\n");
    printf("    0 - Filter by line number\n");
    printf("mode:\n");
    printf("    0 - Bus\n");
    printf("    1 - Metro\n");
    printf("    2 - Train\n");
    printf("    3 - Tram\n");
    printf("    4 - Ship\n");
    printf("data: i.e. line number or destination\n");
    exit(2);
}

int main(int argc, char *argv[]){

    if(argc != 3 && argc != 9){
        arg_usage_error();
    }
    int option;


    char * station = NULL;
    int filter = -1;
    int vehicle = -1;
    char * data = NULL;

    int sflag = 0, fflag = 0, mflag = 0, dflag = 0;

    while((option = getopt(argc, argv, "s:f:m:d:")) != -1){
        switch(option){
            case 's':{
                if(sflag) arg_usage_error();
                if(!isalpha(optarg[0])) arg_usage_error();
                sflag++;
                station = malloc(strlen(optarg) + 1);
                strcpy(station, optarg);
                break;
            }
            case 'f':{
                if(!sflag || fflag) arg_usage_error();
                if(!isdigit(optarg[0]) || strlen(optarg) > 1) arg_usage_error();
                if(atoi(optarg) < 0 || atoi(optarg) > 2) arg_usage_error();

                fflag++;
                filter = atoi(optarg);
                break;
            }
            case 'm':{
                if(!sflag || !fflag || mflag) arg_usage_error();
                if(!isdigit(optarg[0]) || strlen(optarg) > 1) arg_usage_error();
                if(atoi(optarg) < 0 || atoi(optarg) > 5) arg_usage_error();

                mflag++;
                vehicle = atoi(optarg);
                break;
            }
            case 'd':{
                if(!sflag || !fflag || !mflag || dflag) arg_usage_error();
                if(!isdigit(optarg[0])) arg_usage_error();

                dflag++;
                data = malloc(strlen(optarg) + 1);
                strcpy(data, optarg);
                break;
            }
            default:{
                printf("Error\n");
            }
        }
    }
    json_t pu_root;
    long site_id = retrieve_site_id(station, &pu_root);
    draw_GUI(site_id, filter, vehicle, data);
    free(station);
    free(data);
    json_decref(&pu_root);

    return 0;
}

static void cleanup(SLResponse *queryResults){

    free(queryResults -> responseData.latestUpdate);
    free(queryResults -> message);

    for(int i = 0; i < queryResults->responseData.noOfMetros; i++){

        free(queryResults -> responseData.metros[i].groupOfLine);
        free(queryResults -> responseData.metros[i].displayTime);
        free(queryResults -> responseData.metros[i].transportMode);
        free(queryResults -> responseData.metros[i].lineNumber);
        free(queryResults -> responseData.metros[i].destination);
        free(queryResults -> responseData.metros[i].stopAreaName);
        free(queryResults -> responseData.metros[i].stopPointDesignation);
        free(queryResults -> responseData.metros[i].timeTabledDateTime);
        free(queryResults -> responseData.metros[i].expectedDateTime);

        for(int j = 0; j < queryResults->responseData.metros[i].noOfDeviations; j++){
            free(queryResults -> responseData.metros[i].deviations[j].text);
            free(queryResults -> responseData.metros[i].deviations[j].consequence);
        }
        free(queryResults -> responseData.metros[i].deviations);
    }
    free(queryResults -> responseData.metros);

    for(int i = 0; i < queryResults->responseData.noOfBuses; i++){

        free(queryResults -> responseData.buses[i].groupOfLine);
        free(queryResults -> responseData.buses[i].displayTime);
        free(queryResults -> responseData.buses[i].transportMode);
        free(queryResults -> responseData.buses[i].lineNumber);
        free(queryResults -> responseData.buses[i].destination);
        free(queryResults -> responseData.buses[i].stopAreaName);
        free(queryResults -> responseData.buses[i].stopPointDesignation);
        free(queryResults -> responseData.buses[i].timeTabledDateTime);
        free(queryResults -> responseData.buses[i].expectedDateTime);

        for(int j = 0; j < queryResults->responseData.buses[i].noOfDeviations; j++){
            free(queryResults -> responseData.buses[i].deviations[j].text);
            free(queryResults -> responseData.buses[i].deviations[j].consequence);
        }
        free(queryResults -> responseData.buses[i].deviations);
    }
    free(queryResults -> responseData.buses);

    for(int i = 0; i < queryResults->responseData.noOfTrains; i++){
        free(queryResults -> responseData.trains[i].secondaryDestinationName);
        free(queryResults -> responseData.trains[i].groupOfLine);
        free(queryResults -> responseData.trains[i].displayTime);
        free(queryResults -> responseData.trains[i].transportMode);
        free(queryResults -> responseData.trains[i].lineNumber);
        free(queryResults -> responseData.trains[i].destination);
        free(queryResults -> responseData.trains[i].stopAreaName);
        free(queryResults -> responseData.trains[i].stopPointDesignation);
        free(queryResults -> responseData.trains[i].timeTabledDateTime);
        free(queryResults -> responseData.trains[i].expectedDateTime);

        for(int j = 0; j < queryResults->responseData.trains[i].noOfDeviations; j++){
            free(queryResults -> responseData.trains[i].deviations[j].text);
            free(queryResults -> responseData.trains[i].deviations[j].consequence);
        }
        free(queryResults -> responseData.trains[i].deviations);
    }
    free(queryResults -> responseData.trains);

    for(int i = 0; i < queryResults->responseData.noOfTrams; i++){

        free(queryResults -> responseData.trams[i].groupOfLine);
        free(queryResults -> responseData.trams[i].displayTime);
        free(queryResults -> responseData.trams[i].transportMode);
        free(queryResults -> responseData.trams[i].lineNumber);
        free(queryResults -> responseData.trams[i].destination);
        free(queryResults -> responseData.trams[i].stopAreaName);
        free(queryResults -> responseData.trams[i].stopPointDesignation);
        free(queryResults -> responseData.trams[i].timeTabledDateTime);
        free(queryResults -> responseData.trams[i].expectedDateTime);

        for(int j = 0; j < queryResults->responseData.trams[i].noOfDeviations; j++){
            free(queryResults -> responseData.trams[i].deviations[j].text);
            free(queryResults -> responseData.trams[i].deviations[j].consequence);
        }
        free(queryResults -> responseData.trams[i].deviations);
    }
    free(queryResults -> responseData.trams);

    for(int i = 0; i < queryResults->responseData.noOfShips; i++){

        free(queryResults -> responseData.ships[i].groupOfLine);
        free(queryResults -> responseData.ships[i].displayTime);
        free(queryResults -> responseData.ships[i].transportMode);
        free(queryResults -> responseData.ships[i].lineNumber);
        free(queryResults -> responseData.ships[i].destination);
        free(queryResults -> responseData.ships[i].stopAreaName);
        free(queryResults -> responseData.ships[i].stopPointDesignation);
        free(queryResults -> responseData.ships[i].timeTabledDateTime);
        free(queryResults -> responseData.ships[i].expectedDateTime);

        for(int j = 0; j < queryResults->responseData.ships[i].noOfDeviations; j++){
            free(queryResults -> responseData.ships[i].deviations[j].text);
            free(queryResults -> responseData.ships[i].deviations[j].consequence);
        }
        free(queryResults -> responseData.ships[i].deviations);
    }
    free(queryResults -> responseData.ships);
    free(queryResults);
}
