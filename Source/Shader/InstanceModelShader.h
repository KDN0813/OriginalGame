#pragma once
#include "Shader/Shader.h"
#include "Model/InstancingModelResource.h"
#include "Model/ModelResource.h"

#include "Component/InstancingModelComponent.h"

class InstanceModelShader : public Shader
{
public:
	static const int MaxBones = 128;
	static const int MAX_INSTANCES = 512;
public:
	InstanceModelShader(ID3D11Device* device);
	~InstanceModelShader() override {}

	void Render(ID3D11DeviceContext* dc, const RenderContext& rc)override;

	bool SetInstancingResource(
		ModelResource * model_resource,
		InstancingModelResource* instancing_model_resource
	) override;
	void InstancingAdd(const InstanceData instance_data) override;
private:
	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) override;
	void Draw(ID3D11DeviceContext* dc) override;
	void End(ID3D11DeviceContext* dc) override;

	void DrawSubset(ID3D11DeviceContext* dc, const ModelResource::Subset& subset);
private:

	struct SceneConstantBuffer
	{
		DirectX::XMFLOAT4X4	viewProjection;
	};
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
		UINT offset;				// �o�b�t�@���Ń��b�V���̊J�n�ʒu�������I�t�Z�b�g�l
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

	// �C���X�^���V���O�`��ɕK�v�ȃp�����[�^
	ModelResource* model_resource = nullptr;
	InstancingModelResource* instancing_model_resource = nullptr;

	// �C���X�^���X���̃��[���h�g�����X�t�H�[����GPU�ɓn�����߂̃f�[�^
	UINT instance_count = 0;
	std::vector<InstanceData>instance_data_vector{};	// �C���X�^���X���̃f�[�^
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;

private:
	std::weak_ptr<InstancingModelComponent> instancing_model_Wptr;
};

