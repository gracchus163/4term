#include "4ch.h"
#include <string.h>
#include <stdio.h>
#include <termbox.h>
#include "termutil.h"

void print_window(struct window* w)
{
	int win_edge = w->ypos+w->height;
	int rows = w->no_rows;
	for(int y = w->ypos; y < win_edge && y<=rows; y ++)
	{
		for(int x = w->xpos; x < (int)strlen(*(w->rows+y)); x++)
		{//garbage from blank lines.  maybe something wrong with quoting post numbers also
			if(*(*(w->rows+y+w->scrollpos)+x)==0) break;
			tb_change_cell(x,y, *(*(w->rows+y+w->scrollpos)+x), TB_DEFAULT, TB_BLACK);
		}
	}

}
void write_window(struct window* win, struct thread_data* thrd)
{
	int w = win->width; int h = win->height;
	char* title = malloc(sizeof(char)*w+1);
	snprintf(title, w,"No.%d - %s", thrd->no, thrd->subject); 
	char** r = win->rows;
	
	*r = malloc(sizeof(char)*w);
	char* it = title;
for(int x = 0; x < w; x++) 
{
	if(*it == '\0')
		*((*r)+x) = '=';
	else
		*((*r)+x) = *(it++);
}
	free(title);
	win->no_rows = 0;
	int n = 0; int thrd_len = thrd->replies;
	struct post_data p = thrd->posts[n];
	char* str = p.post;
	char* com = str;
	//TODO: not right, need to reiterate what window is
	//should contain the entire display and track
	//the sliding window over it
	//height of window and height of screen are different.
	//redo the outer loop, shouldn't stop until thrd_len is reached
	for(int y =1; 1; y++)
	{
		*(r+y) = malloc(sizeof(char)*w);
		win->no_rows += 1;
		for(int x = 0; x < w; x++)
		{
			if(*com == '\0')
			{
				*((*(r+y))+x) = *com;
				if(++n >= thrd_len) return;
				p = thrd->posts[n];
				str = p.post;
				com = str;
				break;
			}
			if(*com == '\n')
			{
				*((*(r+y))+x) = 0;
				++com;
				break;
			}
			*((*(r+y))+x) = *(com++);
		}
	}
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
	tb_init();
	tb_set_clear_attributes(TB_DEFAULT, TB_BLACK);
	tb_clear();

	get_thread_data(data, &thrd);
	json_decref(data);
	struct tb_event ev;
	struct window win;
	win.no_rows = tb_width(); win.selected = 0; win.xpos = 0; win.ypos = 0; win.height = tb_height(); win.width = tb_width();
	win.scrollpos = 0;

	write_window(&win, &thrd);
	print_window(&win);
	tb_present();
	tb_poll_event(&ev);
	tb_shutdown();

}
