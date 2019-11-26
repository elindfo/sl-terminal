#include <stdlib.h>

#ifndef SLTERMINAL_CURL_H
#define SLTERMINAL_CURL_H

#define BUFFER_SIZE  (256 * 1024)

struct write_result {
    char *data;
    int pos;
};

typedef struct write_result WriteResult;

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream);
char *request(const char *url);

#endif
