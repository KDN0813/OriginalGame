#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/RenderContext.h"
#include "Model/Model.h"

// TODO(080/08)このクラスを継承しているクラスを削除する
class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	// 描画開始
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// 描画
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model) = 0;
	
	// 描画終了
	virtual void End(ID3D11DeviceContext* context) = 0;
};
