#include <jansson.h>

#ifndef REALTID_H
#define REALTID_H

const char * PLATSUPPSLAG_API_KEY;
long retrieve_site_id(char * station, json_t * pu_root);

#endif
