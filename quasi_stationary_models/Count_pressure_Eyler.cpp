#include "Count_pressure_Eyler.h"

Count_pressure_Eyler::Count_pressure_Eyler(Input_data& input_data_task_1, vector<double>& current_layer )
{
	this->viscosity = current_layer;
	this->input_data_task_1 = input_data_task_1;
	this->inner_d = input_data_task_1.get_inner_d();
	this->v = input_data_task_1.v;
}

void Count_pressure_Eyler::count_pressure_Eyler()
{
	/// Вектор чисел Рейнольдса на текущем слое метода характеристик
	vector <double> Re (input_data_task_1.n);
	for (int i = 0; i <= input_data_task_1.n; i++) {
		Re[i] = inner_d * v / viscosity[i];
	}
	/// @param Относительная эквивалентная шероховатость (e)
	double relative_roughness = input_data_task_1.get_relative_roughness();
	
	// Вектор коэффициентов гидравлического сопротивления (lambda)
	vector <double> hydraulic_resistance(input_data_task_1.n);
	for (int i = 0; i <= input_data_task_1.n; i++) {
		Hydraulic_resistance_coefficient hydraulic_resistance_tak_1(Re[i], relative_roughness);
		hydraulic_resistance[i] = hydraulic_resistance_tak_1.calculation_hydraulic_resistance_coefficient();
	}


	// pressure_previous - давление на предыдущей итерации (граничное условие) [Па]
	double pressure_previous = input_data_task_1.p0;
	// pressure_current - давление на текущей итерации(рассчитанное значение) [Па]
	double pressure_current;

	for (size_t i = 1; i <= input_data_task_1.n; i++) {
		pressure_current = (pressure_previous + dx);
		pressure_previous = pressure_current;
		}

}
