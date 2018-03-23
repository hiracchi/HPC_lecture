#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "matrix.h"

struct Matrix* create_matrix(int row, int col)
{
	struct Matrix* pMatrix = NULL;

	pMatrix = malloc(sizeof(struct Matrix));
	if (pMatrix == NULL) {
		return NULL;
	}
	
	pMatrix->max_row = row;
	pMatrix->max_col = col;
	pMatrix->buf = NULL;
	
	double* buf = (double*)malloc(sizeof(double) * row * col);
	if (buf == NULL) {
		destroy_matrix(pMatrix);
		return NULL;
	}
	pMatrix->buf = buf;
	
	return pMatrix;
}

struct Matrix* copy_matrix(const struct Matrix* pMatrix)
{
    struct Matrix* pAnswer = NULL;
    int row = 0;
    int col = 0;
    
    if (pMatrix != NULL) {
        row = pMatrix->max_row;
        col = pMatrix->max_col; 
        pAnswer = create_matrix(row, col);
        memcpy(pAnswer->buf, pMatrix->buf, sizeof(double) * row * col);
    }

    return pAnswer;
}


struct Matrix* load_matrix(const char* path)
{
	struct Matrix* pMatrix = NULL;
	FILE* fp;
	int row, col;
	double value;
	int i, j;
	
	fp = fopen(path, "rb");
	if (fp != NULL) {
		fread(&row, sizeof(int), 1, fp);
		fread(&col, sizeof(int), 1, fp);

		pMatrix = create_matrix(row, col);
		for (i = 0; i < row; ++i) {
			for (j = 0; j < col; ++j) {
				fread(&value, sizeof(double), 1, fp);
				set_matrix(pMatrix, i, j, value);
			}
		}
	}
	fclose(fp);
	
	return pMatrix;
}


void save_matrix(const struct Matrix* pMatrix, const char* path)
{
	FILE* fp;
	int row, col;
	int i, j;
	double value;
	
	fp = fopen(path, "wb");
	if (fp != NULL) {
		row = pMatrix->max_row;
		col = pMatrix->max_col;
		
		fwrite(&row, sizeof(int), 1, fp);
		fwrite(&col, sizeof(int), 1, fp);

		for (i = 0; i < row; ++i) {
			for (j = 0; j < col; ++j) {
				value = get_matrix(pMatrix, i, j);
				fwrite(&value, sizeof(double), 1, fp);
			}
		}
	}
	fclose(fp);
}


void set_matrix(struct Matrix* pMatrix,
				const int row, const int col, const double value)
{
	assert(pMatrix != NULL);
	assert((0 <= row) && (row < pMatrix->max_row));
	assert((0 <= col) && (col < pMatrix->max_col));
	
	const int index = row * pMatrix->max_col + col;
	pMatrix->buf[index] = value;
}


double get_matrix(const struct Matrix* pMatrix,
				  const int row, const int col)
{
	const int index = row * pMatrix->max_col + col;

	assert(pMatrix != NULL);
	assert((0 <= row) && (row < pMatrix->max_row));
	assert((0 <= col) && (col < pMatrix->max_col));
	
	return pMatrix->buf[index];
}


void print_matrix(const struct Matrix* pMatrix)
{
    const int max_row = pMatrix->max_row;
    const int max_col = pMatrix->max_col;
	int ord;
	int i, j;
	double value = 0.0;
	
	assert(pMatrix != NULL);

    for (ord = 0; ord < max_col; ord += 10) {
		printf("       ");
		for (j = ord; ((j < ord+10) && (j < max_col)); ++j) {
			printf("   %5d th", j+1);
		}
		printf("\n ----");

		for (j = ord; ((j < ord+10) && (j < max_col)); ++j) {
			printf("-----------");
		}
		printf("----\n");

		for (i = 0; i < max_row; ++i) {
			printf(" %5d  ", i+1);

			for (j = ord; ((j < ord+10) && (j < max_col)); ++j) {
				value = get_matrix(pMatrix, i, j);
				printf(" %10.6lf", value);
			}
			printf("\n");
		}
		printf("\n\n");
	}
}


void destroy_matrix(struct Matrix* pMatrix)
{
	if (pMatrix != NULL) {
		if (pMatrix->buf != NULL) {
			free(pMatrix->buf);
		}
		pMatrix->buf = NULL;
		
		free(pMatrix);
		pMatrix = NULL;
	}
}


