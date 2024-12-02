#pragma once
class MyMathf
{
public:
	// 指定範囲のランダム値を計算する
	static float RandomRange(float min, float max);

	// 指定した角度（ラジアン）が範囲外の場合、適切な範囲内に補正します
	static float ClampRadian(float radian);
	
	// 指定した角度（度数）が範囲外の場合、適切な範囲内に補正します
	static float ClampDegree(float degree);

	// 指定された値にラップします
	static float WrapToRange(float v, float low, float hight);
};

