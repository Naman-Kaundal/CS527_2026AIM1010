#include "gemm.h"

#include <stdexcept>
#include <algorithm>

static void validateDimensions(
    const Matrix& A,
    const Matrix& B
)
{
    if (A.empty() || B.empty())
    {
        throw std::invalid_argument(
            "Matrices must not be empty."
        );
    }

    const std::size_t K = A[0].size();

    for (const auto& row : A)
    {
        if (row.size() != K)
        {
            throw std::invalid_argument(
                "Matrix A is not rectangular."
            );
        }
    }

    const std::size_t BColumns = B[0].size();

    for (const auto& row : B)
    {
        if (row.size() != BColumns)
        {
            throw std::invalid_argument(
                "Matrix B is not rectangular."
            );
        }
    }

    if (K != B.size())
    {
        throw std::invalid_argument(
            "Matrix dimensions are incompatible."
        );
    }
}


Matrix gemmSimple(
    const Matrix& A,
    const Matrix& B
)
{
    validateDimensions(A, B);

    const std::size_t M = A.size();
    const std::size_t K = A[0].size();
    const std::size_t N = B[0].size();

    Matrix C(
        M,
        std::vector<long long>(N, 0)
    );

    for (std::size_t i = 0; i < M; ++i)
    {
        for (std::size_t k = 0; k < K; ++k)
        {
            for (std::size_t j = 0; j < N; ++j)
            {
                C[i][j] +=
                    A[i][k] * B[k][j];
            }
        }
    }

    return C;
}


Matrix gemmBlocked(
    const Matrix& A,
    const Matrix& B,
    int blockSize
)
{
    validateDimensions(A, B);

    if (blockSize <= 0)
    {
        throw std::invalid_argument(
            "Block size must be positive."
        );
    }

    const std::size_t M = A.size();
    const std::size_t K = A[0].size();
    const std::size_t N = B[0].size();

    Matrix C(
        M,
        std::vector<long long>(N, 0)
    );

    for (std::size_t ii = 0; ii < M; ii += blockSize)
    {
        for (std::size_t kk = 0; kk < K; kk += blockSize)
        {
            for (std::size_t jj = 0; jj < N; jj += blockSize)
            {
                const std::size_t iEnd =
                    std::min(
                        ii + static_cast<std::size_t>(blockSize),
                        M
                    );

                const std::size_t kEnd =
                    std::min(
                        kk + static_cast<std::size_t>(blockSize),
                        K
                    );

                const std::size_t jEnd =
                    std::min(
                        jj + static_cast<std::size_t>(blockSize),
                        N
                    );

                for (std::size_t i = ii; i < iEnd; ++i)
                {
                    for (std::size_t k = kk; k < kEnd; ++k)
                    {
                        for (std::size_t j = jj; j < jEnd; ++j)
                        {
                            C[i][j] +=
                                A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }

    return C;
}


bool matricesEqual(
    const Matrix& A,
    const Matrix& B
)
{
    if (A.size() != B.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < A.size(); ++i)
    {
        if (A[i].size() != B[i].size())
        {
            return false;
        }

        for (std::size_t j = 0; j < A[i].size(); ++j)
        {
            if (A[i][j] != B[i][j])
            {
                return false;
            }
        }
    }

    return true;
}