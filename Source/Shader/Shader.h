#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "Graphics/RenderContext.h"

class ShaderComponent;

class Shader
{
	friend ShaderComponent;
public:
	Shader() {}
	virtual ~Shader() {}

	/**
	 * \fn Begin
	 * \brief 描画処理(開始から終了までをまとめた関数)
	 * 
	 * \param dc
	 * \param rc
	 */
	virtual void Render(ID3D11DeviceContext* dc, const RenderContext& rc) {};

private:
	/**
	 * \fn Begin
	 * \brief 描画開始
	 * 
	 * \param dc ID3D11DeviceContextのポインタ
	 * \param rc RenderContext
	 */
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;
	/**
	 * \fn Draw
	 * \brief 描画
	 * 
	 * \param dc ID3D11DeviceContextのポインタ
	 */
	virtual void Draw(ID3D11DeviceContext* dc) = 0;
	/**
	 * \fn End
	 * \brief 描画終了
	 * 
	 * \param context
	 */
	virtual void End(ID3D11DeviceContext* context) = 0;

protected:
	std::vector<ShaderComponent*> shader_components;
};