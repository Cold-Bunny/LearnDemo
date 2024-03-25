#ifndef __SOC_SETTINGS_H
#define __SOC_SETTINGS_H

/*************************************************************************************************************************/
/*                                                                          模型参数                                                                                */
/*************************************************************************************************************************/
#define T (double)100				//采样间隔(s)
#define Step_Size 12				//采样次数

#define Qn (double)(1*3600)		//总电量(A·s)
#define R0 (double)0.06			// 固定电池内阻(Ω)
#define R1 (double)0.03			// 电化学极化电阻(Ω)
#define C1 (double)1000			// 电化学极化电容(F)
#define R2 (double)0.03			// 浓度差极化电阻(Ω)
#define C2 (double)10000			// 浓度差极化电容(F)

#define X_U1_Ini (double)0			//状态向量中U1初始值
#define X_U2_Ini (double)0			//状态向量中U2初始值
#define X_SOC_Ini (double)1			//状态向量中SOC初始值（0~1）

#define X_Variance (double)1		//状态向量方差值（默认单位矩阵）
#define Q_Variance (double)1		//系统噪声方差值（默认单位矩阵）
#define R_Variance (double)1		//测量噪声方差值（默认单位矩阵）

//开路电压：Uoc=19*SOC^6-56*SOC^5+61*SOC^4-28*SOC^3+5*SOC^2+SOC+3;
#define   Uoc_MacroFunction(SOC) (19.068*pow(SOC,6)-56.276*pow(SOC,5)+60.844*pow(SOC,4)-28.32*pow(SOC,3)+4.766*pow(SOC,2)+0.809*SOC+3.288)

//开路电压对SOC的导数：(dUoc/dSOC)=114*SOC^5-280*SOC^4+244*SOC^3-84*SOC^2+10*SOC+1
#define dUoc_MacroFunction(SOC) (114.408*pow(SOC,5)-281.38*pow(SOC,4)+243.376*pow(SOC,3)-84.96*pow(SOC,2)+9.532*SOC+0.809)

/*************************************************************************************************************************/
/*                                                                          表格参数                                                                                */
/*************************************************************************************************************************/
#define input_file "data/soc_input.csv"		//数据输入文件名（只支持.csv文件）
#define output_file "data/soc_output.csv"	//数据输出文件名（只支持.csv文件）

#define Table_Column 4				//表格总列数（总行数默认为“Step_Size+Tabel_Head"）
#define Table_Head	1					//是否有表头（1表示有，0表示没有）
#define Num_Char 20					//单元格中字符的最大长度

#define Uoc_Column 2					//观测变量（端电压）位于表格第几列
#define Current_Column 3			//控制变量（电流）位于表格第几列

#endif // !__SOC_SETTINGS_H
