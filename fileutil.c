#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileutil.h"

char (*loadFile2D(char *filename, int *size))[COLS]
{
	FILE *in = fopen(filename, "r");
	if (!in)
	{
	    perror("Can't open file");
	    exit(1);
	}
	
	// inital allocation for the 2D array
	// starting with space for 10 lines
	int capacity = 10; 
	char (*lines)[COLS] = malloc (capacity * sizeof(*lines));
	if (!lines)
	{
		perror("Initial memory allocation failed");
		fclose(in);
		exit(1);
	}
	
	// buffer to hole each line
	char buffer[COLS];
	// number of valid entries
	*size = 0;
	
	// read the file line by line into buffer
	while(fgets(buffer, sizeof(buffer), in))
	{
		// trim newline
		buffer[strcspn(buffer, "\n")] = '\0';

		// check if we need to expand the array
		if (*size == capacity)
		{
			// inc capacity by 10
			capacity += 10;
			lines = realloc(lines, capacity * sizeof(*lines));
			if (!lines)
			{
				perror("Memoery reallocation failed");
				fclose(in);
				exit(1);
			}
		}
		
		// copy each line from buffer into the array
		strcpy(lines[*size], buffer);
		(*size)++;
	}

	// close file
	fclose(in);

	// return pointer to array
	return lines;
}

char * substringSearch2D(char *target, char (*lines)[COLS], int size)
{
	// iterate through the array for the given size
	for (int i = 0; i < size; i++)
	{
		// use strstr to locate the target string in each line
		if (strstr(lines[i], target))
		{
			// return the found string if a match is found
			return lines[i];
		}
	}
	// return NULL if target string is not found anywhere
	return NULL;
}

void free2D(char (*arr)[COLS])
{
	//free the memory associated with the 2D array
	free(arr);
}