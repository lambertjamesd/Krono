#include "HLSLBuiltInFunctions.h"
#include "HLSLParser.h"
#include <sstream>
#include <cstdio>

using namespace std;

void HLSLBuiltInFunctions::PopulateVisitor(HLSLTypeVisitor& typeVisitor)
{
	istringstream parserInput(
		"void abort();\n"
		"void clip(__numerical__<float> a);\n"

		// math
		"__numerical__<__int_float__> abs(__numerical__<__int_float__> a);\n"
		"__numerical__<float> ceil(__numerical__<float> a);\n"
		"__numerical__<__int_float__> clamp(__numerical__<__int_float__> x, __numerical__<__int_float__> min, __numerical__<__int_float__> max);\n"
		"__numerical__<float> exp(__numerical__<float> a);\n"
		"__numerical__<float> exp2(__numerical__<float> a);\n"
		"__numerical__<float> floor(__numerical__<float> a);\n"
		"__numerical__<double> fma(__numerical__<double> a, __numerical__<double> b, __numerical__<double> c);\n"
		"__numerical__<float> fmod(__numerical__<float> x, __numerical__<float> y);\n"
		"__numerical__<float> frac(__numerical__<float> a);\n"
		"__numerical__<float> frexp(__numerical__<float> x, __numerical__<float> exp);\n"
		"__numerical__<float> fwidth(__numerical__<float> x);\n"
		"bool isfinite(__numerical__<float> x);\n"
		"bool isinf(__numerical__<float> x);\n"
		"bool isnan(__numerical__<float> x);\n"
		"__numerical__<float> ldexp(__numerical__<float> x, __numerical__<float> exp);\n"
		"__numerical__<float> log(__numerical__<float> x);\n"
		"__numerical__<float> log10(__numerical__<float> x);\n"
		"__numerical__<float> log2(__numerical__<float> x);\n"
		"__numerical__<__int_float__> mad(in __numerical__<__int_float__> mvalue, in __numerical__<__int_float__> avalue, in __numerical__<__int_float__> bvalue);\n"
		"__numerical__<__int_float__> max(__numerical__<__int_float__> x, __numerical__<__int_float__> y);\n"
		"__numerical__<__int_float__> min(__numerical__<__int_float__> x, __numerical__<__int_float__> y);\n"
		"__numerical__<__int_float__> modf(in __numerical__<__int_float__> x, out __numerical__<__int_float__> ip);\n"
		"uint4 msad4(uint reference, uint2 source, uint4 accum);\n"
		"float noise(__variable_vector__<float> x);\n"
		"__numerical__<float> pow(__numerical__<float> x, __numerical__<float> y);\n"
		"__numerical__<float> rcp(__numerical__<float> x);\n" // TODO add __float_double__ scalar type
		"__numerical__<float> round(__numerical__<float> x);\n"
		"__numerical__<float> rsqrt(__numerical__<float> x);\n"
		"__numerical__<float> saturate(__numerical__<float> a);\n"
		"__numerical__<int> sign(__numerical__<__int_float__> a);\n"
		"__numerical__<float> sqrt(__numerical__<float> x);\n"
		"__numerical__<float> step(__numerical__<float> y, __numerical__<float> x);\n"
		"__numerical__<float> trunc(__numerical__<float> x);\n"

		// boolean
		"bool all(__numerical__<__bool_int_float__> a);\n"
		"bool any(__numerical__<__bool_int_float__> a);\n"

		// casting
		"double asdouble(in uint lowbits, in uint highbits);\n"
		"__numerical__<float> asfloat(__numerical__<__bool_int_float__> x);\n"
		"int asint(__bool_int_float__ x);\n"
		"__numerical__<int> asint(__numerical__<__int_float__> x);\n"
		"void asuint(in double value, out uint lowbits, out uint highbits);\n"
		"__numerical__<int> asint(__numerical__<__int_float__> x);\n"
		"float f16tof32(in uint value);\n"
		"uint f32tof16(in float value);\n"

		// bit stuff
		"__variable_vector__<uint> countbits(__variable_vector__<uint> a);\n"
		"__variable_vector__<int> firstbithigh(in __variable_vector__<int> value);\n"
		"__variable_vector__<int> firstbitlow(in __variable_vector__<int> value);\n"
		"__variable_vector__<uint> reversebits(__variable_vector__<uint> a);\n"

		// vector
		"float3 cross(float3 x, float3 y);\n"
		"float distance(__variable_vector__<float> a, __variable_vector__<float> b);\n"
		"float dot(__variable_vector__<float> a, __variable_vector__<float> b);\n"
		"__variable_vector__<__bool_int_float__> dst(__variable_vector__<__bool_int_float__> a, __variable_vector__<__bool_int_float__> b);\n"
		"__variable_vector__<float> faceforward(__variable_vector__<float> n, __variable_vector__<float> i, __variable_vector__<float> ng);\n"
		"float length(__variable_vector__<float> a);\n"
		"__numerical__<float> lerp(__numerical__<float> x, __numerical__<float> y, __numerical__<float> s);\n"
		"__variable_vector__<float> normalize(__variable_vector__<float> x);\n"
		"__variable_vector__<float> reflect(__variable_vector__<float> i, __variable_vector__<float> n);\n"
		"__variable_vector__<float> refract(__variable_vector__<float> i, __variable_vector__<float> n, float m);\n"
		"__numerical__<float> smoothstep(__numerical__<float> min, __numerical__<float> max, __numerical__<float> x);\n"

		// lighting
		"float4 lit(float n_dot_l, float n_dot_h, float m);\n"

		// derivative
		"__numerical__<float> ddx(__numerical__<float> a);\n"
		"__variable_vector__<float> ddx_coarse(__variable_vector__<float> a);\n"
		"__variable_vector__<float> ddx_fine(__variable_vector__<float> a);\n"
		"__numerical__<float> ddy(__numerical__<float> a);\n"
		"__variable_vector__<float> ddy_coarse(__variable_vector__<float> a);\n"
		"__variable_vector__<float> ddy_fine(__variable_vector__<float> a);\n"
		
		// trig
		"__numerical__<float> acos(__numerical__<float> a);\n"
		"__numerical__<float> asin(__numerical__<float> a);\n"
		"__numerical__<float> atan(__numerical__<float> a);\n"
		"__numerical__<float> atan2(__numerical__<float> y, __numerical__<float> x);\n"
		"__numerical__<float> cos(__numerical__<float> a);\n"
		"__numerical__<float> cosh(__numerical__<float> a);\n"
		"__numerical__<float> sin(__numerical__<float> a);\n"
		"void sin(__numerical__<float> x, __numerical__<float> s, __numerical__<float> c);\n"
		"__numerical__<float> sinh(__numerical__<float> a);\n"
		"__numerical__<float> tan(__numerical__<float> a);\n"
		"__numerical__<float> tanh(__numerical__<float> a);\n"
		
		"__numerical__<float> degress(__numerical__<float> a);\n"
		"__numerical__<float> radians(__numerical__<float> a);\n"

		// matrix
		// TODO add determinant
		// TODO add transpose

		// special case, the type resovler determines the return type
		"float1x1 mul(__numerical__<__bool_int_float__> a, __numerical__<__bool_int_float__> b);");
	
	HLSLParser parser(parserInput);
	typeVisitor.SetBuiltInDefinitions(move(parser.ParseFile()));
}
