#pragma once

enum class CAMERA_TYPE
{
    NONE = -1,
    MAIN = 0,   // メインカメラ
#ifdef _DEBUG
    DEBUG,      // デバッグカメラ
#endif // _DEBUG
    MAX,
};