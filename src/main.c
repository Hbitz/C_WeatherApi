#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "weather.h"
#include "http.h"
#include <curl/curl.h>

http result;

int main()
{         
    setlocale(LC_ALL, "");   
    printf("Welcome\n");

    while(1)
    {
        print_all_cities();

        printf("Select one of the following cities:");
        char city[max_city_name];
        if (scanf("%29s", city) != 1)
        {
            printf("Error scanning city name\n");
            continue;
        }
        char lat[max_lat];
        char lon[max_lon];
        char url[max_url];

        if (!find_city(city, lat, lon)) {
            printf("Error: \"%s\" not found in the list. Try again.\n", city);
            continue; // start over
        }

        build_weather_url(lat, lon, url);

        printf("Success!\n");
        printf("URL: \"%s\"\r\n", url);
        printf("Now trying to use curl\n");

        // http_get returns 0 if succesfull, or other negative number for different kinds of errors
        // in C, 0 = success, nonzero = failure
        // However, evaluating if(express) with 0 in C returns false.
        if (http_get(url, &result)) {
            printf("HTTP GET request failed\n");
            continue; // start over
        }

        if (result.data == NULL) {
            printf("Buffer is NULL!\n");
        } else {
            printf("Buffer has %lu bytes\n", (unsigned long)result.size);
        }

        // If we get here, the get request was successfully.
        printf("Now printing data...\n");
        printf("%s\n", result.data);
                
    }
    
    return 0;
}