#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader/DebugShader.h"

class FormerInstanceShader
{
public:
	FormerInstanceShader(ID3D11Device* device);
	~FormerInstanceShader(){}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
	void Draw(ID3D11DeviceContext* dc, class InstancingModel* model_resource);
	void End(ID3D11DeviceContext* dc);

	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);
private:
	static const int MaxBones = 128;

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};

	static const int MAX_INSTANCES = 512;

	struct SubsetConstantBuffer
	{
		DirectX::XMFLOAT4	materialColor;
	};

	// �C���X�^���V���O�`��Ŏg�p���鋤�ʂ̒萔
	struct CommonDataConstantBuffer
	{
		UINT bone_transform_count; // 1��̕`��Ŏg�p����{�[���g�����X�t�H�[���̐�
		DirectX::XMUINT3 dummy;
	};
	struct MeshConstantBuffer
	{
		UINT offset;            // �o�b�t�@���Ń��b�V���̊J�n�ʒu�������I�t�Z�b�g�l
		DirectX::XMUINT3 dummy;
	};

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
	int instancing_count;
};