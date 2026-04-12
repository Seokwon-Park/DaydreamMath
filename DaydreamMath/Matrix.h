#pragma once

#include "MathCommon.h"
#include "Vector.h"

#include <cstddef>
#include <vector>

namespace Daydream::Math {

class Matrix final
{
public:
    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, Float32 fill = 0.0f);

    [[nodiscard]] static Matrix Identity(std::size_t size);

    [[nodiscard]] std::size_t Rows() const { return rows_; }
    [[nodiscard]] std::size_t Cols() const { return cols_; }

    Float32& At(std::size_t row, std::size_t col);
    const Float32& At(std::size_t row, std::size_t col) const;

    Matrix& operator+=(const Matrix& rhs);
    Matrix& operator-=(const Matrix& rhs);

private:
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::vector<Float32> values_{};
};

[[nodiscard]] Matrix operator+(Matrix lhs, const Matrix& rhs);
[[nodiscard]] Matrix operator-(Matrix lhs, const Matrix& rhs);
[[nodiscard]] Matrix operator*(const Matrix& lhs, const Matrix& rhs);
[[nodiscard]] Vec4 operator*(const Matrix& lhs, const Vec4& rhs);

} // namespace Daydream::Math
