struct window
{
	int xpos, ypos; //top-left corner
	int selected; //selected row
	int scrollpos;
	char** rows;
	int no_rows;
	int width;//window dimensions(not necessarily screen dimensions)
	int height;

};

void redraw(struct window* w_) 
{
	struct window w = *w_;
	for(int i = w.scrollpos; i < (w.height+w.scrollpos); i++)
	{
		for(int j = 0; j < w.width; j++)
		{
			//print *(*(char+i)+j) @ xpos++
		}
		//ypos++
	}
}
void print_window(struct window* w);

//change window scroll value and then just call redraw?
void scroll_window(struct window w, int scroll);
