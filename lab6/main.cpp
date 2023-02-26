#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include "Example.hpp"


// https://www.scss.tcd.ie/~jones/vivio/caches/ALL%20protocols.htm


using Matrix = std::vector<std::vector<int>>;

/*
    * 1: LxM;
    * 2: MxN;
*/

const size_t L = 2000;
const size_t M = 2000;
const size_t N = 2000;

Matrix matrix_mul_column(const Matrix& f, const Matrix& s) {
    Matrix result(L, std::vector<int>(N, 0));

#pragma omp parallel for shared(f, s, result, L, M, N) num_threads(1)
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++j) {
            for (size_t k = 0; k < L; ++k) {
                result[k][i] += f[k][j] * s[j][i];
            }
        }
    }

    return result;
}

Matrix matrix_mul_line(const Matrix& f, const Matrix& s) {
    Matrix result(L, std::vector<int>(N, 0));

#pragma omp parallel for shared(f, s, result, L, M, N) num_threads(1)
    for (size_t i = 0; i < L; ++i) {
        for (size_t j = 0; j < M; ++j) {
            for (size_t k = 0; k < N; ++k) {
                result[i][k] += f[i][j] * s[j][k];
            }
        }
    }

    return result;
}

int main() {
//    std::srand(std::time(nullptr));
//    Matrix matrix1(L, std::vector<int>(M));
//    for (size_t i = 0; i < L; ++i) {
//        for (size_t j = 0; j < M; ++j) {
//            matrix1[i][j] = rand() % 10;
//        }
//    }
//    Matrix matrix2(M, std::vector<int>(N));
//    for (size_t i = 0; i < M; ++i) {
//        for (size_t j = 0; j < N; ++j) {
//            matrix2[i][j] = rand() % 10;
//        }
//    }
//
//    auto start = std::chrono::high_resolution_clock::now();
//    auto result = matrix_mul_column(matrix1, matrix2);
//    auto end = std::chrono::high_resolution_clock::now();
//
//    std::chrono::duration<double> d = end - start;
//    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << " milliseconds" << std::endl;

//    std::cout << "Matrix1:\n";
//    for (auto str : matrix1) {
//        for (auto elem : str) {
//            std::cout << elem << ' ';
//        }
//        std::cout << std::endl;
//    }
//    std::cout << "Matrix2:\n";
//    for (auto str : matrix2) {
//        for (auto elem : str) {
//            std::cout << elem << ' ';
//        }
//        std::cout << std::endl;
//    }
//    std::cout << "Result:\n";
//    for (auto str : result) {
//        for (auto elem : str) {
//            std::cout << elem << ' ';
//        }
//        std::cout << std::endl;
//    }
    example();
}