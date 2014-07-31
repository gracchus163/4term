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
		json_t *post = json_array_get(data, 0);
		json_t *op = json_object_get(post, "com");
		
		printf("%s", json_string_value(op));
	}

	return 0;
}
