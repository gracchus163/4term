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


struct post_data
{
	int no;
	int time;
	char* name;
	char* trip;
	char* subject;
	char* post;
	char* filename;
		
};
struct thread_data
{
	char* no;
	char* subject;
	int replies;
	struct post_data *posts; //array of posts

};

int get_thread_json(json_t** thread, char* board, int id);
int get_thread_data(json_t* thread, struct thread_data* data);
int update_thread(struct thread_data* thread);
int get_post_json(json_t* post, char* board, int post_id);
int get_post_data(json_t* post, struct post_data* ch);
int get_page_json(json_t** page, char* board, int index);
int get_page_data(json_t* page, struct thread_data threads[]);
int get_catalog_json(json_t** threads, char* board);
int get_post_attr(json_t* post, json_t** obj, char* attr);

#endif

