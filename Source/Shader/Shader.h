#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Graphics/RenderContext.h"

class ShaderComponent;
class ModelResource;
class InstancingModelComponent;
class InstancingModelResource;
class InstanceData;

class Shader
{
	friend ShaderComponent;
public:
	Shader() {}
	virtual ~Shader() {}

	void AddShaderComponent(const std::shared_ptr<ShaderComponent>& shader_component);

	/**
	 * \fn Render
	 * \brief 描画処理(開始から終了までをまとめた関数)
	 * 
	 * \param dc
	 * \param rc
	 */
	virtual void Render(ID3D11DeviceContext* dc, const RenderContext& rc) {};

	/**
	* \fn SetInstancingResource
	* \brief インスタンシングリソースの設定
	* 
	* \param model_resource モデルリソースのポインタ
	* \param instancing_model_resource インスタンシングモデルリソースのポインタ
	* \return 正しく設定できたらtrueを返す
	*/
	virtual bool SetInstancingResource(
		ModelResource* model_resource,
		InstancingModelResource * instancing_model_resource
	) { return false; };
	/**
	 * \fn AddInstance
	 * \brief インスタンスの追加
	 */
	virtual void InstancingAdd(const InstanceData) {};

	// TODO(08/14)シェーダーコンポーネントクラスを作成する関数作る
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
	std::vector<std::weak_ptr<ShaderComponent>> shader_component_Wptr_vector;
};