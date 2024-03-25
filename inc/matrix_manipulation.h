#ifndef __MATRIX_MANIPULATION_H
#define __MATRIX_MANIPULATION_H

double* matrix_mul(double* matrix0, double* matrix1, int row0, int column0_row1, int column1);//矩阵相乘
double* matrix_add(double* matrix0, double* matrix1, int row, int column);//矩阵相加
double* matrix_sub(double* matrix0, double* matrix1, int row, int column);//矩阵相减
double* matrix_tra(double* matrix, int row, int column);//矩阵转置
double* matrix_neye(int dimension, double n);//生成n*单位矩阵

void matrix_cpy(double* matrix0, double* matrix1, int row, int column);//矩阵复制
void matrix_check(double* matrix);//矩阵指针检查

#endif // !__MATRIX_MANIPULATION_H
