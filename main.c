#include <stdio.h>
#include <string.h>
#include <locale.h>
#include "weather.h"


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
        
    }
    
    return 0;
}