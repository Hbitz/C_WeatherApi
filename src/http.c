#include <stdio.h>
#include <stdlib.h>
#include "http.h"
#include <curl/curl.h>

int http_get(const char *url, http *response) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return -1; // Curl initialization failed
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set up a write callback to capture the response data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return -2; // Request failed
    }

    curl_easy_cleanup(curl);
    return 0; // Success
}


