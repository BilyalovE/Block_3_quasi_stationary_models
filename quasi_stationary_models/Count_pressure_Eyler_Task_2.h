#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Input_struct.h"
#include "Time_series.h"
#include "Hydraulic_resistance_coefficient.h"
#include "Transport_equation.h"

class Count_pressure_Eyler_Task_2
{
	/// @param Параметры трубы
	Input_data input_data_task_1;
	/// @param Скорость перекачки, [м/с]
	double v;
	/// @param Внутренний диаметр, [м]
	double inner_d;
	/// @param Вектор плотности, [кг/м3]
	vector <double> density;
	/// @param Вектор вязкости, [м2/с]
	vector <double> viscosity;
	/// @param Вектор распределения давления по трубе, [Па]
	vector <double> pressure;
	/// @param Шаг расчетной сетки (расстояние между узловыми точками), [м]
	double dx;
	/// @param Синтетический временной ряд изменения параметров
	Synthetic_time_series synthetic_time;

public:
	Count_pressure_Eyler_Task_2(Input_data& input_data_task_1, Synthetic_time_series& synthetic_time, vector<double>& current_layer_po, vector<double>& current_layer_v);

	vector <double> count_pressure_Eyler(vector<double>& pressure, int j, double dt);
};

