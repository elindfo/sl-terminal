#include <jansson.h>
#include "SLResponse.h"

#ifndef REALTIDSINFO_H
#define REALTIDSINFO_H

const char *REALTIDSINFO_API_KEY;
SLResponse *query(long site_id, json_t *ri_root);

#endif
