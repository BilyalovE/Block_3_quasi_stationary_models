/*!
	\brief Блок 3 - Реализация квазистационарного гидравлического расчета с учетом движения партий
	\author Bilyalov Eldar
	\version 1 - Решение задачи 1
	\date 11.03.2024
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h>
#include <pde_solvers/pde_solvers.h>
#include "Input_struct.h"




struct Boundaru_conditions {
	/// @param Начальная плотность сырья в трубе, [кг/м3]
	double density = 800;
	/// @param Начальная вязкость сырья в трубе, [м2/с]
	double viscosity = 10e-6;
};



int main()
{
	
	/// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_density
	/// @param Структура параметров трубы задачи 1
	Input_data input_data_task_1;
	/// @param Начальный слой по плотности (начальное заполнение трубы)
	std::vector <double> initial_density_layer(input_data_task_1.n, input_data_task_1.initial_density);
	/// @param Начальный слой по вязкости (начальное заполнение трубы)
	std::vector <double> initial_viscosity_layer(input_data_task_1.n, input_data_task_1.initial_viscosity);
	/// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
	int number_layers_buffer = 2;
	ring_buffer_t <std::vector<std::vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_viscosity_layer });

}

