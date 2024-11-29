#include <iostream>
#include <vector>
#include <string>

void OffsetTable(const std::string& pattern, std::vector<int>& table) {
    int patternLength = pattern.length();
    for (int i = 0; i < 256; ++i) {
        table[i] = patternLength;
    }
    for (int i = 0; i < patternLength - 1; i++) {
        table[static_cast<unsigned char>(pattern[i])] = patternLength - 1 - i; // ��������� �������
    }
}

int BoyerMooreSearchFirst(const std::string& string, const std::string& pattern) {
    int stringLength = string.length();
    int patternLength = pattern.length();

    if (patternLength == 0 || stringLength < patternLength) {
        return -1;
    }

    std::vector<int> table(256);
    OffsetTable(pattern, table);

    int shift = 0;

    while (shift <= stringLength - patternLength) {
        int j = patternLength - 1;

        // ���������� ��������� � �������
        while (j >= 0 && pattern[j] == string[shift + j]) {
            --j;
        }

        // ���� ��� ������� �������, ���������� ������ ������� ���������
        if (j < 0) {
            return shift;
        }

        // ��������� ��������
        char offset = string[shift + j];
        shift += std::max(1, j - table[static_cast<unsigned char>(offset)]);
    }

    return -1;
}

std::vector<int> findAllOccurrencesInRange(const std::string& string, const std::string& pattern, int start, int end) {
    std::vector<int> indices;
    int stringLength = string.length();
    int patternLength = pattern.length();

    if (start < 0 || end >= stringLength || start > end || patternLength == 0 || stringLength < patternLength) {
        return indices; // ���������� ������ ������
    }

    std::vector<int> table(256);
    OffsetTable(pattern, table);

    int shift = start;

    while (shift <= end - patternLength + 1) {
        int j = patternLength - 1;

        // ���������� ��������� � �������
        while (j >= 0 && pattern[j] == string[shift + j]) {
            --j;
        }

        // ���� ��� ������� �������, ��������� ������ � ������
        if (j < 0) {
            indices.push_back(shift);
            if (shift + patternLength <= end) {
                shift += patternLength;
            }
            else {
                shift += 1;
            }
        }
        else {
            // ��������� ��������
            char offset = string[shift + j];
            shift += std::max(1, j - table[static_cast<unsigned char>(offset)]);
        }
    }

    return indices;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string string;
    std::cout << "������� ������: ";
    std::getline(std::cin, string);
    std::string pattern;
    std::cout << "������� ���������: ";
    std::getline(std::cin, pattern);

    int firstIndex = BoyerMooreSearchFirst(string, pattern);
    std::cout << "������ ���������: " << firstIndex << "\n";

    int start, end;
    std::cout << "������� ��������: ";
    std::cin >> start >> end;
    std::vector<int> allIndices = findAllOccurrencesInRange(string, pattern, start, end);

    std::cout << "��������� �� �������� ���������: ";
    for (int index : allIndices) {
        std::cout << index << " ";
    }
    std::cout << "\n";

    return 0;
}