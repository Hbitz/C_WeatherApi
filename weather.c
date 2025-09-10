#include <stdio.h>
#include <string.h>
#include "weather.h"

const char* cities =
    "Stockholm:59.3293:18.0686\n"
    "Göteborg:57.7089:11.9746\n"
    "Malmö:55.6050:13.0038\n"
    "Uppsala:59.8586:17.6389\n"
    "Västerås:59.6099:16.5448\n"
    "Örebro:59.2741:15.2066\n"
    "Linköping:58.4109:15.6216\n"
    "Helsingborg:56.0465:12.6945\n"
    "Jönköping:57.7815:14.1562\n"
    "Norrköping:58.5877:16.1924\n"
    "Lund:55.7047:13.1910\n"
    "Gävle:60.6749:17.1413\n"
    "Sundsvall:62.3908:17.3069\n"
    "Umeå:63.8258:20.2630\n"
    "Luleå:65.5848:22.1567\n"
    "Kiruna:67.8558:20.2253\n";

void print_all_cities(void) {
    const char* p = cities;
    char city_name[max_city_name];
    char latitude[max_lat];
    char longitude[max_lon];

    while (*p) {
        // Select one row at a time
        const char* line_start = p;
        const char* nl = strchr(line_start, '\n');
        
        // Since all city entries always end in a newline, we can safely assume nl is not NULL here.
        // But to ensure consistency, we handle the case where it might be NULL.
        size_t line_len = nl ? (size_t)(nl - line_start) : strlen(line_start);
            
        // Copy to a temporary buffer
        char line[256];

        // This line ensures we don't overflow the buffer
        if (line_len >= sizeof(line)) line_len = sizeof(line) - 1;
        
        // This line copies the relevant part of the string
        memcpy(line, line_start, line_len);
        line[line_len] = '\0';

        // Use sscanf to extract city name, latitude and longitude
        if (sscanf(line, "%29[^:]:%15[^:]:%15[^\n]", city_name, latitude, longitude) == 3) {
            printf("%s\n", city_name);
        }

        p = nl ? nl + 1 : line_start + line_len;
    }
}

int find_city(const char* input_city, char* latitude, char* longitude) {
    const char* p = cities;
    char city_name[max_city_name];

    while (*p) {
        const char* line_start = p;
        const char* nl = strchr(line_start, '\n');
        size_t line_len = nl ? (size_t)(nl - line_start) : strlen(line_start);

        char line[256];
        if (line_len >= sizeof(line)) line_len = sizeof(line) - 1;
        memcpy(line, line_start, line_len);
        line[line_len] = '\0';

        if (sscanf(line, "%29[^:]:%15[^:]:%15[^\n]", city_name, latitude, longitude) == 3) {
            if (strcmp(city_name, input_city) == 0) {
                return 1; 
            }
        }

        p = nl ? nl + 1 : line_start + line_len;
    }

    return 0; 
}

void build_weather_url(const char* latitude, const char* longitude, char* url) {
    snprintf(url, max_url,
        "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&current_weather=true",
        latitude, longitude);
}