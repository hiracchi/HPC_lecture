#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAXPATHLEN 1024

void show_help()
{
    printf("USAGE: make_unit file_path dim\n");
}


int main(int argc, char* argv[])
{
	char path[MAXPATHLEN];
	struct Matrix* pMatrix = NULL;
	int row = 0;
	int col = 0;
	int i = 0;
	
	if (argc == 3) {
		strcpy(path, argv[1]);
		row = atoi(argv[2]);
        col = row;

		if (row > 0) {
			printf("create (%d x %d) unit matrix...\n", row, col);
			
			pMatrix = create_matrix(row, col);
			
			for (i = 0; i < row; ++i) {
                set_matrix(pMatrix, i, i, 1.0);
			}

			printf("save the matrix as %s.\n", path);
			save_matrix(pMatrix, path);
			
			destroy_matrix(pMatrix);
		}
    } else {
        show_help();
    }

	return EXIT_SUCCESS;
}

