#pragma once
class MyMathf
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
};

