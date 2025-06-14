#include "../code/include/Utilities.hpp"
#include <cassert>

int main(void) {
	static_assert(areEqual(degToRadian<float>(0.0f)		, 0.0f));
	static_assert(areEqual(degToRadian<float>(30.0f)	, (float)M_PI/6.0f));
	static_assert(areEqual(degToRadian<float>(45.0f)	, (float)M_PI/4.0f));
	static_assert(areEqual(degToRadian<float>(60.0f)	, (float)M_PI/3.0f));
	static_assert(areEqual(degToRadian<float>(90.0f)	, (float)M_PI/2.0f));
	static_assert(areEqual(degToRadian<float>(180.0f)	, (float)M_PI*1.0f));	
	static_assert(areEqual(degToRadian<float>(360.0f)	, (float)M_PI*2.0f));

	static_assert(areEqual(degToRadian<double>(0.0)		, 0.0));
	static_assert(areEqual(degToRadian<double>(30.0)	, M_PI/6.0));
	static_assert(areEqual(degToRadian<double>(45.0)	, M_PI/4.0));
	static_assert(areEqual(degToRadian<double>(60.0)	, M_PI/3.0));
	static_assert(areEqual(degToRadian<double>(90.0)	, M_PI/2.0));
	static_assert(areEqual(degToRadian<double>(180.0)	, M_PI*1.0));
	static_assert(areEqual(degToRadian<double>(360.0)	, M_PI*2.0));

	for(double i = -360; i < 360; i++) {
		assert(areApproxEqual(degToRadian(radToDegree(i), true), i, 3));	//values outide of [-360; 360] may need a less precise approximation
		assert(areApproxEqual(radToDegree(degToRadian(i), true), i, 3));
	}

	using namespace std::numbers;
	for(double num = -360; num < 360; num++) {
		const double princVal = anglePrincipalValue(num);
		assert(-pi < princVal && princVal <= pi);
	}
}