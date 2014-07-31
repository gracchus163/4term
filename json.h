#ifndef JSN
#define JSN

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int newline_offset(const char *text);

size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream);
char *request(const char *url);

#endif
