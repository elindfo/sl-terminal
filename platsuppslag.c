#include "platsuppslag.h"
#include "curl.h"
#include <jansson.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

const char * PLATSUPPSLAG_API_KEY = "{Replace with YOUR api key}"; // https://www.trafiklab.se/api/sl-platsuppslag

static char * generateURL(char *station);

long retrieve_site_id(char * station, json_t *pu_root){

    char *pu_url = NULL;
    char *pu_json_response;

    json_error_t pu_error;

    pu_url = generateURL(station);
    pu_json_response = request(pu_url);
    free(pu_url);

    if(!pu_json_response){
        exit(1);
    }

    pu_root = json_loads(pu_json_response, 0, &pu_error);
    free(pu_json_response);

    if(!pu_root)
    {
        fprintf(stderr, "pu_error: on line %d: %s\n", pu_error.line, pu_error.text);
        exit(1);
    }

    json_t *responseData;
    responseData = json_object_get(pu_root, "ResponseData");

    if(!json_is_array(responseData))
    {
        fprintf(stderr, "error: responseData is not an array\n");
        json_decref(pu_root);
        exit(1);
    }

    json_t *data, *siteId;

    data = json_array_get(responseData, 0);
    siteId = json_object_get(data, "SiteId");

    return (long)json_string_value(siteId);
}

static char* generateURL(char *station){
    const char *placeholder = "http://api.sl.se/api2/typeahead.json?key=%s&searchstring=%s&stationsonly=True&maxresults=1 ";
    int len = snprintf(NULL, 0, placeholder, PLATSUPPSLAG_API_KEY, station);
    char *url = malloc(len);
    snprintf(url, len, placeholder, PLATSUPPSLAG_API_KEY, station);
    printf("url: %s\n", url);
    return url;
}
