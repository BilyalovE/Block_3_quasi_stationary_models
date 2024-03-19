#pragma once
#include <cmath>
#include <vector>
#include <pde_solvers/pde_solvers.h>
/// @brief Параметры трубы
struct Input_data {
	/// @param Внешний диаметр трубы, [м]
	double dn = 0.720;
	/// @param Толщина стенки трубы, [м]
	double ds = 0.01;

	/// @param Метод определения внутреннего диаметра, [м]
	double get_inner_d() const {
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
	double v = 0.5;
	/// @param Начальная плотность сырья в трубе, [кг/м3]
	double initial_density = 900;
	/// @param Начальная вязкость сырья в трубе, [м2/с]
	double initial_viscosity = 15e-6;
	/// @param Длина трубопровода, [м]
	double L = 500;
	/// @param Число узлов расчетной сетки
	int n = 5 + 1;
	/// @param Синтетический ряд давлений в начале трубы, [Па]
	vector <double> pressure_0 = { 6e6, 5.8e6, 5.8e6, 5.9e6, 5.9e6, 5.8e6, 5.8e6 };
	/// @param Синтетический ряд объемного расхода перекачки, [м^3,c]
	vector <double> volumetric_flow = { 0.192325, 0.200000, 0.210000, 0.200000, 0.180000, 0.210000, 0.210000 };

	/// @param Синтетический временной ряд для технологического режима перекачки, [с]
	vector <double> time = { 0, 205, 470, 600, 900, 980, 1256 };
	/// @brief Метод определения шага расчетной сетки (расстояние между узловыми точками), [м]
	double get_dx() const {
		return L / (n - 1);
	}
	/// @brief Метод определения шага расчетной сетки по времени, [с]
	double get_dt() const {
		double dx = get_dx();
		return dx * v; 
	}
	/// @brief Метод расчета относительной шероховатости
	double get_relative_roughness() const {
		double inner_diameter = get_inner_d();
		return delta / inner_diameter;
	}
	double get_inner_square() const {
		double inner_diameter = get_inner_d();
		return M_PI * pow(inner_diameter, 2) / 4;
	}
};

