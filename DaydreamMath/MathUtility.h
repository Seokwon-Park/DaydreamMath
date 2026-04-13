#pragma once

namespace Daydream
{
	template <typename T>
	T Max(const T& _lhs, const T& _rhs)
	{
		if (_lhs > _rhs) return _lhs;
		else return _rhs;
	}

}