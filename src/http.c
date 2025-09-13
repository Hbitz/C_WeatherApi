#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include <curl/curl.h>

/* This is the callback libcurl will call for ecah chunk of data it recieves from the server.
/ Parameters:
  void *contents:  pointer to the recieved bytes
  size_t size: size of each element
  nmemb: number of elements receieved
  void *userp: user-provided pointer to save data to(in our case, the http struct)
  
  returns:
  the number of bytes actually handled.

  purpose:
  accumulate the chunks into a dynamically growing buffer
  keep track of total size so far
  null-terminate so the buffer can be safeely treated as a string


  extra:
  size is usally 1 byte(but it could be more): libcurl treats data as a block of bytes, like a file.
  for HTTP responses, this means each "element" is typically 1 byte(an unsigned char)
  so size * nmemb equals total length of chunks in bytes.
  Why not just set a "char buffer[1000]"?
  - HTTP respones vary in length/size. This is a problem, because:
      - response longer than buffer -> buffer overflow, bad crash.
      - guess a max size -> bad practice, unreliable
*/
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) 
{
    // Total amount of bytes in this callback
    size_t realsize = size * nmemb;
    // Cast user pointer to our struct
    http *mem = (http *)userp;

    // Reallocate memory to append new chunk while adding +1 for null terminator '\0'
    char *ptr = realloc(mem->data, mem->size + realsize + 1); // +1 for '\0'
    if (ptr == NULL) {
        // out of memory, abort
        return 0;
    }
    /* update the pointer in our struct. Why?
    when we use "realloc" above, we resize the memory block pointed by old_ptr
    It may:
      - Extend current block in place -> the pointer stays the same
      - OR
      - allocate a new block elsewhere, copy old data -> returns a new pointer.
      If it copies the old data, we need to set the new pointer so we don't use old pointer.

      mem->data is the pointer inside our struct that tracks the buffer.
      since realloc might return a different memory address, we need to set the new pointer so we odn't use old pointer(repeat of notes above) 

      in short:
      1. mem->data keep track of buffer in struct
      2. realloc may move buffer to new memory location
      3. we need to update mem->data to our new pointer so we don't crash or corrupt our data

    */
    mem->data = ptr; // update pointer in struct
    // copy new data into the buffer at current end)
    memcpy(&(mem->data[mem->size]), contents, realsize);
    // update total size
    mem->size += realsize;
    // null terminator for string safety
    mem->data[mem->size] = '\0'; // null terminate

    // we done, we handled all the bytes
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