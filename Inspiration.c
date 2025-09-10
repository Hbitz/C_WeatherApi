// main.c
#include <stdio.h>
#include <string.h>
#include <locale.h>

int main(void) {
    // Sätt lokal så att svenska tecken (å, ä, ö) skrivs/visas korrekt i terminalen
    setlocale(LC_ALL, "");

    // Hårdkodad "databas"
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

    // Buffertar
    char input[128];
    char city[128], name[128];
    char lat[64], lon[64];
    char url[256];

    printf("Välkommen! Detta program bygger en väder-URL från vald stad.\n");

    // Huvudloop: visa lista, fråga, slå upp, bygg URL, skriv ut, börja om
    while (1) {
        // 1) Skriv ut listan med städer (endast stadsnamn före första ':')
        printf("\nTillgängliga städer:\n");
        {
            const char* p = cities;
            while (*p) {
                // Plocka en rad i taget
                const char* line_start = p;
                const char* nl = strchr(line_start, '\n');
                // Since all city entries always end in a newline, we can safely assume nl is not NULL here.
                // But to ensure consistency, we handle the case where it might be NULL.
                size_t line_len = nl ? (size_t)(nl - line_start) : strlen(line_start);

                // Kopiera raden till en temporär buffert
                char line[256];
                // This line ensures we don't overflow the buffer
                if (line_len >= sizeof(line)) line_len = sizeof(line) - 1;
                // This line copies the relevant part of the string
                memcpy(line, line_start, line_len);
                line[line_len] = '\0';

                // Dela upp "Stad:lat:lon" -> skriv bara ut Stad
                if (sscanf(line, "%127[^:]:%63[^:]:%63[^\n]", name, lat, lon) == 3) {
                    printf("%s\n", name);
                }

                // Hoppa till nästa rad
                p = nl ? nl + 1 : line_start + line_len;
            }
        }

        // 2) Fråga användaren
        printf("\nAnge stad (exakt stavning, inkl. å/ä/ö): ");
        if (!fgets(input, sizeof(input), stdin)) {
            // EOF/avbrott: avsluta snyggt
            printf("\nIngen inmatning. Avslutar.\n");
            return 0;
        }
        // Ta bort ev. avslutande radbrytning
        size_t len = strlen(input);
        if (len > 0 && (input[len - 1] == '\n' || input[len - 1] == '\r')) {
            input[len - 1] = '\0';
            len--;
            if (len > 0 && input[len - 1] == '\r') { // hantera \r\n
                input[len - 1] = '\0';
            }
        }

        // Kopiera till city-bufferten
        strncpy(city, input, sizeof(city) - 1);
        city[sizeof(city) - 1] = '\0';

        // 3) Slå upp stadens koordinater i "databasen" med sscanf per rad
        int found = 0;
        {
            const char* p = cities;
            while (*p && !found) {
                const char* line_start = p;
                const char* nl = strchr(line_start, '\n');
                size_t line_len = nl ? (size_t)(nl - line_start) : strlen(line_start);

                char line[256];
                if (line_len >= sizeof(line)) line_len = sizeof(line) - 1;
                memcpy(line, line_start, line_len);
                line[line_len] = '\0';

                // Förväntat format: Stad:lat:lon
                // Vi använder sscanf för att plocka ut delarna
                if (sscanf(line, "%127[^:]:%63[^:]:%63[^\n]", name, lat, lon) == 3) {
                    if (strcmp(name, city) == 0) {
                        found = 1;
                        break;
                    }
                }

                p = nl ? nl + 1 : line_start + line_len;
            }
        }

        if (!found) {
            printf("Fel: \"%s\" finns inte i listan. Försök igen.\n", city);
            continue; // börja om
        }

        // 4) Bygg URL med sprintf enligt formatet
        // https://api.open-meteo.com/v1/forecast?latitude=<lat>&longitude=<lon>&current_weather=true
        // lat/lon är redan strängar från "databasen"
        snprintf(
            url, sizeof(url),
            "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&current_weather=true",
            lat, lon
        );

        // 5) Skriv ut URL exakt enligt krav
        printf("URL: \"%s\"\r\n", url);

        // 6) Börja om (loopen fortsätter)
    }

    return 0;
}