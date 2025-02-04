#include <stdlib.h>
#include <algorithm>
#include "MyMathf.h"
#include "GridObjectManager\GridObjectManager.h"
#include "Object\Constant\StageConstant.h"

float MyMath::RandomRange(float min, float max)
{
	// 0.0�`1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(rand()) / RAND_MAX;

	// min�`max�܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;
}

size_t MyMath::RandomRange(size_t min, size_t max)
{
	// 0.0�`1.0�̊Ԃ܂ł̃����_���l
	size_t value = static_cast<size_t>(rand()) / RAND_MAX;

	// min�`max�܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;
}

int MyMath::RandomRange(int min, int max)
{
	int value = rand() / RAND_MAX;
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
	return GetRandomPointInRing({}, inner_radius, outer_radius);
}

const DirectX::XMFLOAT3 MyMath::GetRandomPointInRing(DirectX::XMFLOAT3 center_point, float inner_radius, float outer_radius)
{
	const float theta = MyMath::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	const float range = MyMath::RandomRange(inner_radius, outer_radius);

	return DirectX::XMFLOAT3
	{
		center_point.x + sinf(theta) * range,
		center_point.y + StageConstant::STAGE_FLOOR_Y,
		center_point.z + cosf(theta) * range
	};
}

const DirectX::XMFLOAT3 MyMath::GetNonOverlappingPointInRing(float inner_radius, float outer_radius)
{
	return GetRandomPointInRing({}, inner_radius, outer_radius);
}

const DirectX::XMFLOAT3 MyMath::GetNonOverlappingPointInRing(DirectX::XMFLOAT3 center_point, float inner_radius, float outer_radius)
{
	DirectX::XMFLOAT3 point{};

	const int max_loop_count = 3;
	int count = 0;
	while (true)
	{
		point = GetRandomPointInRing(center_point, inner_radius, outer_radius);

		GridObjectManager::Instance grid_object_manager = GridObjectManager::GetInstance();
		const int grid_index = grid_object_manager->GetGridlIndex(point);
		if (0 <= grid_index)
		{
			// �O���b�h���ɃI�u�W�F�N�g���݂��Ȃ��Ȃ烋�[�v���甲����
			if (!grid_object_manager->IsObjectInGrid(grid_index)) break;

			++count;
			if (max_loop_count <= count) break;	// ���[�v�񐔂��ő�𒴂����烋�[�v���甲����
		}
	}

	return point;
}
