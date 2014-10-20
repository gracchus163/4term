struct window
{
	int xpos, ypos;
	int selected; //selected row
	int scrollpos;
	char** rows;
	int no_rows;
	int width;
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
