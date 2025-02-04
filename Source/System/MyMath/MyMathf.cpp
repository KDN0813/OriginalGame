#include <stdlib.h>
#include <algorithm>
#include "MyMathf.h"
#include "GridObjectManager\GridObjectManager.h"
#include "Object\Constant\StageConstant.h"

float MyMath::RandomRange(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min～maxまでのランダム値に変換
	return min + (max - min) * value;
}

float MyMath::ClampRadian(float radian)
{
	return MyMath::WrapToRange(radian, 0.0f, 36.0f);
}

float MyMath::ClampDegree(float degree)
{
	return MyMath::WrapToRange(degree, 0.0f, 360.0f);
}

float MyMath::WrapToRange(float v, float low, float hight)
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

const DirectX::XMFLOAT3 MyMath::GetRandomPointInRing(float inner_radius, float outer_radius)
{
	const float theta = MyMath::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	const float range = MyMath::RandomRange(inner_radius, outer_radius);

	return DirectX::XMFLOAT3
	{
		sinf(theta) * range,
		StageConstant::STAGE_FLOOR_Y,
		cosf(theta) * range
	};
}

const DirectX::XMFLOAT3 MyMath::GetNonOverlappingPointInRing(float inner_radius, float outer_radius)
{
	DirectX::XMFLOAT3 point{};

	const int max_loop_count = 3;
	int count = 0;
	while (true)
	{
		point = GetRandomPointInRing(inner_radius, outer_radius);

		GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
		const int grid_index = grid_object_manager->GetGridlIndex(point);
		// グリッド内にオブジェクト存在しないならループから抜ける
		if (!grid_object_manager->IsObjectInGrid(grid_index)) break;

		++count;
		if (max_loop_count <= count) break;	// ループ回数が最大を超えたらループから抜ける
	}

	return point;
}