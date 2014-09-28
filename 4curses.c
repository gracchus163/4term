#include "4ch.h"
#include <curses.h>
#include <string.h>
#include <stdio.h>

int remove_html(char** str)
{
	char * pt = strpbrk(*str, ">");
	char * pt2;
	int len = strlen(*str);
	char * temp = malloc(sizeof(char)*len);
	int n = 0;
	char* marker = *str;
	while(pt != NULL)
	{
		if(*pt == '>')
		{
			pt2 = strpbrk(pt+1, "<");
			if(pt2 != NULL)
			{
				strncpy((temp+n), pt+1, (pt2-pt-1));
				n += pt2-pt;
			} else break;	
		}
		marker = pt;	
		pt = strpbrk(pt2+1, ">");
	}
	//marker should end of tags
	//"blah/> my post is still going."
	strcat(temp, marker);
	if(n)
			strncpy(*str, temp, n-1);
	strcat(*str, "");
	free(temp);
	return 0;
}
int test(struct post_data* post, char* ret)
{
	if(post->post == NULL) return 1;
	char *str= malloc(sizeof(post->post)*strlen(post->post)) , *read = post->post;
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
	//printf("%s\n\n", post->post);
	//printf("%s\n\n", str);
	strcpy(ret, str);
	free(str);
	return 0;
}
int print_post(struct post_data* post, WINDOW* win)
{
	char* comment = malloc(sizeof(char)*strlen(post->post));
	strcpy(comment, post->post);
	remove_html(&comment);
	wprintw(win, "%s %d No.%d\n%s\n", 
		post->name, post ->time, post->no, comment);
	refresh();
	free(comment);
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
	char* com = NULL;
	get_thread_data(data, &thrd);
	for(int i = 0; i < thrd.replies; i++)
	{
	//	print_post(thrd.posts+i, stdscr);
		struct post_data pst = thrd.posts[i];
		com = (char*) malloc(sizeof(char*)*strlen(pst.post));
		test(thrd.posts+i, com);
		printf(com);
		printf("\n");
		free(com);
	}
	//finish
//	refresh();
	while(!getch()) {}
	//endwin();
	return 0;
}
