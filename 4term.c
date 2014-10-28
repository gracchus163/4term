#include "4ch.h"
#include <string.h>
#include <stdio.h>
#include <termbox.h>
#include "termutil.h"

void print_window(struct window w)
{

}
void write_window(struct window win, struct thread_data thrd)
{
	int w = win.width; int h = win.height;
	char* title = (char*)malloc(sizeof(char)*w+1);
	snprintf(title, w,"No.%d - %s", thrd.no, thrd.subject); 
	char** r = win.rows;
	*r = malloc(sizeof(char)*w);
for(int x = 0; x < w; x++) 
{
	if(*title == '\0')
		*((*r)+x) = '=';
	else
		*((*r)+x) = *(title++);
}

	int n = 0; int thrd_len = thrd.replies;
	struct post_data p = thrd.posts[n];
	char* str = p.post;
	char* com = str;
	for(int y =1; y < h; y++)
	{
		for(int x = 0; x < w; x++)
		{
			if(*com == '\0')
			{
				if(n++ >= thrd_len) return;
				p = thrd.posts[n];
				free(str);
				str = p.post;
				com = str;
				break;
			}
			if(*com == '\n')
			{
				++com;
				break;
			}
//tb_change_cell(x,y, *(com++), TB_DEFAULT, TB_BLACK);
			*((*(r+y))+x) = *(com++);
		}
	}
	free(str);
	return;
}
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
	json_t* data = NULL;

	if(get_thread_json(&data, "g", thread_id))
	{
		printf("Error getting thread, try another id");
		return 1;
	}
	//char* com = NULL;
	get_thread_data(data, &thrd);
	json_decref(data);
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
