#include "Transport_equation.h"


Transport_equation::Transport_equation(Input_data& input_data_task, Synthetic_time_series& synthetic_time, int j)

{
     /// @param n - количество точек расчетной сетки;
    this->n = input_data_task.n;
    /// @param j - счетчик слоя 
    this->j = j;
    /// @param input_data_task - исходные данные
    this->input_data_task = input_data_task;
    /// @param synthetic_time - синтетические временные ряды
    this->synthetic_time = synthetic_time;
    /// @param dx - величина шага между узлами расчетной сетки, м;
    
    this->dx = input_data_task.get_dx();
}

void Transport_equation::method_characteristic(vector<double>& current_layer, vector<double>& previous_layer, 
                                                        double left_condition)
{
    // Получение ссылок на текущий и предыдущий слои буфера
    for (size_t i = 1; i < n; i++)
    {
        current_layer[i] = previous_layer[i - 1];
    }
    // Слой current_layer на следующем шаге должен стать предыдущим. 
    // Для этого сместим индекс текущего слоя в буфере на единицу
    current_layer[0] = left_condition;
}





/// @brief get_speed - метод расчета скорости по расходу (расход может быть интерполирован)
double Transport_equation::get_speed() {
    double square = input_data_task.get_inner_square();
    double speed{};
    if (j == 0) {
        speed = (synthetic_time.volumetric_flow)[0] / square;
    }
    else {
       double interpolation_Q = line_interpolation(synthetic_time.volumetric_flow, synthetic_time.time, dt);
       speed = interpolation_Q / square;
    }
    return speed;
}

/// @brief get_dt - метод получения шага времени dt
double Transport_equation::get_dt()
{
    double speed = get_speed();
    dt = dx / speed;
    return dt;
}

/// @brief output_data - метод вывода слоев в файл формата csv
   /// @param i - счётчик слоев;
   /// @param solver_parameters - структура параметров, необходимых для реализации функции солвера ;
   /// @param buffer - буфер, рассчитанный после солвера;
   /// @param sum_dt - текущее время моделирования
void Transport_equation::output_data(ring_buffer_t<vector<vector<double>>>& buffer, double sum_dt, vector<double>& pressure)
{
    // Используем пространство имен std
    using namespace std;
    setlocale(LC_ALL, "rus");
    vector<vector<double>>& previous_layer = buffer.previous();
    //1 слой с записью заголовка
    if (j == 0) {
        ofstream outFile("Output.csv");
        outFile << "Время,Координата,Плотность,Сера,Давление" << endl;
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer[0].size(); i++) {
            outFile << sum_dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << "," << pressure[i ]<< endl;
            

        }
        outFile.close();
    }
    //последующие слои
    else {
        ofstream outFile("Output.csv", ios::app);
        // Записать значения текущего слоя в файл
        for (size_t i = 0; i < previous_layer[0].size(); i++) {
            outFile << sum_dt << "," << i * dx << "," << previous_layer[0][i] << "," << previous_layer[1][i] << "," << pressure[i] << endl;

        }
        outFile.close();
    }
}
/// @brief interpolation_flow - метод линейной интерполяции расхода
/// @param array_param - синтетический ряд изменения параметра
/// @param time - временной ряд изменения параметра
/// @return interpolation_param - интерполированный параметр
double Transport_equation::line_interpolation(vector <double> array_param, vector <double> time, double dt)
{
    /// @param interpolation_param - интерполированный параметр
    double interpolation_param{};
    /// @param size_array_param - размер синтетического временного ряда параметра
    int size_array_param = time.size();
    // Проверка наличия элементов в синтетическом ряде при интерполяции
    if (size_array_param > j) {
        // Выбор прямой, на которой интерполируется расход
        for (int i = 1; i < size_array_param; i++) {
            if (dt >= time[i - 1] && dt <= time[i]) {
                interpolation_param = (dt - time[i - 1]) / (time[i] - time[i - 1]) * (array_param[i] - array_param[i - 1]) + array_param[i - 1];
            }
        }
    }
    // Расход становится постоянным
    else {
        interpolation_param = (array_param)[size_array_param - 1];
    }
    return interpolation_param;
}


