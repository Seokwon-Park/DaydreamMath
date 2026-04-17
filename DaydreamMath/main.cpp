#include <iostream>

#include "DaydreamMath.h"

using namespace Daydream;

int main()
{
	Vector3 A = { 1.0f, 2.0f , 4.0f };
	Vector3 B = { 5.0f, 1.0f , 3.0f };

	Vector3 C = Vector3::Max(A, B);

	return 0;
}
