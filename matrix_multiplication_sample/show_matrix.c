#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAXPATHLEN 1024

void show_help()
{
    printf("USAGE: show_matrix matrix ... \n");
}

int main(int argc, char* argv[])
{
	char path[MAXPATHLEN];
	struct Matrix* pMatrix = NULL;
	int i;
	
	if (argc > 1) {
		for (i = 1; i < argc; ++i) {
			strcpy(path, argv[i]);
			pMatrix = load_matrix(path);

			printf("%s:\n", path);
			printf("row=%d, col=%d\n",
				   pMatrix->max_row,
				   pMatrix->max_col);
			print_matrix(pMatrix);
			printf("\n");
			
			destroy_matrix(pMatrix);
		}
	} else {
        show_help();
    }

	return EXIT_SUCCESS;
}

