#pragma once

enum class CAMERA_TYPE
{
    NONE = -1,
    MAIN = 0,   // メインカメラ
    DEATH,      // デスカメラ

#ifdef _DEBUG
    DEBUG,      // デバッグカメラ
#endif // _DEBUG
    MAX,
};