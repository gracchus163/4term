#include "4ch.h"
#include <string.h>
#include <stdio.h>
#include <termbox.h>



void print_row(char* str)
{
	for(int i = 0; i < tb_width(); i++)
	{
		tb_change_cell(i,0, *str++, TB_BLACK, TB_DEFAULT);
	}
}	
//takes potentially multiline string and prints it in appropriate rows
void print_post(char* str)
{
	for(int y = 0; y < tb_height(); y++)
	{
		for(int x = 0; x < tb_width(); x++)
		{
			tb_change_cell(x,y, *(str++), TB_DEFAULT, TB_BLACK);
		
		}
	}
}
int print_thread(struct thread_data thrd)
{//loops through screen, printing lines from post until reaching the bottom
	//modify to print to window struct and then have a print_window func
	int w = tb_width(); int h = tb_height();
	char* title = (char*)malloc(sizeof(char)*w+1);
	snprintf(title, w,"No.%d - %s", thrd.no, thrd.subject); 
for(int x = 0; x < w; x++) 
{
	if(*title == '\0')
		tb_change_cell(x, 0, '=', TB_DEFAULT, TB_BLACK);
	else
		tb_change_cell(x,0, *(title++), TB_DEFAULT, TB_BLACK);
}

	int n = 0; int thrd_len = thrd.replies;
	struct post_data p = thrd.posts[n];
	char* com = p.post;
	for(int y =1; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			if(*com == '\0')
			{
				//free(com);
				if(n++ >= thrd_len) return 1;
				p = thrd.posts[n];
				com = p.post;
				break;
			}
			if(*com == '\n')
			{
				++com;
				break;
			}
			tb_change_cell(x,y, *(com++), TB_DEFAULT, TB_BLACK);
		}
	}
	//free(com);
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
	struct thread_data thrd;
	/*initscr();
	cbreak();
	noecho();
	scrollok(stdscr, true);*/
	json_t* data = NULL;

	if(get_thread_json(&data, "g", thread_id))
	{
		printf("Error getting thread, try another id");
		return 1;
	}
	//char* com = NULL;
	get_thread_data(data, &thrd);
	tb_init();
	tb_set_clear_attributes(TB_DEFAULT, TB_BLACK);
	tb_clear();
	struct tb_event ev;
	print_thread(thrd);
	tb_present();
	tb_poll_event(&ev);
	tb_shutdown();
	return 0;
}
