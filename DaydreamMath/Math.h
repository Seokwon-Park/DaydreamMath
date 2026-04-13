#pragma once

namespace Daydream
{
	namespace Math
	{
		template <typename T>
		T Max(const T& _lhs, const T& _rhs)
		{
			if (_lhs > _rhs) return _lhs;
			else return _rhs;
		}
	}
}