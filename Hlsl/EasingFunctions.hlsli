
float EaseOutQuad(float t)
{
    return (1.0f - (1.0f - t) * (1.0f - t));
}

float EaseOutQuadInRange(float min, float max, float t)
{
    float easedValue = EaseOutQuad(t);

	// 最小値と最大値の範囲にスケール
    return min + (max - min) * easedValue;
}