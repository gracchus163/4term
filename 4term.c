#include "4ch.h"
#include <string.h>
#include <stdio.h>
#include <termbox.h>
#include "termutil.h"

int FG = TB_DEFAULT;
int BG = TB_BLACK;
void print_window(struct window* w)
{
	int win_edge = w->ypos+w->height;
	int rows = w->no_rows;
	for(int y = w->ypos; y < win_edge && y<=rows; y++)
	{
		for(int x = w->xpos; x < (int)strlen(*(w->rows+y)); x++)
		{//maybe something wrong with quoting post numbers also
			if(*(*(w->rows+y+w->scrollpos)+x)==0) break;
			tb_change_cell(x,y, *(*(w->rows+y+w->scrollpos)+x), FG, BG);
		}
	}

}
void write_window(struct window* win, struct thread_data* thrd)
{
	int w = win->width;
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
	int thrd_len = thrd->replies;
	char* str;
	char* com;
	//TODO: not right, need to reiterate what window is
	//should contain the entire display and track
	//the sliding window over it
	//height of window and height of screen are different.
	//redo the outer loop, shouldn't stop until thrd_len is reached
	for(int n = 0, y = 1; n <thrd_len; n++,y++)
	{
		str = thrd->posts[n].post;
		com = str;
		*(r+y) = malloc(sizeof(char)*w);
		for(int x = 0; *com!=0; x++,com++)
		{
			if(*com == '\n' || x >= w)
			{
				if(*com == '\n') *((*(r+y))+x) = 0;
				x=0,y++;
				*(r+y) = malloc(sizeof(char)*w);
				win->no_rows += 1;
			}
			*((*(r+y))+x) = *(com);
		}
	}
	return;
}
void print_boards(json_t* b, struct window* win)
{
	int n = json_array_size(b);
	//7 chars = /lgbt/\0 enough to store every brd
	char** r = win->rows;
	for(int i = 0; i < n; i++)
	{
		*(r+i) = malloc(7*sizeof(char));
		snprintf(win->rows[i], 7, "/%s/", json_string_value(json_object_get(json_array_get(b, i), "board")));
	}
	win->no_rows = tb_height(); win->selected = 0; win->xpos = 0; win->ypos = 0; win->height = tb_height(); win->width = tb_width();
	win->scrollpos = 0;
}
int main()
{
	int thread_id;
	char line[256];
	/*
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
	*/
	json_t* brds = NULL;
	struct window* boards = malloc(sizeof(struct window));
	if(get_boards(&brds)) printf("get boards: 1");
	boards->rows = malloc(sizeof(char*)*json_array_size(brds));
	print_boards(brds, boards);
	

	tb_init();
	tb_set_clear_attributes(TB_BLUE, TB_GREEN);
	tb_clear();

	print_window(boards);
	tb_present();

	struct tb_event ev;
	tb_poll_event(&ev);
	/*
	get_thread_data(data, &thrd);
	json_decref(data);

	struct window win;
	win.no_rows = tb_height(); win.selected = 0; win.xpos = 0; win.ypos = 0; win.height = tb_height(); win.width = tb_width();
	win.scrollpos = 0;

	write_window(&win, &thrd);
	print_window(&win);
	//dodgy characters are causing the clear issues Ithink
	tb_present();
	while(1)
	{
		tb_poll_event(&ev);
		if(ev.key == TB_KEY_ESC || ev.ch == 'q') break;
		if(ev.key == TB_KEY_SPACE) tb_clear(),tb_present();
		if(ev.ch == 'r') tb_clear(), print_window(&win), tb_present();
		if(ev.ch == 'j')
		{
			win.scrollpos++;
			tb_clear();
			print_window(&win);
			tb_present();
		}
	}
	*/
	tb_shutdown();
	
}
