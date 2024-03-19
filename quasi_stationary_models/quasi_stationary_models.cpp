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
#include "Count_pressure_Eyler.h"
#include "gtest/gtest.h"


/// Задание 1. Задача QP.Вытеснение начальной партии другой партией при неизменном расходе перекачки.
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
		Count_pressure_Eyler count_pressure_Eyler_task_1(input_data_task_1, buffer.previous()[0], buffer.previous()[1]);
		pressure_current = count_pressure_Eyler_task_1.count_pressure_Eyler(pressure_current);
		transport_equation_task_1.output_data(buffer, sum_dt, pressure_current);
		buffer.advance(1);
		sum_dt += input_data_task_1.get_dt();
		j++;
	} while (sum_dt <= T);
}

/// Задание 2. Задача QP. Учет различия временных сеток краевых условий и метода характеристик.
TEST(Block_3, Task_QP_2) {
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

	/// @param Синтетический вектор плотности входных партий (краевые условия), [кг/м3]
	vector <double> density = { 900, 880, 880, 890, 890, 890, 880, 880 };
	/// @param Синтетический вектор вязкости входных партий (краевые условия), [м2/с]
	vector <double> viscosity = { 15e-6, 13e-6, 13e-6, 14e-6, 14e-6, 13e-6, 13e-6 };
	/// @param Синтетический временной ряд для краевых условий, [с]
	vector <double> time_input_parties = { 0, 60, 120, 180, 240, 300, 360 };
	/// @param Синтетическая матрица параметров нефти входных партий
	vector <vector <double>> input_conditions(3, (density, viscosity, time_input_parties));
	/// @param Время моделирования
	double T = 400;
	/// @param j - счетчик слоев
	int j = 0;
	double sum_dt = 0;
	/// @param Вектор распределения давления по трубе, [Па]
	std::vector <double> pressure_current(input_data_task_2.n);
	do {
		Transport_equation transport_equation_task_2(input_data_task_2, j);
		// Проверка выхода за границы массива входных параметров нефти
		if (j < density.size()) {
			for (size_t i{ 0 }; i < num_parameters; i++) {
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
		/// Передаю текущий слой для дополнительного расчета распределения давления по трубе методом Эйлера
		Count_pressure_Eyler count_pressure_Eyler_task_1(input_data_task_2, buffer.previous()[0], buffer.previous()[1]);
		pressure_current = count_pressure_Eyler_task_1.count_pressure_Eyler(pressure_current);
		/// Вывод данных
		transport_equation_task_2.output_data(buffer, sum_dt, pressure_current);
		buffer.advance(1);
		/// Увеличение временного шага метода характеристик
		sum_dt += transport_equation_task_2.get_dt();
		j++;
	} while (sum_dt <= T);
}
 
