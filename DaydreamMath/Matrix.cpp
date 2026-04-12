#include "Matrix.h"

#include <cassert>

namespace Daydream::Math {

Matrix::Matrix(const std::size_t rows, const std::size_t cols, const Float32 fill)
    : rows_(rows), cols_(cols), values_(rows * cols, fill)
{
}

Matrix Matrix::Identity(const std::size_t size)
{
    Matrix matrix(size, size, 0.0f);
    for (std::size_t i = 0; i < size; ++i)
    {
        matrix.At(i, i) = 1.0f;
    }
    return matrix;
}

Float32& Matrix::At(const std::size_t row, const std::size_t col)
{
    assert(row < rows_ && col < cols_);
    return values_[row * cols_ + col];
}

const Float32& Matrix::At(const std::size_t row, const std::size_t col) const
{
    assert(row < rows_ && col < cols_);
    return values_[row * cols_ + col];
}

Matrix& Matrix::operator+=(const Matrix& rhs)
{
    assert(rows_ == rhs.rows_ && cols_ == rhs.cols_);
    for (std::size_t i = 0; i < values_.size(); ++i)
    {
        values_[i] += rhs.values_[i];
    }
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs)
{
    assert(rows_ == rhs.rows_ && cols_ == rhs.cols_);
    for (std::size_t i = 0; i < values_.size(); ++i)
    {
        values_[i] -= rhs.values_[i];
    }
    return *this;
}

Matrix operator+(Matrix lhs, const Matrix& rhs)
{
    lhs += rhs;
    return lhs;
}

Matrix operator-(Matrix lhs, const Matrix& rhs)
{
    lhs -= rhs;
    return lhs;
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    assert(lhs.Cols() == rhs.Rows());

    Matrix result(lhs.Rows(), rhs.Cols(), 0.0f);
    for (std::size_t row = 0; row < lhs.Rows(); ++row)
    {
        for (std::size_t col = 0; col < rhs.Cols(); ++col)
        {
            Float32 cell = 0.0f;
            for (std::size_t k = 0; k < lhs.Cols(); ++k)
            {
                cell += lhs.At(row, k) * rhs.At(k, col);
            }
            result.At(row, col) = cell;
        }
    }
    return result;
}

Vec4 operator*(const Matrix& lhs, const Vec4& rhs)
{
    assert(lhs.Cols() == 4 && lhs.Rows() == 4);

    Vec4 result = Vec4::Zero();
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col)
        {
            result[row] += lhs.At(row, col) * rhs[col];
        }
    }
    return result;
}

} // namespace Daydream::Math
