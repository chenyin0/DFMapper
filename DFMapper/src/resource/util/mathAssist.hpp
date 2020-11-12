#pragma once

#include "../define/define.hpp"
#include "util.hpp"
#include "../../lib/Eigen/Core"

namespace Simulator::Mpr
{
	class Math
	{
	public:
		static uint factorial(uint n)
		{
			if (n == 0)
				return 1;
			uint sum = 1;
			for (uint i = 1; i <= n; i++)
				sum *= i;
			return sum;
		}
		
		static uint calCombination(uint set, uint select)
		{
			if (select == 0 || set == 0)
				return 1;
			return factorial(set) / (factorial(select) * factorial(set - select));
		}
	};
	
}