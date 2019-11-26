#include "realtidsinfo.h"
#include "SLResponse.h"
#include "curl.h"

#include <string.h>
#include <unistd.h>

const char *REALTIDSINFO_API_KEY = "{Replace with YOUR api key}"; //https://www.trafiklab.se/api/sl-realtidsinformation-4
static char * generateURL(long station);

SLResponse *query(long site_id, json_t *ri_root){

    char *ri_url = NULL;
    char *ri_json_response;

    json_error_t ri_error;

    ri_url = generateURL(site_id);
    ri_json_response = request(ri_url);
    free(ri_url);

    if(!ri_json_response){
        exit(1);
    }

    ri_root = json_loads(ri_json_response, 0, &ri_error);
    free(ri_json_response);

    if(!ri_root){
        fprintf(stderr, "ri_error: on line %d: %s\n", ri_error.line, ri_error.text);
        exit(1);
    }

    json_t *statusCode, *message, *executionTime, *responseData;
    json_t *latestUpdate, *dataAge;
    json_t *metros, *buses, *trains, *trams, *ships, *stopPointDeviations;
    json_t *groupOfLine, *displayTime, *transportMode, *lineNumber, *destination, *journeyDirection;
    json_t *stopAreaName, *stopAreaNumber, *stopPointNumber, *stopPointDesignation;
    json_t *timeTabledDateTime, *expectedDateTime, *journeyNumber, *deviations;
    json_t *secondaryDestinationName, *stopInfo;

    json_t *text, *consequence, *importanceLevel;

    SLResponse *queryResults = malloc(sizeof(SLResponse));

    statusCode = json_object_get(ri_root, "StatusCode");
    queryResults -> statusCode = (long)json_integer_value(statusCode);

    message = json_object_get(ri_root, "Message");
    if(json_string_value(message) != NULL){
        queryResults -> message = malloc(strlen(json_string_value(message)) + 1);
        strcpy(queryResults -> message, json_string_value(message));
    }
    else{
        queryResults -> message = malloc(0);
    }

    executionTime = json_object_get(ri_root, "ExecutionTime");
    queryResults -> executionTime = (long)json_integer_value(executionTime);

    responseData = json_object_get(ri_root, "ResponseData");

    latestUpdate = json_object_get(responseData, "LatestUpdate");
    if(json_string_value(latestUpdate) != NULL){
        queryResults -> responseData.latestUpdate = malloc(strlen(json_string_value(latestUpdate)) + 1);
        strcpy(queryResults -> responseData.latestUpdate, json_string_value(latestUpdate));
    }
    else{
        queryResults -> responseData.latestUpdate = malloc(0);
    }

    dataAge = json_object_get(responseData, "DataAge");
    queryResults -> responseData.dataAge = (long)json_integer_value(dataAge);



    metros = json_object_get(responseData, "Metros");
    queryResults -> responseData.noOfMetros = (int)json_array_size(metros);
    queryResults -> responseData.metros = malloc(sizeof(MetroData) * queryResults -> responseData.noOfMetros);

    for(size_t currentMetro = 0; currentMetro < json_array_size(metros); currentMetro++){
        json_t *metro_data = json_array_get(metros, currentMetro);

        groupOfLine = json_object_get(metro_data, "GroupOfLine");
        if(json_string_value(groupOfLine) != NULL){
            queryResults -> responseData.metros[currentMetro].groupOfLine = malloc(strlen(json_string_value(groupOfLine)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].groupOfLine, json_string_value(groupOfLine));
        }
        else{
            queryResults -> responseData.metros[currentMetro].groupOfLine = malloc(0);
        }

        displayTime = json_object_get(metro_data, "DisplayTime");
        if(json_string_value(displayTime) != NULL){
            queryResults -> responseData.metros[currentMetro].displayTime = malloc(strlen(json_string_value(displayTime)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].displayTime, json_string_value(displayTime));
        }
        else{
            queryResults -> responseData.metros[currentMetro].displayTime = malloc(0);
        }

        transportMode = json_object_get(metro_data, "TransportMode");
        if(json_string_value(transportMode) != NULL){
            queryResults -> responseData.metros[currentMetro].transportMode = malloc(strlen(json_string_value(transportMode)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].transportMode, json_string_value(transportMode));
        }
        else{
            queryResults -> responseData.metros[currentMetro].transportMode = malloc(0);
        }

        lineNumber = json_object_get(metro_data, "LineNumber");
        if(json_string_value(lineNumber) != NULL){
            queryResults -> responseData.metros[currentMetro].lineNumber = malloc(strlen(json_string_value(lineNumber)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].lineNumber, json_string_value(lineNumber));
        }
        else{
            queryResults -> responseData.metros[currentMetro].lineNumber = malloc(0);
        }

        destination = json_object_get(metro_data, "Destination");
        if(json_string_value(destination) != NULL){
            queryResults -> responseData.metros[currentMetro].destination = malloc(strlen(json_string_value(destination)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].destination, json_string_value(destination));
        }
        else{
            queryResults -> responseData.metros[currentMetro].destination = malloc(0);
        }

        journeyDirection = json_object_get(metro_data, "JourneyDirection");
        queryResults -> responseData.metros[currentMetro].journeyDirection = (long)json_integer_value(journeyDirection);

        stopAreaName = json_object_get(metro_data, "StopAreaName");
        if(json_string_value(stopAreaName) != NULL){
            queryResults -> responseData.metros[currentMetro].stopAreaName = malloc(strlen(json_string_value(stopAreaName)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].stopAreaName, json_string_value(stopAreaName));
        }
        else{
            queryResults -> responseData.metros[currentMetro].stopAreaName = malloc(0);
        }

        stopAreaNumber = json_object_get(metro_data, "StopAreaNumber");
        queryResults -> responseData.metros[currentMetro].stopAreaNumber = (long)json_integer_value(stopAreaNumber);

        stopPointDesignation = json_object_get(metro_data, "StopPointDesignation");
        if(json_string_value(stopPointDesignation) != NULL){
            queryResults -> responseData.metros[currentMetro].stopPointDesignation = malloc(strlen(json_string_value(stopPointDesignation)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].stopPointDesignation, json_string_value(stopPointDesignation));
        }
        else{
            queryResults -> responseData.metros[currentMetro].stopPointDesignation = malloc(0);
        }

        stopPointNumber = json_object_get(metro_data, "StopPointNumber");
        queryResults -> responseData.metros[currentMetro].stopPointNumber = (long)json_integer_value(stopPointNumber);

        timeTabledDateTime = json_object_get(metro_data, "TimeTabledDateTime");
        if(json_string_value(timeTabledDateTime) != NULL){
            queryResults -> responseData.metros[currentMetro].timeTabledDateTime = malloc(strlen(json_string_value(timeTabledDateTime)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].timeTabledDateTime, json_string_value(timeTabledDateTime));
        }
        else{
            queryResults -> responseData.metros[currentMetro].timeTabledDateTime = malloc(0);
        }

        expectedDateTime = json_object_get(metro_data, "ExpectedDateTime");
        if(json_string_value(expectedDateTime) != NULL){
            queryResults -> responseData.metros[currentMetro].expectedDateTime = malloc(strlen(json_string_value(expectedDateTime)) + 1);
            strcpy(queryResults -> responseData.metros[currentMetro].expectedDateTime, json_string_value(expectedDateTime));
        }
        else{
            queryResults -> responseData.metros[currentMetro].expectedDateTime = malloc(0);
        }

        journeyNumber = json_object_get(metro_data, "JourneyNumber");
        queryResults -> responseData.metros[currentMetro].journeyNumber = (long)json_integer_value(journeyNumber);

        deviations = json_object_get(metro_data, "Deviations");
        queryResults -> responseData.metros[currentMetro].noOfDeviations = json_array_size(deviations);
        queryResults -> responseData.metros[currentMetro].deviations = malloc(sizeof(DeviationData) * queryResults -> responseData.metros[currentMetro].noOfDeviations);

        for(size_t currentDeviation = 0; currentDeviation < json_array_size(deviations); currentDeviation++){
            json_t *d_data;
            d_data = json_array_get(deviations, currentDeviation);

            text = json_object_get(d_data, "Text");
            if(json_string_value(text) != NULL){
                queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].text = malloc(strlen(json_string_value(text)) + 1);
                strcpy(queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].text, json_string_value(text));
            }
            else{
                queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].text = malloc(0);
            }

            consequence = json_object_get(d_data, "Consequence");
            if(json_string_value(consequence) != NULL){
                queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].consequence = malloc(strlen(json_string_value(consequence)) + 1);
                strcpy(queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].consequence, json_string_value(consequence));
            }
            else{
                queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].consequence = malloc(0);
            }

            importanceLevel = json_object_get(d_data, "ImportanceLevel");
            queryResults -> responseData.metros[currentMetro].deviations[currentDeviation].importanceLevel = (long)json_integer_value(importanceLevel);
        }
    }


    buses = json_object_get(responseData, "Buses");
    queryResults -> responseData.noOfBuses = (int)json_array_size(buses);
    queryResults -> responseData.buses = malloc(sizeof(BusData) * queryResults -> responseData.noOfBuses);

    for(size_t currentBus = 0; currentBus < json_array_size(buses); currentBus++){
        json_t *bus_data = json_array_get(buses, currentBus);

        groupOfLine = json_object_get(bus_data, "GroupOfLine");
        if(json_string_value(groupOfLine) != NULL){
            queryResults -> responseData.buses[currentBus].groupOfLine = malloc(strlen(json_string_value(groupOfLine)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].groupOfLine, json_string_value(groupOfLine));
        }
        else{
            queryResults -> responseData.buses[currentBus].groupOfLine = malloc(0);
        }

        displayTime = json_object_get(bus_data, "DisplayTime");
        if(json_string_value(displayTime) != NULL){
            queryResults -> responseData.buses[currentBus].displayTime = malloc(strlen(json_string_value(displayTime)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].displayTime, json_string_value(displayTime));
        }
        else{
            queryResults -> responseData.buses[currentBus].displayTime = malloc(0);
        }

        transportMode = json_object_get(bus_data, "TransportMode");
        if(json_string_value(transportMode) != NULL){
            queryResults -> responseData.buses[currentBus].transportMode = malloc(strlen(json_string_value(transportMode)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].transportMode, json_string_value(transportMode));
        }
        else{
            queryResults -> responseData.buses[currentBus].transportMode = malloc(0);
        }

        lineNumber = json_object_get(bus_data, "LineNumber");
        if(json_string_value(lineNumber) != NULL){
            queryResults -> responseData.buses[currentBus].lineNumber = malloc(strlen(json_string_value(lineNumber)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].lineNumber, json_string_value(lineNumber));
        }
        else{
            queryResults -> responseData.buses[currentBus].lineNumber = malloc(0);
        }

        destination = json_object_get(bus_data, "Destination");
        if(json_string_value(destination) != NULL){
            queryResults -> responseData.buses[currentBus].destination = malloc(strlen(json_string_value(destination)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].destination, json_string_value(destination));
        }
        else{
            queryResults -> responseData.buses[currentBus].destination = malloc(0);
        }

        journeyDirection = json_object_get(bus_data, "JourneyDirection");
        queryResults -> responseData.buses[currentBus].journeyDirection = (long)json_integer_value(journeyDirection);

        stopAreaName = json_object_get(bus_data, "StopAreaName");
        if(json_string_value(stopAreaName) != NULL){
            queryResults -> responseData.buses[currentBus].stopAreaName = malloc(strlen(json_string_value(stopAreaName)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].stopAreaName, json_string_value(stopAreaName));
        }
        else{
            queryResults -> responseData.buses[currentBus].stopAreaName = malloc(0);
        }

        stopAreaNumber = json_object_get(bus_data, "StopAreaNumber");
        queryResults -> responseData.buses[currentBus].stopAreaNumber = (long)json_integer_value(stopAreaNumber);

        stopPointDesignation = json_object_get(bus_data, "StopPointDesignation");
        if(json_string_value(stopPointDesignation) != NULL){
            queryResults -> responseData.buses[currentBus].stopPointDesignation = malloc(strlen(json_string_value(stopPointDesignation)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].stopPointDesignation, json_string_value(stopPointDesignation));
        }
        else{
            queryResults -> responseData.buses[currentBus].stopPointDesignation = malloc(0);
        }

        stopPointNumber = json_object_get(bus_data, "StopPointNumber");
        queryResults -> responseData.buses[currentBus].stopPointNumber = (long)json_integer_value(stopPointNumber);

        timeTabledDateTime = json_object_get(bus_data, "TimeTabledDateTime");
        if(json_string_value(timeTabledDateTime) != NULL){
            queryResults -> responseData.buses[currentBus].timeTabledDateTime = malloc(strlen(json_string_value(timeTabledDateTime)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].timeTabledDateTime, json_string_value(timeTabledDateTime));
        }
        else{
            queryResults -> responseData.buses[currentBus].timeTabledDateTime = malloc(0);
        }

        expectedDateTime = json_object_get(bus_data, "ExpectedDateTime");
        if(json_string_value(expectedDateTime) != NULL){
            queryResults -> responseData.buses[currentBus].expectedDateTime = malloc(strlen(json_string_value(expectedDateTime)) + 1);
            strcpy(queryResults -> responseData.buses[currentBus].expectedDateTime, json_string_value(expectedDateTime));
        }
        else{
            queryResults -> responseData.buses[currentBus].expectedDateTime = malloc(0);
        }

        journeyNumber = json_object_get(bus_data, "JourneyNumber");
        queryResults -> responseData.buses[currentBus].journeyNumber = (long)json_integer_value(journeyNumber);

        deviations = json_object_get(bus_data, "Deviations");
        queryResults -> responseData.buses[currentBus].noOfDeviations = json_array_size(deviations);
        queryResults -> responseData.buses[currentBus].deviations = malloc(sizeof(DeviationData) * queryResults -> responseData.buses[currentBus].noOfDeviations);

        for(size_t currentDeviation = 0; currentDeviation < json_array_size(deviations); currentDeviation++){
            json_t *d_data;
            d_data = json_array_get(deviations, currentDeviation);

            text = json_object_get(d_data, "Text");
            if(json_string_value(text) != NULL){
                queryResults -> responseData.buses[currentBus].deviations[currentDeviation].text = malloc(strlen(json_string_value(text)) + 1);
                strcpy(queryResults -> responseData.buses[currentBus].deviations[currentDeviation].text, json_string_value(text));
            }
            else{
                queryResults -> responseData.buses[currentBus].deviations[currentDeviation].text = malloc(0);
            }

            consequence = json_object_get(d_data, "Consequence");
            if(json_string_value(consequence) != NULL){
                queryResults -> responseData.buses[currentBus].deviations[currentDeviation].consequence = malloc(strlen(json_string_value(consequence)) + 1);
                strcpy(queryResults -> responseData.buses[currentBus].deviations[currentDeviation].consequence, json_string_value(consequence));
            }
            else{
                queryResults -> responseData.buses[currentBus].deviations[currentDeviation].consequence = malloc(0);
            }

            importanceLevel = json_object_get(d_data, "ImportanceLevel");
            queryResults -> responseData.buses[currentBus].deviations[currentDeviation].importanceLevel = (long)json_integer_value(importanceLevel);
        }
    }

    trains = json_object_get(responseData, "Trains");
    queryResults -> responseData.noOfTrains = (int)json_array_size(trains);
    queryResults -> responseData.trains = malloc(sizeof(TrainData) * queryResults -> responseData.noOfTrains);

    for(size_t currentTrain = 0; currentTrain < json_array_size(trains); currentTrain++){
        json_t *train_data = json_array_get(trains, currentTrain);

        secondaryDestinationName = json_object_get(train_data, "SecondaryDestinationName");
        if(json_string_value(secondaryDestinationName) != NULL){
            queryResults -> responseData.trains[currentTrain].secondaryDestinationName = malloc(strlen(json_string_value(secondaryDestinationName)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].secondaryDestinationName, json_string_value(secondaryDestinationName));
        }
        else{
            queryResults -> responseData.trains[currentTrain].secondaryDestinationName = malloc(0);
        }

        groupOfLine = json_object_get(train_data, "GroupOfLine");
        if(json_string_value(groupOfLine) != NULL){
            queryResults -> responseData.trains[currentTrain].groupOfLine = malloc(strlen(json_string_value(groupOfLine)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].groupOfLine, json_string_value(groupOfLine));
        }
        else{
            queryResults -> responseData.trains[currentTrain].groupOfLine = malloc(0);
        }

        displayTime = json_object_get(train_data, "DisplayTime");
        if(json_string_value(displayTime) != NULL){
            queryResults -> responseData.trains[currentTrain].displayTime = malloc(strlen(json_string_value(displayTime)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].displayTime, json_string_value(displayTime));
        }
        else{
            queryResults -> responseData.trains[currentTrain].displayTime = malloc(0);
        }

        transportMode = json_object_get(train_data, "TransportMode");
        if(json_string_value(transportMode) != NULL){
            queryResults -> responseData.trains[currentTrain].transportMode = malloc(strlen(json_string_value(transportMode)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].transportMode, json_string_value(transportMode));
        }
        else{
            queryResults -> responseData.trains[currentTrain].transportMode = malloc(0);
        }

        lineNumber = json_object_get(train_data, "LineNumber");
        if(json_string_value(lineNumber) != NULL){
            queryResults -> responseData.trains[currentTrain].lineNumber = malloc(strlen(json_string_value(lineNumber)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].lineNumber, json_string_value(lineNumber));
        }
        else{
            queryResults -> responseData.trains[currentTrain].lineNumber = malloc(0);
        }

        destination = json_object_get(train_data, "Destination");
        if(json_string_value(destination) != NULL){
            queryResults -> responseData.trains[currentTrain].destination = malloc(strlen(json_string_value(destination)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].destination, json_string_value(destination));
        }
        else{
            queryResults -> responseData.trains[currentTrain].destination = malloc(0);
        }

        journeyDirection = json_object_get(train_data, "JourneyDirection");
        queryResults -> responseData.trains[currentTrain].journeyDirection = (long)json_integer_value(journeyDirection);

        stopAreaName = json_object_get(train_data, "StopAreaName");
        if(json_string_value(stopAreaName) != NULL){
            queryResults -> responseData.trains[currentTrain].stopAreaName = malloc(strlen(json_string_value(stopAreaName)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].stopAreaName, json_string_value(stopAreaName));
        }
        else{
            queryResults -> responseData.trains[currentTrain].stopAreaName = malloc(0);
        }

        stopAreaNumber = json_object_get(train_data, "StopAreaNumber");
        queryResults -> responseData.trains[currentTrain].stopAreaNumber = (long)json_integer_value(stopAreaNumber);

        stopPointDesignation = json_object_get(train_data, "StopPointDesignation");
        if(json_string_value(stopPointDesignation) != NULL){
            queryResults -> responseData.trains[currentTrain].stopPointDesignation = malloc(strlen(json_string_value(stopPointDesignation)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].stopPointDesignation, json_string_value(stopPointDesignation));
        }
        else{
            queryResults -> responseData.trains[currentTrain].stopPointDesignation = malloc(0);
        }

        stopPointNumber = json_object_get(train_data, "StopPointNumber");
        queryResults -> responseData.trains[currentTrain].stopPointNumber = (long)json_integer_value(stopPointNumber);

        timeTabledDateTime = json_object_get(train_data, "TimeTabledDateTime");
        if(json_string_value(timeTabledDateTime) != NULL){
            queryResults -> responseData.trains[currentTrain].timeTabledDateTime = malloc(strlen(json_string_value(timeTabledDateTime)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].timeTabledDateTime, json_string_value(timeTabledDateTime));
        }
        else{
            queryResults -> responseData.trains[currentTrain].timeTabledDateTime = malloc(0);
        }

        expectedDateTime = json_object_get(train_data, "ExpectedDateTime");
        if(json_string_value(expectedDateTime) != NULL){
            queryResults -> responseData.trains[currentTrain].expectedDateTime = malloc(strlen(json_string_value(expectedDateTime)) + 1);
            strcpy(queryResults -> responseData.trains[currentTrain].expectedDateTime, json_string_value(expectedDateTime));
        }
        else{
            queryResults -> responseData.trains[currentTrain].expectedDateTime = malloc(0);
        }

        journeyNumber = json_object_get(train_data, "JourneyNumber");
        queryResults -> responseData.trains[currentTrain].journeyNumber = (long)json_integer_value(journeyNumber);

        deviations = json_object_get(train_data, "Deviations");
        queryResults -> responseData.trains[currentTrain].noOfDeviations = json_array_size(deviations);
        queryResults -> responseData.trains[currentTrain].deviations = malloc(sizeof(DeviationData) * queryResults -> responseData.trains[currentTrain].noOfDeviations);

        for(size_t currentDeviation = 0; currentDeviation < json_array_size(deviations); currentDeviation++){
            json_t *d_data;
            d_data = json_array_get(deviations, currentDeviation);

            text = json_object_get(d_data, "Text");
            if(json_string_value(text) != NULL){
                queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].text = malloc(strlen(json_string_value(text)) + 1);
                strcpy(queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].text, json_string_value(text));
            }
            else{
                queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].text = malloc(0);
            }

            consequence = json_object_get(d_data, "Consequence");
            if(json_string_value(consequence) != NULL){
                queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].consequence = malloc(strlen(json_string_value(consequence)) + 1);
                strcpy(queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].consequence, json_string_value(consequence));
            }
            else{
                queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].consequence = malloc(0);
            }

            importanceLevel = json_object_get(d_data, "ImportanceLevel");
            queryResults -> responseData.trains[currentTrain].deviations[currentDeviation].importanceLevel = (long)json_integer_value(importanceLevel);
        }
    }

    trams = json_object_get(responseData, "Trams");
    queryResults -> responseData.noOfTrams = (int)json_array_size(trams);
    queryResults -> responseData.trams = malloc(sizeof(TramData) * queryResults -> responseData.noOfTrams);

    for(size_t currentTram = 0; currentTram < json_array_size(trams); currentTram++){
        json_t *tram_data = json_array_get(trams, currentTram);

        groupOfLine = json_object_get(tram_data, "GroupOfLine");
        if(json_string_value(groupOfLine) != NULL){
            queryResults -> responseData.trams[currentTram].groupOfLine = malloc(strlen(json_string_value(groupOfLine)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].groupOfLine, json_string_value(groupOfLine));
        }
        else{
            queryResults -> responseData.trams[currentTram].groupOfLine = malloc(0);
        }

        displayTime = json_object_get(tram_data, "DisplayTime");
        if(json_string_value(displayTime) != NULL){
            queryResults -> responseData.trams[currentTram].displayTime = malloc(strlen(json_string_value(displayTime)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].displayTime, json_string_value(displayTime));
        }
        else{
            queryResults -> responseData.trams[currentTram].displayTime = malloc(0);
        }

        transportMode = json_object_get(tram_data, "TransportMode");
        if(json_string_value(transportMode) != NULL){
            queryResults -> responseData.trams[currentTram].transportMode = malloc(strlen(json_string_value(transportMode)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].transportMode, json_string_value(transportMode));
        }
        else{
            queryResults -> responseData.trams[currentTram].transportMode = malloc(0);
        }

        lineNumber = json_object_get(tram_data, "LineNumber");
        if(json_string_value(lineNumber) != NULL){
            queryResults -> responseData.trams[currentTram].lineNumber = malloc(strlen(json_string_value(lineNumber)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].lineNumber, json_string_value(lineNumber));
        }
        else{
            queryResults -> responseData.trams[currentTram].lineNumber = malloc(0);
        }

        destination = json_object_get(tram_data, "Destination");
        if(json_string_value(destination) != NULL){
            queryResults -> responseData.trams[currentTram].destination = malloc(strlen(json_string_value(destination)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].destination, json_string_value(destination));
        }
        else{
            queryResults -> responseData.trams[currentTram].destination = malloc(0);
        }

        journeyDirection = json_object_get(tram_data, "JourneyDirection");
        queryResults -> responseData.trams[currentTram].journeyDirection = (long)json_integer_value(journeyDirection);

        stopAreaName = json_object_get(tram_data, "StopAreaName");
        if(json_string_value(stopAreaName) != NULL){
            queryResults -> responseData.trams[currentTram].stopAreaName = malloc(strlen(json_string_value(stopAreaName)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].stopAreaName, json_string_value(stopAreaName));
        }
        else{
            queryResults -> responseData.trams[currentTram].stopAreaName = malloc(0);
        }

        stopAreaNumber = json_object_get(tram_data, "StopAreaNumber");
        queryResults -> responseData.trams[currentTram].stopAreaNumber = (long)json_integer_value(stopAreaNumber);

        stopPointDesignation = json_object_get(tram_data, "StopPointDesignation");
        if(json_string_value(stopPointDesignation) != NULL){
            queryResults -> responseData.trams[currentTram].stopPointDesignation = malloc(strlen(json_string_value(stopPointDesignation)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].stopPointDesignation, json_string_value(stopPointDesignation));
        }
        else{
            queryResults -> responseData.trams[currentTram].stopPointDesignation = malloc(0);
        }

        stopPointNumber = json_object_get(tram_data, "StopPointNumber");
        queryResults -> responseData.trams[currentTram].stopPointNumber = (long)json_integer_value(stopPointNumber);

        timeTabledDateTime = json_object_get(tram_data, "TimeTabledDateTime");
        if(json_string_value(timeTabledDateTime) != NULL){
            queryResults -> responseData.trams[currentTram].timeTabledDateTime = malloc(strlen(json_string_value(timeTabledDateTime)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].timeTabledDateTime, json_string_value(timeTabledDateTime));
        }
        else{
            queryResults -> responseData.trams[currentTram].timeTabledDateTime = malloc(0);
        }

        expectedDateTime = json_object_get(tram_data, "ExpectedDateTime");
        if(json_string_value(expectedDateTime) != NULL){
            queryResults -> responseData.trams[currentTram].expectedDateTime = malloc(strlen(json_string_value(expectedDateTime)) + 1);
            strcpy(queryResults -> responseData.trams[currentTram].expectedDateTime, json_string_value(expectedDateTime));
        }
        else{
            queryResults -> responseData.trams[currentTram].expectedDateTime = malloc(0);
        }

        journeyNumber = json_object_get(tram_data, "JourneyNumber");
        queryResults -> responseData.trams[currentTram].journeyNumber = (long)json_integer_value(journeyNumber);

        deviations = json_object_get(tram_data, "Deviations");
        queryResults -> responseData.trams[currentTram].noOfDeviations = json_array_size(deviations);
        queryResults -> responseData.trams[currentTram].deviations = malloc(sizeof(DeviationData) * queryResults -> responseData.trams[currentTram].noOfDeviations);

        for(size_t currentDeviation = 0; currentDeviation < json_array_size(deviations); currentDeviation++){
            json_t *d_data;
            d_data = json_array_get(deviations, currentDeviation);

            text = json_object_get(d_data, "Text");
            if(json_string_value(text) != NULL){
                queryResults -> responseData.trams[currentTram].deviations[currentDeviation].text = malloc(strlen(json_string_value(text)) + 1);
                strcpy(queryResults -> responseData.trams[currentTram].deviations[currentDeviation].text, json_string_value(text));
            }
            else{
                queryResults -> responseData.trams[currentTram].deviations[currentDeviation].text = malloc(0);
            }

            consequence = json_object_get(d_data, "Consequence");
            if(json_string_value(consequence) != NULL){
                queryResults -> responseData.trams[currentTram].deviations[currentDeviation].consequence = malloc(strlen(json_string_value(consequence)) + 1);
                strcpy(queryResults -> responseData.trams[currentTram].deviations[currentDeviation].consequence, json_string_value(consequence));
            }
            else{
                queryResults -> responseData.trams[currentTram].deviations[currentDeviation].consequence = malloc(0);
            }

            importanceLevel = json_object_get(d_data, "ImportanceLevel");
            queryResults -> responseData.trams[currentTram].deviations[currentDeviation].importanceLevel = (long)json_integer_value(importanceLevel);
        }
    }

    ships = json_object_get(responseData, "Ships");
    queryResults -> responseData.noOfShips = (int)json_array_size(ships);
    queryResults -> responseData.ships = malloc(sizeof(ShipData) * queryResults -> responseData.noOfShips);

    for(size_t currentShip = 0; currentShip < json_array_size(ships); currentShip++){
        json_t *ship_data = json_array_get(ships, currentShip);

        groupOfLine = json_object_get(ship_data, "GroupOfLine");
        if(json_string_value(groupOfLine) != NULL){
            queryResults -> responseData.ships[currentShip].groupOfLine = malloc(strlen(json_string_value(groupOfLine)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].groupOfLine, json_string_value(groupOfLine));
        }
        else{
            queryResults -> responseData.ships[currentShip].groupOfLine = malloc(0);
        }

        displayTime = json_object_get(ship_data, "DisplayTime");
        if(json_string_value(displayTime) != NULL){
            queryResults -> responseData.ships[currentShip].displayTime = malloc(strlen(json_string_value(displayTime)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].displayTime, json_string_value(displayTime));
        }
        else{
            queryResults -> responseData.ships[currentShip].displayTime = malloc(0);
        }

        transportMode = json_object_get(ship_data, "TransportMode");
        if(json_string_value(transportMode) != NULL){
            queryResults -> responseData.ships[currentShip].transportMode = malloc(strlen(json_string_value(transportMode)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].transportMode, json_string_value(transportMode));
        }
        else{
            queryResults -> responseData.ships[currentShip].transportMode = malloc(0);
        }

        lineNumber = json_object_get(ship_data, "LineNumber");
        if(json_string_value(lineNumber) != NULL){
            queryResults -> responseData.ships[currentShip].lineNumber = malloc(strlen(json_string_value(lineNumber)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].lineNumber, json_string_value(lineNumber));
        }
        else{
            queryResults -> responseData.ships[currentShip].lineNumber = malloc(0);
        }

        destination = json_object_get(ship_data, "Destination");
        if(json_string_value(destination) != NULL){
            queryResults -> responseData.ships[currentShip].destination = malloc(strlen(json_string_value(destination)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].destination, json_string_value(destination));
        }
        else{
            queryResults -> responseData.ships[currentShip].destination = malloc(0);
        }

        journeyDirection = json_object_get(ship_data, "JourneyDirection");
        queryResults -> responseData.ships[currentShip].journeyDirection = (long)json_integer_value(journeyDirection);

        stopAreaName = json_object_get(ship_data, "StopAreaName");
        if(json_string_value(stopAreaName) != NULL){
            queryResults -> responseData.ships[currentShip].stopAreaName = malloc(strlen(json_string_value(stopAreaName)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].stopAreaName, json_string_value(stopAreaName));
        }
        else{
            queryResults -> responseData.ships[currentShip].stopAreaName = malloc(0);
        }

        stopAreaNumber = json_object_get(ship_data, "StopAreaNumber");
        queryResults -> responseData.ships[currentShip].stopAreaNumber = (long)json_integer_value(stopAreaNumber);

        stopPointDesignation = json_object_get(ship_data, "StopPointDesignation");
        if(json_string_value(stopPointDesignation) != NULL){
            queryResults -> responseData.ships[currentShip].stopPointDesignation = malloc(strlen(json_string_value(stopPointDesignation)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].stopPointDesignation, json_string_value(stopPointDesignation));
        }
        else{
            queryResults -> responseData.ships[currentShip].stopPointDesignation= malloc(0);
        }

        stopPointNumber = json_object_get(ship_data, "StopPointNumber");
        queryResults -> responseData.ships[currentShip].stopPointNumber = (long)json_integer_value(stopPointNumber);

        timeTabledDateTime = json_object_get(ship_data, "TimeTabledDateTime");
        if(json_string_value(timeTabledDateTime) != NULL){
            queryResults -> responseData.ships[currentShip].timeTabledDateTime = malloc(strlen(json_string_value(timeTabledDateTime)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].timeTabledDateTime, json_string_value(timeTabledDateTime));
        }
        else{
            queryResults -> responseData.ships[currentShip].timeTabledDateTime = malloc(0);
        }

        expectedDateTime = json_object_get(ship_data, "ExpectedDateTime");
        if(json_string_value(expectedDateTime) != NULL){
            queryResults -> responseData.ships[currentShip].expectedDateTime = malloc(strlen(json_string_value(expectedDateTime)) + 1);
            strcpy(queryResults -> responseData.ships[currentShip].expectedDateTime, json_string_value(expectedDateTime));
        }
        else{
            queryResults -> responseData.ships[currentShip].expectedDateTime = malloc(0);
        }

        journeyNumber = json_object_get(ship_data, "JourneyNumber");
        queryResults -> responseData.ships[currentShip].journeyNumber = (long)json_integer_value(journeyNumber);

        deviations = json_object_get(ship_data, "Deviations");
        queryResults -> responseData.ships[currentShip].noOfDeviations = json_array_size(deviations);
        queryResults -> responseData.ships[currentShip].deviations = malloc(sizeof(DeviationData) * queryResults -> responseData.ships[currentShip].noOfDeviations);

        for(size_t currentDeviation = 0; currentDeviation < json_array_size(deviations); currentDeviation++){
            json_t *d_data;
            d_data = json_array_get(deviations, currentDeviation);

            text = json_object_get(d_data, "Text");
            if(json_string_value(text) != NULL){
                queryResults -> responseData.ships[currentShip].deviations[currentDeviation].text = malloc(strlen(json_string_value(text)) + 1);
                strcpy(queryResults -> responseData.ships[currentShip].deviations[currentDeviation].text, json_string_value(text));
            }
            else{
                queryResults -> responseData.ships[currentShip].deviations[currentDeviation].text = malloc(0);
            }

            consequence = json_object_get(d_data, "Consequence");
            if(json_string_value(consequence) != NULL){
                queryResults -> responseData.ships[currentShip].deviations[currentDeviation].consequence = malloc(strlen(json_string_value(consequence)) + 1);
                strcpy(queryResults -> responseData.ships[currentShip].deviations[currentDeviation].consequence, json_string_value(consequence));
            }
            else{
                queryResults -> responseData.ships[currentShip].deviations[currentDeviation].consequence = malloc(0);
            }

            importanceLevel = json_object_get(d_data, "ImportanceLevel");
            queryResults -> responseData.ships[currentShip].deviations[currentDeviation].importanceLevel = (long)json_integer_value(importanceLevel);
        }
    }

    return queryResults;
}

static char * generateURL(long station){
    char *placeholder = "http://api.sl.se/api2/realtimedeparturesV4.json?key=%s&siteid=%s&timewindow=60 ";
    int len = snprintf(NULL, 0, placeholder, REALTIDSINFO_API_KEY, (char *)station);
    char *url = malloc(len);
    snprintf(url, len, placeholder, REALTIDSINFO_API_KEY, (char *)station);
    return url;
}


