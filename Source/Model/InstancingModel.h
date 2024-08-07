#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Model/ModelResource.h"
#include "Graphics/Shader/Shader.h"

using WorldTransform = DirectX::XMFLOAT4X4;

// インスタンシングモデル
class InstancingModel
{
public:
	static const int MAX_INSTANCES = 512;

public:
	// TODO(08/04)各オブジェクト毎に直接値をいじれるようにシェアドポインタに変更する
	// 個々のtransformを管理するための構造体
	struct TransformData
	{
		DirectX::XMFLOAT4X4	transform;
		UINT anime_frame;	// アニメーションの再生時間
		int anime_index = 0;
		bool anime_loop = true;
		bool anime_play = true;
		bool exist;		// 使用中か
		bool dummy[2];
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

	// ボーントランスフォーム用構造体
	struct BoneTransform
	{
		DirectX::XMFLOAT4X4 transform;
	};
	// ボーントランスフォームテクスチャ用構造体
	struct BoneTransformTextureData
	{
		std::vector<BoneTransform> bone_transforms;
	};

	/**
	 * \param animation_start_offset;    // バッファ内で使用するアニメーションの開始位置を示すオフセット値
	 * \param frame;                     // 現在のフレーム
	 * \param world_transform ワールドトランスフォーム
	 */
	struct InstanceData
	{
		UINT animation_start_offset;    
		UINT frame;                     
		WorldTransform world_transform{};
	};
public:
	InstancingModel(ID3D11Device* dc,const char* filename);
	~InstancingModel() {};

	// 個々のtransformを管理するための関数
	// 使われていない番号を割り当てて返す
	int AllocateInstancingIndex();
	// 割り当てられた番号を解放する
	void FreeInstancingIndex(int instancingIndex);
	// 行列計算
	void UpdateTransform(int instancingIndex, const DirectX::XMFLOAT4X4& transform);
	// アニメーション更新
	void UpdateAnimationFrame(int instancingIndex);

	// InstanceDataの更新
	void UpdateInstanceData(ID3D11DeviceContext* device, int& instancing_count);

	// 各種取得・設定関数
	const ModelResource* GetResource() const { return resource.get(); }
	ID3D11ShaderResourceView** GetBoneTransformTexture() { return this->bone_transform_texture.GetAddressOf(); }
	ID3D11ShaderResourceView** GetWorldTransformStructuredBuffer() { return this->instance_data_structured_buffer.GetAddressOf(); }
	const UINT& GetBoneTransformCount()const { return this->bone_transform_count; }
	const std::vector<UINT>& GetMeshOffsets()const { return this->mesh_offsets; }

	// TODO(08/04)計算位置変更する
	// BTT計算用関数
	void PlayAnimation(int index);
	void UpdateAnimation(float elapsed_time);
	bool IsPlayAnimation()const;
	void UpdateTransform();
private:
	std::shared_ptr<ModelResource>	resource;

	// BTT
	Microsoft::WRL::ComPtr<ID3D11Buffer> bone_transform_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bone_transform_texture;

	// インスタンス毎のワールドトランスフォームをGPUに渡すためのデータ
	InstanceData* instance_data = nullptr;	// インスタンス毎のデータ配列の先頭のポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_data_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> instance_data_structured_buffer;
	
	UINT bone_transform_count;				// ボーントランスフォームの数
	std::vector<UINT> mesh_offsets;			// BTTで使用するメッシュ毎の開始位置までのオフセット値
	std::vector<UINT> animation_lengths;	// アニメーションの長さ(フレーム数)
	std::vector<UINT> animation_offsets;	// アニメーションの長さ(フレーム数)


	// TODO(08/04)計算位置変更する
	// BTT計算用変数
	std::vector<Node> nodes;
	int current_animation_index = -1;	// 値が(-1)なら停止している
	float current_animation_seconds = 0.0f;	// 経過時間

	// TODO(08/04)各オブジェクト毎に直接値をいじれるようにシェアドポインタに変更する
	std::vector<TransformData> transform_datas;	// 個々のtransformを管理するコンテナ
};

