#include "4ch.h"

int main()
{
	json_t *data = NULL;
	int thread_id;
	while(1)
	{
		char line[256];
		if (fgets(line, sizeof(line), stdin))
		{
			if (0 == sscanf(line, "%d", &thread_id)) return 1;
		}

		if(get_thread(&data, "g", thread_id))
		{
			printf("Error getting thread, try another id");
			break;
		}
		for(size_t i=0; i < json_array_size(data);i++)
		{
		json_t *post = json_array_get(data, i);
		json_t *subject = json_object_get(post,SUBJECT );
		json_t *name = json_object_get(post, POSTER_NAME);
		json_t *post_number = json_object_get(post, POST_NUMBER);
		json_t *text = json_object_get(post, POST);
		
			printf("%s %s - No.%d\n%s\n", json_string_value(subject), json_string_value(name), (int)json_number_value(post_number), json_string_value(text));
		}
		
	}

	return 0;
}
