#pragma once
/// @brief Параметры трубы
struct Input_data {
	/// @param Внешний диаметр трубы, [м]
	double dn = 0.720;
	/// @param Толщина стенки трубы, [м]
	double ds = 0.01;
	/// @param Метод определния внутреннего диаметра, [м]
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
	double const v = 0.5;
	/// @param Начальная плотность сырья в трубе, [кг/м3]
	double initial_density = 900;
	/// @param Начальная вязкость сырья в трубе, [м2/с]
	double initial_viscosity = 15e-6;
	/// @param Длина трубопровода, [м]
	double L = 500;
	/// @param Число узлов расчетной сетки
	int n = 100;
	double get_dx() const {
		return L / n;
	}
};