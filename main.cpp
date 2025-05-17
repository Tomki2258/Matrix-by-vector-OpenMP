#include <iostream>
#include <random>
#include <iomanip>
#include <omp.h>

// Liczbę wątków należy ustawić przez zmienną środowiskową:
// export OMP_NUM_THREADS=X

// Tutaj można zmienić rozmiar macierzy
// Wykonanie programu sekwencyjnego na serwerze z tymi danymi powinno trwać +5 minut
#define N 100000
#define M 120000

int* multiplyMatrixByVectorParallel(int** matrix, const int* vector) {
    int* result = new int[N];

    // Równoległe wykonanie pętli przy użyciu dyrektywy OpenMP
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        // Każdy wątek przetwarza osobny wiersz
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

    int** matrix = new int*[N];
    for (int i = 0; i < N; i++) {
        matrix[i] = new int[M];
        for (int j = 0; j < M; j++) {
            matrix[i][j] = dis(gen);
        }
    }

    int* vector = new int[M];
    for (int i = 0; i < M; i++) {
        vector[i] = dis(gen);
    }

    // Zmienna przechowująca czas rozpoczęcia pomiaru przy pomocy funkcji OpenMP
    double start_time = omp_get_wtime();

    int* result = multiplyMatrixByVectorParallel(matrix, vector);

    // Zmienna przechowująca czas zakończenia pomiaru przy pomocy funkcji OpenMP
    double end_time = omp_get_wtime();

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

    for (int i = 0; i < N; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vector;
    delete[] result;

    return 0;
}