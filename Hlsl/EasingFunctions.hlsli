
float EaseOutQuad(float t)
{
    return (1.0f - (1.0f - t) * (1.0f - t));
}

float EaseOutQuadInRange(float min, float max, float t)
{
    float easedValue = EaseOutQuad(t);

	// �ŏ��l�ƍő�l�͈̔͂ɃX�P�[��
    return min + (max - min) * easedValue;
}

float FadeInOut(float t)
{
    return sin(t * 3.14159); // t��0�`1�͈̔�
}