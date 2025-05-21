#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include "omp.h"

int main() {
    constexpr int N = 100'000;
    constexpr int M = 145'000;

    std::vector<std::vector<long long>> matrix;
    try {
        matrix.reserve(N);
        for (int i = 0; i < N; ++i) {
            matrix.emplace_back(std::vector<long long>(M));
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Manual allocation failed: " << e.what() << std::endl;
        return 1;
    }
    std::vector<long long> vector(M);
    std::vector<long long> result(N);

    std::random_device rd;

    #pragma omp parallel
    {
        std::mt19937 gen(rd());
        std::uniform_int_distribution<long long> dis(-1'000'000'000, 1'000'000'000);
        //std::uniform_int_distribution<long long> dis(-10, 10);

        #pragma omp for collapse(2)
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                matrix[i][j] = dis(gen);
            }
        }

        #pragma omp for
        for (int i = 0; i < M; ++i) {
            vector[i] = dis(gen);
        }
    }

    double start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        long long sum = 0;
        for (int j = 0; j < M; j++) {
            long long val = matrix[i][j] * vector[j];
            double tmp = std::sin(static_cast<double>(val));
            int int_val = static_cast<int>(tmp * 1000);

            sum += int_val;
        }
        result[i] = sum;
    }

    double end_time = omp_get_wtime();

    std::cout << "\nMatrix " << N << "x" << M << "\n";
    std::cout << "\nExecution Time (OpenMP): " << (end_time - start_time) << " seconds\n";

    // Część kodu wyświetlająca liczbę użytych wątków
    #pragma omp parallel
    {
        #pragma omp single
        {
            // Funkcja omp_get_num_threads() zwraca liczbę uruchomionych wątków
            std::cout << "Threads used: " << omp_get_num_threads() << std::endl;
        }
    }



    // std::cout << "\nMatrix:\n";
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < M; ++j) {
    //         std::cout << matrix[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << "\nVector:\n";
    // for (int i = 0; i < M; ++i) {
    //     std::cout << vector[i] << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "\nResult:\n";
    // for (int i = 0; i < N; ++i) {
    //     std::cout << "Result[" << i << "] = " << result[i] << std::endl;
    // }

    return 0;
}
