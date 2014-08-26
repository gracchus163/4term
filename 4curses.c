#include "4ch.h"
#include <curses.h>
#include <string.h>
#include <stdio.h>
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
	get_thread_data(data, &thrd);
	while(1) {}
	//finish
//	endwin();
	return 0;
}
