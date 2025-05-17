#include <iostream>
#include <random>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>

#define N 3
#define M 3
int main() {
    int** matrix = new int*[N];

    // Równoległe generowanie danych do matrix
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        std::mt19937 gen(42 + i);
        std::uniform_int_distribution dis(-10, 10);

        matrix[i] = new int[M];
        for (int j = 0; j < M; j++) {
            matrix[i][j] = dis(gen);
        }
    }

    int* vector = new int[M];
    #pragma omp parallel for
    for (int i = 0; i < M; i++) {
        std::mt19937 gen(1337 + i);
        std::uniform_int_distribution dis(-10, 10);
        vector[i] = dis(gen);
    }

    std::string test_type = "small";
    long long count = static_cast<long long>(N) * M;

    if(count > 3600000){
        test_type = "big";
    }
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            std::ofstream file_matrix("tests/" + test_type + "_test_matrix" + std::to_string(N) + "x" + std::to_string(M) + ".txt");
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    file_matrix << matrix[i][j] << ",";
                }
            }
            file_matrix.close();
        }

        #pragma omp section
        {
            std::ofstream file_vector("tests/" + test_type + "_test_vector" + std::to_string(N) + "x" + std::to_string(M) + ".txt");
            for (int i = 0; i < M; i++) {
                file_vector << vector[i] << ",";
            }
            file_vector.close();
        }
    }

    for (int i = 0; i < N; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    delete[] vector;

    return 0;
}
