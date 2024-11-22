
// 動きが最初は速く、終わりに近づくほどゆっくりと減速する
float EaseOutQuad(float t)
{
    return (1.0f - (1.0f - t) * (1.0f - t));
}

// 動きが最初は速く、終わりに近づくほどゆっくりと減速する
// 最大値・最小値をしていできる
float EaseOutQuadInRange(float min, float max, float t)
{
    float easedValue = EaseOutQuad(t);

	// 最小値と最大値の範囲にスケール
    return min + (max - min) * easedValue;
}

// 動きが最初は非常にゆっくりで、終わりに近づくほど急激に加速する
float EaseInExpo(float t)
{
    return t == 0 ? 0 : pow(2, 10 * (t - 1));
}

// 動きが最初は速く、終わりに近づくほどゆっくりと減速する
// 最大値・最小値をしていできる
float EaseInExpoLerp(float s, float e, float t)
{
    float easedValue = EaseInExpo(t);

	// 最小値と最大値の範囲にスケール
    return s + (e - s) * easedValue;
}

float EaseInQuart(float t)
{
    return t * t * t * t;
}

float EaseInQuartLerp(float s, float e, float t)
{
    float easedValue = EaseInQuart(t);

	// 最小値と最大値の範囲にスケール
    return s + (e - s) * easedValue;
}

float FadeInOut(float t)
{
    return sin(t * 3.14159); // tは0～1の範囲
}