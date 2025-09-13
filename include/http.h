#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t size;
} http;

int http_get(const char *url, http *response);
//void http_response(http *response);

#endif