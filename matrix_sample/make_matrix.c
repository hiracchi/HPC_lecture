#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAXPATHLEN 1024

void show_help()
{
    printf("USAGE: make_matrix file_path row col\n");
    printf(" or\n");
    printf("USAGE: make_matrix file_path dim\n");
}


int main(int argc, char* argv[])
{
	char path[MAXPATHLEN];
	struct Matrix* pMatrix = NULL;
	int row = 0;
	int col = 0;
	int i, j;
	double value;
	int index = 0;
	
	if (argc == 3) {
		strcpy(path, argv[1]);
		row = atoi(argv[2]);
        col = row;

		if (row > 0) {
			printf("create (%d x %d) symmetric matrix...\n", row, col);
			
			pMatrix = create_matrix(row, col);
			
			for (i = 0; i < row; ++i) {
				for (j = 0; j <= i; ++j) {
					value = 0.000001 * index;
					set_matrix(pMatrix, i, j, value);
					set_matrix(pMatrix, j, i, value);
					++index;
				}
			}

			printf("save the matrix as %s.\n", path);
			save_matrix(pMatrix, path);
			
			destroy_matrix(pMatrix);
		}
    } else if (argc == 4) {
		strcpy(path, argv[1]);
		row = atoi(argv[2]);
		col = atoi(argv[3]);

		if ((row > 0) && (col > 0)) {
			printf("create (%d x %d) general matrix...\n", row, col);
			
			pMatrix = create_matrix(row, col);
			
			for (i = 0; i < row; ++i) {
				for (j = 0; j < col; ++j) {
					value = 0.000001 * index;
					set_matrix(pMatrix, i, j, value);
					++index;
				}
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

