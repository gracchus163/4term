#ifndef ch4
#define ch4
#include <jansson.h>

#define URL_SIZE 50
#define THREAD_URL "http://a.4cdn.org/%s/thread/%d.json"
#define INDEX_URL "http://a.4cdn.org/%s/%d.json"
#define CATALOG_URL "http://a.4cdn.org/%s/catalog.json"
#define BOARD_LIST "http://a.4cdn.org/boards.json"

#define POST_NUMBER "no"
#define REPLY_TO "resto"
#define STICKY "sticky"
#define CLOSED "closed"
#define TIME "now"
#define TIME_UNIX "time"
#define POSTER_NAME "name"
#define POSTER_TRIP "trip"
#define POSTER_ID "id"
#define CAPCODE "capcode"
#define COUNTRY "country"
#define COUNTRY_NAME "country_name"
#define EMAIL "email"
#define SUBJECT "sub"
#define POST "com"
#define FILE_UNIX "tim"
#define FILENAME "filename"
#define FILE_EXTENSION "ext"
#define FILE_SIZE "fsize"
#define FILE_MD5 "md5"
#define FILE_WIDTH "w"
#define FILE_HEIGHT "h"
#define FILE_DELETED "filedeleted"
#define FILE_SPOILER "spoiler"
#define POSTS_OMITTED "omitted_posts"
#define IMAGES_OMITTED "omitted_images"
#define THREAD_REPLIES "replies"
#define THREAD_IMAGES "images"
#define THREAD_AT_BUMPLIMIT "bumplimit"
#define THREAD_AT_IMAGELIMIT "imagelimit"


int get_thread(json_t** thread, char* board, int id);
int get_thread_chars(char* board, int id, struct post_chars* ch, int n);
int get_post(json_t* post, char* board, int post_id);
int get_page(json_t** page, char* board, int index);
int get_catalog(json_t* threads, char* board);
int get_post_attr(json_t* post, json_t** obj, char* attr);

struct post_chars
{
	char* no;
	char* time;
	char* name;
	char* trip;
	char* subject;
	char* post;
	char* filename;
		
};
#endif

