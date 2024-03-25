#include <stdio.h>
#include "..\\inc\\matrix_manipulation.h"

double* matrix_mul(double* matrix0, double* matrix1, int row0, int column0_row1, int column1)
{
    int i, j, k;
    double* matrix_temp;
    double sum;
    // 创建结果矩阵
    matrix_temp = (double*)calloc(row0 * column1, sizeof(double));
    if (matrix_temp == NULL)
    {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    // 执行矩阵相乘运算
    for (i = 0; i < row0; i++)
        for (j = 0; j < column1; j++)
        {
            sum = 0;
            for (k = 0; k < column0_row1; k++)
                sum += matrix0[i * column0_row1 + k] * matrix1[j + k * column1];
            matrix_temp[i * column1 + j] = sum;
        }
    return matrix_temp;
}

double* matrix_add(double* matrix0, double* matrix1, int row, int column)
{
    int i;
    double* matrix_temp;
    // 创建结果矩阵
    matrix_temp = (double*)calloc(row * column, sizeof(double));
    if (matrix_temp == NULL)
    {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    //执行矩阵相加运算
    for (i = 0; i < row * column; i++)
        matrix_temp[i] = matrix0[i] + matrix1[i];
    return matrix_temp;
}

double* matrix_sub(double* matrix0, double* matrix1, int row, int column)
{
    int i;
    double* matrix_temp;
    // 创建结果矩阵
    matrix_temp = (double*)calloc(row * column, sizeof(double));
    if (matrix_temp == NULL)
    {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    //执行矩阵相加运算
    for (i = 0; i < row * column; i++)
        matrix_temp[i] = matrix0[i] - matrix1[i];
    return matrix_temp;
}

double* matrix_tra(double* matrix, int row, int column)
{
    int i, j;
    double* matrix_temp;
    // 创建结果矩阵
    matrix_temp = (double*)calloc(row * column, sizeof(double));
    if (matrix_temp == NULL)
    {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    //执行矩阵转置运算
    for (i = 0; i < row; i++)
        for (j = 0; j < column; j++)
            matrix_temp[i + j * row] = matrix[i * column + j];
    return matrix_temp;
}

double* matrix_neye(int dimension, double n)
{
    int i, j;
    double* matrix_temp;
    // 创建结果矩阵
    matrix_temp = (double*)calloc(dimension * dimension, sizeof(double));
    if (matrix_temp == NULL)
    {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }
    //生成n*单位矩阵
    for (i = 0; i < dimension; i++)
        for (j = 0; j < dimension; j++)
            matrix_temp[i * dimension + j] = (i == j) * n;
    return matrix_temp;
}

void matrix_cpy(double* matrix0, double* matrix1, int row, int column)
{
    int i;
    for (i = 0; i < row * column; i++)
        matrix0[i] = matrix1[i];
}

void matrix_check(double* matrix)
{
    if (matrix == NULL)
        return -1;
}