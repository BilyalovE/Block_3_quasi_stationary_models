#include "Count_pressure_Eyler.h"

Count_pressure_Eyler::Count_pressure_Eyler(Input_data& input_data_task_1, Synthetic_time_series& synthetic_time, 
											vector<double>& current_layer_density, vector<double>& current_layer_viscosity)
{
	this->density = current_layer_density;
	this->viscosity = current_layer_viscosity;
	this->input_data_task_1 = input_data_task_1;
	this->inner_d = input_data_task_1.get_inner_d();
	this->v = input_data_task_1.v;
	this->pressure = pressure;
	this->dx = input_data_task_1.get_dx();
	this->synthetic_time = synthetic_time;
}

/// @brief 
/// @param pressure - ������ ������������� �������� �� ����� 
/// @param j - ������� ����
/// @return 
vector <double> Count_pressure_Eyler::count_pressure_Eyler(vector<double>& pressure, int j, double dt)
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

	if (j == 0) {
		/// @param pressure[0] - �������� � ������ ����� (��������� �������), [��]
 		pressure[0] = input_data_task_1.p0;
	}
	else {
		Transport_equation interpolation_p0(input_data_task_1, synthetic_time, j);
		pressure[0] = interpolation_p0.line_interpolation(synthetic_time.pressure_0, synthetic_time.time, dt);
	}
	
	/// ������� ������ �������� � ������ ��������� ����� �� ������� ����
	for (size_t i = 1; i < input_data_task_1.n; i++) {
		pressure[i] = pressure[i-1] + dx * ( hydraulic_resistance[i-1] / inner_d * density[i-1] * pow(v,2) / 2 - (input_data_task_1.zl - input_data_task_1.z0) / dx * density[i - 1] * M_G);
		}
	return pressure;
}
