
// �������ŏ��͑����A�I���ɋ߂Â��قǂ������ƌ�������
float EaseOutQuad(float t)
{
    return (1.0f - (1.0f - t) * (1.0f - t));
}

// �������ŏ��͑����A�I���ɋ߂Â��قǂ������ƌ�������
// �ő�l�E�ŏ��l�����Ă��ł���
float EaseOutQuadInRange(float min, float max, float t)
{
    float easedValue = EaseOutQuad(t);

	// �ŏ��l�ƍő�l�͈̔͂ɃX�P�[��
    return min + (max - min) * easedValue;
}

// �������ŏ��͔��ɂ������ŁA�I���ɋ߂Â��قǋ}���ɉ�������
float EaseInExpo(float t)
{
    return t == 0 ? 0 : pow(2, 10 * (t - 1));
}

// �������ŏ��͑����A�I���ɋ߂Â��قǂ������ƌ�������
// �ő�l�E�ŏ��l�����Ă��ł���
float EaseInExpoLerp(float s, float e, float t)
{
    float easedValue = EaseInExpo(t);

	// �ŏ��l�ƍő�l�͈̔͂ɃX�P�[��
    return s + (e - s) * easedValue;
}

float EaseInQuart(float t)
{
    return t * t * t * t;
}

float EaseInQuartLerp(float s, float e, float t)
{
    float easedValue = EaseInQuart(t);

	// �ŏ��l�ƍő�l�͈̔͂ɃX�P�[��
    return s + (e - s) * easedValue;
}

float FadeInOut(float t)
{
    return sin(t * 3.14159); // t��0�`1�͈̔�
}