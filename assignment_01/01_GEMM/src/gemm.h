#ifndef GEMM_H
#define GEMM_H

#include <vector>

using Matrix = std::vector<std::vector<long long>>;

// Simple O(M*K*N) matrix multiplication.
Matrix gemmSimple(
    const Matrix& A,
    const Matrix& B
);

// Blocked matrix multiplication.
// blockSize controls the tile size.
Matrix gemmBlocked(
    const Matrix& A,
    const Matrix& B,
    int blockSize
);

// Checks whether two matrices are identical.
bool matricesEqual(
    const Matrix& A,
    const Matrix& B
);

#endif