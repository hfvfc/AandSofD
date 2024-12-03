#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <conio.h>
#include <cmath>

void ShellSort(std::vector<int>& array, int gap) {
    int n = array.size();
    for (int i = gap; i < n; i++) {
        int temp = array[i];
        int j;
        for (j = i; j >= gap && array[j - gap] > temp; j -= gap) {
            array[j] = array[j - gap];
        }
        array[j] = temp;
    }
}

bool IsSorted(const std::vector<int>& array) {
    for (int i = 1; i < array.size(); i++) {
        if (array[i] < array[i - 1]) {
            return false;
        }
    }
    return true;
}

void HibbardGaps(std::vector<int>& array) {
    int n = array.size();
    std::vector<int> gaps;
    for (int m = 1; ; m++) {
        int gap = pow(2, m) - 1;
        if (gap >= n) break;
        gaps.push_back(gap);
    }
    for (int i = gaps.size() - 1; i >= 0; i--) {
        ShellSort(array, gaps[i]);
    }
}

void KnuthGaps(std::vector<int>& array) {
    int n = array.size();
    std::vector<int> gaps;
    for (int gap = 1; gap < n; gap = 3 * gap + 1) {
        gaps.push_back(gap);
    }
    for (int i = gaps.size() - 1; i >= 0; i--) {
        ShellSort(array, gaps[i]);
    }
}

void SedgwickGaps(std::vector<int>& array) {
    int n = array.size();
    std::vector<int> gaps;
    for (int m = 0; ; m++) {
        int gap;
        if (m % 2 == 0) {
            gap = 9 * (1 * pow(2, m / 2)) - 9;
        }
        else {
            gap = 8 * (1 * pow(2, (m + 1) / 2)) - 6;
        }
        if (gap >= n) break;
        gaps.push_back(gap);
    }
    for (int i = gaps.size() - 1; i >= 0; i--) {
        ShellSort(array, gaps[i]);
    }
}

std::vector<int> GenerateRandomArray(int size, int minValue, int maxValue) {
    std::vector<int> array(size);
    std::random_device rd; // случайное число от устройства
    std::mt19937 gen(rd()); // инициализация генератора
    std::uniform_int_distribution<> dis(minValue, maxValue); // распределение
    for (int i = 0; i < size; ++i) {
        array[i] = dis(gen); // генерируем числа в диапазоне [minValue, maxValue]
    }
    return array;
}

void SaveArrayToFile(const std::vector<int>& array, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int num : array) {
            file << num << "\n";
        }
        file.close();
    }
    else {
        std::cout << "Не удалось открыть файл для записи: " << filename << "\n";
    }
}

unsigned int MeasureSortTime(void (*sortFunction)(std::vector<int>&), std::vector<int> array) {
    unsigned int start_time = clock();
    sortFunction(array);
    unsigned int end_time = clock();
    unsigned int result_time = end_time - start_time;
    return result_time;
}

int main() {
    setlocale(LC_ALL, "Russian");
    const std::vector<int> sizes = { 10000, 100000, 1000000 };
    const std::vector<std::pair<int, int>> ranges = { {-10, 10}, {-1000, 1000}, {-100000, 100000} };
    for (int i = 0; i < sizes.size(); ++i) {
        int size = sizes[i];
        for (int j = 0; j < ranges.size(); ++j) {
            std::pair<int, int> range = ranges[j];
            std::vector<int> arrayHibbard = GenerateRandomArray(size, range.first, range.second);
            std::string filename = "массив_размерности_" +
                std::to_string(size) + "_в диапазоне_" +
                std::to_string(range.first) + "_" +
                std::to_string(range.second) + ".txt";
            SaveArrayToFile(arrayHibbard, filename);

            unsigned int TimeHibbard = 0;
            unsigned int TimeKnuth = 0;
            unsigned int TimeSedgwick = 0;
            for (int k = 0; k < 3; ++k) {
                auto arrayCopyHibbard = arrayHibbard;
                auto arrayCopyKnuth = arrayHibbard;
                auto arrayCopySedgwick = arrayHibbard;
                TimeHibbard += MeasureSortTime(HibbardGaps, arrayCopyHibbard);
                TimeKnuth += MeasureSortTime(KnuthGaps, arrayCopyKnuth);
                TimeSedgwick += MeasureSortTime(SedgwickGaps, arrayCopySedgwick);
            }
            unsigned int AverageTimeHibbard = TimeHibbard / 3;
            unsigned int AverageTimeKnuth = TimeKnuth / 3;
            unsigned int AverageTimeSedgwick = TimeSedgwick / 3;

            std::cout << "Размер массива: " << size << ", Диапазон: [" << range.first << ", " << range.second << "]\n";
            std::cout << "Время работы Hibbard (в миллисекундах): " << AverageTimeHibbard << "\n";
            std::cout << "Время работы Knuth (в миллисекундах): " << AverageTimeKnuth << "\n";
            std::cout << "Время работы Sedgwick (в миллисекундах): " << AverageTimeSedgwick << "\n";
            std::cout << "\n";
        }
    }
    return 0;
}