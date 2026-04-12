#include "Matrix.h"
#include "Vector.h"

int main()
{
    using namespace Daydream::Math;

    const Vec3 a{1.0f, 2.0f, 3.0f};
    const Vec3 b{3.0f, 2.0f, 1.0f};
    const auto sum = a + b;

    Matrix m = Matrix::Identity(4);
    const Vec4 v{sum[0], sum[1], sum[2], 1.0f};
    const auto transformed = m * v;

    return transformed[3] == 1.0f ? 0 : 1;
}
