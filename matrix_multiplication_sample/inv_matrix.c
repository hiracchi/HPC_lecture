#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define MAXPATHLEN 1024
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))


void dgetrf_(int *M, int *N, double *A, int *LDA, int *IPIV, int* info);
void dgetri_(int *N, double *A, int *lda, int *IPIV, double *WORK, int *LWORK, int* info);

struct Matrix* inv_matrix(const struct Matrix* pA);



void show_help()
{
    printf("USAGE: inv_matrix input_matrix output\n");
}

int main(int argc, char* argv[])
{
	char A_path[MAXPATHLEN];
	char Ainv_path[MAXPATHLEN];
	struct Matrix* pA = NULL;
	struct Matrix* pAinv = NULL;
	
	if (argc == 3) {
        strcpy(A_path, argv[1]);
        strcpy(Ainv_path, argv[2]);
        
        pA = load_matrix(A_path);
        pAinv = inv_matrix(pA);

        save_matrix(pAinv, Ainv_path);
	} else {
        show_help();
    }

    destroy_matrix(pA);
    destroy_matrix(pAinv);
    
	return EXIT_SUCCESS;
}


struct Matrix* inv_matrix(const struct Matrix* pA)
{
	struct Matrix* pAinv = copy_matrix(pA);
    int A_row = 0;
    int A_col = 0;

    int M = 0;
    int N = 0;
    int LDA = 0;
    int *IPIV = NULL;
    int info = 0;
    double *WORK = NULL;
    int LWORK = 0;
    
    if (pA != NULL) {
        A_row = pA->max_row;
        A_col = pA->max_col;

        M = A_row;
        N = A_col;
        LDA = max(1, M);
        IPIV = (int*)malloc(sizeof(int) * min(M, N));
        
        dgetrf_(&M, &N, pAinv->buf, &M, IPIV, &info);
        if (info == 0) {
            LDA = max(1, N);
            
            LWORK = -1;
            WORK = (double*)malloc(sizeof(double));
            dgetri_(&N, pAinv->buf, &N, IPIV, WORK, &LWORK, &info);
            LWORK = (int)WORK[0];
            free(WORK);
            WORK = NULL;
            
            WORK = (double*)malloc(sizeof(double) * LWORK);
            dgetri_(&N, pAinv->buf, &N, IPIV, WORK, &LWORK, &info);
            free(WORK);
            WORK = NULL;

            if (info < 0) {
                printf("dgetrf error: %d-th argument had an illegal value.", -info);
            } else if (info > 0) {
                printf("dgetrf error: A(%d, %d) is exactly zero.", info, info);
            }
            
        } else {
            if (info < 0) {
                printf("dgetrf error: %d-th argument had an illegal value.", -info);
            } else {
                printf("dgetrf error: A(%d, %d) is exactly zero.", info, info);
            }
        }

        free(IPIV);
        IPIV = NULL;
    }

    return pAinv;
}
