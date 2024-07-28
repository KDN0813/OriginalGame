#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

// インスタンシングモデル
class InstancingModel
{
public:
	struct InstancingData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// 使用中か
		bool dummy[3];
	};

	struct Node
	{
		Node() = default;
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};
public:
	InstancingModel(const char* filename);
	~InstancingModel() {};

	// 使われていない番号を割り当てて返す
	int AllocateInstancingIndex();

	// 割り当てられた番号を解放する
	void FreeInstancingIndex(int instancingIndex);

	// 行列計算
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// 各種取得・設定関数
	const ModelResource* GetResource() const { return resource.get(); }
	const size_t GetInstanceCount()const { return this->instance_cout; }
	// シェーダーへの設定用バッファ情報取得
	BufferData GetBufferData(const ModelResource::Mesh& mesh) const;

	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<InstancingData> instancing_data;
	size_t instance_cout;

	// 
	struct BoneTransformTextureData
	{
		std::vector<DirectX::XMFLOAT4X4> matrices;

	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	// BTT計算用関数
	void PlayAnimation(int index);
	void UpdateAnimation(float elapsed_time);
	bool IsPlayAnimation()const;
	void UpdateTransform();
	// BTT計算用変数
	std::vector<Node> nodes;
	int current_animation_index = -1;	// 値が(-1)なら停止している
	float current_animation_seconds = 0.0f;	// 経過時間
};

