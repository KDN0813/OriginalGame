#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader/Shader.h"

class InstanceShader : public Shader
{
public:
	InstanceShader(ID3D11Device* device);
	~InstanceShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) override;
	void Draw(ID3D11DeviceContext* dc, const Model* model) override;
	void DrawInstance(ID3D11DeviceContext* dc, const Model* model, ID3D11Buffer* inputBuffer/*インスタンス毎のtransform*/, int instance_count) override;
	void End(ID3D11DeviceContext* dc) override;

private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};

	struct MeshConstantBuffer
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
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