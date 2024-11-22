#include <iostream>
#include <vector>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <iomanip>

void RandMas(std::vector<std::vector<int>>& cost_matrix, int n) {
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                cost_matrix[i][j] = 0;
            }
            else {
                cost_matrix[i][j] = std::rand() % 10 + 1;
            }
        }
    }
}

void OutputMatr(const std::vector<std::vector<int>>& matr) {
    int size1 = matr.size();
    if (size1 == 0) { return; } // проверка на пустую матрицу
    int size2 = matr[0].size();

    for (int i = 0; i < size1; ++i) {
        for (int j = 0; j < size2; ++j) {
            std::cout << matr[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Enumeration(const std::vector<std::vector<int>>& matr, std::vector<bool>& visit, int firstCity, int n, int count, int cost, int& minimum, int& maximum, std::vector<int>& path, std::vector<int>& bestPathMin, std::vector<int>& bestPathMax) {
    path.push_back(firstCity);
    if (count == n && matr[firstCity][0]) { // проверка на возврат в стартовый город
        cost += matr[firstCity][0];
        if (cost < minimum) {
            minimum = cost;
            bestPathMin = path; // сохраняем лучший путь для минимальной стоимости
        }
        if (cost > maximum) {
            maximum = cost;
            bestPathMax = path; // сохраняем лучший путь для максимальной стоимости
        }
    }
    for (int city = 0; city < n; city++) {
        if (!visit[city] && matr[firstCity][city]) { // город не посещен
            visit[city] = true;
            Enumeration(matr, visit, city, n, count + 1, cost + matr[firstCity][city], minimum, maximum, path, bestPathMin, bestPathMax);
            visit[city] = false; // возвращаемся к предыдущему состоянию
        }
    }
    path.pop_back();
}

int GreedyAlgorithm(int start_city, const std::vector<std::vector<int>>& cost_matrix, std::vector<int>& greedy_route) {
    int n = cost_matrix.size();
    std::vector<bool> visited(n, false);
    greedy_route.push_back(start_city);
    visited[start_city] = true;
    int current_city = start_city;
    int greedy_cost = 0;

    while (greedy_route.size() < n) {
        int next_city = -1;
        int min_cost = std::numeric_limits<int>::max();
        for (int city = 0; city < n; ++city) {
            if (!visited[city] && cost_matrix[current_city][city] < min_cost) {
                min_cost = cost_matrix[current_city][city];
                next_city = city;
            }
        }
        if (next_city == -1) {
            return -1;
        }
        greedy_route.push_back(next_city);
        visited[next_city] = true;
        greedy_cost += min_cost;
        current_city = next_city;
    }

    greedy_cost += cost_matrix[current_city][start_city];
    greedy_route.push_back(start_city);
    return greedy_cost;
}

int main() {
    setlocale(LC_ALL, "Russian");

    for (int i = 0; i < 3; ++i) { // цикл для 3 наборов
        for (int num_cities : {4, 6, 8, 10}) {
            std::vector<std::vector<int>> cost_matrix(num_cities, std::vector<int>(num_cities));
            RandMas(cost_matrix, num_cities);

            std::cout << "Матрица стоимостей для " << num_cities << " городов:\n";
            OutputMatr(cost_matrix);

            int start_city = 0;

            // точное решение
            std::vector<bool> visit(num_cities, false);
            visit[start_city] = true;

            int minimum = std::numeric_limits<int>::max();
            int maximum = 0;
            std::vector<int> path;
            std::vector<int> bestPathMin;
            std::vector<int> bestPathMax;
            unsigned int start_time = clock();
            Enumeration(cost_matrix, visit, start_city, num_cities, 1, 0, minimum, maximum, path, bestPathMin, bestPathMax);
            unsigned int end_time = clock();

            unsigned int exact_time = end_time - start_time;

            // Эвристическое решение
            std::vector<int> greedy_route;
            unsigned int start_time_greedy = clock();
            int greedy_cost = GreedyAlgorithm(start_city, cost_matrix, greedy_route);
            unsigned int end_time_greedy = clock();

            unsigned int heuristic_time = end_time_greedy - start_time_greedy;

            // вывод результатов
            std::cout << "Точное решение:\n";
            std::cout << "Минимальная стоимость: " << minimum << "\n";
            std::cout << "Максимальная стоимость: " << maximum << "\n";

            std::cout << "Лучший маршрут для минимальной стоимости: ";
            for (int city : bestPathMin) {
                std::cout << city + 1 << " ";
            }
            std::cout << "\n";

            std::cout << "Лучший маршрут для максимальной стоимости: ";
            for (int city : bestPathMax) {
                std::cout << city + 1 << " ";
            }
            std::cout << "\n";

            std::cout << "Время работы точного алгоритма: " << exact_time << " миллисекунд\n";

            std::cout << "\nЭвристическое решение:\n";
            std::cout << "Стоимость маршрута: " << greedy_cost << "\n";

            std::cout << "Маршрут: ";
            for (int city : greedy_route) {
                std::cout << city + 1 << " ";
            }
            std::cout << "\nВремя работы эвристического алгоритма: " << heuristic_time << " миллисекунд\n";

            float quality_percentage = abs(100 - ((100 * greedy_cost - 100 * minimum) / (maximum - minimum)));

            std::cout << "Качество эвристического решения относительно минимальной стоимости: " << quality_percentage << "%\n";

            std::cout << "\n";
        }
    }

    return 0;
}