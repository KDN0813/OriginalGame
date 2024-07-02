#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader/Shader.h"

class InstanceShader
{
public:
	InstanceShader(ID3D11Device* device);
	~InstanceShader(){}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
	void SetBuffers(ID3D11DeviceContext* dc, const BufferData& buffer_data);
	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);
	void End(ID3D11DeviceContext* dc);

private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};

	static const int MAX_INSTANCES = 512;
	struct InstancingMeshConstantBuffer
	{
		DirectX::XMFLOAT4X4 worldTransforms[MAX_INSTANCES];
	};

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			instancing_mesh_constantt_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;
	int instancing_count;
};