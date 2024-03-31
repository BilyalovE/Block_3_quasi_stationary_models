/*!
	\ brief Блок 3 - Реализация квазистационарного гидравлического расчета с учетом движения партий
	\ author Bilyalov Eldar
	\ version 1 - Решение задачи 2 (Задача QP. Учет различия временных сеток краевых условий и метода характеристик.)
	\ date 11.03.2024
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <fixed/fixed.h> 
#include <pde_solvers/pde_solvers.h>
#include "Input_struct.h"
#include "Transport_equation.h"
#include "Count_pressure_Eyler_Task_1.h"
#include "Count_pressure_Eyler_Task_2.h"
#include "gtest/gtest.h"
#include "Time_series.h"

///// Задание 1. Задача QP.Вытеснение начальной партии другой партией при неизменном расходе перекачки.
TEST(Block_3, Task_QP_1)
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
	double T = 200000;
	/// @param j - счетчик слоев
	int j = 0;
	double sum_dt = 0;
	Synthetic_time_series synthetic_time_1;
	/// @param Вектор распределения давления по трубе, [Па]
	std::vector <double> pressure_current(input_data_task_1.n);
	do {
		Transport_equation transport_equation_task_1(input_data_task_1, synthetic_time_1, j);
		for (size_t i{ 0 }; i < num_parameters; i++) {
			transport_equation_task_1.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i]);
		}
		/// Передаю текущий слой
		Count_pressure_Eyler_Task_1 count_pressure_Eyler_task_1(input_data_task_1, buffer.current()[0], buffer.current()[1], pressure_current);
		pressure_current = count_pressure_Eyler_task_1.count_pressure_Eyler();
		transport_equation_task_1.output_data("Output_task_1", buffer, sum_dt, pressure_current);
		buffer.advance(1);
		sum_dt += input_data_task_1.get_dt();
		j++;
	} while (sum_dt <= T);
}




/// Задание 01. Задача QP. Учет различия временных сеток краевых условий и метода характеристик.
TEST(Block_3, Task_QP_01) {
	/// Предполагаем, что в начальный момент времени всю трубу заполняют нефть с начальными параметрами initial_density, initial_viscosity
	/// @param Структура параметров трубы задачи 2
	Input_data input_data_task_2;
	/// @param Начальный слой по плотности (начальное заполнение трубы)
	std::vector <double> initial_density_layer(input_data_task_2.n, input_data_task_2.initial_density);
	/// @param Начальный слой по вязкости (начальное заполнение трубы)
	std::vector <double> initial_viscosity_layer(input_data_task_2.n, input_data_task_2.initial_viscosity);
	/// @param number_layers_buffer - количество слоев в буфере (для метода характеристик достаточно хранить 2 слоя - предыдущий и текущий слои)
	int number_layers_buffer = 2;
	ring_buffer_t <std::vector<std::vector<double>>> buffer(number_layers_buffer, { initial_density_layer, initial_viscosity_layer });
	/// @param Число рассчитываемых параметров (2 параметра - сера и вязкость)
	int num_parameters = 2;
	Synthetic_time_series synthetic_time_2;
	/// @param Синтетическая матрица параметров нефти входных партий
	vector <vector <double>> input_conditions(3, vector<double>(synthetic_time_2.density.size()));
	input_conditions = { synthetic_time_2.density, synthetic_time_2.viscosity, synthetic_time_2.time_input_parties };
	/// @param Время моделирования
	double T = 4000;
	/// @param j - счетчик слоев
	int j = 0;
	double sum_dt = 0;
	/// @param Вектор распределения давления по трубе, [Па]
	std::vector <double> pressure_current(input_data_task_2.n);
	do {
		Transport_equation transport_equation_task_2(input_data_task_2, synthetic_time_2, j);
		
		
		/// Передаю текущий слой для дополнительного расчета распределения давления по трубе методом Эйлера
		Count_pressure_Eyler_Task_2 count_pressure_Eyler_task_2(input_data_task_2, synthetic_time_2, buffer.current()[0], buffer.current()[1]);
		pressure_current = count_pressure_Eyler_task_2.count_pressure_Eyler(pressure_current, j, sum_dt);
		/// Вывод данных
		transport_equation_task_2.output_data("Output_task_01", buffer, sum_dt, pressure_current);
		/// Увеличение временного шага метода характеристик
		sum_dt += transport_equation_task_2.get_dt();
		j++;
		// Проверка выхода за границы массива входных параметров нефти
		if (j < synthetic_time_2.density.size()) {
			for (size_t i{ 0 }; i < num_parameters; i++) {
				Transport_equation interpolation_input_conditions(input_data_task_2, synthetic_time_2, j);
				if (i == 0) {
					input_conditions[i][j] = interpolation_input_conditions.line_interpolation(synthetic_time_2.density, synthetic_time_2.time_input_parties, sum_dt);
				}
				else {
					input_conditions[i][j] = interpolation_input_conditions.line_interpolation(synthetic_time_2.viscosity, synthetic_time_2.time_input_parties, sum_dt);
				}
				transport_equation_task_2.method_characteristic(buffer.current()[i], buffer.previous()[i], input_conditions[i][j]);
			}
		}
		// Если элементы в векторе входных партий закончились, то значит труба пустая
		else {
			for (size_t i{ 0 }; i < num_parameters; i++) {
				double empty_pipe{ 0 };
				transport_equation_task_2.method_characteristic(buffer.current()[i], buffer.previous()[i], empty_pipe);
			}
		}
		buffer.advance(1);
	} while (sum_dt <= T);
}

