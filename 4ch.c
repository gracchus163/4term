#include "4ch.h"
#include "json.h"

int get_thread_json(json_t** thread, char *board, int id)
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
int get_thread_data(json_t* thread, struct thread_data* data)
{
	int len = json_integer_value(json_object_get(json_array_get(thread, 0), THREAD_REPLIES));
	(*data).posts = malloc(sizeof(struct post_data)*len);
	for (int i = 0; i < len; i++)
	{
		get_post_data(json_array_get(thread, i),(&data->posts[i]));
	}
	return 0;
}
int get_post_data(json_t* post, struct post_data* ch)
{
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
int get_page_json(json_t** threads, char* board, int index)
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
int get_page_data(json_t* page, struct thread_data threads[])
{//15 threads per page and 6 posts per thread?  skip to end of thread if null
	threads->subject = "eeeeee";
	json_object_get(page, "ee");
	return 0;
}
int get_post_attr(json_t* post, json_t** obj, char* attr)
{
	*obj = json_object_get(post, attr);	
	if(json_is_object(*obj)) return 0;
	return 1;
}
