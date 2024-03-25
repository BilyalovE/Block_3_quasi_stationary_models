/*!
	\ brief Блок 3 - Реализация квазистационарного гидравлического расчета с учетом движения партий
	\ author Bilyalov Eldar
	\ version 1 - Решение задачи 1 (расчет давления во всех точках расчетной сетки трубы )
	\ date 11.03.2024
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h> 
#include <pde_solvers/pde_solvers.h>
#include "Input_struct.h"
#include "Transport_equation.h"
#include "Count_pressure_Eyler.h"


int main()
{
	
	/// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_density, initial_viscosity
	/// @param Структура параметров трубы задачи 1
	Input_data input_data_task_1;
	/// @param Начальный слой по плотности (начальное заполнение трубы)
	std::vector <double> initial_density_layer(input_data_task_1.n, input_data_task_1.initial_density);
	/// @param Начальный слой по вязкости (начальное заполнение трубы)
	std::vector <double> initial_viscosity_layer(input_data_task_1.n, input_data_task_1.initial_viscosity);
	/// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
	int number_layers_buffer = 2;
	ring_buffer_t <std::vector<std::vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_viscosity_layer });
	/// @param Число рассчитываемых параметров (2 параметра - сера и вязкость)
	int num_parameters = 2;
	/// @param Начальная плотность сырья в трубе, [кг/м3]
	double density = 800;
	/// @param Начальная вязкость сырья в трубе, [м2/с]
	double viscosity = 10e-6;
	/// @param Вектор параметров нефти входных партий
	vector <double> input_conditions(num_parameters);
	input_conditions = { density, viscosity };
	/// @param Время моделирования
	double T = 300;
	/// @param j - счетчик слоев
	int j = 0;
	double sum_dt = 0;
	/// @param Вектор распределения давления по трубе, [Па]
	std::vector <double> pressure_current(input_data_task_1.n);
	do {
		Transport_equation transport_equation_task_1(input_data_task_1, j);
		for (size_t i{ 0 }; i < num_parameters; i++) {
			transport_equation_task_1.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i]);
		}
		/// Передаю текущий слой
		Count_pressure_Eyler count_pressure_Eyler_task_1(input_data_task_1, buffer.previous()[0], buffer.previous()[1], pressure_current);
		pressure_current = count_pressure_Eyler_task_1.count_pressure_Eyler();
		transport_equation_task_1.output_data(buffer, sum_dt, pressure_current);
		buffer.advance(1);
		sum_dt += input_data_task_1.get_dt();
		j++;
	} while (sum_dt <= T);
}
 
