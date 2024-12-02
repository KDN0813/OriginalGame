#include <stdlib.h>
#include <algorithm>
#include "MyMathf.h"

float MyMathf::RandomRange(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min～maxまでのランダム値に変換
	return min + (max - min) * value;
}

float MyMathf::ClampRadian(float radian)
{
	return MyMathf::WrapToRange(radian, 0.0f, 36.0f);
}

float MyMathf::ClampDegree(float degree)
{
	return MyMathf::WrapToRange(degree, 0.0f, 360.0f);
}

float MyMathf::WrapToRange(float v, float low, float hight)
{
	if (v <= low)
	{
		v += (hight - low);
	}
	else if (hight <= v)
	{
		v -= hight;
	}
	return v;
}