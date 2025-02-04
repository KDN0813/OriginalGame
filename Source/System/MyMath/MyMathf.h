#pragma once
#include <DirectXMath.h>

class MyMath
{
public:
	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);

	// �w�肵���p�x�i���W�A���j���͈͊O�̏ꍇ�A�K�؂Ȕ͈͓��ɕ␳���܂�
	static float ClampRadian(float radian);
	
	// �w�肵���p�x�i�x���j���͈͊O�̏ꍇ�A�K�؂Ȕ͈͓��ɕ␳���܂�
	static float ClampDegree(float degree);

	// �w�肳�ꂽ�l�Ƀ��b�v���܂�
	static float WrapToRange(float v, float low, float hight);

	// �~��̃����_���Ȉʒu���擾
	static const DirectX::XMFLOAT3 GetRandomPointInRing(float inner_radius, float outer_radius);
	// ���I�u�W�F�N�g�Əd�Ȃ�Ȃ��ʒu���擾
	static const DirectX::XMFLOAT3 GetNonOverlappingPointInRing(float inner_radius, float outer_radius);
};

