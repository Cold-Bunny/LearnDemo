#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "..\inc\matrix_manipulation.h"
#include "..\inc\soc_settings.h"

#define X_Dimension 3		//状态向量维数
#define U_Dimension 1		//控制向量维数
#define Z_Dimension 1		//观测向量维数

#define e (double)2.718			//自然对数

int main(void)
{
	FILE* fp;
	char data_soc[Table_Column * Num_Char + 1] = { 0 };
	char* token;
	char* strtok_set;

	int i, j;
	double* matrix_temp0 = NULL;
	double* matrix_temp1 = NULL;
	double* matrix_temp2 = NULL;

	double X[X_Dimension][1] = {
		{X_U1_Ini},
		{X_U2_Ini},
		{X_SOC_Ini}
	};//状态向量
	double U[Step_Size][U_Dimension][1] = { 
		0 
	};//控制向量
	double Z[Step_Size][Z_Dimension][1] = { 
		0 
	};//观测向量
	double P[X_Dimension][X_Dimension] = {
		{X_Variance,0,0},
		{0,X_Variance,0},
		{0,0,X_Variance}
	};//状态向量协方差矩阵
	double Q[X_Dimension][X_Dimension] = {
		{Q_Variance,0,0},
		{0,Q_Variance,0},
		{0,0,Q_Variance}
	};//系统噪声协方差矩阵
	double R[Z_Dimension][Z_Dimension] = {
		R_Variance
	};//测量噪声协方差矩阵

	double A[X_Dimension][X_Dimension] = {
		{pow(e,-T / (R1 * C1)),0,0},
		{0,pow(e,-T / (R2 * C2)),0},
		{0,0,1}
	};//状态转移矩阵
	//double A[X_Dimension][X_Dimension] = {
	//	{1 - T / (C1 * R1),0,0},
	//	{0,1 - T / (C2 * R2),0},
	//	{0,0,1}
	//};//状态转移矩阵（A的线性近似，当极化电阻电容为变量时可能有用）
	double B[X_Dimension][U_Dimension] = {
		{(1 - pow(e,-T / (R1 * C1))) * R1},
		{(1 - pow(e,-T / (R2 * C2))) * R2 },
		{-T / Qn}
	};//状态控制矩阵
	//double B[X_Dimension][U_Dimension] = {
	//	{T / C1},
	//	{T / C2 },
	//	{-T / Qn}
	//};//状态控制矩阵（B的线性近似，当极化电阻电容为变量时可能有用）
	double C[Z_Dimension][X_Dimension] = {
		{-1,-1,0}
	};//测量转移矩阵
	double D[Z_Dimension][U_Dimension] = {
		R0
	};//测量控制矩阵
	double K[X_Dimension][Z_Dimension] = {
		0
	};//卡尔曼增益矩阵

	double Z_pre[Z_Dimension][1] = {
	0
	};//预测的观测向量

	//读取.csv表格数据
	fp = fopen(input_file, "r");
	if (fp == NULL)
	{
		printf("Error: Input file not found, unable to read!\n");
		return -2;
	} 
#if Table_Column
	fgets(data_soc, Table_Column * Num_Char + 1, fp);//处理表头
#endif

	//读取数据
	for (i = 0; fgets(data_soc, Table_Column * Num_Char + 1, fp) != NULL; i++)
	{
		printf("Row%d: %s\n", i, data_soc);
		strtok_set = data_soc;
		for (j = 0; (token = strtok(strtok_set, ",")) != NULL; j++)
		{
			printf("token%d: %s\n", j, token);
			if (j == Uoc_Column - 1)
				Z[i][0][0] = atof(token);
			else if (j == Current_Column - 1)
				U[i][0][0] = atof(token);
			strtok_set = NULL;
		}
	}
	fclose(fp);

	//打开输出表格文件（.csv）
	fp = fopen(output_file, "w");
	if (fp == NULL)
	{
		printf("Error: Output file creation/writing failed!\n");
		return -3;
	}
	//进行扩展卡尔曼滤波
	for (i = 0; i < Step_Size; i++)
	{
		//预测状态向量X
		matrix_temp0 = matrix_mul(A, X, X_Dimension, X_Dimension, 1);// A*X
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_mul(B, U[i], X_Dimension, U_Dimension, 1);// B*U
		matrix_check(matrix_temp1);
		matrix_temp2 = matrix_add(matrix_temp0, matrix_temp1, X_Dimension, 1);// A*X+B*U
		matrix_check(matrix_temp2);
		matrix_cpy(X, matrix_temp2, X_Dimension, 1);// X=A*X+B*U
		free(matrix_temp0);
		free(matrix_temp1);
		free(matrix_temp2);
		//预测状态向量协方差矩阵P
		matrix_temp0 = matrix_mul(A, P, X_Dimension, X_Dimension, X_Dimension);// A*P
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_tra(A, X_Dimension, X_Dimension);// A_T
		matrix_check(matrix_temp1);
		matrix_temp2 = matrix_mul(matrix_temp0, matrix_temp1, X_Dimension, X_Dimension, X_Dimension);// A*P*A_T
		matrix_check(matrix_temp2);
		matrix_cpy(P, matrix_temp2, X_Dimension, X_Dimension);// P=A*P*A_T
		free(matrix_temp0);
		free(matrix_temp1);
		free(matrix_temp2);
		matrix_temp0 = matrix_add(P, Q, X_Dimension, X_Dimension);// P+Q
		matrix_check(matrix_temp0);
		matrix_cpy(P, matrix_temp0, X_Dimension, X_Dimension); //P= P + Q
		free(matrix_temp0);
		//计算“(dUoc/dSOC)|SOC=SOC(k)”并更新测量转移矩阵C
		C[0][2] = dUoc_MacroFunction(X[2][0]);
		//计算卡尔曼增益矩阵K
		matrix_temp0 = matrix_tra(C, Z_Dimension, X_Dimension);// C_T
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_mul(P, matrix_temp0, X_Dimension, X_Dimension, Z_Dimension);// P*C_T
		matrix_check(matrix_temp1);
		matrix_cpy(K, matrix_temp1, X_Dimension, Z_Dimension);// K=P*C_T
		free(matrix_temp0);
		free(matrix_temp1);
		matrix_temp0 = matrix_mul(C, K, Z_Dimension, X_Dimension, Z_Dimension);// C*K
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_add(matrix_temp0, R, Z_Dimension, Z_Dimension);// C*K+R
		matrix_check(matrix_temp1);
		free(matrix_temp0);
		*matrix_temp1 = 1 / (*matrix_temp1);//按通用公式应该是求逆矩阵的，但由于此处待求矩阵为1*1矩阵，为图方便按标量处理（标量简化处理）
		matrix_temp0 = matrix_mul(K, matrix_temp1, X_Dimension, Z_Dimension, Z_Dimension);// K/(C*K+R)
		matrix_check(matrix_temp0);
		matrix_cpy(K, matrix_temp0, X_Dimension, Z_Dimension);// K=K/(C*K+R)
		free(matrix_temp0);
		free(matrix_temp1);
		//预测观测向量Z_pre
		matrix_temp0 = matrix_mul(C, X, Z_Dimension, X_Dimension, 1);// C*X
		matrix_check(matrix_temp0);
		matrix_cpy(Z_pre, matrix_temp0, Z_Dimension, 1);// Z_pre=C*X
		free(matrix_temp0);
		matrix_temp0 = matrix_mul(D, U[i], Z_Dimension, U_Dimension, 1);// D*U
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_add(Z_pre, matrix_temp0, Z_Dimension, 1);// Z_pre+D*U
		matrix_check(matrix_temp1);
		matrix_cpy(Z_pre, matrix_temp1, Z_Dimension, 1);// Z_pre=Z_pre+D*U
		free(matrix_temp0);
		free(matrix_temp1);
		Z_pre[0][0] = Z_pre[0][0] + Uoc_MacroFunction(X[2][0]) - dUoc_MacroFunction(X[2][0]);//由于观测向量为1*1矩阵，为图方便按标量处理（标量简化处理）
		//更新状态向量X
		Z_pre[0][0] = Z[i][0][0] - Z_pre[0][0];//由于观测向量为1*1矩阵，为图方便按标量处理（标量简化处理）
		matrix_temp0 = matrix_mul(K, Z_pre, X_Dimension, Z_Dimension, 1);// K*Z_pre
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_add(X, matrix_temp0, X_Dimension, 1);// X+K*Z_pre
		matrix_check(matrix_temp1);
		matrix_cpy(X, matrix_temp1, X_Dimension, 1);// X=X+K*Z_pre
		free(matrix_temp0);
		free(matrix_temp1);
		//更新状态向量协方差矩阵P
		matrix_temp0 = matrix_mul(K, C, X_Dimension, Z_Dimension, X_Dimension);// K*C
		matrix_check(matrix_temp0);
		matrix_temp1 = matrix_mul(matrix_temp0, P, X_Dimension, X_Dimension, X_Dimension);// K*C*P
		matrix_check(matrix_temp1);
		matrix_temp2 = matrix_sub(P, matrix_temp1, X_Dimension, X_Dimension);// P-K*C*P
		matrix_check(matrix_temp2);
		matrix_cpy(P, matrix_temp2, X_Dimension, X_Dimension);// P=P-K*C*P
		free(matrix_temp0);
		free(matrix_temp1);
		free(matrix_temp2);
		//结果写入输出文件
		fprintf(fp, "%f\n", X[2][0]);
	}

	fclose(fp);

	return 0;
}