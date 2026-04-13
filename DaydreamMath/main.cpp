#include <iostream>

#include "DaydreamMath.h"

using namespace Daydream;

int main()
{
	Vector3 A = { 1.0f,1.0f, 1.0f };
	Vector3 B = { 1.0f,1.0f, 1.0f };

	Float32 Result = Dot(A, B);

	return 0;
}
