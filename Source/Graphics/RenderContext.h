#pragma once

#include <DirectXMath.h>

// �����_�[�R���e�L�X�g
struct RenderContext
{
	DirectX::XMFLOAT4X4 view;			// �r���[�s��
	DirectX::XMFLOAT4X4 projection;	// �v���W�F�N�V�����s��
};
