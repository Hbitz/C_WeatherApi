#ifndef WEATHER_H
#define WEATHER_H

#define max_city_name 30
#define max_lat 16
#define max_lon 16
#define max_url 256

// Print out cities
void print_all_cities(void);

// Find city and return lat/lon if found
int find_city(const char* input_city, char* latitude, char* longitude);

// Build the URL to the weather API
void build_weather_url(const char* latitude, const char* longitude, char* url);

#endif