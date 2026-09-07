#include "../src/gemm.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

// Reads a rows x cols matrix of integers.
static bool readMatrix(std::ifstream& input, Matrix& matrix, int rows, int cols)
{
    matrix.assign(rows, std::vector<long long>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(input >> matrix[i][j])) return false;
        }
    }
    return true;
}

static void printMatrix(const Matrix& matrix)
{
    for (const auto& row : matrix) {
        for (std::size_t j = 0; j < row.size(); ++j) {
            if (j > 0) std::cout << ' ';
            std::cout << row[j];
        }
        std::cout << '\n';
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [block_size]\n";
        return 1;
    }

    int blockSize = 32; // default tile size for blocking GEMM
    if (argc == 3) blockSize = std::stoi(argv[2]);

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Error: cannot open input file: " << argv[1] << '\n';
        return 1;
    }

    // --- Read input (preprocessing, not timed) ---
    int M, K, N;
    if (!(input >> M >> K >> N)) {
        std::cerr << "Error: invalid matrix header.\n";
        return 1;
    }
    if (M <= 0 || K <= 0 || N <= 0) {
        std::cerr << "Error: matrix dimensions must be positive.\n";
        return 1;
    }

    Matrix A, B;
    if (!readMatrix(input, A, M, K)) {
        std::cerr << "Error: invalid matrix A.\n";
        return 1;
    }
    if (!readMatrix(input, B, K, N)) {
        std::cerr << "Error: invalid matrix B.\n";
        return 1;
    }

    // --- Simple GEMM (timed) ---
    auto simpleStart = std::chrono::high_resolution_clock::now();
    Matrix simpleResult = gemmSimple(A, B);
    auto simpleEnd = std::chrono::high_resolution_clock::now();
    double simpleTime = std::chrono::duration<double, std::milli>(simpleEnd - simpleStart).count();

    // --- Blocking GEMM (timed separately) ---
    auto blockedStart = std::chrono::high_resolution_clock::now();
    Matrix blockedResult = gemmBlocked(A, B, blockSize);
    auto blockedEnd = std::chrono::high_resolution_clock::now();
    double blockedTime = std::chrono::duration<double, std::milli>(blockedEnd - blockedStart).count();

    bool correct = matricesEqual(simpleResult, blockedResult);

    // --- Output, exactly matching the assignment format ---
    std::cout << "Algorithm: GEMM Simple\n";
    std::cout << "Result matrix:\n";
    printMatrix(simpleResult);
    std::cout << "Execution time: " << simpleTime << " ms\n";

    std::cout << "\nAlgorithm: GEMM Blocking\n";
    std::cout << "Result matrix:\n";
    printMatrix(blockedResult);
    std::cout << "Execution time: " << blockedTime << " ms\n";

    if (!correct) {
        std::cerr << "\nWarning: simple and blocking results differ!\n";
    }

    return correct ? 0 : 1;
}
