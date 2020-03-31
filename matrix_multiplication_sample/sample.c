#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
	struct Matrix* pMatrix = NULL;
	const int max_row = 10;
	const int max_col = 10;

	// 10 x 10 の行列を作成
	pMatrix = create_matrix(max_row, max_col);

	// (3, 5)に7.0を代入
	set_matrix(pMatrix, 3, 5, 7.0);

	// 表示
	print_matrix(pMatrix);

	// 後片付け
	destroy_matrix(pMatrix);
	
	return EXIT_SUCCESS;
}

