#pragma once

enum class CAMERA_TYPE
{
    NONE = -1,
    MAIN = 0,   // ���C���J����
    DEATH,      // �f�X�J����

#if defined(_DEBUG) || defined(RELEASE_DEBUG)
    DEBUG,      // �f�o�b�O�J����
#endif // _DEBUG
    MAX,
};