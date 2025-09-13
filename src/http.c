#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include <curl/curl.h>

// This is the callback libcurl will call with response chunks
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) 
{
    size_t realsize = size * nmemb;
    http *mem = (http *)userp;

    char *ptr = realloc(mem->data, mem->size + realsize + 1); // +1 for '\0'
    if (ptr == NULL) {
        // out of memory
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = '\0'; // null terminate

    return realsize;
}

int http_get(const char *url, http *response) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("1111\n");
        return -1; // Curl initialization failed
    }

    response->data = malloc(1);
    response->size = 0;

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set up a write callback to capture the response data
    // WRITEFUNCTION: how to handle incomming data.
    // If set to NULL, it uses its default callback to print to console.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {         
        printf("222222\n");   
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return -2; // Request failed
    }

    curl_easy_cleanup(curl);
    return 0; // Success
}