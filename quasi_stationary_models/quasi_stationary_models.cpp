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

/// @brief Параметры трубы
struct Input_data {
	/// @param Внешний диаметр трубы, [м]
	double dn = 0.720;
	/// @param Толщина стенки трубы, [м]
	double ds = 0.01;
	/// @param Метод определния внутреннего диаметра, [м]
	double inner_d() const {
		return dn - 2 * ds;
	}
	/// @param Относительная шероховатость, [м]
	double delta = 0.00015;
	/// @param Перепад высот в начале трубы, [м]
	double z0 = 50;
	/// @param Перепад высот в конце трубы, [м]
	double zl = 100;
	/// @param Давление в начале трубы, [Па]
	double p0 = 6e6;
	/// @param Скорость перекачки, [м/с]
	double const v = 0.5;
	/// @param Начальная плотность сырья в трубе, [кг/м3]
	double initial_density = 900;
	/// @param Начальная вязкость сырья в трубе, [м2/с]
	double initial_viscosity = 15e-6;
	/// @param Число узлов расчетной сетки
	int n = 100;
};


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
	std::vector <double> initial_viscosity_layer(input_data_task_1.n, input_data_task_1.viscosity);
}

