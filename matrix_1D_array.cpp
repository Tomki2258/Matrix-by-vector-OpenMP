#include <iostream>
#include <random>
#include <iomanip>
#include <omp.h>
// Liczbę wątków należy ustawić przez zmienną środowiskową:
// export OMP_NUM_THREADS=X

// Tutaj można zmienić rozmiar macierzy
// Wykonanie programu sekwencyjnego na serwerze z tymi danymi powinno trwać +5 minut
#define N 1000000
#define M 120000

int* multiplyMatrixByVectorParallel(const int* matrix, const int* vector) {
    int* result = new int[N];

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int sum = 0;
        for (int j = 0; j < M; j++) {
            sum += matrix[i * M + j] * vector[j];
        }
        result[i] = sum;
    }

    return result;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Tutaj można zmienić zakres losowanych liczb
    std::uniform_int_distribution<int> dis(-10, 10);
    // size_t jest potrzebny bo inaczej mysli ze zmienną jest int i 60000000 * 30000000 daje -120249494 coś takiego
    size_t totalSize = static_cast<size_t>(N) * M;

    int* matrix = new int[totalSize];
    int* vector = new int[M];

    #pragma omp parallel for
    for (size_t i = 0; i < totalSize; i++) {
        matrix[i] = dis(gen);
    }

    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        vector[i] = dis(gen);
    }

    // Zmienna przechowująca czas rozpoczęcia pomiaru przy pomocy funkcji OpenMP
    double start_time = omp_get_wtime();

    int* result = multiplyMatrixByVectorParallel(matrix, vector);

    // Zmienna przechowująca czas zakończenia pomiaru przy pomocy funkcji OpenMP
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

    // Test part for printing the matrix and vector

    // std::cout << "Matrix:" << std::endl;

    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < M; j++) {
    //         std::cout << matrix[i * M + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    // std::cout << "Vector:" << std::endl;

    // for (int i = 0; i < M; i++) {
    //     std::cout << vector[i] << " ";
    // }

    // std::cout << std::endl;
    // std::cout << "Result:" << std::endl;
    // for (int i = 0; i < N; i++) {
    //     std::cout << "Result[" << i << "] = " << result[i] << std::endl;
    // }

    // Print the result end

    delete[] matrix;
    delete[] vector;
    delete[] result;

    return 0;
}
