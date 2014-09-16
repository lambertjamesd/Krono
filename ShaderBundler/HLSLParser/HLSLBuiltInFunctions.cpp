#include "HLSLBuiltInFunctions.h"
#include "HLSLParser.h"
#include <sstream>
#include <cstdio>

using namespace std;

void HLSLBuiltInFunctions::PopulateVisitor(HLSLTypeVisitor& typeVisitor)
{
	istringstream parserInput(
		"void abort();"
		"void clip(__numerical__<float> a);"

		"__numerical__<__int_float__> abs(__numerical__<__int_float__> a);"
		"__numerical__<float> ceil(__numerical__<float> a);"
		"__numerical__<__int_float__> clamp(__numerical__<__int_float__> x, __numerical__<__int_float__> min, __numerical__<__int_float__> max);"

		"bool all(__numerical__<__bool_int_float__> a);"
		"bool any(__numerical__<__bool_int_float__> a);"

		// casting
		"double asdouble(in uint lowbits, in uint highbits);"
		"__numerical__<float> asfloat(__numerical__<__bool_int_float__> x);"
		"int asint(__bool_int_float__ x);"
		"__numerical__<int> asint(__numerical__<__int_float__> x);"
		"void asuint(in double value, out uint lowbits, out uint highbits);"
		"__numerical__<int> asint(__numerical__<__int_float__> x);"
		
		// bit stuff
		"__variable_vector__<uint> countbits(__variable_vector__<uint> a);"

		// vector
		"float3 cross(float3 x, float3 y);"
		"float dot(__variable_vector__<float> a, __variable_vector__<float> b);"

		// derivative
		"__numerical__<float> ddx(__numerical__<float> a);"
		"__variable_vector__<float> ddx_coarse(__variable_vector__<float> a);"
		"__variable_vector__<float> ddx_fine(__variable_vector__<float> a);"
		"__numerical__<float> ddy(__numerical__<float> a);"
		"__variable_vector__<float> ddy_coarse(__variable_vector__<float> a);"
		"__variable_vector__<float> ddy_fine(__variable_vector__<float> a);"
		
		// trig
		"__numerical__<float> acos(__numerical__<float> a);"
		"__numerical__<float> asin(__numerical__<float> a);"
		"__numerical__<float> atan(__numerical__<float> a);"
		"__numerical__<float> atan2(__numerical__<float> y, __numerical__<float> x);"
		"__numerical__<float> cos(__numerical__<float> a);"
		"__numerical__<float> cosh(__numerical__<float> a);"
		"__numerical__<float> sin(__numerical__<float> a);"
		"__numerical__<float> sinh(__numerical__<float> a);"
		"__numerical__<float> tan(__numerical__<float> a);"
		"__numerical__<float> tanh(__numerical__<float> a);"
		
		"__numerical__<float> degress(__numerical__<float> a);"

		// TODO add determinant

		"float length(__variable_vector__<float> a);"
		"__numerical__<__int_float__> max(__numerical__<__int_float__> a, __numerical__<__int_float__> b);"
	
		"__numerical__<float> saturate(__numerical__<float> a);"

		// special case, the type resovler determines the return type
		"float1x1 mul(__numerical__<__bool_int_float__> a, __numerical__<__bool_int_float__> b);");
	
	HLSLParser parser(parserInput);
	typeVisitor.SetBuiltInDefinitions(move(parser.ParseFile()));
}