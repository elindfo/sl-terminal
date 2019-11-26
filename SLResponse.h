#include <jansson.h>

#ifndef SLRESPONSE_H
#define SLRESPONSE_H

#define SL_RESPONSE_STRING_BUFFER 256

struct stop_point_deviation_data{

};
typedef struct stop_point_deviation_data StopPointDeviationData;

struct deviation_data{
    char        *text;
    char        *consequence;
    long        importanceLevel;
};
typedef struct deviation_data DeviationData;

struct metro_data{
    char                *groupOfLine;
    char                *displayTime;
    char                *transportMode;
    char                *lineNumber;
    char                *destination;
    long                journeyDirection;
    char                *stopAreaName;
    long                stopAreaNumber;
    char                *stopPointDesignation;
    long                stopPointNumber;
    char                *timeTabledDateTime;
    char                *expectedDateTime;
    long                journeyNumber;
    DeviationData       *deviations;
    int                 noOfDeviations;
};
typedef struct metro_data MetroData;

struct bus_data{
    char                *groupOfLine;
    char                *displayTime;
    char                *transportMode;
    char                *lineNumber;
    char                *destination;
    long                journeyDirection;
    char                *stopAreaName;
    long                stopAreaNumber;
    long                stopPointNumber;
    char                *stopPointDesignation;
    char                *timeTabledDateTime;
    char                *expectedDateTime;
    long                journeyNumber;
    DeviationData       *deviations;
    int                 noOfDeviations;
};
typedef struct bus_data BusData;

struct train_data{
    char                *secondaryDestinationName;
    char                *groupOfLine;
    char                *displayTime;
    char                *transportMode;
    char                *lineNumber;
    char                *destination;
    long                journeyDirection;
    char                *stopAreaName;
    long                stopAreaNumber;
    long                stopPointNumber;
    char                *stopPointDesignation;
    char                *timeTabledDateTime;
    char                *expectedDateTime;
    long                journeyNumber;
    DeviationData       *deviations;
    int                 noOfDeviations;
};
typedef struct train_data TrainData;

struct tram_data{
    char                *groupOfLine;
    char                *displayTime;
    char                *transportMode;
    char                *lineNumber;
    char                *destination;
    long                journeyDirection;
    char                *stopAreaName;
    long                stopAreaNumber;
    long                stopPointNumber;
    char                *stopPointDesignation;
    char                *timeTabledDateTime;
    char                *expectedDateTime;
    long                journeyNumber;
    DeviationData       *deviations;
    int                 noOfDeviations;
};
typedef struct tram_data TramData;

struct ship_data{
    char                *groupOfLine;
    char                *displayTime;
    char                *transportMode;
    char                *lineNumber;
    char                *destination;
    long                journeyDirection;
    char                *stopAreaName;
    long                stopAreaNumber;
    long                stopPointNumber;
    char                *stopPointDesignation;
    char                *timeTabledDateTime;
    char                *expectedDateTime;
    long                journeyNumber;
    DeviationData       *deviations;
    int                 noOfDeviations;
};
typedef struct ship_data ShipData;

struct response_data{
    char                                *latestUpdate;
    long                                dataAge;
    MetroData                           *metros;
    int                                 noOfMetros;
    BusData                             *buses;
    int                                 noOfBuses;
    TrainData                           *trains;
    int                                 noOfTrains;
    TramData                            *trams;
    int                                 noOfTrams;
    ShipData                            *ships;
    int                                 noOfShips;
    StopPointDeviationData              *stopPointDeviations;
    int                                 noOfStopPointDeviations;
};
typedef struct response_data ResponseData;

struct sl_response{
    long                    statusCode;
    char                    *message;
    long                    executionTime;
    ResponseData            responseData; //Working on
};
typedef struct sl_response SLResponse;

#endif
