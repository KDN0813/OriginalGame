#pragma once

enum class CAMERA_TYPE
{
    NONE = -1,
    MAIN = 0,   // ���C���J����
#ifdef _DEBUG
    DEBUG,      // �f�o�b�O�J����
#endif // _DEBUG
    MAX,
};