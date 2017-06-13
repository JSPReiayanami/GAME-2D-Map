#ifndef __MY_MATH_DLL_H_
#define __MY_MATH_DLL_H_
#include <math.h>
#if defined(_USERMYDLL)
#define MY_MATH_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define MY_MATH_DLL     __declspec(dllimport)
#endif


namespace  MathHelper
{

	struct MY_MATH_DLL FloatPos
	{
		float x;
		float y;
		FloatPos();
		FloatPos(const float & x_v, const float & y_v);
	};

	float MY_MATH_DLL GetAngleToOneRotation(const float & angle);

	FloatPos MY_MATH_DLL GetPostionWithVectorAndLength(const FloatPos & vec, const float & length, const float & angle);

	float MY_MATH_DLL GetAngleToOneRotation(const FloatPos & s_pos, const FloatPos & e_pos);
};


#endif