#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader/DebugShader.h"

// 仮のシェーダ
class TemporaryShader : public DebugShader
{
public:
	TemporaryShader(ID3D11Device* device);
	~TemporaryShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) override;
	void Draw(ID3D11DeviceContext* dc, const Model* model) override;
	void End(ID3D11DeviceContext* dc) override;

private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};

	struct MeshConstantBuffer
	{
		DirectX::XMFLOAT4X4	worldTransform[MaxBones];
		DirectX::XMFLOAT4X4	offsetTransform[MaxBones];
		DirectX::XMFLOAT4X4	boneTransform[MaxBones];
		DirectX::XMFLOAT4X4	worldTransforms;
	};

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			meshConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;
};
