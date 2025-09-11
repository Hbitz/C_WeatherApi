#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "weather.h"

#include <curl/curl.h>

// size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignore)
// {
//     size_t bytes = itemsize * nitems;
//     printf("New data (%zu bytes)\n", bytes);
//     return bytes;
// }

int main()
{         
    setlocale(LC_ALL, "");   
    printf("Welcome\n");

    while(1)
    {
        print_all_cities();

        printf("Select one of the following cities:");
        char city[max_city_name];
        scanf("%29s", city);

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

        // 
        CURL *curl = curl_easy_init();

        if (!curl) {
            printf("curl init failed\n");
            return EXIT_FAILURE;
        }

        // set options
         curl_easy_setopt(curl, CURLOPT_URL, url);
         // By default it prints out to terminal.
         // But here we set a callback function "got_data"
         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

         // perform out action
         CURLcode result = curl_easy_perform(curl);
         if (result != CURLE_OK)
         {
            printf("ERROR\n");
         }

         curl_easy_cleanup(curl);
        
    }
    
    return 0;
}