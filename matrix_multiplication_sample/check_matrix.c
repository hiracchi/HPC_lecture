#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

#define MAXPATHLEN 1024
#define EPSILON (1.0E-12)

void show_help()
{
    printf("USAGE: check_matrix matrixA matrixB\n");
}

int main(int argc, char* argv[])
{
	char A_path[MAXPATHLEN];
	char B_path[MAXPATHLEN];
	struct Matrix* pA = NULL;
	struct Matrix* pB = NULL;
    int A_row = 0;
    int A_col = 0;
    int B_row = 0;
    int B_col = 0;
    int row = 0;
    int col = 0;
    double A_value = 0.0;
    double B_value = 0.0;
    int answer = 0;
    
	if (argc == 3) {
        strcpy(A_path, argv[1]);
        strcpy(B_path, argv[2]);
        
        pA = load_matrix(A_path);
        pB = load_matrix(B_path);

        if ((pA != NULL) && (pB != NULL)) {
            A_row = pA->max_row;
            A_col = pA->max_col;
            B_row = pB->max_row;
            B_col = pB->max_col;

            if (A_row != B_row) {
                printf("[ERR] A_row=%d, B_row=%d.\n", A_row, B_row);
                answer = 1;
            }
            if (A_col != B_col) {
                printf("[ERR] A_col=%d, B_col=%d.\n", A_col, B_col);
                answer = 1;
            }

            if (answer == 0) {
                for (row = 0; row < A_row; ++row) {
                    for (col = 0; col < A_col; ++col) {
                        A_value = get_matrix(pA, row, col);
                        B_value = get_matrix(pB, row, col);
                        if (fabs(A_value - B_value) > EPSILON) {
                            printf("[ERR] A(%d, %d)=%10.6lf, B(%d, %d)=%10.6f\n",
                                   row, col, A_value,
                                   row, col, B_value);
                            answer = 2;
                        }
                    }
                }
            }
        }
	} else {
        show_help();
    }

    destroy_matrix(pA);
    destroy_matrix(pB);
    
	return answer;
}

