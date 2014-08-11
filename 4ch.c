#include "4ch.h"
#include "json.h"

int get_thread(json_t** thread, char *board, int id)
{
	char url[URL_SIZE];
	char* text;

	json_t *root;
	json_error_t error;
	snprintf(url, URL_SIZE, THREAD_URL, board, id);

	text = request(url);
	if(!text) return 1;
	root = json_loads(text, 0 ,&error);
	free(text);
	if(!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}
	if(!json_is_object(root)) printf("root not obj");
	*thread = json_object_get(root, "posts");
	
	return 0;
}
int get_thread_chars(char* board, int id, struct post_chars* ch, int n)
{
	json_t* data = NULL;
	if(get_thread(&data, board, id)) return 1;

	json_t *post = json_array_get(data, n);
	json_t *subject = json_object_get(post,SUBJECT );
	json_t *name = json_object_get(post, POSTER_NAME);
	json_t *post_number = json_object_get(post, POST_NUMBER);
	json_t *text = json_object_get(post, POST);

	ch->no = json_string_value(post_number);
	ch->subject = json_string_value(subject);
	ch->name = json_string_value(name);
	ch->post = json_string_value(text);
	return 0;
}
int get_page(json_t** threads, char* board, int index)
{
	char url[URL_SIZE];
	char* text;

	json_t *root;
	json_error_t error;
	snprintf(url, URL_SIZE, INDEX_URL, board, index);

	text = request(url);
	if(!text) return 1;

	root = json_loads(text, 0, &error);
	free(text);

	if(!root)
	{
		fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		return 1;
	}

	*threads = json_object_get(root,  "threads");

	json_decref(root);
	return 0;
}
int get_post_attr(json_t* post, json_t** obj, char* attr)
{
	*obj = json_object_get(post, attr);	
	if(json_is_object(*obj)) return 0;
	return 1;
}
/*
 json_t *thread_id, *thread_topic, *thread_comment;
json_t *post = json_array_get(thread, 0);

thread_id = json_object_get(post, "no");
thread_topic = json_object_get(post, "sub");
thread_comment = json_object_get(post, "com");

printf("%d - %s\n%s\n",(int) json_integer_value(thread_id),json_string_value(thread_topic), json_string_value(thread_comment));
*/
