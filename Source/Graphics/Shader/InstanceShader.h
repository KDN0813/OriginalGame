#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader/Shader.h"

class InstanceShader
{
public:
	InstanceShader(ID3D11Device* device);
	~InstanceShader(){}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc, ID3D11ShaderResourceView** bone_transform_texture);
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
	struct WorldTransform
	{
		DirectX::XMFLOAT4X4 transforms;
	};
	WorldTransform* world_transforms;
	Microsoft::WRL::ComPtr<ID3D11Buffer> world_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> world_transform_structured_buffer;

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
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