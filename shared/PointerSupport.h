#ifndef POINTERSUPPORT__H
#define POINTERSUPPORT__H
#include "ProbFunc.h"
#include "../../Eigen/Core"

namespace Eigen {
	template<> struct NumTraits<ProbFunc*>
		: NumTraits<unsigned int> // permits to get the epsilon, dummy_precision, lowest, highest functions
		{
			typedef ProbFunc* Real;
			typedef ProbFunc* NonInteger;
			typedef ProbFunc* Nested;
			enum {
				IsComplex = 0,
				IsInteger = 1,
				IsSigned = 0,
				RequireInitialization = 0,
				ReadCost = 1,
				AddCost = 1,
				MulCost = 1
			};
		};
}
#endif
