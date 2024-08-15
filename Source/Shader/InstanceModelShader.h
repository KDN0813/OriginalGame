#pragma once
#include "Shader/Shader.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"

#include "Component/InstancingModelComponent.h"

class InstancingModelShaderComponent;

class InstancingModelShader : public Shader
{
public:
	static const int MaxBones = 128;
	static const int MAX_INSTANCES = 512;
private:
	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};
	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};
	// インスタンシング描画で使用する共通の定数
	struct CommonDataConstantBuffer
	{
		UINT bone_transform_count; // 1回の描画で使用するボーントランスフォームの数
		DirectX::XMUINT3 dummy;
	};
	struct MeshConstantBuffer
	{
		UINT offset;				// バッファ内でメッシュの開始位置を示すオフセット値
		DirectX::XMUINT3 dummy;
	};

public:
	InstancingModelShader(ID3D11Device* device);
	~InstancingModelShader() override {}

	void Render(ID3D11DeviceContext* dc, const RenderContext& rc)override;

	// インスタンシング描画開始
	void InstancingStart() {};
	// インスタンスの追加
	void InstancingAdd(const InstanceData instance_data);
	// インスタンシング描画修了
	void InstancingEnd();

private:
	// インスタンシング描画
	void InstancingRender() {};
	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			common_data_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	// インスタンシング描画に必要なパラメータ
	ModelResource* model_resource = nullptr;
	InstancingModelResource* instancing_model_resource = nullptr;

	// インスタンス毎のワールドトランスフォームをGPUに渡すためのデータ
	UINT instance_count = 0;
	InstanceData instance_datas[MAX_INSTANCES];	// インスタンス毎のデータ
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;

	// 描画するインスタンスのシェーダー
	std::vector<std::weak_ptr<InstancingModelShaderComponent>> shader_component_Wptr_vector;
private:
	std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;
};

