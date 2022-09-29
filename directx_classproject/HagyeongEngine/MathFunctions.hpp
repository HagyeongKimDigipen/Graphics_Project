/*
* Kim Hagyeong (hakyung.kim)
* Class Project
* cs200
* FALL 2019
*/
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

inline float DegreeToRadian(float p_Degree)
{
		return static_cast<float>(p_Degree * (M_PI / 180.0f));
}