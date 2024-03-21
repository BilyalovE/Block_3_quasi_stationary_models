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

class Count_pressure_Eyler
{
	/// @param ��������� �����
	Input_data input_data_task_1;
	/// @param �������� ���������, [�/�]
	double v;
	/// @param ���������� �������, [�]
	double inner_d;
	/// @param ������ ���������, [��/�3]
	vector <double> density;
	/// @param ������ ��������, [�2/�]
	vector <double> viscosity;
	/// @param ������ ������������� �������� �� �����, [��]
	vector <double> pressure;
	/// @param ��� ��������� ����� (���������� ����� �������� �������), [�]
	double dx;
	/// @param ������������� ��������� ��� ��������� ����������
	Synthetic_time_series synthetic_time;

public:
	Count_pressure_Eyler(Input_data& input_data_task_1, Synthetic_time_series& synthetic_time, vector<double>& current_layer_po, vector<double>& current_layer_v);

	vector <double> count_pressure_Eyler(vector<double>& pressure, int j, double dt);
};

