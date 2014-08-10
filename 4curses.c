#include "4ch.h"
#include <curses.h>
#include <string.h>
#include <stdio.h>
char* huh(int thread_id)
{
	json_t *data = NULL;
	while(1)
	{

		if(get_thread(&data, "g", thread_id))
		{
			printf("Error getting thread, try another id");
			break;
		}
		/*for(size_t i=0; i < json_array_size(data);i++)
		{
		json_t *post = json_array_get(data, i);
		json_t *subject = json_object_get(post,SUBJECT );
		json_t *name = json_object_get(post, POSTER_NAME);
		json_t *post_number = json_object_get(post, POST_NUMBER);
		json_t *text = json_object_get(post, POST);
		
			//printf("%s %s - No.%d\n%s\n", json_string_value(subject), json_string_value(name), (int)json_number_value(post_number), json_string_value(text));
		//addstr("%s %s - No.%d\n%s\n", json_string_value(subject), json_string_value(name), (int)json_number_value(post_number), json_string_value(text));
		return json_string_value(text);
		}*/
		return json_string_value(json_object_get(json_array_get(data, 1), POST));	
	}

	return 0;
}

int main()
{
	int thread_id;
	char line[256];
	if (fgets(line, sizeof(line), stdin))
	{
		if (0 == sscanf(line, "%d", &thread_id)) return 1;
	}
	char* post = huh(thread_id);
	initscr();
	cbreak();
	noecho();

		/*for(int i = 'A';i < 'z';i++)
		{
			addch(i);
			refresh();

		}*/
		addstr(post);
		refresh();
	while(1)
	{
		if(getch()) break;
	}
	//finish
	endwin();
	return 0;
}
