#include <chrono>
#include <iostream>
#include <random>
#include <iomanip>

// Tutaj można zmienić rozmiar macierzy
#define N 8000
#define M 10000

void printInTableFormat(int** matrix, const int* vector, const int* result) {
    constexpr auto maxRows = std::max(N, M);
    for (int i = 0; i < maxRows; i++) {
        if (i < N) {
            std::cout << "Row " << std::setw(2) << i << ": ";
            for (int j = 0; j < M; j++) {
                std::cout << std::setw(4) << matrix[i][j];
            }
        } else {
            std::cout << "         ";
            for (int j = 0; j < M; j++) {
                std::cout << "    ";
            }
        }

        std::cout << "  |  ";
        if (i < M) {
            std::cout << std::setw(4) << vector[i];
        } else {
            std::cout << "    ";
        }

        std::cout << "  |  ";
        if (i < N) {
            std::cout << std::setw(4) << result[i];
        }

        std::cout << std::endl;
    }
}

int* multiplyMatricesPerVector(int** matrix, const int* vector) {
    auto* result = new int[N];

    for (int i = 0; i < N; i++) {
        result[i] = 0;
        for (int j = 0; j < M; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    return result;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Tutaj można zmienić zakres losowanych liczb
    std::uniform_int_distribution dis(-10, 10);

    auto** matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[M];
        for (int j = 0; j < M; j++) {
            matrix[i][j] = dis(gen);
        }
    }

    auto* vector = new int[M];
    for (int i = 0; i < M; i++) {
        vector[i] = dis(gen);
    }

    const auto start = std::chrono::high_resolution_clock::now();

    const auto* result = multiplyMatricesPerVector(matrix, vector);

    const auto end = std::chrono::high_resolution_clock::now();

    // printInTableFormat(matrix, vector, result);

    std::cout << std::endl;
    const auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    const auto duration_s  = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "\nExecution Time:\n";
    std::cout << "-----------------------------\n";
    std::cout << std::left << std::setw(15) << "Microseconds:" << duration_us << "\n";
    std::cout << std::left << std::setw(15) << "Milliseconds:" << duration_ms << "\n";
    std::cout << std::left << std::setw(15) << "Seconds:"      << duration_s  << "\n";
    std::cout << "-----------------------------\n";

    for (int i = 0; i < N; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vector;
    delete[] result;
}

