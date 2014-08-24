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
		return json_string_value(json_object_get(json_array_get(data, 1), POST));	
	}

	return 0;
}
int print_page(int index, char* board)
{
	json_t* page = NULL;
	if(get_page(&page, board, index)) return 1;
	struct post_data ch; 
	for(int i = 0; i < 15; i++)
	{
		json_t* thread = json_object_get(json_array_get(page, i), "posts");
		for(int j = 0; j < 6; j++)
		{
			json_t* post = json_array_get(thread, j);
			get_post_data(post, &ch);
			printw(ch);
		}
	}

	return 0
}
int main()
{
	int thread_id;
	char line[256];
	if (fgets(line, sizeof(line), stdin))
	{
		if (0 == sscanf(line, "%d", &thread_id)) return 1;
	}
	struct post_data ch; 
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, true);
	json_t* data = NULL;

	if(get_thread_json(&data, "g", thread_id))
	{
		printf("Error getting thread, try another id");
		return 1;
	}
	int thread_length = json_integer_value(json_object_get(json_array_get(data, 0), THREAD_REPLIES));
	for(int i = 0; i < thread_length; i++)
	{
		if(get_post_data(json_array_get(data, i), &ch)) return 1;
		printw("%s %s - %s\n%s", ch.subject, ch.name, ch.time, ch.post);
		refresh();
	}
	while(1)
	{
		if(getch()=='j') 
		{
			scrl(-1);
		}
		else if(getch()=='k')
		{
			scrl(1);
		}
		else if(getch()=='q')
		{
			break;
		}
	}
	//finish
	endwin();
	return 0;
}
