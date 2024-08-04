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
	static const int MAX_INSTANCES = 512;

public:
	struct TransformData
	{
		DirectX::XMFLOAT4X4	transform;
		bool exist;		// 使用中か
		bool dummy[3];
	};

	struct Node
	{
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
	InstancingModel(ID3D11Device* dc,const char* filename);
	~InstancingModel() {};

	// 使われていない番号を割り当てて返す
	int AllocateInstancingIndex();

	// 割り当てられた番号を解放する
	void FreeInstancingIndex(int instancingIndex);

	// 行列計算
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);

	// world_transform_structured_bufferの更新
	void UpdateWorldTransformBuffer(ID3D11DeviceContext* device, int& instancing_count);

	// 各種取得・設定関数
	const ModelResource* GetResource() const { return resource.get(); }
	const size_t GetInstanceCount()const { return this->instance_cout; }
	// シェーダーへの設定用バッファ情報取得
	BufferData GetBufferData(const ModelResource::Mesh& mesh) const;

	const std::vector<InstancingModel::TransformData>& GetInstancingData() { return this->instancing_data; }

	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
	ID3D11ShaderResourceView** GetWorldTransformStructuredBuffer() { return this->world_transform_structured_buffer.GetAddressOf(); }
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<TransformData> instancing_data;
	size_t instance_cout;

	struct BoneTransform
	{
		DirectX::XMFLOAT4X4 transform;
	};
	struct BoneTransformTextureData
	{
		std::vector<BoneTransform> bone_transforms;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	/**
	* ボーントランスフォームを取得するために必要なパラメータ(VsInBoneTransformData)
	* \param bone_size 1回の描画で使用するボーントランスフォームの数
	* \param mesh_first_bone_index 描画するメッシュの先頭のボーントランスフォームのインデックス
	* \param animation_first_bone_index 使うアニメーションの先頭のボーントランスフォームのインデックス
	* \param frame アニメーションのフレーム
	*/
	struct BoneTransformData
	{
		UINT bone_size = { 0 };
		UINT mesh_first_bone_index = { 0 };
		UINT animation_first_bone_index = { 0 };
		UINT frame = { 0 };
	};
	std::vector<BoneTransformData>			bone_transform_datas;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	bone_transform_data_buffer;
	
	// インスタンス毎のデータ(VsInInstancing)
	struct WorldTransform
	{
		DirectX::XMFLOAT4X4 transform{};
	};
	// インスタンス毎のワールドトランスフォームをGPUに渡すためのデータ
	WorldTransform* world_transforms = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> world_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> world_transform_structured_buffer;

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

