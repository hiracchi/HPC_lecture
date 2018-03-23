#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAXPATHLEN 1024

int dgemm_(char *TRANSA, char *TRANSB,
           int *M, int *N, int *K,
           double *alpha, double *A, int *LDA, double *B, int *LDB,
           double *beta, double *C, int *LDC);
struct Matrix* mul_matrix(const struct Matrix* pA,
                          const struct Matrix* pB);


void show_help()
{
    printf("USAGE: mul_matrix input_matrix1 input_matrix2 output\n");
}

int main(int argc, char* argv[])
{
	char A_path[MAXPATHLEN];
	char B_path[MAXPATHLEN];
	char C_path[MAXPATHLEN];
	struct Matrix* pA = NULL;
	struct Matrix* pB = NULL;
	struct Matrix* pC = NULL;
	
	if (argc == 4) {
        strcpy(A_path, argv[1]);
        strcpy(B_path, argv[2]);
        strcpy(C_path, argv[3]);
        
        pA = load_matrix(A_path);
        pB = load_matrix(B_path);
        pC = mul_matrix(pA, pB);

        save_matrix(pC, C_path);
	} else {
        show_help();
    }

    destroy_matrix(pA);
    destroy_matrix(pB);
    destroy_matrix(pC);
    
	return EXIT_SUCCESS;
}


/*
 * C = A * B
 */
struct Matrix* mul_matrix(const struct Matrix* pA,
                          const struct Matrix* pB)
{
	struct Matrix* pC = NULL;
    int A_row = 0;
    int A_col = 0;
    int B_row = 0;
    int B_col = 0;

    char TRANSA = 'N';
    char TRANSB = 'N';
    int M = 0;
    int N = 0;
    int K = 0;
    int LDA = 0;
    int LDB = 0;
    int LDC = 0;
    double ALPHA = 1.0;
    double BETA = 0.0;
    int info = 0;
    
    if ((pA != NULL) && (pB != NULL)) {
        A_row = pA->max_row;
        A_col = pA->max_col;
        B_row = pB->max_row;
        B_col = pB->max_col;

        if (A_col == B_row) {
            M = A_row;
            N = B_col;
            K = A_col;
            LDA = M;
            LDB = K;
            LDC = M;
            pC = create_matrix(A_row, B_col);
            
            info = dgemm_(&TRANSA, &TRANSB,
                          &M, &N, &K,
                          &ALPHA, pA->buf, &LDA, pB->buf, &LDB,
                          &BETA, pC->buf, &LDC);
            if (info != 0) {
                printf("dgemm_ error: info=%d.", info);
            }
        }
    }

    return pC;
}
