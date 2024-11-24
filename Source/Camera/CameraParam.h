#pragma once

enum class CAMERA_TYPE
{
    NONE = -1,
    MAIN = 0,   // メインカメラ
    DEATH,      // デスカメラ

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    DEBUG,      // デバッグカメラ
#endif // _DEBUG
    MAX,
};