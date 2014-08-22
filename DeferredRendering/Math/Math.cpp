
#include "Math.h"

namespace krono
{

////////////////////////////////
// float
template <>
const float Constant<float>::Zero = 0.0f;
template <>
const float Constant<float>::One = 1.0f;
template <>
const float Constant<float>::PI = (float)(M_PI);
template <>
const float Constant<float>::E = (float)(M_E);
template <>
const float Constant<float>::Deg2Rad = (float)(M_PI / 180.0);
template <>
const float Constant<float>::Rad2Deg = (float)(180.0 / M_PI);

////////////////////////////////
// double
template <>
const double Constant<double>::Zero = 0.0;
template <>
const double Constant<double>::One = 1.0;
template <>
const double Constant<double>::PI = M_PI;
template <>
const double Constant<double>::E = M_E;
template <>
const double Constant<double>::Deg2Rad = M_PI / 180.0;
template <>
const double Constant<double>::Rad2Deg = 180.0 / M_PI;

////////////////////////////////
// int
template <>
const int Constant<int>::Zero = 0;
template <>
const int Constant<int>::One = 1;

}
