#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <locale.h>
#include <fstream>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Input_struct.h"
#include "Hydraulic_resistance_coefficient.h"

class Count_pressure_Eyler
{
	/// @param Параметры трубы
	Input_data input_data_task_1;
	/// @param Скорость перекачки, [м/с]
	double v;
	/// @param Внутренний диаметр, [м]
	double inner_d;
	/// @param Вектор вязкости, [м2/с]
	vector <double> viscosity;
	/// @param Вектор распределения давления по трубе, [Па]
	vector <double> pressure;
	/// @param Шаг расчетной сетки (расстояние между узловыми точками), [м]
	double dx;

public:
	Count_pressure_Eyler(Input_data& input_data_task_1, vector<double>& current_layer);

	void count_pressure_Eyler();
};

