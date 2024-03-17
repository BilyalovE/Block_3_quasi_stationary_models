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
	/// @param ��������� �����
	Input_data input_data_task_1;
	/// @param �������� ���������, [�/�]
	double v;
	/// @param ���������� �������, [�]
	double inner_d;
	/// @param ������ ��������, [�2/�]
	vector <double> viscosity;
	/// @param ������ ������������� �������� �� �����, [��]
	vector <double> pressure;
	/// @param ��� ��������� ����� (���������� ����� �������� �������), [�]
	double dx;

public:
	Count_pressure_Eyler(Input_data& input_data_task_1, vector<double>& current_layer);

	void count_pressure_Eyler();
};

