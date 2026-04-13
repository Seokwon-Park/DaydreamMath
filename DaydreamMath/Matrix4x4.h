#pragma once

#include "Fwd.h"

namespace Daydream
{
	template<typename T>
	struct Matrix<4, 4, T>
	{
        union 
        {
            float values[16];
            float mat[4][4];

            struct {
                Vector4 row0;
                Vector4 row1;
                Vector4 row2;
                Vector4 row3;
            };
        }
	};
}