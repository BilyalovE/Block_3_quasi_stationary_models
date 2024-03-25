#include "Count_pressure_Eyler.h"

Count_pressure_Eyler::Count_pressure_Eyler(Input_data& input_data_task_1, vector<double>& current_layer_po, vector<double>& current_layer_v, vector<double>& pressure)
{
	this->density = current_layer_po;
	this->viscosity = current_layer_v;
	this->input_data_task_1 = input_data_task_1;
	this->inner_d = input_data_task_1.get_inner_d();
	this->v = input_data_task_1.v;
	this->pressure = pressure;
	this->dx = input_data_task_1.get_dx();
}

vector <double> Count_pressure_Eyler::count_pressure_Eyler()
{
	/// @param ������ ����� ���������� �� ������� ���� ������ �������������
	vector <double> Re (input_data_task_1.n);
	for (int i = 0; i < input_data_task_1.n; i++) {
		Re[i] = inner_d * v / viscosity[i];
	}

	/// @param ������������� ������������� ������������� (e)
	double relative_roughness = input_data_task_1.get_relative_roughness();
	
	/// @param ������ ������������� ��������������� ������������� (lambda)
	vector <double> hydraulic_resistance(input_data_task_1.n);
	for (int i = 0; i < input_data_task_1.n; i++) {
		Hydraulic_resistance_coefficient hydraulic_resistance_tak_1(Re[i], relative_roughness);
		hydraulic_resistance[i] = hydraulic_resistance_tak_1.calculation_hydraulic_resistance_coefficient();
	}


	/// @param pressure[0] - �������� � ������ ����� (��������� �������), [��]
	pressure[0] = input_data_task_1.p0;
	/// ������� ������ �������� � ������ ��������� ����� �� ������� ����
	for (size_t i = 1; i < input_data_task_1.n; i++) {
		pressure[i] = pressure[i-1] + dx * ( hydraulic_resistance[i-1] / inner_d * density[i-1] * pow(v,2) / 2 - (input_data_task_1.zl - input_data_task_1.z0) / dx * density[i - 1] * M_G);
		}
	return pressure;
}
