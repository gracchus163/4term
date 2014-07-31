/*
 * Copyright (c) 2009-2013 Petri Lehtinen <petri@digip.org>
 *
 * Jansson is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <curl/curl.h>

#define BUFFER_SIZE  (256 * 1024)  /* 256 KB */

#define URL_FORMAT "http://a.4cdn.org/%s/%d.json"
#define URL_SIZE     256

/* Return the offset of the first newline in text or the length of
   text if there's no newline */
static int newline_offset(const char *text)
{
    const char *newline = strchr(text, '\n');
    if(!newline)
        return strlen(text);
    else
        return (int)(newline - text);

}

struct write_result
{
    char *data;
    int pos;
};

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *stream)
{
    struct write_result *result = (struct write_result *)stream;

    if(result->pos + size * nmemb >= BUFFER_SIZE - 1)
    {
        fprintf(stderr, "error: too small buffer\n");
        return 0;
    }

    memcpy(result->data + result->pos, ptr, size * nmemb);
    result->pos += size * nmemb;

    return size * nmemb;
}

static char *request(const char *url)
{
    CURL *curl = NULL;
    CURLcode status;
    struct curl_slist *headers = NULL;
    char *data = NULL;
    long code;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(!curl)
        goto error;

    data = malloc(BUFFER_SIZE);
    if(!data)
        goto error;

    struct write_result write_result = {
        .data = data,
        .pos = 0
    };

    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* GitHub commits API v3 requires a User-Agent header */
    headers = curl_slist_append(headers, "User-Agent: Jansson-Tutorial");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

    status = curl_easy_perform(curl);
    if(status != 0)
    {
        fprintf(stderr, "error: unable to request data from %s:\n", url);
        fprintf(stderr, "%s\n", curl_easy_strerror(status));
        goto error;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    if(code != 200)
    {
        fprintf(stderr, "error: server responded with code %ld\n", code);
        goto error;
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_global_cleanup();

    /* zero-terminate the result */
    data[write_result.pos] = '\0';

    return data;

error:
    if(data)
        free(data);
    if(curl)
        curl_easy_cleanup(curl);
    if(headers)
        curl_slist_free_all(headers);
    curl_global_cleanup();
    return NULL;
}

int main()
{
    size_t i;
    char *text;
    char url[URL_SIZE];

	json_t *head;
    json_t *root;
    json_error_t error;


    snprintf(url, URL_SIZE, URL_FORMAT, "/g/", 1);

    text = request(url);
    if(!text)
        return 1;

    head = json_loads(text, 0, &error);
	root = json_object_get(head, "threads");	
    free(text);

    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return 1;
    }

    if(!json_is_array(root))
    {
		fprintf(stderr, "error: root is not an array\n");
        json_decref(root);
        return 1;
    }
	json_t *thread_id, *thread_topic, *thread_comment;
	json_t *data = json_array_get(root, 1);

	json_t *thread = json_object_get(data, "posts");
	json_t *post = json_array_get(thread, 0);

	thread_id = json_object_get(post, "no");
	thread_topic = json_object_get(post, "sub");
	thread_comment = json_object_get(post, "com");

	printf("%d - %s\n%s\n",(int) json_integer_value(thread_id),json_string_value(thread_topic), json_string_value(thread_comment));
    json_decref(root);
    return 0;
}
