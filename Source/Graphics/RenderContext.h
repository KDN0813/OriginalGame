#pragma once

#include <DirectXMath.h>

// レンダーコンテキスト
struct RenderContext
{
	DirectX::XMFLOAT4X4 view;			// ビュー行列
	DirectX::XMFLOAT4X4 projection;	// プロジェクション行列
};
