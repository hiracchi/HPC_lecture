#ifndef MATRIX_H
#define MATRIX_H

struct Matrix {
	int max_row;
	int max_col;
	double* buf;
};


/**
 *  行列を作成する 
 */
struct Matrix* create_matrix(int row, int col);

/**
 *  行列の複製
 */
struct Matrix* copy_matrix(const struct Matrix* pMatrix);


/**
 *  行列を読み込む
 */
struct Matrix* load_matrix(const char* path);

    
/**
 *  行列を保存する 
 */
void save_matrix(const struct Matrix* pMatrix, const char* path);

    
/**
 *  行列要素に値を代入する
 */
void set_matrix(struct Matrix* pMatrix, int row, int col, double value);


/**
 *  行列要素を取得する
 */
double get_matrix(const struct Matrix* pMatrix, int row, int col);


/**
 *  行列要素を表示する
 */
void print_matrix(const struct Matrix* pMatrix);


/**
 *  行列を廃棄する
 */
void destroy_matrix(struct Matrix* pMatrix);

#endif // MATRIX_H
