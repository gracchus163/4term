#include "4ch.h"
#include "json.h"
#include <string.h>

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
	data->replies = len;
	int sub = json_string_length(json_object_get(json_array_get(thread,0), SUBJECT));
	if(sub)
	{
		data->subject = malloc(sizeof(char)*sub+1);
		strncpy(data->subject, json_string_value(json_object_get(json_array_get(thread, 0), SUBJECT)), sub);
		data->subject[sub] = 0;//emergency null termination
	}
	else
	{
		data->subject = malloc(sizeof(char));
		data->subject[0] = '\0';
	}
	data->no = json_integer_value(json_object_get(json_array_get(thread, 0), POST_NUMBER));
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
	json_t *time = json_object_get(post, TIME);

	ch->no = json_integer_value(post_number);
	ch->time = json_integer_value(time);
	ch->subject = malloc(1+json_string_length(subject)*sizeof(char));
	strncpy(ch->subject, json_string_value(subject), json_string_length(subject));
	
	ch->name = malloc(1+json_string_length(name)*sizeof(char));
	strncpy(ch->name, json_string_value(name), json_string_length(name));
	
	json_t *text = json_object_get(post, POST);
	if(json_string_length(text))
	{
		char* tmp;
		strip_html(json_string_value(text), &tmp);
		ch->post = malloc(1+strlen(tmp)*sizeof(char));
		strncpy(ch->post, tmp, strlen(tmp)+1);
		free(tmp);
	}
	else
	{//TODO: Can't free when cleanup
		ch->post = malloc(8*sizeof(char));
		strncpy(ch->post, "[BLANK]", 8);
	}
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
int strip_html(char* post, char** ret)
{
	//TODO: Figure out segfaults here when using strlen with malloc
	char *str= malloc(2+strlen(post)*sizeof(char)) , *read = post;
	char *write = str;
	do 
	{
		if(strncmp(read, "&gt;", 4)==0)
		{
			*(write++) = '>';
			read += 3;
			continue;
		}
		if(strncmp(read, "&#039;", 6)==0)
		{
			*(write++) = '\'';
			read += 5;
			continue;
		}
		if(*read == '<')
		{
			if(read[1] == 'b' && read[2] == 'r') 
				*(write++) = '\n';

			while(*read != '>') read++;
		}
		else *(write++) = *read;
	} while (*(read++));	
	*write = '\0';
	*ret = malloc(strlen(str)+1);
	strcpy(*ret, str);
	
	free(str);
	return 0;
}
int get_boards(json_t** boards)
{
	char *url = "https://a.4cdn.org/boards.json";	
	json_error_t error;
	char *text = request(url);
	if(!text) return 1;
	json_t* root = json_loads(text, 0, &error);
	free(text);
	*boards = json_object_get(root, "boards");
	return 0;
}
